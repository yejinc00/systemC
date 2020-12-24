#pragma once
#include <systemc.h>
#include "adder.h"
#include "tb.h"

SC_MODULE(SYSTEM) {
	/////// make pointer of module tb and adder ///////
	tb *tb0;
	adder *adder0;

	/////// signal for connect module tb and adder ///////
	sc_signal<sc_int<32>> sig_A;
	sc_signal<sc_int<1>> sig_A_valid;
	sc_signal<sc_int<1>> sig_A_ready;

	sc_signal<sc_int<32>> sig_B;
	sc_signal<sc_int<1>> sig_B_valid;
	sc_signal<sc_int<1>> sig_B_ready;

	sc_signal<sc_int<1>> sig_Cin;
	sc_signal<sc_int<1>> sig_Cin_valid;
	sc_signal<sc_int<1>> sig_Cin_ready;

	sc_signal<sc_int<1>> sig_Cout;
	sc_signal<sc_int<1>> sig_Cout_valid;
	sc_signal<sc_int<1>> sig_Cout_ready;

	sc_signal<sc_int<32>> sig_result;
	sc_signal<sc_int<1>> sig_result_valid;
	sc_signal<sc_int<1>> sig_result_ready;

	/////// period : 1ns ///////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		/////// for waveform ///////
		sc_trace_file *wf = sc_create_vcd_trace_file("adder");
		if (wf == NULL)
			cout << "wf err\n" << endl;
		wf->set_time_unit(100, SC_PS);

		/////// main port signal ///////
		sc_trace(wf, clk_sig, "clk");
		sc_trace(wf, sig_A, "A");
		sc_trace(wf, sig_B, "B");
		sc_trace(wf, sig_Cin, "Cin");
		sc_trace(wf, sig_Cout, "Cout");
		sc_trace(wf, sig_result, "result");

		/////// handshake signal ///////
		sc_trace(wf, sig_A_valid, "A_valid");
		sc_trace(wf, sig_A_ready, "A_ready");
		sc_trace(wf, sig_B_valid, "B_valid");
		sc_trace(wf, sig_B_ready, "B_ready");
		sc_trace(wf, sig_Cin_valid, "Cin_valid");
		sc_trace(wf, sig_Cin_ready, "Cin_ready");
		sc_trace(wf, sig_Cout_valid, "Cout_valid");
		sc_trace(wf, sig_Cout_ready, "Cout_ready");
		sc_trace(wf, sig_result_valid, "result_valid");
		sc_trace(wf, sig_result_ready, "result_ready");

		/////// connecting tb and adder ///////
		tb0 = new tb("tb0");
		tb0->clk(clk_sig);
		tb0->result(sig_result);
		tb0->result_valid(sig_result_valid);
		tb0->Cout(sig_Cout);
		tb0->Cout_valid(sig_Cout_valid);
		tb0->A_ready(sig_A_ready);
		tb0->B_ready(sig_B_ready);
		tb0->Cin_ready(sig_Cin_ready);

		tb0->A(sig_A);
		tb0->A_valid(sig_A_valid);
		tb0->B(sig_B);
		tb0->B_valid(sig_B_valid);
		tb0->Cin(sig_Cin);
		tb0->Cin_valid(sig_Cin_valid);
		tb0->result_ready(sig_result_ready);
		tb0->Cout_ready(sig_Cout_ready);

		adder0 = new adder("adder0");
		adder0->clk(clk_sig);
		adder0->A(sig_A);
		adder0->A_valid(sig_A_valid);
		adder0->B(sig_B);
		adder0->B_valid(sig_B_valid);
		adder0->Cin(sig_Cin);
		adder0->Cin_valid(sig_Cin_valid);
		adder0->result_ready(sig_result_ready);
		adder0->Cout_ready(sig_Cout_ready);

		adder0->result(sig_result);
		adder0->result_valid(sig_result_valid);
		adder0->Cout(sig_Cout);
		adder0->Cout_valid(sig_Cout_valid);
		adder0->A_ready(sig_A_ready);
		adder0->B_ready(sig_B_ready);
		adder0->Cin_ready(sig_Cin_ready);

	}

	/////// Destructor ///////
	~SYSTEM() {
		delete tb0;
		delete adder0;
	}
};