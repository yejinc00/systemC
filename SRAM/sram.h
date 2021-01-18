#pragma once
#include <systemc.h>

SC_MODULE(sram) {
	///// sram input port /////
	sc_in<bool> clk;
	sc_in<sc_int<8>> addr;
	sc_in<sc_int<1>> addr_valid;
	sc_in<sc_int<16>> Din;
	sc_in<sc_int<1>> Din_valid;
	sc_in<sc_int<1>> Cen;
	sc_in<sc_int<1>> Cen_valid;
	sc_in<sc_int<1>> Wen;
	sc_in<sc_int<1>> Wen_valid;
	sc_in<sc_int<1>> Dout_ready;

	///// sram output port /////
	sc_out<sc_int<16>> Dout;
	sc_out<sc_int<1>> Dout_valid;
	sc_out<sc_int<1>> addr_ready;
	sc_out<sc_int<1>> Din_ready;
	sc_out<sc_int<1>> Cen_ready;
	sc_out<sc_int<1>> Wen_ready;

	void sram_main();

	SC_CTOR(sram) {
		SC_CTHREAD(sram_main, clk.pos())
	}
};

sc_int<16> input_arr[100];			 // use like memory

void sram::sram_main(void) {
	///// initialize handshake signal /////
	Dout_valid.write(0);
	addr_ready.write(0);
	Din_ready.write(0);
	Cen_ready.write(0);
	Wen_ready.write(0);
	wait();


	while (1) {
		///// sram module local variable /////
		sc_int<8> input_addr;
		sc_int<16> input_Din;
		sc_int<1> input_Cen = 0;
		sc_int<1> input_Wen = 0;
		sc_int<16> output_Dout;

		///// wait until valid input addr, Din, Cen, Wen /////
		do {
			wait();
		} while (!addr_valid.read() || !Din_valid.read() || !Cen_valid.read() || !Wen_valid.read())
			;
		input_addr = addr.read();
		input_Din = Din.read();
		input_Cen = Cen.read();
		input_Wen = Wen.read();

		////// sram write or read //////
		if (input_Wen == -1) {
			cout << "sram: "<< input_Din << endl;
			input_arr[input_addr] = input_Din;
		}
		else if (input_Cen == -1) {
			output_Dout = input_arr[input_addr];
		}

		////// send ACK to tb //////
		addr_ready.write(1);
		Din_ready.write(1);
		Cen_ready.write(1);
		Wen_ready.write(1);
		wait();

		////// initialize ready signal ///////
		addr_ready.write(0);
		Din_ready.write(0);
		Cen_ready.write(0);
		Wen_ready.write(0);


		if (input_Cen == -1) {
			////// send output to tb //////
			Dout.write(output_Dout);
			Dout_valid.write(1);

			////// wait ready signal from tb //////
			do {
				wait();
			} while (!Dout_ready.read())
				;
		}

		////// initialize valid signal //////
		Dout_valid.write(0);
		wait();
	}

}