#include "XTracer.h"
#include <iostream>
#include <sstream>

using namespace std;

KNOB<string> KnobOutputFolder(KNOB_MODE_WRITEONCE, "pintool",
	"o", "xtrace_output", "specify output folder");

VOID Init(VOID* v)
{
	LOG("Starting application...");

	TracerContext* ctx = new TracerContext();
	PIN_SetThreadData(ctx_key, ctx, 0);

	INS_AddInstrumentFunction(InstrumentInstruction, 0);
}

VOID Fini(INT32 code, VOID *v)
{
	LOG("Application terminated.");
}



INT32 Usage()
{
	PIN_ERROR("This Pintool trace all the memory operations directed to memory regions that are at least one time executables during the lifetime of the process\n"
		+ KNOB_BASE::StringKnobSummary() + "\n");
	return -1;
}

int main(int argc, char * argv[])
{
	PIN_InitSymbols();

	if (PIN_Init(argc, argv))
		return Usage();
	
	InitLog("");
	ctx_key = PIN_CreateThreadDataKey(0);

	OS_MkDir(KnobOutputFolder.Value().c_str(), OS_FILE_ATTRIBUTES_DIRECTORY);

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