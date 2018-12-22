#include "XTracer.h"
#include <iostream>
#include <fstream>

using namespace std;

VOID Init(VOID* v)
{
	cerr << "Starting application...\n";
	INS_AddInstrumentFunction(InstrumentInstruction, 0);
}

VOID Fini(INT32 code, VOID *v)
{
	cerr << "Saving dump...\n";
	ofstream fs(("xtrace_" + main_executable_name + ".out").c_str());
	ctx.dump(fs);
	fs.close();
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

	PIN_AddInternalExceptionHandler(InternalExceptionHandler, NULL);
	PIN_AddContextChangeFunction(ApplicationExceptionHandler, NULL);

	IMG_AddInstrumentFunction(InstrumentImage, 0);
	
	PIN_AddApplicationStartFunction(Init, 0);
	PIN_AddFiniFunction(Fini, 0);

	PIN_StartProgram();

	return 0;
}