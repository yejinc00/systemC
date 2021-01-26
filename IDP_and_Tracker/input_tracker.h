#pragma once
#include <systemc.h>

SC_MODULE(tracker) {
	///// tracker input port /////
	sc_in<bool> clk;
	sc_in<bool> addr_ready;

	///// tracker output port /////
	sc_out<sc_int<8>> addr;
	sc_out<bool> addr_valid;

	///// tracker internal function /////
	void tracker_main();					// tracker main function
	sc_int<16> get_data(char*);				// data processing function

	SC_CTOR(tracker) {
		SC_CTHREAD(tracker_main, clk.pos());
	}
};

sc_int<16> tracker::get_data(char *data) {
	sc_int<16> result_data;
	int addr_binary[16];
	memset(addr_binary, 0, sizeof(addr_binary));
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

	int position = 0;
	unsigned int decimal = 0;
	for (int i = 7; i >=0; i--) {
		if (addr_binary[i] == 1) {
			decimal += 1 << position;
		}
		position++;
	}
	result_data = decimal;

	return result_data;
}


sc_int<8> input_addr;
void tracker::tracker_main() {
	///// handshake signal initialize /////
	addr_valid.write(0);
	wait();

	///// read data from sparsity_map.txt /////
	char line[128];
	memset(line, 0, sizeof(line));
	FILE* fp = NULL;
	fopen_s(&fp, "./sparsity_map.txt", "r");
	if (fp == NULL) {
		cout << "failed to open file" << endl;
		exit(0);
	}

	int end_signal = 0, num = 0;
	while (end_signal != 1) {
		for (int i = 0; i < 3; i++) {
			fgets(line, sizeof(line), fp);
			if (i == 0) {
				input_addr = get_data(line);
			}
		}

		///// write address to idp /////
		addr.write(input_addr);
		addr_valid.write(1);

		do {
			wait();
		} while (!addr_ready.read())
			;
		addr_valid.write(0);
		wait();
	}
}