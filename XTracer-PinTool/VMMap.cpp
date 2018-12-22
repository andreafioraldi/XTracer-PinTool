#include "XTracer.h"

namespace WINH {
#include "WinHeaderStub.h"
}

using namespace std;

vector< pair<ANYADDR, ANYADDR> > GetExecutablePages()
{
	WINH::LPBYTE base = NULL;
	WINH::MEMORY_BASIC_INFORMATION mbi;
	
	vector< pair<ANYADDR, ANYADDR> > xranges;

	while (WINH::VirtualQuery(base, &mbi, sizeof(WINH::MEMORY_BASIC_INFORMATION)) > 0)
	{
		switch (mbi.Protect)
		{
		case PAGE_EXECUTE:
		case PAGE_EXECUTE_READ:
		case PAGE_EXECUTE_READWRITE:
		case PAGE_EXECUTE_WRITECOPY:
			xranges.push_back(pair<ANYADDR, ANYADDR>((ANYADDR)mbi.BaseAddress, (ANYADDR)mbi.BaseAddress + mbi.RegionSize));
		/*	LOG((size_t)mbi.BaseAddress << " " << (size_t)mbi.BaseAddress + mbi.RegionSize << "   EXEC");
			break;
		default:
			LOG((size_t)mbi.BaseAddress << " " << (size_t)mbi.BaseAddress + mbi.RegionSize << "   NOT EXEC");*/
		}

		base += mbi.RegionSize;
	}

	LOG(xranges.size() << " executable pages");

	return xranges;
}