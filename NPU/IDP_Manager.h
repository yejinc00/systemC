#pragma once
#include <systemc.h>

SC_MODULE(IDP_Manager) {
	///// IDP_Manager input port /////
	sc_in<bool> clk;
	sc_in<bool> mem_update;
	sc_in<sc_int<16>> Dout;
	sc_in<bool> Dout_valid;
	sc_in<bool> IDP_ready;

	///// IDP_Manager output port /////
	sc_out<sc_int<8>> IDP_addr;
	sc_out<bool> D_read;
	sc_out<bool> IDP_valid;
	sc_out<bool> Dout_ready;

	void IDP_Manager_main();				// IDP_Manager main function
	void IDP_decode();						// decode function
	void IDP_data_get(int);					// get data from pixel memory

	SC_CTOR(IDP_Manager) {
		SC_CTHREAD(IDP_Manager_main, clk.pos());
	}
};

void IDP_Manager::IDP_data_get(int location) {
	///// handshake signal initialize /////
	IDP_valid.write(0);
	Dout_ready.write(0);
	D_read.write(0);
	wait();

	///// request to read data from pixel memory /////
	IDP_addr.write(location);
	IDP_valid.write(1);
	D_read.write(1);

	do {
		wait();
	} while (!IDP_ready.read())
		;
	IDP_valid.write(0);
	D_read.write(0);
	wait();

	/////// wait until Dout is valid data ///////
	do {
		wait();
	} while (!Dout_valid.read())
		;

	/////// get pixel memory's result ///////
	output_Dout = Dout.read();
	cout << "IDP_Manager: " << output_Dout << endl << endl;

	Dout_ready.write(1);
	wait();

	///// initialize ready signal /////
	Dout_ready.write(0);
}

void IDP_Manager::IDP_decode() {

}

void IDP_Manager::IDP_Manager_main() {
	do {
		wait();
	} while (!mem_update.read())
		;
	for (int i = 0; i < 10; i++) {
		int location;
		cout << "input address: ";
		cin >> location;
		IDP_data_get(location);
	}
}
