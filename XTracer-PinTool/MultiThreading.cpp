#include "XTracer.h"
#include <fstream>

TLS_KEY ctx_key;

VOID ThreadInit(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID *v)
{
	LOG("Starting thread " << threadid << "...");
	TracerContext* ctx = new TracerContext();
	PIN_SetThreadData(ctx_key, ctx, threadid);
}


VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 code, VOID *v)
{
	LOG("Ended thread " << threadid << "...");
	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, threadid);

	cerr << "Saving dump...\n";
	stringstream ss;
	ss << "xtrace_" + main_executable_name + "_" << threadid << ".out";
	ofstream fs(ss.str().c_str());
	ctx->dump(fs);
	fs.close();
}
