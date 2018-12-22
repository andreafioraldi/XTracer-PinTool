#include "XTracer.h"

using namespace std;

TracedUnit::TracedUnit(ANYADDR addr)
{
	dependencies.insert(addr);
}

TracedUnit* TracedUnit::merge(TracedUnit* tup)
{
	TracedUnit* r = new TracedUnit(*this);
	r->ref_count = 0;
	r->dependencies.insert(tup->dependencies.begin(), tup->dependencies.end());
	return r;
}

set<ANYADDR>& TracedUnit::getDepends()
{
	return dependencies;
}

void TracedUnit::dump(ostream& os)
{
	set<ANYADDR>::iterator it = dependencies.begin();
	os << "[" << *it;
	for (++it; it != dependencies.end(); ++it)
	{
		os << ", " << *it;
	}
	os << "]";
}

void TracedUnit::addRef()
{
	ref_count += 1;
}

void TracedUnit::delRef()
{
	ref_count -= 1;

	if (ref_count == 0) {
		delete this;
	}
}


void TracerContext::setReg(REG reg, TracedUnit* tup)
{
	if (regs.find(reg) != regs.end() && regs[reg])
		regs[reg]->delRef();

	if (tup)
		tup->addRef();

	regs[reg] = tup;
}

void TracerContext::setMem(ANYADDR addr, TracedUnit* tup)
{
	if (mem.find(addr) != mem.end())
		mem[addr]->delRef();

	tup->addRef();
	mem[addr] = tup;
}

void TracerContext::removeMem(ANYADDR addr)
{
	if (mem.find(addr) != mem.end())
	{
		mem[addr]->delRef();
		mem.erase(addr);
	}
}

TracedUnit* TracerContext::getReg(REG reg)
{
	if (regs.find(reg) == regs.end())
		return nullptr;

	return regs[reg];
}

TracedUnit* TracerContext::getMem(ANYADDR addr)
{
	if (mem.find(addr) == mem.end())
		mem[addr] = new TracedUnit(addr);

	return mem[addr];
}

size_t TracerContext::memSize()
{
	return mem.size();
}

size_t TracerContext::regsSize()
{
	return regs.size();
}


void TracerContext::dump(ostream& os)
{
	vector< pair<ANYADDR, ANYADDR> > xpages = GetExecutablePages();
	bool notfirst = false;

	os << "[";
	for (auto& it : mem)
	{
		ANYADDR addr = it.first;
		
		for (int i = 0; i < xpages.size(); ++i)
		{
			if (addr >= xpages[i].first && addr < xpages[i].second)
			{
				for (set<ANYADDR>::iterator dit = mem[addr]->getDepends().begin(); dit != mem[addr]->getDepends().end(); ++dit)
				{
					if (notfirst)
					{
						os << ",\n";
					}
					else notfirst = true;

					os << "[" << *dit << ", " << addr << "]";
					//LOG("[" << *dit << ", " << addr << "]");
				}
				break;
			}
		}
	}
	os << "]\n";
}