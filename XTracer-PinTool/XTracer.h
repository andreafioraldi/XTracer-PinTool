#pragma once

#include "pin.H"
#include <set>
#include <map>
#include <ostream>

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
	std::map<ANYADDR, TracedUnit*> mem;

public:
	void setReg(REG reg, TracedUnit* tup);
	void setMem(ANYADDR addr, TracedUnit* tup);
	void removeMem(ANYADDR addr);

	TracedUnit* getReg(REG reg);
	TracedUnit* getMem(ANYADDR addr);

	void dump(ostream& os);
};

VOID InstrumentInstruction(INS ins, VOID *v);
VOID InstrumentImage(IMG img, VOID* ptr);
VOID Fini(INT32 code, VOID *v);

extern TracerContext ctx;
extern vector< pair<ANYADDR, ANYADDR> > xsecs;
