#pragma once
#include <systemc.h>
#define ADDDELAY 0
#define MULDELAY 2

SC_MODULE(mac) {
	sc_in<bool> clk;

	///// CCM controller to MAC input port /////
	sc_in<sc_int<16>> CCM_pixel;
	sc_in<bool> CCM_data_valid;

	///// MAC to CCM controller output port /////
	sc_out<bool> CCM_data_ready;


	///// kernel memory to MAC input port /////
	sc_in<sc_int<16>> SRAM_kernel;
	sc_in<bool> SRAM_kernel_valid;

	///// MAC to kernel memory output port /////
	sc_out<bool> SRAM_kernel_ready;

	///// tb to MAC input port /////
	sc_in<bool> mac_result_ready;

	///// MAC to tb output port /////
	sc_out<sc_int<32>> mac_result;
	sc_out<bool> mac_result_valid;

	///// mac internal function /////
	void mac_main();								// mac controller function
	sc_int <32> buffer(sc_int <32>);				// buffer module
	sc_int <32> adder(sc_int <32>);					// 32-bit fixed point adder module
	sc_int <32> multiplier(sc_int<16>, sc_int<16>);	// 16-bit fixed point multiplier

	SC_CTOR(mac) {
		SC_CTHREAD(mac_main, clk.pos());
	}
};


sc_int<32> mac::multiplier(sc_int<16>pixel , sc_int<16>kernel) {
	sc_int <32> mul_result;

	mul_result = pixel * kernel;

	for (int i = 0; i < MULDELAY; i++) {
		wait();
	}
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
	do {
		wait();
	} while (!CCM_data_valid.read())
		;
	pixel = CCM_pixel.read();
	CCM_data_ready.write(1);

	do {
		wait();
	} while (!SRAM_kernel_valid.read())
		;
	kernel = SRAM_kernel.read();
	SRAM_kernel_ready.write(1);

	input_result = multiplier(pixel, kernel);
	buff_result = adder(input_result);

	cout << "[MAC]" << endl;
	cout << "MAC result: " << input_result << endl;
	cout << "biffer value: " << buff_result << endl << endl;

	mac_result.write(input_result);
	mac_result_valid.write(1);
	do {
		wait();
	} while (!mac_result_ready.read())
		;
	mac_result_valid.write(0);

}