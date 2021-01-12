#pragma once
#include <systemc.h>

SC_MODULE(sram) {
	///// sram input port /////
	sc_in<bool> clk;
	sc_in<sc_int<8>> addr;
	sc_in<sc_int<1>> addr_valid;
	sc_in<sc_int<16>> Din;
	sc_in<sc_int<1>> Din_valid;
	sc_in<sc_int<1>> Cen;
	sc_in<sc_int<1>> Cen_valid;
	sc_in<sc_int<1>> Wen;
	sc_in<sc_int<1>> Wen_valid;
	sc_in<sc_int<1>> Dout_ready;
	
	///// sram output port /////
	sc_out<sc_int<16>> Dout;
	sc_out<sc_int<1>> Dout_valid;
	sc_out<sc_int<1>> addr_ready;
	sc_out<sc_int<1>> Din_ready;
	sc_out<sc_int<1>> Cen_ready;
	sc_out<sc_int<1>> Wen_ready;

	void sram_main();

	SC_CTOR(sram) {
		SC_CTHREAD(sram_main, clk.pos())
	}
};

void sram::sram_main(void) {
	///// initialize handshake signal /////
	Dout_valid.write(0);


	while (1) {

	}

}