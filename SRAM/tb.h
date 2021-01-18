#pragma once
#include <systemc.h>

SC_MODULE(tb) {
	///// testbench input port /////
	sc_in<bool> clk;
	sc_in<sc_int<16>> Dout;
	sc_in<sc_int<1>> Dout_valid;
	sc_in<sc_int<1>> addr_ready;
	sc_in<sc_int<1>> Din_ready;
	sc_in<sc_int<1>> Cen_ready;
	sc_in<sc_int<1>> Wen_ready;

	///// testbench output port /////
	sc_out<sc_int<8>> addr;
	sc_out<sc_int<1>> addr_valid;
	sc_out<sc_int<16>> Din;
	sc_out<sc_int<1>> Din_valid;
	sc_out<sc_int<1>> Cen;
	sc_out<sc_int<1>> Cen_valid;
	sc_out<sc_int<1>> Wen;
	sc_out<sc_int<1>> Wen_valid;
	sc_out<sc_int<1>> Dout_ready;

	void tb_main();							// testbench main function
	void tb_put();
	void tb_get(int);
	sc_int<16> get_data(char *, int );		// data processing function

	SC_CTOR(tb) {
		SC_CTHREAD(tb_main, clk.pos());
	}
};

sc_int<16> tb::get_data(char *data, int flag) {
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

void tb::tb_put() {
	///// handshake signal initialize /////
	addr_valid.write(0);
	Din_valid.write(0);
	Cen_valid.write(0);
	Wen_valid.write(0);
	Dout_ready.write(0);
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
	while (end_signal != 1) {
		for (int i = 0; i < 3; i++) {
			fgets(line, sizeof(line), fp);
			if (i == 0) {
				input_addr = get_data(line, 1);
			}
			else if (i == 1) {
				input_Din = get_data(line, 0);
			}
			else if (i == 2) {
				if (line[0] != '<') {
					end_signal = 1;
				}
			}
		}
		sc_bv<8> b_addr = input_addr;
		sc_bv<16> b_Din = input_Din;

		///// write data to sram -> Wen == 1 /////
		addr.write(input_addr);
		addr_valid.write(1);
		Din.write(input_Din);
		Din_valid.write(1);
		Cen.write(0);
		Cen_valid.write(1);
		Wen.write(1);
		Wen_valid.write(1);

		do {
			wait();
		} while (!addr_ready.read() || !Din_ready.read() || !Cen_ready.read() || !Wen_ready.read())
			;
		addr_valid.write(0);
		Din_valid.write(0);
		Cen_valid.write(0);
		Wen_valid.write(0);
		wait();

	}

	///// initialize ready signal /////
	Dout_ready.write(0);
}

void tb::tb_get(int location) {
	///// handshake signal initialize /////
	addr_valid.write(0);
	Din_valid.write(0);
	Cen_valid.write(0);
	Wen_valid.write(0);
	Dout_ready.write(0);
	wait();

	///// read data from sram -> Cen == 1 /////
	addr.write(location);
	addr_valid.write(1);
	Din.write(input_Din);
	Din_valid.write(1);
	Cen.write(1);
	Cen_valid.write(1);
	Wen.write(0);
	Wen_valid.write(1);

	do {
		wait();
	} while (!addr_ready.read() || !Din_ready.read() || !Cen_ready.read() || !Wen_ready.read())
		;
	addr_valid.write(0);
	Din_valid.write(0);
	Cen_valid.write(0);
	Wen_valid.write(0);
	wait();
	
	/////// wait until Dout is valid data ///////
	do {
		wait();
	} while (!Dout_valid.read())
		;

	/////// get sram's result ///////
	output_Dout = Dout.read();
	cout << "tb: " << output_Dout << endl;

	Dout_ready.write(1);
	wait();

	///// initialize ready signal /////
	Dout_ready.write(0);
}

void tb::tb_main() {
	for (int i = 0; i < 10; i++) {
		char input;
		cout << "input(p or g): ";
		cin >> input;
		if (input == 'p') {
			tb_put();
		}
		else if (input == 'g') {
			int location;
			cout << "input address: ";
			cin >> location;
			tb_get(location);
		}
	}
}