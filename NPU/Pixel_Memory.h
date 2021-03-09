#pragma once
#include <systemc.h>

SC_MODULE(Pixel_Memory) {
	///// Pixel_Memory input port /////
	sc_in<bool> clk;
	sc_in<sc_int<8>> DRAM_addr;
	sc_in<sc_int<8>> IDP_addr;
	sc_in<sc_int<16>> Din;
	sc_in<bool> D_write;
	sc_in<bool> D_read;
	sc_in<bool> DRAM_valid;
	sc_in<bool> IDP_valid;
	sc_in<bool> Dout_ready;

	///// Pixel_Memory output port /////
	sc_out<sc_int<16>> Dout;
	sc_out<bool> Dout_valid;
	sc_out<bool> DRAM_ready;
	sc_out<bool> IDP_ready;

	void Pixel_Memory_main();

	SC_CTOR(Pixel_Memory) {
		SC_CTHREAD(Pixel_Memory_main, clk.pos())
	}
};

sc_int<16> input_arr[100];			 // use like memory
sc_int<16> pixel_arr[100][16];
void Pixel_Memory::Pixel_Memory_main(void) {
	///// initialize handshake signal /////
	Dout_valid.write(0);
	DRAM_ready.write(0);
	IDP_ready.write(0);
	wait();

	FILE *fp = NULL;
	fp = fopen("./NZVL.txt", "r");
	if (fp == NULL) {
		cout << "failed to open file" << endl;
		exit(0);
	}

	while (1) {
		///// Pixel_Memory module local variable /////
		sc_int<8> input_addr;
		sc_int<16> input_Din;
		sc_int<16> output_Dout;
		bool D_w;
		bool D_r;

		char line[128];		
		///// wait until valid input /////
		do {
			wait();
		} while (!DRAM_valid.read()&& !IDP_valid.read())
			;

		D_w = D_write.read();
		D_r = D_read.read();
		if (D_w == 1) {
			input_addr = DRAM_addr.read();
			input_Din = Din.read();
			sc_bv<16> b_input_Din = input_Din;
			
			cout << "pixel value" << endl;
			for (int i = 15; i > -1; i--) {
				if (b_input_Din[i] == 1) {
					fgets(line, sizeof(line), fp);
					pixel_arr[input_addr][15 - i] = atoi(line);
					cout << pixel_arr[input_addr][15 - i] << endl;
				}
			}

			DRAM_ready.write(1);
			input_arr[input_addr] = input_Din;

			cout << "pixel memory area" << endl;
			cout << "addr 1:" << input_arr[1] << endl;
			cout << "addr 2:" << input_arr[2] << endl;
			cout << "addr 3:" << input_arr[3] << endl << endl;
		}
		else if (D_r == 1) {
			input_addr = IDP_addr.read();
			IDP_ready.write(1);
			output_Dout = input_arr[input_addr];

			////// send output to IDP Manager //////
			Dout.write(output_Dout);
			Dout_valid.write(1);

			////// wait ready signal from IDP Manager //////
			do {
				wait();
			} while (!Dout_ready.read())
				;
		}
	
		////// initialize valid signal //////
		Dout_valid.write(0);
		DRAM_ready.write(0);
		IDP_ready.write(0);
		
	}
}
