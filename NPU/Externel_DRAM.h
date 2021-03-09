#pragma once
#include <systemc.h>

SC_MODULE(Externel_DRAM) {
	///// Externel_DRAM input port /////
	sc_in<bool> clk;
	sc_in<bool> DRAM_ready;

	///// Externel_DRAM output port /////
	sc_out<sc_int<8>> DRAM_addr;
	sc_out<sc_int<16>> Din;
	sc_out<bool> D_write;
	sc_out<bool> DRAM_valid;
	sc_out<bool> mem_update;
	
	void Externel_DRAM_main();				// Externel_DRAM main function
	void data_write();						// write data to sram
	sc_int<16> get_data(char *, int );		// data processing function

	SC_CTOR(Externel_DRAM) {
		SC_CTHREAD(Externel_DRAM_main, clk.pos());
	}
};

sc_int<16> Externel_DRAM::get_data(char *data, int flag) {
	sc_int<16> result_data;
	int addr_binary[8];
	int data_binary[16];

	if (flag == 0) {
		///// get valid data from input.txt /////
		int i = 0, k = 0;
		while (data[i] != '\n') {
			if (data[i] == '0') {
				data_binary[k] = 0;
				k++;
			}
			else if (data[i] == '1') {
				data_binary[k] = 1;
				k++;
			}
			i++;
		}

		///// convert to decimal data /////
		int position = 0;
		unsigned int decimal = 0;
		for (int i = 15; i >= 0; i--) {
			if (data_binary[i] == 1) {
				decimal += 1 << position;
			}
			position++;
		}
		result_data = decimal;
	}
	else if (flag == 1) {
		///// get valid data from input.txt /////
		int i = 0, k = 0;
		while (data[i] != '\n') {
			if (data[i] == '0') {
				addr_binary[k] = 0;
				k++;
			}
			else if (data[i] == '1') {
				addr_binary[k] = 1;
				k++;
			}
			i++;
		}

		///// convert to decimal data /////
		int position = 0;
		unsigned int decimal = 0;
		for (int i = 7; i >= 0; i--) {
			if (addr_binary[i] == 1) {
				decimal += 1 << position;
			}
			position++;
		}
		result_data = decimal;
	}


	return result_data;
}


sc_int<8> input_addr;
sc_int<16> input_Din;
sc_int<16> output_Dout;

void Externel_DRAM::data_write() {
	///// initialize /////
	DRAM_valid.write(0);
	D_write.write(0);
	wait();

	///// read data from input.txt /////
	char line[128];
	int binary[128];
	int end_signal = 0;
	FILE *fp = NULL;
	fp = fopen("./input.txt", "r");
	if (fp == NULL) {
		cout << "failed to open file" << endl;
		exit(0);
	}

	int num = 0;
	for (int i = 0; i < 9; i++) {
		fgets(line, sizeof(line), fp);
		cout << i << ": " << line;
		if (i%3 == 0) {
			input_addr = get_data(line, 1);
		}
		else if (i%3 == 1) {
			input_Din = get_data(line, 0);
			sc_bv<8> b_addr = input_addr;
			sc_bv<16> b_Din = input_Din;

			DRAM_addr.write(input_addr);
			Din.write(input_Din);
			D_write.write(1);
			DRAM_valid.write(1);

			do {
				wait();
			} while (DRAM_ready.read())
				;
			DRAM_valid.write(0);
			D_write.write(0);
			wait();
		}
		else if (i % 3 == 2) {
			if (line[0] != '<') {
				mem_update.write(1);
			}
		}

	}
	cout << endl;
	return;
}

void Externel_DRAM::Externel_DRAM_main() {
	
	data_write();
}
