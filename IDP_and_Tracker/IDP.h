#pragma once
#include <systemc.h>

SC_MODULE(idp) {
	///// IDP input port /////
	sc_in<bool> clk;
	sc_in<sc_int<8>> addr;
	sc_in<bool> addr_valid;
	sc_in<sc_int<16>> Din;
	sc_in<bool> Din_valid;
	sc_in<bool> req_addr_ready;
	sc_in<sc_int<16>> pixel;
	sc_in<bool> pixel_valid;

	///// IDP output port /////
	sc_out<bool> addr_ready;
	sc_out<bool> Din_ready;
	sc_out<sc_int<8>> req_addr;
	sc_out<bool> req_addr_valid;
	sc_out<bool> pixel_ready;
	sc_out<sc_uint<8>> pixel_request;

	///// IDP internal function /////
	void idp_main();					// IDP controller function
	void idp_decode(sc_int<16>);					// sparse matrix decode function

	SC_CTOR(idp) {
		SC_CTHREAD(idp_main, clk.pos());
	}
};

void idp::idp_decode(sc_int<16> data) {
	sc_bv<16> b_data = data;
	sc_int<16> input_pixel;
	pixel_request.write(0);
	cout << "decode" << endl;
	for (int i = 15; i >= 0; i--) {
		cout << "t" << " ";
		if (b_data[i] == 1) {
			pixel_ready.write(0);
			pixel_request.write(1);
			do {
				wait();
			} while (!pixel_valid.read())
				;
			input_pixel = pixel.read();
			pixel_request.write(0);
			pixel_ready.write(1);
		}
	}
	pixel_request.write(3);
	cout << endl;
	
}

sc_int<16> get_data;
void idp::idp_main() {
	///// initialize handshake signal /////
	addr_ready.write(0);
	Din_ready.write(0);
	req_addr_valid.write(0);
	wait();
	
	for (int i = 0; i < 2; i++) {
		///// idp module local variable /////
		sc_int<8> input_addr;
		sc_int<16> input_Din;

		///// wait until get valid input addr /////
		do {
			wait();
		} while (!addr_valid.read())
			;
		input_addr = addr.read();
		cout <<"idp input: " <<input_addr << endl;

		///// send ACK to tracker /////
		addr_ready.write(1);
		wait();

		///// initialize ready signal /////
		addr_ready.write(0);

		req_addr.write(input_addr);
		req_addr_valid.write(1);

		do {
			wait();
		} while (!req_addr_ready.read())
			;
		req_addr_valid.write(0);

		do {
			wait();
		} while (!Din_valid.read())
			;
		get_data = Din.read();
		sc_bv<16> b_get_data = get_data;
		cout << "sparsity matrix: " << b_get_data << endl;

		
		///// send ACK to tb /////
		Din_ready.write(1);
		idp_decode(get_data);
		

	}
}