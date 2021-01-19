#pragma once
#include <systemc.h>
#include "pixel_memory.h"
#include "input_tracker.h"
#include "IDP.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	///// pointer of modules /////
	tb* tb0;
	pixel_memory* pixel_memory0;
	idp* idp0;

	///// signal for connect module tb and pixel memory /////
	sc_signal<sc_int<8>> sig_addr;
	sc_signal<sc_int<1>> sig_addr_valid;
	sc_signal<sc_int<1>> sig_addr_ready;

	sc_signal<sc_int<16>> sig_Din;
	sc_signal<sc_int<1>> sig_Din_valid;
	sc_signal<sc_int<1>> sig_Din_ready;

	sc_signal<sc_int<1>> sig_Cen;
	sc_signal<sc_int<1>> sig_Cen_valid;
	sc_signal<sc_int<1>> sig_Cen_ready;

	sc_signal<sc_int<1>> sig_Wen;
	sc_signal<sc_int<1>> sig_Wen_valid;
	sc_signal<sc_int<1>> sig_Wen_ready;

	sc_signal<sc_int<16>> sig_Dout;
	sc_signal<sc_int<1>> sig_Dout_valid;
	sc_signal<sc_int<1>> sig_Dout_ready;

	///// constructor /////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		///// connecting module /////
		tb0 = new tb("tb0");
		tb0->clk(clk_sig);
		tb0->Dout(sig_Dout);
		tb0->Dout_valid(sig_Dout_valid);
		tb0->addr_ready(sig_addr_ready);
		tb0->Din_ready(sig_Din_ready);
		tb0->Cen_ready(sig_Cen_ready);
		tb0->Wen_ready(sig_Wen_ready);

		tb0->addr(sig_addr);
		tb0->addr_valid(sig_addr_valid);
		tb0->Din(sig_Din);
		tb0->Din_valid(sig_Din_valid);
		tb0->Cen(sig_Cen);
		tb0->Cen_valid(sig_Cen_valid);
		tb0->Wen(sig_Wen);
		tb0->Wen_valid(sig_Wen_valid);
		tb0->Dout_ready(sig_Dout_ready);

		pixel_memory0 = new pixel_memory("pixel memory0");
		pixel_memory0->clk(clk_sig);
		pixel_memory0->addr(sig_addr);
		pixel_memory0->addr_valid(sig_addr_valid);
		pixel_memory0->Din(sig_Din);
		pixel_memory0->Din_valid(sig_Din_valid);
		pixel_memory0->Cen(sig_Cen);
		pixel_memory0->Cen_valid(sig_Cen_valid);
		pixel_memory0->Wen(sig_Wen);
		pixel_memory0->Wen_valid(sig_Wen_valid);
		pixel_memory0->Dout_ready(sig_Dout_ready);

		pixel_memory0->Dout(sig_Dout);
		pixel_memory0->Dout_valid(sig_Dout_valid);
		pixel_memory0->addr_ready(sig_addr_ready);
		pixel_memory0->Din_ready(sig_Din_ready);
		pixel_memory0->Wen_ready(sig_Wen_ready);
		pixel_memory0->Cen_ready(sig_Cen_ready);

		idp0 = new idp("idp0");

	}

	///// destructor /////
	~SYSTEM() {
		delete tb0;
		delete pixel_memory0;
		delete idp0;
	}
};