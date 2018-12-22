#include "XTracer.h"
#include <utility>

using namespace std;

vector< pair<ANYADDR, ANYADDR> > xsecs;
string main_executable_name = "__unknown_name__";

VOID InstrumentImage(IMG img, VOID* v)
{
	if (!IMG_Valid(img))
		return;

	if (IMG_IsMainExecutable(img))
	{
		cerr << "Loaded main image " << IMG_Name(img) << endl;
		main_executable_name = IMG_Name(img);
	}
	else
		cerr << "Loaded image " << IMG_Name(img) << endl;

	for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
	{
		if (SEC_IsExecutable(sec))
		{
			ANYADDR s = SEC_Address(sec);
			cerr << hex << "Spotted executable section " << s << " - " << (s + SEC_Size(sec)) << endl;
			xsecs.push_back(pair<ANYADDR, ANYADDR>(s, s + SEC_Size(sec)));
		}
	}
}