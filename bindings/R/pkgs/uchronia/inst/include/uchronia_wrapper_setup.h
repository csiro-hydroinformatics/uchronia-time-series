#pragma once

#include "uchronia_r_exports.h"

class OpaquePointer
{
public:
	OpaquePointer(void* p) : ptr(p){}
	~OpaquePointer()
	{
		if (ptr != nullptr)
			DisposeSharedPointer(ptr);
	}

	void* Get() { return ptr; }

private:
	void* ptr;
};

