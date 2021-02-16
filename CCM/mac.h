#pragma once
#include <systemc.h>
#define ADDDELAY 0
#define MULDELAY 0

SC_MODULE(mac) {
	///// mac input port /////
	sc_in<bool> clk;
	sc_in<sc_int<16>> input_pixel;
	sc_in<bool> input_pixel_valid;
	sc_in<sc_int<16>> input_kernel;
	sc_in<bool> input_kernel_valid;
	sc_in<bool> mac_result_ready;

	///// mac output port /////
	sc_out<sc_int<32>> mac_result;
	sc_out<bool> mac_result_valid;
	sc_out<bool> input_pixel_ready;
	sc_out<bool> input_kernel_ready;

	///// mac internal function /////
	void mac_main();								// mac controller function
	sc_int <32> buffer(sc_int <32>);				// buffer module
	sc_int <32> adder(sc_int <32>);					// 32-bit fixed point adder module
	sc_int <32> multiplier(sc_int<16>, sc_int<16>);	// 16-bit fixed point multiplier

	SC_CTOR(mac) {
		SC_CTHREAD(mac_main, clk.pos());
	}
};


sc_int<32> mac::multiplier(sc_int<16>pixel, sc_int<16>kernel) {
	sc_int <32> mul_result;

	mul_result = pixel * kernel;

	for (int i = 0; i < MULDELAY; i++) {
		wait();
	}

	cout << "func result: " << mul_result << endl;
	return mul_result;
}

sc_int<32> buff;
sc_int <32> mac::buffer(sc_int <32>add_result) {
	if (add_result == -1) {
		// just return previous value
	}
	else {
		buff = add_result;
	}
	return buff;
}

sc_int <32> mac::adder(sc_int <32>mul_result) {
	sc_int <32> add_result;
	sc_int <32> buff_result;

	buff_result = buffer(-1);
	add_result = mul_result + buff_result;

	for (int i = 0; i < ADDDELAY; i++) {
		wait();
	}
	buffer(add_result);

	return add_result;
}

void mac::mac_main() {
	sc_int<16> pixel;
	sc_int<16> kernel;

	sc_int<32> input_result;
	sc_int<32> buff_result;

	buffer(0);							// initialize only once
	wait();
	pixel = input_pixel.read();
	kernel = input_kernel.read();

	input_result = multiplier(pixel, kernel);
	buff_result = adder(input_result);


	cout << "result : " << input_result << endl;
	cout << "result2: " << buff_result << endl;
}