#include <malloc.h>
#include <foundation\PxErrorCallback.h>

namespace physx {

	class PxDefaultErrorCallback : public PxErrorCallback
	{
	public:
		PxDefaultErrorCallback();
		~PxDefaultErrorCallback();

		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
	};


}