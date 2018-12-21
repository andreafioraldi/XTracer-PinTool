#include "XTracer.h"
#include <iostream>
#include <fstream>

using namespace std;

VOID Fini(INT32 code, VOID *v)
{
	cerr << "Saving dump...\n";
	ofstream fs("xtrace.out");
	ctx.dump(fs);
	fs.close();
}

INT32 Usage()
{
	PIN_ERROR("This Pintool prints the IPs of every instruction executed\n"
		+ KNOB_BASE::StringKnobSummary() + "\n");
	return -1;
}

int main(int argc, char * argv[])
{
	if (PIN_Init(argc, argv))
		return Usage();

	INS_AddInstrumentFunction(InstrumentInstruction, 0);
	IMG_AddInstrumentFunction(InstrumentImage, 0);

	PIN_AddFiniFunction(Fini, 0);

	PIN_StartProgram();

	return 0;
}