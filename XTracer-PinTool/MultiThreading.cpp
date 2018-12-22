#include "XTracer.h"
#include <fstream>

TLS_KEY ctx_key;

VOID ThreadInit(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID *v)
{
	LOG("Starting thread " << threadid << "...");
	if (threadid)
	{
		TracerContext* ctx = new TracerContext();
		PIN_SetThreadData(ctx_key, ctx, threadid);
	}
}


VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 code, VOID *v)
{
	LOG("Ended thread " << threadid << "...");
	TracerContext* ctx = (TracerContext*)PIN_GetThreadData(ctx_key, threadid);

	LOG("Saving dump (collected " << dec << ctx->memSize() << " operations)...");
	stringstream ss;
	ss << KnobOutputFolder.Value() << "/xtrace_" << threadid << ".json";
	ofstream fs(ss.str().c_str());
	ctx->dump(fs);
	fs.close();
	LOG("Dump saved to " << ss.str());
}
