#pragma once

#define WIN32_LEAN_AND_MEAN

#define NOMB
#define NOMINMAX
#define NOSERVICE

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"
#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#include "ClibUtil/simpleINI.hpp"
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = F4SE::log;
namespace ini = clib_util::ini;
namespace string = clib_util::string;

using namespace std::literals;

namespace stl
{
	using namespace F4SE::stl;

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		F4SE::AllocTrampoline(14);

		auto& trampoline = F4SE::GetTrampoline();
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

#define FALLOUT_AE true

#if FALLOUT_AE
#	define RELOCATION_ID(og, ae) REL::ID(ae)
#	define OFFSET(og, ae) ae
#else
#	define RELOCATION_ID(og, ae) REL::ID(og)
#	define OFFSET(og, ae) og
#endif

#include "Version.h"
