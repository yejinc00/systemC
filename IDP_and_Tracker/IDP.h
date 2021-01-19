#pragma once
#include <systemc.h>

SC_MODULE(idp) {
	///// IDP input port /////
	sc_in<bool> clk;

	///// IDP output port /////


	///// IDP internal function /////
	void idp_main();					// IDP controller function
	void idp_decode();					// sparse matrix decode function
	
	SC_CTOR(idp) {
		SC_CTHREAD(idp_main, clk.pos());
	}
};

void idp::idp_decode() {

}

void idp::idp_main() {

}