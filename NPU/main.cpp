#include <systemc.h>
#include "Pixel_Memory.h"
#include "Externel_DRAM.h"
#include "IDP_Manager.h"
#include "SYSTEM.h"

SYSTEM *top = NULL;

int sc_main(int argc, char* argv[]) {
	sc_set_time_resolution(1, SC_PS);
	top = new SYSTEM("top");
	sc_start();
	return 0;
}
