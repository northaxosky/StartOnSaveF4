#pragma once

#define NOMB
#define NOSERVICE

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"
#include "CLibUtil/simpleINI.hpp"
#pragma warning(pop)

namespace ini = clib_util::ini;
namespace string = clib_util::string;

using namespace std::literals;

namespace stl
{
	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = REL::GetTrampoline();
		trampoline.create(14);
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class F, size_t index, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[index] };
		T::func = vtbl.write_vfunc(T::vtbl_idx, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		write_vfunc<F, 0, T>();
	}
}

#include "Version.h"
