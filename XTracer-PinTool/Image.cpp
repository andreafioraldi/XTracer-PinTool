#include "XTracer.h"
#include <utility>

using namespace std;

vector< pair<ANYADDR, ANYADDR> > xsecs;

VOID InstrumentImage(IMG img, VOID* v)
{
	if (!IMG_Valid(img))
		return;

	for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
	{
		if (SEC_IsExecutable(sec))
		{
			ANYADDR s = SEC_Address(sec);
			xsecs.push_back(pair<ANYADDR, ANYADDR>(s, s + SEC_Size(sec)));
		}
	}
}