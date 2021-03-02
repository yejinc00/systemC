#pragma once
#include <systemc.h>

SC_MODULE(kernel_memory) {
	sc_in<bool> clk;

	/*
	///// tb to kernel_memory input port /////
	sc_in<sc_int<8>> addr;
	sc_in<sc_int<16>> Din;
	sc_in<bool> Cen;
	sc_in<bool> Wen;
	sc_in<bool> SRAM_input_data_valid;

	///// kernel_memory to tb output port /////
	sc_out<bool> SRAM_input_data_ready;
	*/

	///// MAC to kernel_memory input port /////
	sc_in<bool> SRAM_kernel_ready;

	///// kernel_memory to MAC output port /////
	sc_out<sc_int<16>> SRAM_kernel;
	sc_out<bool> SRAM_kernel_valid;
	

	void kernel_memory_main();

	SC_CTOR(kernel_memory) {
		SC_CTHREAD(kernel_memory_main, clk.pos())
	}
};

sc_int<16> input_arr[100];			 // use like memory

void kernel_memory::kernel_memory_main(void) {
	SRAM_kernel.write(23);
	SRAM_kernel_valid.write(1);
	do {
		wait();
	} while (!SRAM_kernel_ready.read())
		;

	SRAM_kernel_valid.write(0);

}