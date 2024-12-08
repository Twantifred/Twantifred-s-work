#include <nvboard.h>
#include "Vtop.h"

void nvboard_bind_all_pins(Vtop* top) {
	nvboard_bind_pin( &top->clk, 1, BTNC);
	nvboard_bind_pin( &top->din, 8, SW7, SW6, SW5, SW4, SW3, SW2, SW1, SW0);
	nvboard_bind_pin( &top->shamt, 2, SW15, SW14);
	nvboard_bind_pin( &top->data, 8, LD7, LD6, LD5, LD4, LD3, LD2, LD1, LD0);
}
