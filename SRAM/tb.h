#pragma once
#include <systemc.h>

SC_MODULE(tb) {
	///// testbench input port /////
	sc_in<bool> clk;
	sc_in<sc_int<16>> Dout;
	sc_in<sc_int<1>> Dout_valid;
	sc_in<sc_int<1>> addr_ready;
	sc_in<sc_int<1>> Din_ready;
	sc_in<sc_int<1>> Cen_ready;
	sc_in<sc_int<1>> Wen_ready;

	///// testbench output port /////
	sc_out<sc_int<8>> addr;
	sc_out<sc_int<1>> addr_valid;
	sc_out<sc_int<16>> Din;
	sc_out<sc_int<1>> Din_valid;
	sc_out<sc_int<1>> Cen;
	sc_out<sc_int<1>> Cen_valid;
	sc_out<sc_int<1>> Wen;
	sc_out<sc_int<1>> Wen_valid;
	sc_out<sc_int<1>> Dout_ready;

	void tb_main();

	SC_CTOR(tb) {
		SC_CTHREAD(tb_main, clk.pos());
	}
};

void tb::tb_main() {

}