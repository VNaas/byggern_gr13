#include "timer.h"
#include "sam/sam3x/include/sam.h"
#include "sam/interrupt.h"
#include "sam/interrupt/interrupt_sam_nvic.h"

#define F_CPU 84E6 // MCK = 84MHz
#define US 1E6     // microseconds in a second

// Count number of microseconds counted by the timer. Ensures we can wait any period, as LOAD register holds up to 0x00FFFFFF.
static int count = 0;

/**
 * @brief starts a 1 us timer.
 *
 */
void systick_timer_init()
{
    // reset SysTick counter value
    /* period of N processor clock cycles, use a RELOAD value of N-1.*/
    SysTick->LOAD = (int)((F_CPU / US) - 1) << SysTick_LOAD_RELOAD_Pos;

    // set highest interrupt priority
    NVIC_SetPriority(SysTick_IRQn, 0);

    // set SysTick timer to MCK
    SysTick->CTRL |= 1 << SysTick_CTRL_CLKSOURCE_Pos;
    // Enable interrupt
    SysTick->CTRL |= 1 << SysTick_CTRL_TICKINT_Pos;

    // Reset current value
    SysTick->VAL = 0;
    // Enable timer
    SysTick->CTRL |= 1 << SysTick_CTRL_ENABLE_Pos;
    NVIC_EnableIRQ((IRQn_Type) SysTick_IRQn);
}

void _delay_us(int us)
{
    count = us;
    systick_timer_init();
    while (count != 0)
    {
    }
}

void _delay_ms(int ms)
{
    printf("delaying\n\r");
    count = 1000 * ms;
    printf("count = %d\n\r", count);
    systick_timer_init();
    while (count != 0)
    {
    }
}

/* Decrements counter. If reached zero, disable systick clock */
void Systick_handler()
{
    printf("systick interrupt");
    if (count != 0)
    {
        count--;
    }
    else
    {
        SysTick -> CTRL = (SysTick ->CTRL) & ~(SysTick_CTRL_ENABLE_Msk);
    }
    NVIC_ClearPendingIRQ(SysTick_IRQn);
}