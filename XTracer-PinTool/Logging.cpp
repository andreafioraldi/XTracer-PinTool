#include "XTracer.h"
#include <fstream>

using namespace std;

PIN_LOCK log_lock;
ostream* log_stream;

void InitLog(string filename)
{
	PIN_InitLock(&log_lock);
	if (filename == "")
		log_stream = &cerr;
	else
		log_stream = new ofstream(filename.c_str());
}

void FlushLog()
{
	log_stream->flush();
}
