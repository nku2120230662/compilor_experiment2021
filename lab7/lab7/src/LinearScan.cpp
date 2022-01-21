#include <algorithm>
#include "LinearScan.h"
#include "MachineCode.h"
#include "LiveVariableAnalysis.h"

LinearScan::LinearScan(MachineUnit *unit)
{
    this->unit = unit;
    for (int i = 4; i < 11; i++)
        regs.push_back(i);
}

void LinearScan::allocateRegisters()
{
    for (auto &f : unit->getFuncs())
    {
        func = f;
        bool success;
        success = false;
        while (!success) // repeat until all vregs can be mapped
        {
            computeLiveIntervals();
            success = linearScanRegisterAllocation();
            if (success) // all vregs can be mapped to real regs
                modifyCode();
            else // spill vregs that can't be mapped to real regs
                genSpillCode();
        }
    }
}

void LinearScan::makeDuChains()
{
    LiveVariableAnalysis lva;
    lva.pass(func);
    du_chains.clear();
    int i = 0;
    std::map<MachineOperand, std::set<MachineOperand *>> liveVar;
    for (auto &bb : func->getBlocks())
    {
        liveVar.clear();
        for (auto &t : bb->getLiveOut())
            liveVar[*t].insert(t);
        int no;
        no = i = bb->getInsts().size() + i;
        for (auto inst = bb->getInsts().rbegin(); inst != bb->getInsts().rend(); inst++)
        {
            (*inst)->setNo(no--);
            for (auto &def : (*inst)->getDef())
            {
                if (def->isVReg())
                {
                    auto &uses = liveVar[*def];
                    du_chains[def].insert(uses.begin(), uses.end());
                    auto &kill = lva.getAllUses()[*def];
                    std::set<MachineOperand *> res;
                    set_difference(uses.begin(), uses.end(), kill.begin(), kill.end(), inserter(res, res.end()));
                    liveVar[*def] = res;
                }
            }
            for (auto &use : (*inst)->getUse())
            {
                if (use->isVReg())
                    liveVar[*use].insert(use);
            }
        }
    }
}

void LinearScan::computeLiveIntervals()
{
    makeDuChains();
    intervals.clear();
    for (auto &du_chain : du_chains)
    {
        int t = -1;
        for (auto &use : du_chain.second)
            t = std::max(t, use->getParent()->getNo());
        Interval *interval = new Interval({du_chain.first->getParent()->getNo(), t, false, 0, 0, {du_chain.first}, du_chain.second});
        intervals.push_back(interval);
    }
    bool change;
    change = true;
    while (change)
    {
        change = false;
        std::vector<Interval *> t(intervals.begin(), intervals.end());
        for (size_t i = 0; i < t.size(); i++)
            for (size_t j = i + 1; j < t.size(); j++)
            {
                Interval *w1 = t[i];
                Interval *w2 = t[j];
                if (**w1->defs.begin() == **w2->defs.begin())
                {
                    std::set<MachineOperand *> temp;
                    set_intersection(w1->uses.begin(), w1->uses.end(), w2->uses.begin(), w2->uses.end(), inserter(temp, temp.end()));
                    if (!temp.empty())
                    {
                        change = true;
                        w1->defs.insert(w2->defs.begin(), w2->defs.end());
                        w1->uses.insert(w2->uses.begin(), w2->uses.end());
                        w1->start = std::min(w1->start, w2->start);
                        w1->end = std::max(w1->end, w2->end);
                        auto it = std::find(intervals.begin(), intervals.end(), w2);
                        if (it != intervals.end())
                            intervals.erase(it);
                    }
                }
            }
    }
    sort(intervals.begin(), intervals.end(), compareStart);
}

