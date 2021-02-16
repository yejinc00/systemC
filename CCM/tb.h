#pragma once
#include <systemc.h>
#include <stdlib.h>

SC_MODULE(tb) {
	///// testbench input port /////
	sc_in<bool> clk;
	sc_in<sc_int<32>> mac_result;
	sc_in<bool> mac_result_valid;
	sc_in<bool> input_pixel_ready;
	sc_in<bool> input_kernel_ready;

	///// testbench output port /////
	sc_out<sc_int<16>> input_pixel;
	sc_out<bool> input_pixel_valid;
	sc_out<sc_int<16>> input_kernel;
	sc_out<bool> input_kernel_valid;
	sc_out<bool> mac_result_ready;

	void tb_main();							// testbench main function

	SC_CTOR(tb) {
		SC_CTHREAD(tb_main, clk.pos());
	}
};

void tb::tb_main() {
	input_pixel.write(12);
	input_kernel.write(13);

	wait();
}