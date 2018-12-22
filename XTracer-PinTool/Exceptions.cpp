#include "XTracer.h"

using namespace std;

EXCEPT_HANDLING_RESULT InternalExceptionHandler(THREADID threadIndex, EXCEPTION_INFO * pExceptInfo, PHYSICAL_CONTEXT * pPhysCtxt, VOID *v)
{
	LOG("InternalExceptionHandler: Caught exception. " << PIN_ExceptionToString(pExceptInfo));
	return EHR_UNHANDLED;
}

VOID ApplicationExceptionHandler(THREADID threadIndex, CONTEXT_CHANGE_REASON reason, const CONTEXT *from, CONTEXT *to, INT32 info, VOID *v)
{
	if (reason != CONTEXT_CHANGE_REASON_EXCEPTION)
		return;

	LOG("ApplicationExceptionHandler: Caught exception. " << info);
	PIN_ExitApplication(info);
}