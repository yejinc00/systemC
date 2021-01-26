#pragma once
#include <systemc.h>
#include <stdlib.h>

SC_MODULE(tb) {
	///// testbench input port /////
	sc_in<bool> clk;
	sc_in<sc_int<8>> req_addr;
	sc_in<bool> req_addr_valid;
	sc_in<bool> Din_ready;
	sc_in<bool> pixel_ready;
	sc_in<sc_uint<8>> pixel_request;


	///// testbench output port /////
	sc_out<sc_int<16>> Din;
	sc_out<bool> Din_valid;
	sc_out<bool> req_addr_ready;
	sc_out<sc_int<16>> pixel;
	sc_out<bool> pixel_valid;

	void tb_main();							// testbench main function
	void tb_put();							// put data to sram
	sc_int<16> tb_get(int);					// get data from sram
	sc_int<16> get_data(char *, int);		// data processing function

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


sc_int<8> get_addr;
sc_int<16> input_Din;
sc_int<16> output_Dout;
sc_int<16> input_arr[100];
sc_int<16> pixel_arr[100];

void tb::tb_put() {
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
				get_addr = get_data(line, 1);
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
		input_arr[get_addr] = input_Din;
	}

	///// read data from NZVL.txt /////
	char p_line[128];
	FILE *fp2 = NULL;
	fp2 = fopen("./NZVL.txt", "r");
	if (fp2 == NULL) {
		cout << "failed to open file" << endl;
		exit(0);
	}

	for (int i = 0; i < 3; i++) {
		fgets(p_line, sizeof(p_line), fp2);
		pixel_arr[i] = atoi(p_line);
		cout << pixel_arr[i] << endl;
	}

}

sc_int<16> data_to_transfer;
sc_int<16> tb::tb_get(int location) {
	/////// get sram's result ///////
	data_to_transfer = input_arr[location];
	
	return data_to_transfer;
}

sc_int<8> address;
sc_int<16> output_data;
sc_int<16> pixel_count = 0;

void tb::tb_main() {
	///// make initial state /////
	tb_put();
	
	for (int count = 0; count < 2; count++) {
		///// handshake signal initialize /////
		Din_valid.write(0);
		req_addr_ready.write(0);
		wait();

		do {
			wait();
		} while (!req_addr_valid.read())
			;

		address = req_addr.read();

		///// send ACK signal to IDP /////
		req_addr_ready.write(1);
		wait();

		output_data = tb_get(address);

		///// send data to IDP /////
		Din.write(output_data);
		Din_valid.write(1);

		/////// wait until get ready signal from IDP ///////
		do {
			wait();
		} while (!Din_ready.read())
			;
		Din_valid.write(0);

		sc_uint<8> test;
		do {
			test = pixel_request.read();
			cout << test << " ";
			if (pixel_request.read() == 1) {
				pixel.write(pixel_arr[pixel_count]);
				pixel_valid.write(1);
				pixel_count++;
				do {
					wait();
				} while (!pixel_ready.read())
					;
				pixel_valid.write(0);
			}
		} while (pixel_request.read()!=3)
			;

	}
	
}