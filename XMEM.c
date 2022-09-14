#include "XMEM.h"
#include "macros.h"
#include <avr/io.h>

void XMEM_init(void){
    set_bit(MCUCR,SRE);     // Enable externam memory interface
    set_bit(SFIOR,XMM2);    // Mask off JTAG pins (Release PC7-PC4)
}