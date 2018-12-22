#include "XTracer.h"

using namespace std;

EXCEPT_HANDLING_RESULT InternalExceptionHandler(THREADID threadIndex, EXCEPTION_INFO * pExceptInfo, PHYSICAL_CONTEXT * pPhysCtxt, VOID *v)
{
	cerr << "InternalExceptionHandler: Caught exception. " << PIN_ExceptionToString(pExceptInfo) << endl;
	return EHR_UNHANDLED;
}

VOID ApplicationExceptionHandler(THREADID threadIndex, CONTEXT_CHANGE_REASON reason, const CONTEXT *from, CONTEXT *to, INT32 info, VOID *v)
{
	//if (reason != CONTEXT_CHANGE_REASON_EXCEPTION)
	//	return;

	cerr << "ApplicationExceptionHandler: Caught exception. " << info << endl;
	//PIN_ExitApplication(info);
}