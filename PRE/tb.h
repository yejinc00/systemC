#pragma once
#include <systemc.h>
#include <string.h>

SC_MODULE(tb) {
	///// testbench input port /////
	sc_in<bool> clk;
	sc_in<bool> DAT_in_ready;

	///// testbench output port /////
	sc_out<sc_int<16>> DAT_in;
	sc_out<bool> DAT_in_valid;

	void tb_main();							// testbench main function

	SC_CTOR(tb) {
		SC_CTHREAD(tb_main, clk.pos());
	}
};

void tb::tb_main() {
	///// get data from input.txt /////
	char line[128];
	FILE *fp = NULL;
	fp = fopen("./input.txt", "r");
	if (fp == NULL) {
		cout << "failed to open file" << endl;
		exit(0);
	}

	sc_int<16> i_tok;
	for (int i = 0; i < 4; i++) {
		fgets(line, sizeof(line), fp);
		char* tok = strtok(line, " ");

		while (tok != NULL) {
			i_tok = atoi(tok);
			
			DAT_in.write(i_tok);
			DAT_in_valid.write(1);

			do {
				wait();
			} while (!DAT_in_ready.read())
				;
			DAT_in_valid.write(0);

			tok = strtok(NULL, " ");
		}
	}
	
}