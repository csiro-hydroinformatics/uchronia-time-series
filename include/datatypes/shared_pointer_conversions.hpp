#pragma once

#include "datatypes/setup_exports.h"
#include "moirai/reference_handle.hpp"

namespace moirai
{
	//template<> struct known_conversions<Parameterizer>
	//{
	//	static Parameterizer* dyn_cast(void* p, const typeinfo& tinfo)
	//	{
	//		HyperCubeParameterizer* result = known_conversions<HyperCubeParameterizer>::dyn_cast(p, tinfo);
	//		if (result != nullptr)
	//			return static_cast<Parameterizer*>(p);
	//		return nullptr;
	//	}
	//};
}