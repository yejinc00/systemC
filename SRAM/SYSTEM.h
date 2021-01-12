#pragma once
#include <systemc.h>
#include "memory.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	///// pointer of module tb and sram /////


	///// signal for connect module tb and adder /////



	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS){

	}
};