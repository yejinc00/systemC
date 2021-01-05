#pragma once
#include <systemc.h>
#include "memory.h"
#include "tb.h"

SC_MODULE(SYSTEM) {


	
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS){

	}
};