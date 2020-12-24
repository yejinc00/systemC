#pragma once
#include <systemc.h>

SC_MODULE(tb) {
	////// Testbench input port //////
	sc_in<bool> clk;
	sc_in<sc_int<32>> result;
	sc_in<sc_int<1>> result_valid;
	sc_in<sc_int<1>> Cout;
	sc_in<sc_int<1>> Cout_valid;
	sc_in<sc_int<1>> A_ready;
	sc_in<sc_int<1>> B_ready;
	sc_in<sc_int<1>> Cin_ready;

	////// Testbench output port //////
	sc_out<sc_int<32>> A;
	sc_out<sc_int<1>> A_valid;
	sc_out<sc_int<32>> B;
	sc_out<sc_int<1>> B_valid;
	sc_out<sc_int<1>> Cin;
	sc_out<sc_int<1>> Cin_valid;
	sc_out<sc_int<1>> result_ready;
	sc_out<sc_int<1>> Cout_ready;

	void tb_main();					// write input data to rca & get result data from rca
	sc_int<32> get_data(char *);	// file data processing

	SC_CTOR(tb) {
		SC_CTHREAD(tb_main, clk.pos());
	}
};

/////// data processing ///////
sc_int<32> tb::get_data(char *data) {
	sc_int<32> result_data;
	int binary[255];

	/////// get valid data from text ///////
	int i = 0, k = 0;
	while (data[i] != '\n') {
		if (data[i] == '0') {
			binary[k] = 0;
			k++;
		}
		else if (data[i] == '1') {
			binary[k] = 1;
			k++;
		}
		i++;
	}

	/////// convert to decimal data ///////
	int position = 0;
	unsigned int decimal = 0;
	for (int i = sizeof(binary) / sizeof(int); i >= 0; i--) {
		if (binary[i] == 1) {
			decimal += 1 << position;
		}
		position++;
	}
	result_data = decimal;
	return result_data;
}

/////// use in source and sink function //////
sc_int<32> input_A;
sc_int<32> input_B;
sc_int<1> input_Cin;
sc_int<32> input_result;
sc_int<1> input_Cout;

sc_int<32> adder_result;
sc_int<1> adder_Cout;

sc_time t1;
sc_time t2;

/////// write data to rca module ///////
void tb::tb_main() {
	/////// read data from Simulation.txt ///////

	char line[256];
	int binary[256];
	int end_signal = 0;
	FILE *fp = NULL;
	fp = fopen("./Simulation.txt", "r");
	if (fp == NULL) {
		cout << "failed to read file" << endl;
		exit(0);
	}

	cout << "Module: <Model name> [<Simulation time>]" << endl;
	cout << "A:<space><32bit, divide by under bar every 4bits>" << endl;
	cout << "B:<space><32bit, divide by under bar every 4bits>" << endl;
	cout << "Cin:<space><1bit>" << endl;
	cout << "Result:<space><32bit, divide by under bar every 4bits>" << endl;
	cout << "Cout:<space><1bit>" << endl;
	cout << "File_Result:<space><32bit, divide by under bar every 4bits>" << endl;
	cout << "File_Cout:<space><1bit>" << endl;
	cout << "Compare with file: <Correct/Wrong>" << endl;
	cout << "<Next line>" << endl;


	/////// initialize ///////
	A_valid.write(0);
	B_valid.write(0);
	Cin_valid.write(0);
	result_ready.write(0);
	Cout_ready.write(0);
	wait();

	int num = 0;
	while (end_signal != 1)
	{
		/////// processing of each line (simulation.txt) ///////
		for (int i = 0; i < 6; i++) {
			fgets(line, sizeof(line), fp);
			if (i == 0) {
				input_A = get_data(line);
			}
			else if (i == 1) {
				input_B = get_data(line);
			}
			else if (i == 2) {
				if (line[0] == '0') {
					input_Cin = 0;
				}
				else if (line[0] == '1')
					input_Cin = 1;
			}
			else if (i == 3) {
				input_result = get_data(line);
			}
			else if (i == 4) {
				if (line[0] == '0') {
					input_Cout = 0;
				}
				else if (line[0] == '1')
					input_Cout = 1;
			}
			else if (i == 5) {
				if (line[0] != '<') {
					end_signal = 1;
				}
			}
		}

		/////// send data to adder ///////
		A.write(input_A);
		A_valid.write(1);
		B.write(input_B);
		B_valid.write(1);
		Cin.write(input_Cin);
		Cin_valid.write(1);

		/////// wait until adder get data ///////
		do {
			wait();
		} while (!A_ready.read() || !B_ready.read() || !Cin_ready.read())
			;
		A_valid.write(0);
		B_valid.write(0);
		Cin_valid.write(0);
		wait();

		/////// wait until result and cout is valid data ///////
		do {
			wait();
		} while (!result_valid.read() || !Cout_valid.read())
			;

		/////// get adder's result ///////
		adder_result = result.read();
		adder_Cout = Cout.read();

		result_ready.write(1);
		Cout_ready.write(1);
		wait();
		result_ready.write(0);
		Cout_ready.write(0);

		/////// processing data to print console ///////
		sc_bv<32> b_A = input_A;
		sc_bv<32> b_B = input_B;
		sc_bv<32> b_Result = adder_result;
		sc_bv<32> b_File_Result = input_result;

		sc_uint<1> decimal_cin = 0;
		sc_uint<1> decimal_cout = 0;
		sc_uint<1> decimal_File_cout = 0;
		if (input_Cin == -1)
			decimal_cin = 1;
		if (adder_Cout == -1)
			decimal_cout = 1;
		if (input_Cout == -1)
			decimal_File_cout = 1;

		////// get simulation time //////
		if (num == 0)
			t1 = sc_time_stamp();
		else {
			t2 = sc_time_stamp();
			t2 = t2 - t1;
			t1 = t2;
		}


		/////// print to console ///////
		cout << "Module: RCA [";			// RCA, CLA, CSA 
		if (num == 0)
			cout << t1;
		else
			cout << t2;
		cout << "]" << endl;
		cout << "A: ";
		for (int i = 7; i > -1; i--) {
			cout << b_A.range((i * 4) + 3, i * 4);
			if (i != 0)
				cout << "_";
			if (i == 0)
				cout << endl;
		}
		cout << "B: ";
		for (int i = 7; i > -1; i--) {
			cout << b_B.range((i * 4) + 3, i * 4);
			if (i != 0)
				cout << "_";
			if (i == 0)
				cout << endl;
		}
		cout << "Cin: " << decimal_cin << endl;
		cout << "Result: ";
		for (int i = 7; i > -1; i--) {
			cout << b_Result.range((i * 4) + 3, i * 4);
			if (i != 0)
				cout << "_";
			if (i == 0)
				cout << endl;
		}
		cout << "Cout: " << decimal_cout << endl;
		cout << "File_Result: ";
		for (int i = 7; i > -1; i--) {
			cout << b_File_Result.range((i * 4) + 3, i * 4);
			if (i != 0)
				cout << "_";
			if (i == 0)
				cout << endl;
		}
		cout << "File_cout: " << decimal_File_cout << endl;
		cout << "Compare with file: ";

		if ((adder_result == input_result) && (adder_Cout == input_Cout))
			cout << "Correct";
		else
			cout << "Wrong";
		cout << endl << endl;
		num++;
	}

	fclose(fp);				// close file
	sc_stop();				// end of simulation
}