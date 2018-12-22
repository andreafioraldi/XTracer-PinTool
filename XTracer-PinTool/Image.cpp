#include "XTracer.h"
#include <utility>

using namespace std;

//vector< pair<ANYADDR, ANYADDR> > xsecs;
//string main_executable_name = "__unknown_name__";

VOID InstrumentImage(IMG img, VOID* v)
{
	if (!IMG_Valid(img))
		return;

	if (IMG_IsMainExecutable(img))
	{
		LOG("Loaded main image " << IMG_Name(img));
		//main_executable_name = IMG_Name(img);
	}
	else
		LOG("Loaded image " << IMG_Name(img));

	/*
	for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
	{
		LOG(SEC_Name(sec) << "  " << SEC_IsReadable(sec) << " " << SEC_IsWriteable(sec) << " " << SEC_IsExecutable(sec));
		if (SEC_IsExecutable(sec))
		{
			ANYADDR s = SEC_Address(sec);

			LOG(hex << "Spotted executable section " << SEC_Name(sec) << "[" << s << ", " << (s + SEC_Size(sec)) << "]");

			xsecs.push_back(pair<ANYADDR, ANYADDR>(s, s + SEC_Size(sec)));
		}
	}*/
}