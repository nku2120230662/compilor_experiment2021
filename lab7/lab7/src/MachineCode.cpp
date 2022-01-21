#include "MachineCode.h"
extern FILE* yyout;

MachineOperand::MachineOperand(int tp, int val)
{
    this->type = tp;
    if(tp == MachineOperand::IMM)
        this->val = val;
    else 
        this->reg_no = val;
}

MachineOperand::MachineOperand(std::string label)
{
    this->type = MachineOperand::LABEL;
    this->label = label;
}

MachineOperand::MachineOperand(std::string label,bool isfunc)
{
    this->type = MachineOperand::LABEL;
    this->label = label;
    this->isfunc=isfunc;
}

bool MachineOperand::operator==(const MachineOperand&a) const
{
    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

bool MachineOperand::operator<(const MachineOperand&a) const
{
    if(this->type == a.type)
    {
        if(this->type == IMM)
            return this->val < a.val;
        return this->reg_no < a.reg_no;
    }
    return this->type < a.type;

    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

void MachineOperand::PrintReg()
{
    switch (reg_no)
    {
    case 11:
        fprintf(yyout, "fp");
        break;
    case 13:
        fprintf(yyout, "sp");
        break;
    case 14:
        fprintf(yyout, "lr");
        break;
    case 15:
        fprintf(yyout, "pc");
        break;
    default:
        fprintf(yyout, "r%d", reg_no);
        break;
    }
}

void MachineOperand::output() 
{
    /* HINT：print operand
    * Example:
    * immediate num 1 -> print #1;
    * register 1 -> print r1;
    * lable addr_a -> print addr_a; */
    switch (this->type)
    {
    case IMM:
        fprintf(yyout, "#%d", this->val);
        break;
    case VREG:
        fprintf(yyout, "v%d", this->reg_no);
        break;
    case REG:
        PrintReg();
        break;
    case LABEL:
        if (this->label.substr(0, 2) == ".L"||isfunc){
            fprintf(yyout, "%s", this->label.c_str());
        }
        else{
            fprintf(yyout, "addr_%s", this->label.c_str());
        }
    default:
        break;
    }
}

void MachineInstruction::PrintCond()
{
    // TODO
    switch (cond)
    {
    case LT:
        fprintf(yyout, "lt ");
        break;
    case EQ:
        fprintf(yyout, "eq ");
        break;
    case NE:
        fprintf(yyout, "ne ");
        break;
    case LE:
        fprintf(yyout, "le ");
        break;
    case GT:
        fprintf(yyout, "gt ");
        break;
    case GE:
        fprintf(yyout, "ge ");
        break;
    default:
        fprintf(yyout, " ");
        break;
    }
}

BinaryMInstruction::BinaryMInstruction(
    MachineBlock* p, int op, 
    MachineOperand* dst, MachineOperand* src1, MachineOperand* src2, 
    int cond)
{
    this->parent = p;
    this->type = MachineInstruction::BINARY;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    src2->setParent(this);
}

void BinaryMInstruction::output() 
{
    // TODO: 
    // Complete other instructions
    switch (this->op)
    {
    case BinaryMInstruction::ADD:
        fprintf(yyout, "\tadd ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::SUB:
        fprintf(yyout, "\tsub ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::MUL:
        fprintf(yyout, "\tmul ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::DIV:
        fprintf(yyout, "\tsdiv ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::MOD:
        fprintf(yyout, "\tsrem ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::AND:
        fprintf(yyout, "\tand ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::OR:
        fprintf(yyout, "\torr ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::XOR:
        fprintf(yyout, "\teor ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    default:
        break;
    }
}

LoadMInstruction::LoadMInstruction(MachineBlock* p,
    MachineOperand* dst, MachineOperand* src1, MachineOperand* src2,
    int cond)
{
    this->parent = p;
    this->type = MachineInstruction::LOAD;
    this->op = -1;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    if (src2)
        this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    if (src2)
        src2->setParent(this);
}

void LoadMInstruction::output()
{
    fprintf(yyout, "\tldr ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");

    // Load immediate num, eg: ldr r1, =8
    if(this->use_list[0]->isImm())
    {
        fprintf(yyout, "=%d\n", this->use_list[0]->getVal());
        return;
    }

    // Load address
    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "[");

    this->use_list[0]->output();
    if( this->use_list.size() > 1 )
    {
        fprintf(yyout, ", ");
        this->use_list[1]->output();
    }

    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "]");
    fprintf(yyout, "\n");
}

StoreMInstruction::StoreMInstruction(MachineBlock* p,
    MachineOperand* src1, MachineOperand* src2, MachineOperand* src3, 
    int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::STORE;
    this->op = -1;
    this->cond = cond;
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    if (src3)
        this->use_list.push_back(src3);
    src1->setParent(this);
    src2->setParent(this);
    if (src3)
        src3->setParent(this);
}

void StoreMInstruction::output()
{
    // TODO
    // eg1. str r0 [r1, #4]
    // eg2. str r0 [r1]
    fprintf(yyout, "\tstr ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");

    if (this->use_list[1]->isReg() || this->use_list[1]->isVReg())
        fprintf(yyout, "[");
    this->use_list[1]->output();

    // if use_list[2] is existed
    if (this->use_list.size() > 2)
    {
        fprintf(yyout, ", ");
        this->use_list[2]->output();
    }

    if (this->use_list[1]->isReg() || this->use_list[1]->isVReg())
        fprintf(yyout, "]");

    fprintf(yyout, "\n");
}

MovMInstruction::MovMInstruction(MachineBlock* p, int op, 
    MachineOperand* dst, MachineOperand* src,
    int cond)
{
    // TODO
    this->parent = p;
    this->op = op;
    this->type = MachineInstruction::MOV;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src);
    dst->setParent(this);
    src->setParent(this);
}

void MovMInstruction::output() 
{
    // TODO
    switch (this->op)
    {
    case MovMInstruction::MOV:
        fprintf(yyout, "\tmov");
        break;
    case MovMInstruction::MVN:
        fprintf(yyout, "\tmvn");
        break;
    case MovMInstruction::MOVS:
        fprintf(yyout, "\tmovs");
    }
    this->PrintCond();
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, "\n");
}

BranchMInstruction::BranchMInstruction(MachineBlock* p, int op, 
    MachineOperand* dst, 
    int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::BRANCH;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    dst->setParent(this);
}

void BranchMInstruction::output()
{
    // TODO
    switch (this->op)
    {
    case BranchMInstruction::EQ:
        fprintf(yyout, "\tbeq ");
        break;
    case BranchMInstruction::NE:
        fprintf(yyout, "\tbne ");
        break;
    case BranchMInstruction::LT:
        fprintf(yyout, "\tblt ");
        break;
    case BranchMInstruction::LE:
        fprintf(yyout, "\tble ");
        break;
    case BranchMInstruction::GT:
        fprintf(yyout, "\tbgt ");
        break;
    case BranchMInstruction::GE:
        fprintf(yyout, "\tbge ");
        break;
    case BranchMInstruction::B:
        fprintf(yyout, "\tb ");
        break;
    case BranchMInstruction::BL:
        fprintf(yyout, "\tbl ");
        break;
    case BranchMInstruction::BX:
        fprintf(yyout, "\tbx ");
        break;
    default:
        break;
    }
    this->def_list[0]->output();
    fprintf(yyout, "\n");
}

CmpMInstruction::CmpMInstruction(MachineBlock* p, 
    MachineOperand* src1, MachineOperand* src2, 
    int cond)
{
    // TODO
    this->parent = p;
    this->cond = cond;
    this->type = MachineInstruction::CMP;
    this->cond=cond;
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    src1->setParent(this);
    src2->setParent(this);
}

void CmpMInstruction::output()
{
    // TODO
    // Jsut for reg alloca test
    // delete it after test
    fprintf(yyout, "\tcmp ");
    use_list[0]->output();
    fprintf(yyout, ", ");
    use_list[1]->output();
    fprintf(yyout, "\n");
}

StackMInstrcuton::StackMInstrcuton(MachineBlock* p, int op, 
    MachineOperand* src,
    int cond)
{
    // TODO
    this->parent = p;
    this->op = op;
    this->type = MachineInstruction::STACK;
    this->cond = cond;
    this->use_list.push_back(src);
    src->setParent(this);
}

void StackMInstrcuton::output()
{
    // TODO
    switch (this->op)
    {
    case StackMInstrcuton::POP:
        fprintf(yyout, "\tpop ");
        break;
    case StackMInstrcuton::PUSH:
        fprintf(yyout, "\tpush ");
        break;
    default:
        break;
    }
    fprintf(yyout,"{");
    this->use_list[0]->output();
    fprintf(yyout,"}");
    fprintf(yyout, "\n");
}

ItMInstruction:: ItMInstruction(MachineBlock* p, int op)
{
    this->parent=p;
    this->op=op;
}

void ItMInstruction::output()
{
    fprintf(yyout,"\tit");
    switch (op)
    {
    case EQ:
        fprintf(yyout,"\teq");
        break;
    case NE:
        fprintf(yyout,"\tne");
        break;
    case LT:
        fprintf(yyout,"\tlt");
        break;
    case GE:
        fprintf(yyout,"\tge");
        break;
    case GT:
        fprintf(yyout,"\tgt");
        break;
    case LE:
        fprintf(yyout,"\tle");
        break;
    case L:
        fprintf(yyout,"\tl");
        break;
    case G:
        fprintf(yyout,"\tg");
        break;
    default:
        break;
    }
    fprintf(yyout, "\n");
}

UxtbMInstruction::UxtbMInstruction(MachineBlock* p,MachineOperand* src)
{
    this->parent=p;
    this->use_list.push_back(src);
    src->setParent(this);
}

void UxtbMInstruction::output()
{
    fprintf(yyout,"\tuxtb ");
    use_list[0]->output();
    fprintf(yyout," ,");
    use_list[0]->output();
    fprintf(yyout,"\n");
}

GlobalMInstruction::GlobalMInstruction(MachineBlock* p,MachineOperand*dst,MachineOperand*src)
{
    this->parent=p;
    def_list.push_back(dst);
    use_list.push_back(src);
    dst->setParent(this);
    src->setParent(this);
}

void GlobalMInstruction::frontoutput()
{
    fprintf(yyout,"\t.global %s\n",def_list[0]->getLabel().c_str());
    fprintf(yyout,"\t.align 4\n");
    fprintf(yyout,"\t.size %s, 4\n",def_list[0]->getLabel().c_str());
    fprintf(yyout,"%s:\n",def_list[0]->getLabel().c_str());
    fprintf(yyout,"\t.word %d\n",use_list[0]->getVal());
}

void GlobalMInstruction::output()
{
    fprintf(yyout,"addr_%s:\n",def_list[0]->getLabel().c_str());
    fprintf(yyout,"\t.word %s\n",def_list[0]->getLabel().c_str());
}

MachineFunction::MachineFunction(MachineUnit* p, SymbolEntry* sym_ptr) 
{ 
    this->parent = p; 
    this->sym_ptr = sym_ptr; 
    this->stack_size = 0;
};

void MachineBlock::output()
{
    fprintf(yyout, ".L%d:\n", this->no);
    for(auto iter : inst_list)
        iter->output();
}

void MachineFunction::output()
{
    const char *func_name = this->sym_ptr->toStr().c_str() + 1;
    fprintf(yyout, "\t.global %s\n", func_name);
    fprintf(yyout, "\t.type %s , %%function\n", func_name);
    fprintf(yyout, "%s:\n", func_name);
    // TODO
    /* Hint:
    *  1. Save fp
    *  2. fp = sp
    *  3. Save callee saved register
    *  4. Allocate stack space for local variable */
   //prologue
    // 1. Save fp
    fprintf(yyout, "\tpush {");
    // scan all the registers while wll be used
    if (Isleaf())
    {
        fprintf(yyout, "fp}\n");
    }
    else
    {
        fprintf(yyout, "fp, lr}\n");
    }

    // 2. fp = sp
    fprintf(yyout, "\tmov fp, sp\n");

    // 4. Allocate space
    fprintf(yyout, "\tsub sp, sp, #%d\n", this->stack_size);

    // Traverse all the block in block_list to print assembly code.
    for (auto iter : block_list)
        iter->output();

    if(Isleaf())
    {
        fprintf(yyout,"\n");
    }
    else
    {
        fprintf(yyout,"\n");
    }
}

void MachineUnit::PrintGlobalDecl()
{
    // TODO:
    // You need to print global variable/const declarition code;
    for(auto inst:global_list)
    {
        inst->frontoutput();
    }
}

void MachineUnit::printGlobalBridges()
{
    for(auto inst:global_list)
    {
        inst->output();
    }
}

void MachineUnit::output()
{
    // TODO
    /* Hint:
    * 1. You need to print global variable/const declarition code;
    * 2. Traverse all the function in func_list to print assembly code;
    * 3. Don't forget print bridge label at the end of assembly code!! */
    fprintf(yyout, "\t.arch armv8-a\n");
    fprintf(yyout, "\t.arch_extension crc\n");
    fprintf(yyout, "\t.arm\n");
    fprintf(yyout,"\t.data\n");
    PrintGlobalDecl();
    fprintf(yyout,"\t.text\n");
    for (auto iter : func_list)
        iter->output();
    printGlobalBridges();
}
