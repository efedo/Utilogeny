// Copyright 2017-18 The Curators of the University of Missouri

#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/cpuinfo.h"


// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

// Print out supported instruction set extensions
void printCPUinfo()
{
	auto& outstream = std::cout;

	auto support_message = [&outstream](std::string isa_feature, bool is_supported) {
		outstream << isa_feature << (is_supported ? " supported" : " not supported") << std::endl;
	};

	std::cout << InstructionSet::Vendor() << std::endl;
	std::cout << InstructionSet::Brand() << std::endl;

	support_message("3DNOW", InstructionSet::_3DNOW());
	support_message("3DNOWEXT", InstructionSet::_3DNOWEXT());
	support_message("ABM", InstructionSet::ABM());
	support_message("ADX", InstructionSet::ADX());
	support_message("AES", InstructionSet::AES());
	support_message("AVX", InstructionSet::AVX());
	support_message("AVX2", InstructionSet::AVX2());
	support_message("AVX512CD", InstructionSet::AVX512CD());
	support_message("AVX512ER", InstructionSet::AVX512ER());
	support_message("AVX512F", InstructionSet::AVX512F());
	support_message("AVX512PF", InstructionSet::AVX512PF());
	support_message("BMI1", InstructionSet::BMI1());
	support_message("BMI2", InstructionSet::BMI2());
	support_message("CLFSH", InstructionSet::CLFSH());
	support_message("CMPXCHG16B", InstructionSet::CMPXCHG16B());
	support_message("CX8", InstructionSet::CX8());
	support_message("ERMS", InstructionSet::ERMS());
	support_message("F16C", InstructionSet::F16C());
	support_message("FMA", InstructionSet::FMA());
	support_message("FSGSBASE", InstructionSet::FSGSBASE());
	support_message("FXSR", InstructionSet::FXSR());
	support_message("HLE", InstructionSet::HLE());
	support_message("INVPCID", InstructionSet::INVPCID());
	support_message("LAHF", InstructionSet::LAHF());
	support_message("LZCNT", InstructionSet::LZCNT());
	support_message("MMX", InstructionSet::MMX());
	support_message("MMXEXT", InstructionSet::MMXEXT());
	support_message("MONITOR", InstructionSet::MONITOR());
	support_message("MOVBE", InstructionSet::MOVBE());
	support_message("MSR", InstructionSet::MSR());
	support_message("OSXSAVE", InstructionSet::OSXSAVE());
	support_message("PCLMULQDQ", InstructionSet::PCLMULQDQ());
	support_message("POPCNT", InstructionSet::POPCNT());
	support_message("PREFETCHWT1", InstructionSet::PREFETCHWT1());
	support_message("RDRAND", InstructionSet::RDRAND());
	support_message("RDSEED", InstructionSet::RDSEED());
	support_message("RDTSCP", InstructionSet::RDTSCP());
	support_message("RTM", InstructionSet::RTM());
	support_message("SEP", InstructionSet::SEP());
	support_message("SHA", InstructionSet::SHA());
	support_message("SSE", InstructionSet::SSE());
	support_message("SSE2", InstructionSet::SSE2());
	support_message("SSE3", InstructionSet::SSE3());
	support_message("SSE4.1", InstructionSet::SSE41());
	support_message("SSE4.2", InstructionSet::SSE42());
	support_message("SSE4a", InstructionSet::SSE4a());
	support_message("SSSE3", InstructionSet::SSSE3());
	support_message("SYSCALL", InstructionSet::SYSCALL());
	support_message("TBM", InstructionSet::TBM());
	support_message("XOP", InstructionSet::XOP());
	support_message("XSAVE", InstructionSet::XSAVE());
}