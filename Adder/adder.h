#pragma once
#include <systemc.h>

// RCA delay : 17cycles
// CLA delay : 2cycles
// CSA (carry select adder) delay : 5cycles
// CSA (carry save adder) delay : 3cycles
#define DELAY 17

SC_MODULE(adder) {
	////// adder input port //////
	sc_in<bool> clk;
	sc_in<sc_int<32>> A;
	sc_in<sc_int<1>> A_valid;
	sc_in<sc_int<32>> B;
	sc_in<sc_int<1>> B_valid;
	sc_in<sc_int<1>> Cin;
	sc_in<sc_int<1>> Cin_valid;
	sc_in<sc_int<1>> result_ready;
	sc_in<sc_int<1>> Cout_ready;

	////// adder output port //////
	sc_out<sc_int<32>> result;
	sc_out<sc_int<1>> result_valid;
	sc_out<sc_int<1>> Cout;
	sc_out<sc_int<1>> Cout_valid;
	sc_out<sc_int<1>> A_ready;
	sc_out<sc_int<1>> B_ready;
	sc_out<sc_int<1>> Cin_ready;

	void adder_main();

	SC_CTOR(adder) {
		SC_CTHREAD(adder_main, clk.pos());
	}
};

void adder::adder_main(void) {
	////// initialize handshake signal //////
	A_ready.write(0);
	B_ready.write(0);
	Cin_ready.write(0);
	result_valid.write(0);
	Cout_valid.write(0);
	wait();

	while (1) {
		////// adder module local variable //////
		sc_int<32> input_A;
		sc_int<32> input_B;
		sc_int<1> input_Cin;
		sc_int<32> output_result;
		sc_int<1> output_Cout;

		////// wait until valid input A, B, Cin //////
		do {
			wait();
		} while (!A_valid.read() || !B_valid.read() || !Cin_valid.read())
			;
		input_A = A.read();
		input_B = B.read();
		input_Cin = Cin.read();

		////// send ACK to tb //////
		A_ready.write(1);
		B_ready.write(1);
		Cin_ready.write(1);
		wait();

		////// Cin //////
		if (input_Cin != 0) {
			output_result = input_A + input_B + 1;
		}
		else {
			output_result = input_A + input_B;
		}

		////// calculate cout //////
		if ((input_A != 0 || input_B != 0) && (output_result == 0)) {
			output_Cout = 1;
		}
		else {
			output_Cout = 0;
		}

		////// Adder delay //////
		for (int i = 0; i < DELAY; i++) {
			wait();
		}

		////// initialize ready signal ///////
		A_ready.write(0);
		B_ready.write(0);
		Cin_ready.write(0);

		////// send output to tb //////
		result.write(output_result);
		Cout.write(output_Cout);
		result_valid.write(1);
		Cout_valid.write(1);

		////// wait ready signal from tb //////
		do {
			wait();
		} while (!result_ready.read() || !Cout_ready.read())
			;

		////// initialize valid signal ///////
		result_valid.write(0);
		Cout_valid.write(0);
		wait();
	}

}