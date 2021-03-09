#pragma once
#include <systemc.h>
#include "Externel_DRAM.h"
#include "Pixel_Memory.h"
#include "IDP_Manager.h"

SC_MODULE(SYSTEM) {
	///// pointer of module /////
	Externel_DRAM* externel_dram0;
	Pixel_Memory* pixel_memory0;
	IDP_Manager* idp0;

	///// Externel DRAM, Pixel Memory, IDP Manager /////
	sc_signal<sc_int<8>> sig_DRAM_addr;
	sc_signal<sc_int<8>> sig_IDP_addr;
	sc_signal<sc_int<16>> sig_Din;
	sc_signal<bool> sig_D_write;
	sc_signal<bool> sig_D_read;
	sc_signal<bool> sig_DRAM_valid;
	sc_signal<bool> sig_IDP_valid;
	sc_signal<bool> sig_Dout_ready;
	sc_signal<sc_int<16>> sig_Dout;
	sc_signal<bool> sig_Dout_valid;
	sc_signal<bool> sig_DRAM_ready;
	sc_signal<bool> sig_IDP_ready;
	sc_signal<bool> sig_mem_update;


	///// constructor /////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		///// connecting module /////
		externel_dram0 = new Externel_DRAM("externel_dram0");
		externel_dram0->clk(clk_sig);
		externel_dram0->DRAM_ready(sig_DRAM_ready);
		externel_dram0->DRAM_addr(sig_DRAM_addr);
		externel_dram0->Din(sig_Din);
		externel_dram0->D_write(sig_D_write);
		externel_dram0->DRAM_valid(sig_DRAM_valid);
		externel_dram0->mem_update(sig_mem_update);

		pixel_memory0 = new Pixel_Memory("pixel_memory0");
		pixel_memory0->clk(clk_sig);
		pixel_memory0->DRAM_addr(sig_DRAM_addr);
		pixel_memory0->IDP_addr(sig_IDP_addr);
		pixel_memory0->Din(sig_Din);
		pixel_memory0->D_write(sig_D_write);
		pixel_memory0->D_read(sig_D_read);
		pixel_memory0->DRAM_valid(sig_DRAM_valid);
		pixel_memory0->IDP_valid(sig_IDP_valid);
		pixel_memory0->Dout_ready(sig_Dout_ready);
		pixel_memory0->Dout(sig_Dout);
		pixel_memory0->Dout_valid(sig_Dout_valid);
		pixel_memory0->DRAM_ready(sig_DRAM_ready);
		pixel_memory0->IDP_ready(sig_IDP_ready);

		idp0 = new IDP_Manager("idp0");
		idp0->clk(clk_sig);
		idp0->mem_update(sig_mem_update);
		idp0->Dout(sig_Dout);
		idp0->Dout_valid(sig_Dout_valid);
		idp0->IDP_ready(sig_IDP_ready);
		idp0->IDP_addr(sig_IDP_addr);
		idp0->D_read(sig_D_read);
		idp0->IDP_valid(sig_IDP_valid);
		idp0->Dout_ready(sig_Dout_ready);
	}

	///// destructor /////
	~SYSTEM() {
		delete externel_dram0;
		delete pixel_memory0;
		delete idp0;
	}
};
