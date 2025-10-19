#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../libc/function.h"

u32 tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    UNUSED(regs);
}

void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT divisor value: hardware clock at PIT_FREQUENCY Hz */
    u32 divisor = PIT_FREQUENCY / freq;
    u8 low  = low_8(divisor);
    u8 high = high_8(divisor);
    
    /* Send the command byte and divisor */
    port_byte_out(PIT_COMMAND_PORT, PIT_MODE_SQUARE_WAVE);
    port_byte_out(PIT_DATA_PORT, low);
    port_byte_out(PIT_DATA_PORT, high);
}

