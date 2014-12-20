#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"

uint8_t value = GPIO_PIN_1;

#define STATE_T0H 0
#define STATE_T0L 1
#define STATE_T1H 2
#define STATE_T1L 3
uint8_t state = STATE_T0H;

void Timer0IntHandler(void){

    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, value);
    value ^= GPIO_PIN_1;
	// Clear the timer interrupt.
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	if (state) {
        ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, 32);
        state = 0;
    }
    else {
        ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, 14);
        ++state;
    }
    
}

int main(void)
{
    //Set the clock
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    //Enable PortF
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //LED Pins as outputs
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);



    // Enable the peripherals used by this example.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

    // Enable processor interrupts.
	ROM_IntMasterEnable();

    // Configure the two 32-bit periodic timers.
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, 4000000); //ROM_SysCtlClockGet());

    // Setup the interrupts for the timer timeouts.
    IntRegister(INT_TIMER0A, Timer0IntHandler);
	ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable the timers.
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    while(1)
    {
        //Interrupts do everything
    }

}
