#include <nvboard.h>
#include "Vtop.h"

void nvboard_bind_all_pins(Vtop* top) {
	nvboard_bind_pin( &top->A, 4, SW3, SW2, SW1, SW0);
	nvboard_bind_pin( &top->B, 4, SW7, SW6, SW5, SW4);
	nvboard_bind_pin( &top->ALUctr, 3, SW10, SW9, SW8);
	nvboard_bind_pin( &top->ALUout, 4, LD3, LD2, LD1, LD0);
}
