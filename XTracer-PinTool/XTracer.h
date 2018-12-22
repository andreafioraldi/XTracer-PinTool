#pragma once

#include "pin.H"

#include <set>
#include <map>
#include <ostream>
#include <iostream>
#include <hash_map>

extern KNOB<string> KnobOutputFolder;

class TracedUnit
{
	std::set<ANYADDR> dependencies;
	int ref_count = 1;

public:
	TracedUnit() {}
	TracedUnit(ANYADDR addr);

	TracedUnit* merge(TracedUnit* tup);

	std::set<ANYADDR>& getDepends();

	void dump(ostream& os);

	void addRef();
	void delRef();
};

class TracerContext
{
	std::map<REG, TracedUnit*> regs;
	std::hash_map<ANYADDR, TracedUnit*> mem;
	
public:
	void setReg(REG reg, TracedUnit* tup);
	void setMem(ANYADDR addr, TracedUnit* tup);
	void removeMem(ANYADDR addr);

	TracedUnit* getReg(REG reg);
	TracedUnit* getMem(ANYADDR addr);

	size_t memSize();
	size_t regsSize();

	void dump(ostream& os);
};

VOID InstrumentInstruction(INS ins, VOID *v);
VOID InstrumentImage(IMG img, VOID* ptr);
VOID Fini(INT32 code, VOID *v);

EXCEPT_HANDLING_RESULT InternalExceptionHandler(THREADID threadIndex, EXCEPTION_INFO * pExceptInfo, PHYSICAL_CONTEXT * pPhysCtxt, VOID *v);
VOID ApplicationExceptionHandler(THREADID threadIndex, CONTEXT_CHANGE_REASON reason, const CONTEXT *from, CONTEXT *to, INT32 info, VOID *v);

vector< pair<ANYADDR, ANYADDR> > GetExecutablePages();

extern TLS_KEY ctx_key;

VOID ThreadInit(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID *v);
VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 code, VOID *v);

//
// LOGGING
//

extern PIN_LOCK log_lock;
extern ostream* log_stream;

void InitLog(string filename);
void FlushLog();

#define LOG(m) do { \
	PIN_GetLock(&log_lock, PIN_ThreadId()+1); \
	(*log_stream) << m << endl; \
	PIN_ReleaseLock(&log_lock); \
} while(0)