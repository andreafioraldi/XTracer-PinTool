#include "XTracer.h"

using namespace std;

VOID ReadMem(UINT32 opCount, REG reg_r, UINT64 memOp)
{
	if (opCount != 2 || !REG_valid(reg_r))
		return;
	
	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());

	TracedUnit* tup = ctx->getMem(memOp);
	ctx->setReg(reg_r, tup);
}

VOID ReadMergeMem(UINT32 opCount, REG reg_r, UINT64 memOp)
{
	if (opCount != 2 || !REG_valid(reg_r))
		return;
	
	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());

	TracedUnit* tup1 = ctx->getReg(reg_r);
	TracedUnit* tup2 = ctx->getMem(memOp);
	
	if(tup1 == nullptr)
		ctx->setReg(reg_r, tup2);
	else
		ctx->setReg(reg_r, tup2->merge(tup1));
}


VOID WriteMem(UINT32 opCount, REG reg_r, UINT64 memOp)
{
	if (opCount != 2)
		return;
	
	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());

	TracedUnit* tup = ctx->getReg(reg_r);

	if (!REG_valid(reg_r) || tup == nullptr)
	{
		ctx->removeMem(memOp);
		return;
	}

	ctx->setMem(memOp, tup);
}

VOID WriteMergeMem(UINT32 opCount, REG reg_r, UINT64 memOp)
{
	if (opCount != 2)
		return;

	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());
	
	TracedUnit* tup1 = ctx->getReg(reg_r);
	TracedUnit* tup2 = ctx->getMem(memOp);

	if (!REG_valid(reg_r) || tup1 == nullptr)
		return;

	ctx->setMem(memOp, tup2->merge(tup1));
}


VOID SpreadReg(UINT32 opCount, REG reg_r, REG reg_w)
{
	if (opCount != 2)
		return;
	
	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());

	TracedUnit* tup = ctx->getReg(reg_r);
	
	if (!REG_valid(reg_r))
	{
		ctx->setReg(reg_w, nullptr);
		return;
	}

	ctx->setReg(reg_w, tup);
}

VOID ExchangeReg(UINT32 opCount, REG reg_r, REG reg_w)
{
	if (opCount != 2)
		return;
	
	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());

	TracedUnit* tup1 = ctx->getReg(reg_r);
	TracedUnit* tup2 = ctx->getReg(reg_w);
	tup2->addRef();

	ctx->setReg(reg_w, tup1);
	ctx->setReg(reg_r, tup2);
	tup2->delRef();
}

VOID SpreadMergeReg(UINT32 opCount, REG reg_r, REG reg_w)
{
	if (opCount != 2)
		return;

	if (!REG_valid(reg_r))
		return;

	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());

	TracedUnit* tup1 = ctx->getReg(reg_r);
	if (tup1 == nullptr)
		return;

	TracedUnit* tup2 = ctx->getReg(reg_w);

	if(tup2 == nullptr)
		ctx->setReg(reg_w, tup1);
	else
		ctx->setReg(reg_w, tup2->merge(tup1));
}


VOID InstrumentInstruction(INS ins, VOID *v)
{
	if (INS_Opcode(ins) == XED_ICLASS_CMP || INS_Opcode(ins) == XED_ICLASS_TEST)
		return;

	//TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, PIN_ThreadId());
	//cout << hex << INS_Address(ins) << dec << "\t" << INS_Disassemble(ins) << "\t" << ctx->memSize() << endl;
	if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsRead(ins, 0) && INS_OperandIsReg(ins, 0))
	{
		// mem to reg
		if (INS_IsMov(ins))
		{
			INS_InsertCall(
				ins, IPOINT_BEFORE, (AFUNPTR)ReadMem,
				IARG_UINT32, INS_OperandCount(ins),
				IARG_UINT32, INS_OperandReg(ins, 0),
				IARG_MEMORYOP_EA, 0,
				IARG_END);
		}
		else
		{
			INS_InsertCall(
				ins, IPOINT_BEFORE, (AFUNPTR)ReadMergeMem,
				IARG_UINT32, INS_OperandCount(ins),
				IARG_UINT32, INS_OperandReg(ins, 0),
				IARG_MEMORYOP_EA, 0,
				IARG_END);
		}
	}
	else if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsWritten(ins, 0))
	{
		// reg to mem
		if (INS_IsMov(ins))
		{
			INS_InsertCall(
				ins, IPOINT_BEFORE, (AFUNPTR)WriteMem,
				IARG_UINT32, INS_OperandCount(ins),
				IARG_UINT32, INS_OperandReg(ins, 1),
				IARG_MEMORYOP_EA, 0,
				IARG_END);
		}
		else
		{
			INS_InsertCall(
				ins, IPOINT_BEFORE, (AFUNPTR)WriteMergeMem,
				IARG_UINT32, INS_OperandCount(ins),
				IARG_UINT32, INS_OperandReg(ins, 1),
				IARG_MEMORYOP_EA, 0,
				IARG_END);
		}
	}
	else if (INS_OperandCount(ins) > 1 && INS_OperandIsReg(ins, 0))
	{
		// reg to reg
		if (INS_IsMov(ins))
		{
			INS_InsertCall(
				ins, IPOINT_BEFORE, (AFUNPTR)SpreadReg,
				IARG_UINT32, INS_OperandCount(ins),
				IARG_UINT32, INS_RegR(ins, 0),
				IARG_UINT32, INS_RegW(ins, 0),
				IARG_END);
		}
		else if (INS_IsXchg(ins))
		{
			INS_InsertCall(
				ins, IPOINT_BEFORE, (AFUNPTR)ExchangeReg,
				IARG_UINT32, INS_OperandCount(ins),
				IARG_UINT32, INS_RegR(ins, 0),
				IARG_UINT32, INS_RegW(ins, 0),
				IARG_END);
		}
		else
		{
			INS_InsertCall(
				ins, IPOINT_BEFORE, (AFUNPTR)SpreadMergeReg,
				IARG_UINT32, INS_OperandCount(ins),
				IARG_UINT32, INS_RegR(ins, 0),
				IARG_UINT32, INS_RegW(ins, 0),
				IARG_END);
		}
	}
	
}

