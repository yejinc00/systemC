#pragma once
#include<systemc.h>
#include "pixel_allocator.h"
#include "ccm_controller.h"
#include "kernel_memory.h"
#include "mac.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	///// pointer of modules /////
	pixel_allocator* PA0;
	ccm_controller* CCM0;
	kernel_memory* MEM0;
	mac* MAC0;
	tb* tb0;

	///// signal for connect tb and pixel allocator /////
	sc_signal<sc_int<16>> sig_IDP_pixel;
	sc_signal<sc_int<16>> sig_IDP_loc_x;
	sc_signal<sc_int<16>> sig_IDP_loc_y;
	sc_signal<bool> sig_IDP_data_ready;
	sc_signal<bool> sig_IDP_data_valid;

	///// signal for connect pixel allocator and CCM controller /////
	sc_signal<sc_int<16>> sig_PA_pixel;
	sc_signal<sc_int<16>> sig_PA_loc_x;
	sc_signal<sc_int<16>> sig_PA_loc_y;
	sc_signal<bool> sig_PA_data_ready;
	sc_signal<bool> sig_PA_data_valid;

	///// signal for connect CCM controller and MAC /////
	sc_signal<sc_int<16>> sig_CCM_pixel;
	sc_signal<bool> sig_CCM_data_ready;
	sc_signal<bool> sig_CCM_data_valid;

	///// signal for connect kernel memory and MAC /////
	sc_signal<sc_int<16>> sig_SRAM_kernel;
	sc_signal<bool> sig_SRAM_kernel_ready;
	sc_signal<bool> sig_SRAM_kernel_valid;

	///// signal for connect MAC and tb /////
	sc_signal<sc_int<32>> sig_mac_result;
	sc_signal<bool> sig_mac_result_ready;
	sc_signal<bool> sig_mac_result_valid;

	///// constructor /////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		///// connecting module /////
		PA0 = new pixel_allocator("PA0");
		PA0->clk(clk_sig);
		PA0->IDP_pixel(sig_IDP_pixel);
		PA0->IDP_loc_x(sig_IDP_loc_x);
		PA0->IDP_loc_y(sig_IDP_loc_y);
		PA0->IDP_data_ready(sig_IDP_data_ready);
		PA0->IDP_data_valid(sig_IDP_data_valid);

		PA0->PA_pixel(sig_PA_pixel);
		PA0->PA_loc_x(sig_PA_loc_x);
		PA0->PA_loc_y(sig_PA_loc_y);
		PA0->PA_data_ready(sig_PA_data_ready);
		PA0->PA_data_valid(sig_PA_data_valid);

		CCM0 = new ccm_controller("CCM0");
		CCM0->clk(clk_sig);
		CCM0->PA_pixel(sig_PA_pixel);
		CCM0->PA_loc_x(sig_PA_loc_x);
		CCM0->PA_loc_y(sig_PA_loc_y);
		CCM0->PA_data_ready(sig_PA_data_ready);
		CCM0->PA_data_valid(sig_PA_data_valid);

		CCM0->CCM_pixel(sig_CCM_pixel);
		CCM0->CCM_data_valid(sig_CCM_data_valid);
		CCM0->CCM_data_ready(sig_CCM_data_ready);

		MEM0 = new kernel_memory("MEM0");
		MEM0->clk(clk_sig);
		MEM0->SRAM_kernel(sig_SRAM_kernel);
		MEM0->SRAM_kernel_ready(sig_SRAM_kernel_ready);
		MEM0->SRAM_kernel_valid(sig_SRAM_kernel_valid);

		MAC0 = new mac("MAC0");
		MAC0->clk(clk_sig);
		MAC0->SRAM_kernel(sig_SRAM_kernel);
		MAC0->SRAM_kernel_ready(sig_SRAM_kernel_ready);
		MAC0->SRAM_kernel_valid(sig_SRAM_kernel_valid);
		MAC0->CCM_pixel(sig_CCM_pixel);
		MAC0->CCM_data_valid(sig_CCM_data_valid);
		MAC0->CCM_data_ready(sig_CCM_data_ready);

		MAC0->mac_result(sig_mac_result);
		MAC0->mac_result_ready(sig_mac_result_ready);
		MAC0->mac_result_valid(sig_mac_result_valid);

		tb0 = new tb("tb0");
		tb0->clk(clk_sig);
		tb0->IDP_pixel(sig_IDP_pixel);
		tb0->IDP_loc_x(sig_IDP_loc_x);
		tb0->IDP_loc_y(sig_IDP_loc_y);
		tb0->IDP_data_ready(sig_IDP_data_ready);
		tb0->IDP_data_valid(sig_IDP_data_valid);

		tb0->mac_result(sig_mac_result);
		tb0->mac_result_ready(sig_mac_result_ready);
		tb0->mac_result_valid(sig_mac_result_valid);
	}

	///// destructor /////
	~SYSTEM() {
		delete PA0;
		delete CCM0;
		delete tb0;
	}
};