#pragma once
#include <systemc.h>
#include <stdlib.h>

SC_MODULE(tb) {
	sc_in<bool> clk;

	///// pixel_allocator to testbench input port /////
	sc_in<bool> IDP_data_ready;

	///// testbench to pixel_allocator output port /////
	sc_out<sc_int<16>> IDP_pixel;
	sc_out<sc_int<16>> IDP_loc_x;
	sc_out<sc_int<16>> IDP_loc_y;
	sc_out<bool> IDP_data_valid;

	///// MAC to testbench input port /////
	sc_in<sc_int<32>> mac_result;
	sc_in<bool> mac_result_valid;

	///// testbench to MAC output port /////
	sc_out<bool> mac_result_ready;

	void tb_main();								// testbench main function

	SC_CTOR(tb) {
		SC_CTHREAD(tb_main, clk.pos());
	}
};

void tb::tb_main() {
	sc_int<32> mac_result_value;

	IDP_pixel.write(16);
	IDP_loc_x.write(3);
	IDP_loc_y.write(4);
	IDP_data_valid.write(1);
	do {
		wait();
	} while (!IDP_data_ready.read())
		;

	do {
		wait();
	} while (!mac_result_valid.read())
		;
	mac_result_value = mac_result.read();

	mac_result_ready.write(1);
	cout << "[Testbench] " << mac_result << endl << endl;
}