#pragma once
#include <systemc.h>

SC_MODULE(ccm_controller) {
	sc_in<bool> clk;
	
	///// pixel_allocator to CCM controller input port /////
	sc_in<sc_int<16>> PA_pixel;
	sc_in<sc_int<16>> PA_loc_x;
	sc_in<sc_int<16>> PA_loc_y;
	sc_in<bool> PA_data_valid;

	///// CCM controller to pixel_allocator output port /////
	sc_out<bool> PA_data_ready;

	///// MAC to CCM controller input port /////
	sc_in<bool> CCM_data_ready;

	///// CCM controller to MAC output port /////
	sc_out<sc_int<16>> CCM_pixel;
	sc_out<bool> CCM_data_valid;

	///// ccm_controller internal function /////
	void ccm_controller_main();								// ccm_controller function


	SC_CTOR(ccm_controller) {
		SC_CTHREAD(ccm_controller_main, clk.pos());
	}
};

void ccm_controller::ccm_controller_main() {
	
	sc_int<16> PA_input_pixel;
	sc_int<16> PA_input_loc_x;
	sc_int<16> PA_input_loc_y;

	///// get data from pixel allocator /////
	do {
		wait();
	} while (!PA_data_valid.read())
		;

	PA_input_pixel = PA_pixel.read();
	PA_input_loc_x = PA_loc_x.read();
	PA_input_loc_y = PA_loc_y.read();
	PA_data_ready.write(1);
	cout << "[CCM controller] " << PA_input_pixel << " " << PA_input_loc_x << " " << PA_input_loc_y << endl << endl;


	CCM_pixel.write(PA_input_pixel);
	CCM_data_valid.write(1);
	do {
		wait();
	} while (!CCM_data_ready.read())
		;
	CCM_data_valid.write(0);

}