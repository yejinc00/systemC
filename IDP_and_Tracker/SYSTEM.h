#pragma once
#include<systemc.h>
#include "input_tracker.h"
#include "IDP.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	///// pointer of modules /////
	idp* idp0;
	tracker* tracker0;
	tb* tb0;

	///// signal for connect tracker and IDP module /////
	sc_signal<sc_int<8>> sig_addr;
	sc_signal<bool> sig_addr_valid;
	sc_signal<bool> sig_addr_ready;
	
	///// signal for connect tb and IDP module /////
	sc_signal<sc_int<8>> sig_req_addr;
	sc_signal<bool> sig_req_addr_valid;
	sc_signal<bool> sig_req_addr_ready;

	sc_signal<sc_int<16>> sig_Din;
	sc_signal<bool> sig_Din_valid;
	sc_signal<bool> sig_Din_ready;

	sc_signal<sc_int<16>> sig_pixel;
	sc_signal<bool> sig_pixel_valid;
	sc_signal<bool> sig_pixel_ready;

	sc_signal<sc_uint<8>> sig_pixel_request;


	///// constructor /////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		///// connecting module /////
		idp0 = new idp("idp0");
		idp0->clk(clk_sig);
		idp0->addr(sig_addr);
		idp0->addr_valid(sig_addr_valid);
		idp0->addr_ready(sig_addr_ready);
		idp0->req_addr(sig_req_addr);
		idp0->req_addr_valid(sig_req_addr_valid);
		idp0->req_addr_ready(sig_req_addr_ready);
		idp0->Din(sig_Din);
		idp0->Din_valid(sig_Din_valid);
		idp0->Din_ready(sig_Din_ready);
		idp0->pixel(sig_pixel);
		idp0->pixel_valid(sig_pixel_valid);
		idp0->pixel_ready(sig_pixel_ready);
		idp0->pixel_request(sig_pixel_request);

		tracker0 = new tracker("tracker0");
		tracker0->clk(clk_sig);
		tracker0->addr(sig_addr);
		tracker0->addr_valid(sig_addr_valid);
		tracker0->addr_ready(sig_addr_ready);

		tb0 = new tb("tb0");
		tb0->clk(clk_sig);
		tb0->req_addr(sig_req_addr);
		tb0->req_addr_valid(sig_req_addr_valid);
		tb0->req_addr_ready(sig_req_addr_ready);
		tb0->Din(sig_Din);
		tb0->Din_valid(sig_Din_valid);
		tb0->Din_ready(sig_Din_ready);
		tb0->pixel(sig_pixel);
		tb0->pixel_valid(sig_pixel_valid);
		tb0->pixel_ready(sig_pixel_ready);
		tb0->pixel_request(sig_pixel_request);
	}

	///// destructor /////
	~SYSTEM() {
		delete idp0;
		delete tracker0;
		delete tb0;
	}
};