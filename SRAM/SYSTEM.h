#pragma once
#include <systemc.h>
#include "sram.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	///// pointer of module tb and sram /////
	tb *tb0;
	sram *sram0;

	///// signal for connect module tb and adder /////
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

		sram0 = new sram("sram0");
		sram0->clk(clk_sig);
		sram0->addr(sig_addr);
		sram0->addr_valid(sig_addr_valid);
		sram0->Din(sig_Din);
		sram0->Din_valid(sig_Din_valid);
		sram0->Cen(sig_Cen);
		sram0->Cen_valid(sig_Cen_valid);
		sram0->Wen(sig_Wen);
		sram0->Wen_valid(sig_Wen_valid);
		sram0->Dout_ready(sig_Dout_ready);

		sram0->Dout(sig_Dout);
		sram0->Dout_valid(sig_Dout_valid);
		sram0->addr_ready(sig_addr_ready);
		sram0->Din_ready(sig_Din_ready);
		sram0->Cen_ready(sig_Cen_ready);
		sram0->Wen_ready(sig_Wen_ready);

	}

	///// destructor /////
	~SYSTEM() {
		delete tb0;
		delete sram0;
	}
};