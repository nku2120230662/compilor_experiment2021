#include "Instruction.h"
#include "BasicBlock.h"
#include <iostream>
#include <string>
#include <sstream>
#include "Function.h"
#include "Type.h"
extern FILE* yyout;

Instruction::Instruction(unsigned instType, BasicBlock *insert_bb)
{
    prev = next = this;
    opcode = -1;
    this->instType = instType;
    if (insert_bb != nullptr)
    {
        insert_bb->insertBack(this);
        parent = insert_bb;
    }
}

Instruction::~Instruction()
{
    parent->remove(this);
}

BasicBlock *Instruction::getParent()
{
    return parent;
}

void Instruction::setParent(BasicBlock *bb)
{
    parent = bb;
}

void Instruction::setNext(Instruction *inst)
{
    next = inst;
}

void Instruction::setPrev(Instruction *inst)
{
    prev = inst;
}

Instruction *Instruction::getNext()
{
    return next;
}

Instruction *Instruction::getPrev()
{
    return prev;
}

BinaryInstruction::BinaryInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb) : Instruction(BINARY, insert_bb)
{
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

BinaryInstruction::~BinaryInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void BinaryInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[0]->getType()->toStr();
    switch (opcode)
    {
    case ADD:
        op = "add";
        break;
    case SUB:
        op = "sub";
        break;
    case MUL:
        op = "mul";
        break;
    case DIV:
        op = "sdiv";
        break;
    case MOD:
        op = "srem";
        break;
    case XOR:
        op = "xor";
        break;
    default:
        break;
    }
    fprintf(yyout, "  %s = %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

ExtInstruction::ExtInstruction(Operand *dst, Operand *src, BasicBlock *insert_bb) : Instruction(EXT, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

void ExtInstruction::output() const
{
    std::string dst, src, dst_type, src_type;
    dst = operands[0]->toStr();
    src = operands[1]->toStr();
    dst_type = operands[0]->getType()->toStr();
    src_type = operands[1]->getType()->toStr();
    fprintf(yyout, "  %s = zext %s %s to %s\n", dst.c_str(), src_type.c_str(), src.c_str(), dst_type.c_str());
}

CmpInstruction::CmpInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb): Instruction(CMP, insert_bb){
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

CmpInstruction::~CmpInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void CmpInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[1]->getType()->toStr();
    switch (opcode)
    {
    case EQ:
        op = "eq";
        break;
    case NE:
        op = "ne";
        break;
    case LT:
        op = "slt";
        break;
    case LE:
        op = "sle";
        break;
    case GT:
        op = "sgt";
        break;
    case GE:
        op = "sge";
        break;
    default:
        op = "";
        break;
    }

    fprintf(yyout, "  %s = icmp %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

UncondBrInstruction::UncondBrInstruction(BasicBlock *to, BasicBlock *insert_bb) : Instruction(UNCOND, insert_bb)
{
    branch = to;
}

void UncondBrInstruction::output() const
{
    fprintf(yyout, "  br label %%B%d\n", branch->getNo());
}

void UncondBrInstruction::setBranch(BasicBlock *bb)
{
    branch = bb;
}

BasicBlock *UncondBrInstruction::getBranch()
{
    return branch;
}

CondBrInstruction::CondBrInstruction(BasicBlock*true_branch, BasicBlock*false_branch, Operand *cond, BasicBlock *insert_bb) : Instruction(COND, insert_bb){
    this->true_branch = true_branch;
    this->false_branch = false_branch;
    cond->addUse(this);
    operands.push_back(cond);
}

CondBrInstruction::~CondBrInstruction()
{
    operands[0]->removeUse(this);
}

void CondBrInstruction::output() const
{
    std::string cond, type;
    cond = operands[0]->toStr();
    type = operands[0]->getType()->toStr();
    int true_label = true_branch->getNo();
    int false_label = false_branch->getNo();
    fprintf(yyout, "  br %s %s, label %%B%d, label %%B%d\n", type.c_str(), cond.c_str(), true_label, false_label);
}

void CondBrInstruction::setFalseBranch(BasicBlock *bb)
{
    false_branch = bb;
}

BasicBlock *CondBrInstruction::getFalseBranch()
{
    return false_branch;
}

void CondBrInstruction::setTrueBranch(BasicBlock *bb)
{
    true_branch = bb;
}

BasicBlock *CondBrInstruction::getTrueBranch()
{
    return true_branch;
}

RetInstruction::RetInstruction(Operand *src, BasicBlock *insert_bb) : Instruction(RET, insert_bb)
{
    if(src != nullptr)
    {
        operands.push_back(src);
        src->addUse(this);
    }
}

RetInstruction::~RetInstruction()
{
    if(!operands.empty())
        operands[0]->removeUse(this);
}

void RetInstruction::output() const
{
    if(operands.empty())
    {
        fprintf(yyout, "  ret void\n");
    }
    else
    {
        std::string ret, type;
        ret = operands[0]->toStr();
        type = operands[0]->getType()->toStr();
        fprintf(yyout, "  ret %s %s\n", type.c_str(), ret.c_str());
    }
}

CallInstruction::CallInstruction(Operand *dst, std::vector<Operand*> &params, SymbolEntry*se, BasicBlock *insert_bb) : Instruction(CALL, insert_bb)
{
    this->se = se;
    this->opcode = opcode;
    operands.push_back(dst);
    operands.insert(operands.end(), params.begin(), params.end());
    if(dst != nullptr)
        dst->setDef(this);
    for(auto &use:params)
        use->addUse(this);
}

CallInstruction::~CallInstruction() 
{
    if(operands[0] != nullptr)
        operands[0]->setDef(nullptr);
    for (size_t i = 1; i < operands.size(); i++)
    {
        auto operand = operands[i];
        operand->removeUse(this);
    }
}

void CallInstruction::output() const
{
    fprintf(yyout, "  ");
    if (operands[0] != nullptr)
    {
        std::string str = operands[0]->toStr();
        fprintf(yyout, "%s = ", str.c_str());
    }
    FunctionType* funcType = dynamic_cast<FunctionType*>(se->getType());
    Type *retType = funcType->getRetType();
    fprintf(yyout, "call %s %s", retType->toStr().c_str(), se->toStr().c_str());
    if (operands.size() == 1)
        fprintf(yyout, "()\n");
    else
    {
        std::string s;
        s = operands[1]->toStr();
        fprintf(yyout, "(%s %s", operands[1]->getType()->toStr().c_str(), s.c_str());
        for (size_t i = 2; i < operands.size(); i++)
        {
            s = operands[i]->toStr();
            fprintf(yyout, ", %s %s", operands[i]->getType()->toStr().c_str(), s.c_str());
        }
        fprintf(yyout, ")\n");
    }
}

AllocaInstruction::AllocaInstruction(Operand *dst, SymbolEntry *se, BasicBlock *insert_bb) : Instruction(ALLOCA, insert_bb)
{
    operands.push_back(dst);
    dst->setDef(this);
    this->se = se;
}

AllocaInstruction::~AllocaInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
}

void AllocaInstruction::output() const
{
    std::string dst, type;
    dst = operands[0]->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "  %s = alloca %s, align 4\n", dst.c_str(), type.c_str());
}

LoadInstruction::LoadInstruction(Operand *dst, Operand *src_addr, BasicBlock *insert_bb) : Instruction(LOAD, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src_addr);
    dst->setDef(this);
    src_addr->addUse(this);
}

LoadInstruction::~LoadInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}

void LoadInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string src_type;
    std::string dst_type;
    dst_type = operands[0]->getType()->toStr();
    src_type = operands[1]->getType()->toStr();
    fprintf(yyout, "  %s = load %s, %s %s, align 4\n", dst.c_str(), dst_type.c_str(), src_type.c_str(), src.c_str());
}

StoreInstruction::StoreInstruction(Operand *dst_addr, Operand *src, BasicBlock *insert_bb ,int paramno) : Instruction(STORE, insert_bb)
{
    operands.push_back(dst_addr);
    operands.push_back(src);
    dst_addr->addUse(this);
    src->addUse(this);
    this->paramno = paramno;
}

StoreInstruction::~StoreInstruction()
{
    operands[0]->removeUse(this);
    operands[1]->removeUse(this);
}

void StoreInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string dst_type = operands[0]->getType()->toStr();
    std::string src_type = operands[1]->getType()->toStr();

    fprintf(yyout, "  store %s %s, %s %s, align 4\n", src_type.c_str(), src.c_str(), dst_type.c_str(), dst.c_str());
}

MovInstruction::MovInstruction(Operand *dst, Operand *src, BasicBlock *insert_bb, bool isGlobal) : Instruction(MOV, insert_bb)
{
    this->isGlobal = isGlobal;
    operands.push_back(dst);
    operands.push_back(src);
    dst->addUse(this);
    src->addUse(this);
}

