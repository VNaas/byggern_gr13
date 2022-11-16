#include "timer.h"
#include "sam/sam3x/include/sam.h"
#include "sam/interrupt.h"
#include "sam/interrupt/interrupt_sam_nvic.h"
#include "LED.h"

#define F_CPU 84E6 // MCK = 84MHz
#define US 1E6     // microseconds in a second

// Count number of microseconds counted by the timer. Ensures we can wait any period, as LOAD register holds up to 0x00FFFFFF.
static uint64_t us_ticks = 0;

/**
 * @brief starts a 1 us timer.
 *
 */
void systick_timer_init()
{
    // reset SysTick counter value
    /* period of N processor clock cycles, use a RELOAD value of N-1.*/
    SysTick->LOAD = (uint32_t)((F_CPU / US) - 1) << SysTick_LOAD_RELOAD_Pos;

    // set highest interrupt priority
    NVIC_SetPriority(SysTick_IRQn, 0);

    // Reset current value
    SysTick->VAL = 0;

    // set SysTick timer to MCK
    // Enable interrupt
    // Enable timer
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk  |   // Set clock source
                    SysTick_CTRL_TICKINT_Msk    |   // enable interrupts
                    SysTick_CTRL_ENABLE_Msk;        //Enable
    // SysTick->CTRL |= 1 << SysTick_CTRL_ENABLE_Pos;
    NVIC_EnableIRQ((IRQn_Type) SysTick_IRQn);
}

void _delay_us(int us)
{
    us_ticks = us;
    systick_timer_init();
    while (us_ticks != 0)
    {
    }
}

void _delay_ms(int ms)
{
    // printf("delaying\n\r");
    us_ticks = 1000 * ms;
    // printf("count = %d\n\r", us_ticks);
    systick_timer_init();
    while (us_ticks){}
    LED_toggleGreen();
}

/* Decrements counter. If reached zero, disable systick clock */
void SysTick_Handler( void )
{
    if (us_ticks != 0)
    {
        us_ticks--;
    }
    else
    {
        SysTick -> CTRL = (SysTick ->CTRL) & ~(SysTick_CTRL_ENABLE_Msk  |
                                                SysTick_CTRL_TICKINT_Msk);
        NVIC_DisableIRQ(SysTick_IRQn);
    }
    NVIC_ClearPendingIRQ(SysTick_IRQn);
}

// void _delay_count(int us)
// {
//     us_ticks = us*5000;
//     while (us_ticks != 0)
//     {
//         us_ticks--;
//     }
// }