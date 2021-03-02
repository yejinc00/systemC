#pragma once
#include <systemc.h>
#include <fstream>

SC_MODULE(PRE) {
	///// PRE input port /////
	sc_in<bool> clk;
	sc_in<sc_int<16>> DAT_in;
	sc_in<bool> DAT_in_valid;

	///// PRE output port /////
	sc_out<bool> DAT_in_ready;

	///// PRE internal function /////
	void PRE_main();						// PRE controller function
	void pooling();							// Pooling function
	void ReLU();							// ReLU function
	void Encoding();						// Encoding function

	SC_CTOR(PRE) {
		SC_CTHREAD(PRE_main, clk.pos());
	}
};


sc_int<16> arr[16];
sc_int<16> ex_arr[16];
sc_int<16> aver_arr[4];
int count = 0;
void PRE::pooling() {
	sc_int<16> test;
	do {
		wait();
	} while (!DAT_in_valid.read())
		;
	test = DAT_in.read();

	arr[count] = test;
	ex_arr[count] = test;
	count++;

	cout << test << endl;
	DAT_in_ready.write(1);
	wait();
	DAT_in_ready.write(0);

	///// get average value from arr /////
	if (count == 16) {
		
		ex_arr[2] = arr[4];
		ex_arr[3] = arr[5];
		ex_arr[4] = arr[2];
		ex_arr[5] = arr[3];
		ex_arr[10] = arr[12];
		ex_arr[11] = arr[13];
		ex_arr[12] = arr[10];
		ex_arr[13] = arr[11];
		
		for (int j = 0; j < 4; j++) {
			sc_int<16> sum = 0;
			
			for (int k = 4 * j; k < (4*j) + 4; k++) {
				sum += ex_arr[k];
			}
			aver_arr[j] = sum / 4;
			cout << aver_arr[j] << endl;
		}
	}
}

sc_int<16> ReLU_arr[4];
void PRE::ReLU() {
	for (int i = 0; i < 4; i++) {
		if (aver_arr[i] <= 0) {
			ReLU_arr[i] = 0;
		}
		else {
			ReLU_arr[i] = aver_arr[i];
		}
	}
}

void PRE::Encoding() {
	ofstream outfile("./NZVL.txt");
	ofstream outfile2("./sparsity_map.txt");

	for (int i = 0; i < 4; i++) {
		if (ReLU_arr[i] == 0) {
			// outfile -> do nothing
			outfile2 << 0;
		}
		else {
			outfile << ReLU_arr[i];
			outfile2 << 1;
		}
		outfile << " ";
		outfile2 << " ";
	}
	outfile << "\n";
	outfile2 << "\n";
}

void PRE::PRE_main() {
	wait(SC_ZERO_TIME);
	for (int i = 0; i < 16; i++) {
		pooling();
	}
	ReLU();
	Encoding();
}