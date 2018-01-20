#include "PxDefaultErrorCallback.h"

using namespace physx;

void PxDefaultErrorCallback::reportError(PxErrorCode::Enum e, const char* message, const char* file, int line)
{
	const char* errorCode = NULL;

	switch (e)
	{
	case PxErrorCode::eINVALID_PARAMETER:
		errorCode = "invalid parameter";
		break;
	case PxErrorCode::eINVALID_OPERATION:
		errorCode = "invalid operation";
		break;
	case PxErrorCode::eOUT_OF_MEMORY:
		errorCode = "out of memory";
		break;
	case PxErrorCode::eDEBUG_INFO:
		errorCode = "info";
		break;
	case PxErrorCode::eDEBUG_WARNING:
		errorCode = "warning";
		break;
	default:
		errorCode = "unknown error";
		break;
	}

	printf("%s (%d) :", file, line);
	printf("%s", errorCode);
	printf(" : %s\n", message);
}