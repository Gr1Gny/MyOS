#ifndef TIMER_H
#define TIMER_H

#include "types.h"

/* PIT (Programmable Interval Timer) constants */
#define PIT_FREQUENCY 1193180   /* Base frequency of the PIT in Hz */
#define PIT_COMMAND_PORT 0x43   /* PIT command register port */
#define PIT_DATA_PORT 0x40      /* PIT channel 0 data port */
#define PIT_MODE_SQUARE_WAVE 0x36 /* Channel 0, lobyte/hibyte, rate generator */

void init_timer(u32 freq);

#endif