bool LinearScan::linearScanRegisterAllocation()
{
    // Todo
    bool flag=true;
    for (auto unhandled_interval : intervals)
    {
        Interval *usable_interval;
        for (auto handled_interval : actives)
        {
            if (handled_interval->end < unhandled_interval->start)
            {
                eraseActive(handled_interval);
                setReg(0,handled_interval->rreg);
            }
        }
        if(actives.size()==regs.size()) //need to overflow
        {
            Interval* lastactive=actives[actives.size()-1];
            if(lastactive->end < unhandled_interval->end) //unhandled is latter
            {
                unhandled_interval->spill=true;
            }
            else
            {
                lastactive->spill=true;
                setReg(unhandled_interval,lastactive->rreg); //alloc the rreg
                eraseActive(lastactive);
                insertToActive(unhandled_interval);
            }
            flag=false;
        }
        else
        {
            setReg(unhandled_interval,getUnusedReg());
            insertToActive(unhandled_interval);
        }
    }
    return flag;
}

void LinearScan::modifyCode()
{
    for (auto &interval : intervals)
    {
        func->addSavedRegs(interval->rreg);
        for (auto def : interval->defs)
            def->setReg(interval->rreg);
        for (auto use : interval->uses)
            use->setReg(interval->rreg);
    }
}

void LinearScan::genSpillCode()
{
    for (auto &interval : intervals)
    {
        if (!interval->spill)
            continue;
        // TODO
        /* HINT:
         * The vreg should be spilled to memory.
         * 1. insert ldr inst before the use of vreg
         * 2. insert str inst after the def of vreg
         */
        auto cur_func = func;
        MachineInstruction* cur_inst=0;
        MachineBlock* cur_block;
        int offset = cur_func->AllocSpace(4);
        for(auto use:interval->uses)
        {
            auto reg=new MachineOperand(*use);
            cur_block=use->getParent()->getParent();
            auto useinst=use->getParent();
            cur_inst=new LoadMInstruction(cur_block,reg,new MachineOperand(MachineOperand::REG, 11),new MachineOperand(MachineOperand::IMM,-offset));
            for(auto i=cur_block->getInsts().begin();i!=cur_block->getInsts().end();i++)
            {
                if(*i==useinst)
                {
                    cur_block->getInsts().insert(i,1,cur_inst);
                    break;
                }
            }

        }
        for(auto def:interval->defs)
        {
            auto reg=new MachineOperand(*def);
            cur_block=def->getParent()->getParent();
            auto definst=def->getParent();
            cur_inst=new StoreMInstruction(cur_block,reg,new MachineOperand(MachineOperand::REG, 11),new MachineOperand(MachineOperand::IMM,-offset));
            for(auto i=cur_block->getInsts().begin();i!=cur_block->getInsts().end();i++)
            {
                if(*i==definst)
                {
                    i++;
                    cur_block->getInsts().insert(i,1,cur_inst);
                    break;
                }
            }
        }
    }
}

void LinearScan::expireOldIntervals(Interval *interval)
{
    // Todo
}

void LinearScan::spillAtInterval(Interval *interval)
{
    // Todo
}

bool LinearScan::compareStart(Interval *a, Interval *b)
{
    return a->start < b->start;
}

void LinearScan::eraseActive(Interval *handled_interval)
{
    for (auto it = actives.begin(); it != actives.end(); it++)
    {
        if (*it == handled_interval)
        {
            actives.erase(it);
            break;
        }
    }
}

void LinearScan::insertToActive(Interval *handled_interval)
{
    if (actives.size() == 0)
    {
        actives.push_back(handled_interval);
        return ;
    }
    for (auto it = actives.begin(); it != actives.end(); it++)
    {
        if ((*it)->end > handled_interval->end)
        {
            actives.insert(it, 1, handled_interval);
            return;
        }
    }
    actives.push_back(handled_interval);
    return ;
}
 
void LinearScan::setReg(Interval *interval,int reg)
{   
    reg_map[reg]=interval;
    if(interval)
        interval->rreg=reg;
}

int LinearScan::getUnusedReg()
{
    for(int i=4;i<=10;i++)
    {
        if(reg_map[i]==0)
        {
            return i;
        }
    }
    return 0;
}

