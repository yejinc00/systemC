#include <systemc.h>
#include "adder.h"
#include "SYSTEM.h"
#include "tb.h"

SYSTEM *top = NULL;

int sc_main(int argc, char*argv[]) {
	sc_set_time_resolution(1, SC_PS);
	top = new SYSTEM("top");			// system top module instantiation
	sc_start();							// simulation start
	return 0;
}