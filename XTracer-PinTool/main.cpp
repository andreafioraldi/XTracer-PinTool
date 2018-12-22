#include "XTracer.h"
#include <iostream>
#include <sstream>

using namespace std;


VOID Init(VOID* v)
{
	LOG("Starting application...");
	INS_AddInstrumentFunction(InstrumentInstruction, 0);
}

VOID Fini(INT32 code, VOID *v)
{
	/*cerr << "Saving dump...\n";
	string filename = "xtrace_" + main_executable_name + ".out";
	ofstream fs(filename.c_str());
	ctx.dump(fs);
	fs.close();*/
}



INT32 Usage()
{
	PIN_ERROR("This Pintool trace all the memory operations directed to memory regions that are at least one time executables during the lifetime of the process\n"
		+ KNOB_BASE::StringKnobSummary() + "\n");
	return -1;
}

int main(int argc, char * argv[])
{
	if (PIN_Init(argc, argv))
		return Usage();

	InitLog("");
	ctx_key = PIN_CreateThreadDataKey(0);

	PIN_AddInternalExceptionHandler(InternalExceptionHandler, NULL);
	PIN_AddContextChangeFunction(ApplicationExceptionHandler, NULL);

	IMG_AddInstrumentFunction(InstrumentImage, 0);
	
	PIN_AddApplicationStartFunction(Init, 0);
	PIN_AddFiniFunction(Fini, 0);

	PIN_AddThreadStartFunction(ThreadInit, 0);
	PIN_AddThreadFiniFunction(ThreadFini, 0);

	PIN_StartProgram();

	return 0;
}