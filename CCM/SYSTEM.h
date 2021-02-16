#pragma once
#include<systemc.h>
#include "mac.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	///// pointer of modules /////
	mac* mac0;
	tb* tb0;

	///// signal for connect tb and mac module /////
	sc_signal<sc_int<16>> sig_input_pixel;
	sc_signal<sc_int<16>> sig_input_kernel;
	sc_signal<bool> sig_input_pixel_valid;
	sc_signal<bool> sig_input_pixel_ready;
	sc_signal<bool> sig_input_kernel_valid;
	sc_signal<bool> sig_input_kernel_ready;

	sc_signal<sc_int<32>> sig_mac_result;
	sc_signal<bool> sig_mac_result_valid;
	sc_signal<bool> sig_mac_result_ready;

	///// constructor /////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		///// connecting module /////
		mac0 = new mac("mac0");
		mac0->clk(clk_sig);
		mac0->input_pixel(sig_input_pixel);
		mac0->input_pixel_valid(sig_input_pixel_valid);
		mac0->input_pixel_ready(sig_input_pixel_ready);

		mac0->input_kernel(sig_input_kernel);
		mac0->input_kernel_valid(sig_input_kernel_valid);
		mac0->input_kernel_ready(sig_input_kernel_ready);

		mac0->mac_result(sig_mac_result);
		mac0->mac_result_valid(sig_mac_result_valid);
		mac0->mac_result_ready(sig_mac_result_ready);

		tb0 = new tb("tb0");
		tb0->clk(clk_sig);
		tb0->input_pixel(sig_input_pixel);
		tb0->input_pixel_valid(sig_input_pixel_valid);
		tb0->input_pixel_ready(sig_input_pixel_ready);

		tb0->input_kernel(sig_input_kernel);
		tb0->input_kernel_valid(sig_input_kernel_valid);
		tb0->input_kernel_ready(sig_input_kernel_ready);

		tb0->mac_result(sig_mac_result);
		tb0->mac_result_valid(sig_mac_result_valid);
		tb0->mac_result_ready(sig_mac_result_ready);
	}

	///// destructor /////
	~SYSTEM() {
		delete mac0;
		delete tb0;
	}
};