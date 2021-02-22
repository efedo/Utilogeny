// Copyright 2017-20 Eric Fedosejevs

#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/cpuinfo.h"
#include "Utilogeny/source/core/console/termcolor.h"

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

void printCPUinstruction(std::string feature, bool is_supported) {
	std::cout << std::left << std::setw(20) << std::setfill('.') << feature;
	if (is_supported) {
		std::cout << termcolor::bright_green << " supported\n";
	}
	else {
		std::cout << termcolor::bright_red << " not supported\n";
	}
	std::cout << termcolor::reset;
}

// Print out supported instruction set extensions
void printCPUinfo()
{
	std::cout << "CPU vendor: " << InstructionSet::Vendor() << std::endl;
	std::cout << "CPU model: " << InstructionSet::Brand() << std::endl;

	printCPUinstruction("3DNOW", InstructionSet::_3DNOW());
	printCPUinstruction("3DNOWEXT", InstructionSet::_3DNOWEXT());
	printCPUinstruction("ABM", InstructionSet::ABM());
	printCPUinstruction("ADX", InstructionSet::ADX());
	printCPUinstruction("AES", InstructionSet::AES());
	printCPUinstruction("AVX", InstructionSet::AVX());
	printCPUinstruction("AVX2", InstructionSet::AVX2());
	printCPUinstruction("AVX512CD", InstructionSet::AVX512CD());
	printCPUinstruction("AVX512ER", InstructionSet::AVX512ER());
	printCPUinstruction("AVX512F", InstructionSet::AVX512F());
	printCPUinstruction("AVX512PF", InstructionSet::AVX512PF());
	printCPUinstruction("BMI1", InstructionSet::BMI1());
	printCPUinstruction("BMI2", InstructionSet::BMI2());
	printCPUinstruction("CLFSH", InstructionSet::CLFSH());
	printCPUinstruction("CMPXCHG16B", InstructionSet::CMPXCHG16B());
	printCPUinstruction("CX8", InstructionSet::CX8());
	printCPUinstruction("ERMS", InstructionSet::ERMS());
	printCPUinstruction("F16C", InstructionSet::F16C());
	printCPUinstruction("FMA", InstructionSet::FMA());
	printCPUinstruction("FSGSBASE", InstructionSet::FSGSBASE());
	printCPUinstruction("FXSR", InstructionSet::FXSR());
	printCPUinstruction("HLE", InstructionSet::HLE());
	printCPUinstruction("INVPCID", InstructionSet::INVPCID());
	printCPUinstruction("LAHF", InstructionSet::LAHF());
	printCPUinstruction("LZCNT", InstructionSet::LZCNT());
	printCPUinstruction("MMX", InstructionSet::MMX());
	printCPUinstruction("MMXEXT", InstructionSet::MMXEXT());
	printCPUinstruction("MONITOR", InstructionSet::MONITOR());
	printCPUinstruction("MOVBE", InstructionSet::MOVBE());
	printCPUinstruction("MSR", InstructionSet::MSR());
	printCPUinstruction("OSXSAVE", InstructionSet::OSXSAVE());
	printCPUinstruction("PCLMULQDQ", InstructionSet::PCLMULQDQ());
	printCPUinstruction("POPCNT", InstructionSet::POPCNT());
	printCPUinstruction("PREFETCHWT1", InstructionSet::PREFETCHWT1());
	printCPUinstruction("RDRAND", InstructionSet::RDRAND());
	printCPUinstruction("RDSEED", InstructionSet::RDSEED());
	printCPUinstruction("RDTSCP", InstructionSet::RDTSCP());
	printCPUinstruction("RTM", InstructionSet::RTM());
	printCPUinstruction("SEP", InstructionSet::SEP());
	printCPUinstruction("SHA", InstructionSet::SHA());
	printCPUinstruction("SSE", InstructionSet::SSE());
	printCPUinstruction("SSE2", InstructionSet::SSE2());
	printCPUinstruction("SSE3", InstructionSet::SSE3());
	printCPUinstruction("SSE4.1", InstructionSet::SSE41());
	printCPUinstruction("SSE4.2", InstructionSet::SSE42());
	printCPUinstruction("SSE4a", InstructionSet::SSE4a());
	printCPUinstruction("SSSE3", InstructionSet::SSSE3());
	printCPUinstruction("SYSCALL", InstructionSet::SYSCALL());
	printCPUinstruction("TBM", InstructionSet::TBM());
	printCPUinstruction("XOP", InstructionSet::XOP());
	printCPUinstruction("XSAVE", InstructionSet::XSAVE());
}