MovInstruction::~MovInstruction()
{
    operands[0]->removeUse(this);
    operands[1]->removeUse(this);
}

void MovInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string dst_type = operands[0]->getType()->toStr();
    std::string src_type = operands[1]->getType()->toStr();
    if (isGlobal)
    {
        fprintf(yyout, "%s = global %s %s , align 4\n", dst.c_str(), src_type.c_str(), src.c_str());
    }
    else
    {
        fprintf(yyout, "%s = %s %s , align 4\n", dst.c_str(), src_type.c_str(), src.c_str());
    }
}

MachineOperand* Instruction::genMachineOperand(Operand* ope)
{
    auto se = ope->getEntry();
    MachineOperand* mope = nullptr;
    if(se->isConstant())
        mope = new MachineOperand(MachineOperand::IMM, dynamic_cast<ConstantSymbolEntry*>(se)->getValue());
    else if(se->isTemporary())
        mope = new MachineOperand(MachineOperand::VREG, dynamic_cast<TemporarySymbolEntry*>(se)->getLabel());
    else if(se->isVariable())
    {
        auto id_se = dynamic_cast<IdentifierSymbolEntry*>(se);
        if(id_se->isGlobal())
            mope = new MachineOperand(id_se->getName().c_str());
        else
            exit(0);
    }
    return mope;
}

MachineOperand* Instruction::genMachineReg(int reg) 
{
    return new MachineOperand(MachineOperand::REG, reg);
}

MachineOperand* Instruction::genMachineVReg() 
{
    return new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel());
}

MachineOperand* Instruction::genMachineImm(int val) 
{
    return new MachineOperand(MachineOperand::IMM, val);
}

MachineOperand* Instruction::genMachineLabel(int block_no)
{
    std::ostringstream buf;
    buf << ".L" << block_no;
    std::string label = buf.str();
    return new MachineOperand(label);
}

void AllocaInstruction::genMachineCode(AsmBuilder* builder)
{
    /* HINT:
    * Allocate stack space for local variabel
    * Store frame offset in symbol entry */
    auto cur_func = builder->getFunction();
    int offset = cur_func->AllocSpace(4);
    dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->setOffset(-offset);
}

void LoadInstruction::genMachineCode(AsmBuilder* builder)
{
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    // Load global operand
    if(operands[1]->getEntry()->isVariable()
    && dynamic_cast<IdentifierSymbolEntry*>(operands[1]->getEntry())->isGlobal())
    {
        auto dst = genMachineOperand(operands[0]);
        auto internal_reg1 = genMachineVReg();
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        auto src = genMachineOperand(operands[1]);
        // example: load r0, addr_a
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, src);
        cur_block->InsertInst(cur_inst);
        // example: load r1, [r0]
        cur_inst = new LoadMInstruction(cur_block, dst, internal_reg2);
        cur_block->InsertInst(cur_inst);
    }
    // Load local operand
    else if(operands[1]->getEntry()->isTemporary()
    && operands[1]->getDef()
    && operands[1]->getDef()->isAlloc())
    {
        // example: load r1, [r0, #4]
        auto dst = genMachineOperand(operands[0]);
        auto src1 = genMachineReg(11);
        auto src2 = genMachineImm(dynamic_cast<TemporarySymbolEntry*>(operands[1]->getEntry())->getOffset());
        cur_inst = new LoadMInstruction(cur_block, dst, src1, src2);
        cur_block->InsertInst(cur_inst);
    }
    // Load operand from temporary variable
    else
    {
        // example: load r1, [r0]
        auto dst = genMachineOperand(operands[0]);
        auto src = genMachineOperand(operands[1]);
        cur_inst = new LoadMInstruction(cur_block, dst, src);
        cur_block->InsertInst(cur_inst);
    }
}

void StoreInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    // Store to a global var
    if(operands[0]->getEntry()->isVariable()
    && dynamic_cast<IdentifierSymbolEntry*>(operands[0]->getEntry())->isGlobal())
    {
        auto dst = genMachineOperand(operands[0]);
        auto src = genMachineOperand(operands[1]);
        if (src->isImm())
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, src);
            cur_block->InsertInst(cur_inst);
            src = new MachineOperand(*internal_reg);
        }
        auto internal_reg1 = genMachineVReg();
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        // example: load r0, addr_a
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, dst);
        cur_block->InsertInst(cur_inst);
        // example: store r1, [r0]
        cur_inst = new StoreMInstruction(cur_block, src, internal_reg2);
        cur_block->InsertInst(cur_inst);
    }
    else if (paramno != -1) //store to a param
    {
        auto dst1 = genMachineReg(11);
        auto dst2 = genMachineImm(dynamic_cast<TemporarySymbolEntry *>(operands[0]->getEntry())->getOffset());
        if(paramno>3) //is in stack
        {
            auto reg1=genMachineVReg();
            cur_inst=new LoadMInstruction(cur_block,reg1,genMachineReg(11),genMachineImm((paramno-4)*4));
            cur_block->InsertInst(cur_inst);
            cur_inst=new StoreMInstruction(cur_block,new MachineOperand(*reg1),dst1,dst2);
            cur_block->InsertInst(cur_inst);
        }
        else //is in regs
        {
            auto src = genMachineReg(paramno);
            cur_inst = new StoreMInstruction(cur_block, src, dst1, dst2);
            cur_block->InsertInst(cur_inst);
        }
    }
    // store to a local operand
    else if(operands[0]->getEntry()->isTemporary()
    && operands[0]->getDef()
    && operands[0]->getDef()->isAlloc())
    {
        // example: store r1, [r0, #4]
        auto src = genMachineOperand(operands[1]);

        if(src->isImm()){
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, src);
            cur_block->InsertInst(cur_inst);
            src = new MachineOperand(*internal_reg);
        }

        auto dst1 = genMachineReg(11);
        auto dst2 = genMachineImm(dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->getOffset());
        cur_inst = new StoreMInstruction(cur_block, src, dst1, dst2);
        cur_block->InsertInst(cur_inst);
    }
    // Load operand from temporary variable
    else
    {
        // example: store r1, [r0]
        auto src = genMachineOperand(operands[1]);
        if (src->isImm())
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, src);
            cur_block->InsertInst(cur_inst);
            src = new MachineOperand(*internal_reg);
        }
        auto dst = genMachineOperand(operands[0]);
        cur_inst = new StoreMInstruction(cur_block, src, dst);
        cur_block->InsertInst(cur_inst);
    }

}

void MovInstruction::genMachineCode(AsmBuilder *builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    auto cur_unit = builder->getUnit();
    MachineInstruction *cur_inst = 0;
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    cur_inst = new GlobalMInstruction(cur_block, dst, src);
    cur_unit->insertGlobal(dynamic_cast<GlobalMInstruction *>(cur_inst));
}

void BinaryInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO:
    // complete other instructions
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src1 = genMachineOperand(operands[1]);
    auto src2 = genMachineOperand(operands[2]);
    /* HINT:
    * The source operands of ADD instruction in ir code both can be immediate num.
    * However, it's not allowed in assembly code.
    * So you need to insert LOAD/MOV instrucrion to load immediate num into register.
    * As to other instructions, such as MUL, CMP, you need to deal with this situation, too.*/
    MachineInstruction* cur_inst = nullptr;
    if(src1->isImm())
    {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    switch (opcode)//SUB, ADD, MUL, DIV, MOD, XOR
    {
    case ADD:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD, dst, src1, src2);
        break;
    case SUB:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, src1, src2);
        break;
    case MUL:
        if (src2->isImm())
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*internal_reg);
        }
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, dst, src1, src2);
        break;
    case DIV:
        if (src2->isImm())
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*internal_reg);
        }
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst, src1, src2);
        break;
    case MOD:
    {
        //cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MOD, dst, src1, src2);
        //a%b <==> a-(a/b)*b
        //DIV
        if(src2->isImm()){
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*internal_reg);
        }
        auto dst_div = genMachineVReg();
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst_div, src1, src2);
        cur_block->InsertInst(cur_inst);
        //MUL
        auto dst_mul = genMachineVReg();
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, dst_mul, dst_div, src2);
        cur_block->InsertInst(cur_inst);
        //SUB
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, src1, dst_mul);
        break;
    }
    case XOR:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::XOR, dst, src1, src2);
        break;
    default:
        break;
    }
    cur_block->InsertInst(cur_inst);
}

void CmpInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src1 = genMachineOperand(operands[1]);
    auto src2 = genMachineOperand(operands[2]);
    MachineInstruction* cur_inst = nullptr;
    //cmp R,R  or  cmp R,#imm
    if (src1->isImm())
    {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    if (src2->isImm())
    {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
        cur_block->InsertInst(cur_inst);
        src2 = new MachineOperand(*internal_reg);
    }
    cur_inst = new CmpMInstruction(cur_block, src1, src2);
    cur_block->InsertInst(cur_inst);

    CondBrInstruction* nextinst = dynamic_cast<CondBrInstruction*>(getNext());
    if (nextinst != 0)
    {
        switch (this->opcode)
        {
        case EQ:
            nextinst->setop(CondBrInstruction::EQ);
            break;
        case NE:
            nextinst->setop(CondBrInstruction::NE);
            break;
        case LE:
            nextinst->setop(CondBrInstruction::LE);
            break;
        case GE:
            nextinst->setop(CondBrInstruction::GE);
            break;
        case LT:
            nextinst->setop(CondBrInstruction::LT);
            break;
        case GT:
            nextinst->setop(CondBrInstruction::GT);
            break;
        default:
            break;
        }
        std::string label;
        label = ".L";
        std::stringstream s;
        auto bb = nextinst->getTrueBranch();
        s << bb->getNo();
        std::string temp;
        s >> temp;
        label += temp;
        auto dst = new MachineOperand(label);
        int op;
        switch (opcode)
        {
        case EQ:
            op = BranchMInstruction::EQ;
            break;
        case NE:
            op = BranchMInstruction::NE;
            break;
        case LE:
            op = BranchMInstruction::LE;
            break;
        case GE:
            op = BranchMInstruction::GE;
            break;
        case LT:
            op = BranchMInstruction::LT;
            break;
        case GT:
            op = BranchMInstruction::GT;
            break;
        default:
            break;
        }
        cur_inst = new BranchMInstruction(cur_block, op, dst);
        cur_block->InsertInst(cur_inst);
    }
    else //nextinst is not br  ==> need to save the result of cmp
    {
        int op;
        switch (opcode)
        {
        case EQ:
            op = MachineInstruction::EQ;
            break;
        case NE:
            op = MachineInstruction::NE;
            break;
        case LE:
            op = MachineInstruction::LE;
            break;
        case GE:
            op = MachineInstruction::GE;
            break;
        case LT:
            op = MachineInstruction::LT;
            break;
        case GT:
            op = MachineInstruction::GT;
            break;
        default:
            break;
        }

        auto dst = genMachineOperand(operands[0]);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, new MachineOperand(MachineOperand::IMM, 1), op);
        cur_block->InsertInst(cur_inst);
        switch (op)
        {
        case MachineInstruction::EQ:
            op = MachineInstruction::NE;
            break;
        case MachineInstruction::NE:
            op = MachineInstruction::EQ;
            break;
        case MachineInstruction::GE:
            op = MachineInstruction::LT;
            break;
        case MachineInstruction::LE:
            op = MachineInstruction::GT;
            break;
        case MachineInstruction::GT:
            op = MachineInstruction::LE;
            break;
        case MachineInstruction::LT:
            op = MachineInstruction::GE;
            break;
        default:
            break;
        }
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, new MachineOperand(*dst), new MachineOperand(MachineOperand::IMM, 0), op);
        cur_block->InsertInst(cur_inst);
    }
}

void UncondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_bb = builder->getBlock();
    std::string label;
    label = ".L";
    std::stringstream s;
    s << branch->getNo();
    std::string temp;
    s >> temp;
    label += temp;
    auto dst = new MachineOperand(label);
    cur_bb->InsertInst(new BranchMInstruction(cur_bb, BranchMInstruction::B, dst));
}

void CondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_bb = builder->getBlock();
    std::string label;
    label = ".L";
    std::stringstream s;
    s << false_branch->getNo();
    std::string temp;
    s >> temp;
    label += temp;
    auto dst = new MachineOperand(label);
    cur_bb->InsertInst(new BranchMInstruction(cur_bb, BranchMInstruction::B, dst));
}

void RetInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    /* HINT:
    * 1. Generate mov instruction to save return value in r0
    * 2. Restore callee saved registers and sp, fp
    * 3. Generate bx instruction */
    auto bb = builder->getBlock();
    auto func = builder->getFunction();
    auto dst = genMachineReg(0);
    auto src = genMachineOperand(operands[0]);
    //1. Generate mov instruction to save return value in r0
    bb->InsertInst(new MovMInstruction(bb, MovMInstruction::MOV, dst, src));
    //2. Restore callee saved registers and sp, fp
    bb->InsertInst(new BinaryMInstruction(bb, BinaryMInstruction::SUB, genMachineReg(13), genMachineReg(11), genMachineImm(0)));
    //3.Generate bx instruction
    if (func->Isleaf())
    {
        bb->InsertInst(new StackMInstrcuton(bb, StackMInstrcuton::POP, genMachineReg(11)));
        bb->InsertInst(new BranchMInstruction(bb, BranchMInstruction::BX, genMachineReg(14)));
    }
    else
    {
        bb->InsertInst(new StackMInstrcuton(bb, StackMInstrcuton::POP, genMachineReg(11)));
        bb->InsertInst(new StackMInstrcuton(bb, StackMInstrcuton::POP, genMachineReg(15)));
    }
}

void CallInstruction::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = 0;
    
    //pass params
    for (int i = 1; i < operands.size(); i++)
    {
        if (i - 1 > 3)
        {
            auto reg1 = genMachineVReg();
            cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, reg1, genMachineOperand(operands[i]));
            cur_block->InsertInst(cur_inst);
            cur_inst = new StoreMInstruction(cur_block, reg1, genMachineReg(13), genMachineImm((i - 5) * 4));
            cur_block->InsertInst(cur_inst);
            builder->getFunction()->AllocSpace(4);
            continue;
        }
        auto reg1 = genMachineVReg();
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, genMachineReg(i - 1), genMachineOperand(operands[i]));
        cur_block->InsertInst(cur_inst);
    }

    //example:  bl func
    std::string label = dynamic_cast<IdentifierSymbolEntry*>(this->getEntry())->getName();
    auto dst = new MachineOperand(label, 1);
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::BL, dst);
    cur_block->InsertInst(cur_inst);

    //save the return value
    if (dynamic_cast<FunctionType*>(this->getEntry()->getType())->getRetType()->toStr() != "void")
    {
        auto ret = genMachineOperand(operands[0]);
        auto r0 = genMachineReg(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, ret, r0);
        cur_block->InsertInst(cur_inst);
    }
}

void ExtInstruction::genMachineCode(AsmBuilder* builder) {
    MachineInstruction *cur_inst;
    MachineBlock *cur_block = builder->getBlock();
    auto src = genMachineOperand(operands[1]);
    cur_inst = new UxtbMInstruction(cur_block, src);
    cur_block->InsertInst(cur_inst);
    auto dst = genMachineOperand(operands[0]);
    cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, new MachineOperand(*src));
    cur_block->InsertInst(cur_inst);
}
