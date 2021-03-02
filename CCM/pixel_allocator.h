#pragma once
#include <systemc.h>

SC_MODULE(pixel_allocator) {
	sc_in<bool> clk;

	///// IDP to pixel_allocator input port /////
	sc_in<sc_int<16>> IDP_pixel;
	sc_in<sc_int<16>> IDP_loc_x;
	sc_in<sc_int<16>> IDP_loc_y;
	sc_in<bool> IDP_data_valid;

	///// pixel_allocator to IDP output port /////
	sc_out<bool> IDP_data_ready;


	///// CCM controller to pixel_allocator input port /////
	sc_in<bool> PA_data_ready;

	///// pixel_allocator to CCM controller output port /////
	sc_out<sc_int<16>> PA_pixel;
	sc_out<sc_int<16>> PA_loc_x;
	sc_out<sc_int<16>> PA_loc_y;
	sc_out<bool> PA_data_valid;


	///// pixel_allocator internal function /////
	void pixel_allocator_main();								// pixel_allocator function
	

	SC_CTOR(pixel_allocator) {
		SC_CTHREAD(pixel_allocator_main, clk.pos());
	}
};

void pixel_allocator::pixel_allocator_main() {
	sc_int<16> IDP_input_pixel;
	sc_int<16> IDP_input_loc_x;
	sc_int<16> IDP_input_loc_y;

	///// get data from IDP /////
	do {
		wait();
	} while (!IDP_data_valid.read())
		;

	IDP_input_pixel = IDP_pixel.read();
	IDP_input_loc_x = IDP_loc_x.read();
	IDP_input_loc_y = IDP_loc_y.read();
	IDP_data_ready.write(1);
	cout <<endl<<endl<< "[Pixel allocator] " << IDP_input_pixel << " " << IDP_input_loc_x << " " << IDP_input_loc_y << endl << endl;
	
	///// put data to CCM controller /////
	PA_pixel.write(IDP_input_pixel);
	PA_loc_x.write(IDP_input_loc_x);
	PA_loc_y.write(IDP_input_loc_y);
	PA_data_valid.write(1);
	do {
		wait();
	} while (!PA_data_ready.read())
		;
	PA_data_valid.write(0);

}