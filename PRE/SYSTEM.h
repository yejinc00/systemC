#pragma once
#include<systemc.h>
#include "PRE.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	///// pointer of modules /////
	PRE* PRE0;
	tb* tb0;

	///// signal for connect tb and PRE module /////
	sc_signal<sc_int<16>> sig_DAT_in;
	sc_signal<bool> sig_DAT_in_valid;
	sc_signal<bool> sig_DAT_in_ready;

	///// constructor /////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		///// port binding /////
		PRE0 = new PRE("PRE0");
		PRE0->clk(clk_sig);
		PRE0->DAT_in(sig_DAT_in);
		PRE0->DAT_in_ready(sig_DAT_in_ready);
		PRE0->DAT_in_valid(sig_DAT_in_valid);

		tb0 = new tb("tb0");
		tb0->clk(clk_sig);
		tb0->DAT_in(sig_DAT_in);
		tb0->DAT_in_ready(sig_DAT_in_ready);
		tb0->DAT_in_valid(sig_DAT_in_valid);		
		
	}

	///// destructor /////
	~SYSTEM() {
		delete PRE0;
		delete tb0;
	}
};