//Lab 6 part 1

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"



/*
 *          Part 1:

Create a PWM waveform with frequency = 100KHz and variable duty cycle.

The program should begin with d = 50%.

On pressing one switch the duty should be increased by 5% and on pressing other switch it should be decreased by 5%.
 *
 *
 *
 *
 *
 *
 *
 *
 * */




#define frequency 100000 //100 kHz frequency
#define time_period (16000000/frequency)
#define del_duty 15 //Change in duty cycle for each button press
volatile int duty;




void GPIOFHandler(void);        // GPIO Int Handler fxn


void GPIO_PORTF_setup(void);    // setting GPIO
void PWMConfig(void);
void dual_switch(void);





void GPIO_PORTF_setup(void)
{
    SYSCTL_RCGCGPIO_R |= (1<<5);        /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x1F;             /* make PORTF0 configurable */
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_DIR_R = 0x0E;


    GPIO_PORTF_IM_R &= ~0x11; // mask interrupt by clearing bits
    GPIO_PORTF_IS_R &= ~0x11; // edge sensitive interrupts
    GPIO_PORTF_IBE_R &= ~0x11; // interrupt NOT on both edges
    GPIO_PORTF_IEV_R &= ~0x11; // rising edge trigger




    GPIO_PORTF_ICR_R |= 0x11; // clear any prior interrupt
    GPIO_PORTF_IM_R |= 0x11; // unmask interrupt by setting bits




    NVIC_PRI7_R &= 0xFF3FFFFF;    // Prioritize and enable interrupts in NVIC //

    NVIC_EN0_R |= 1 << 30;       // enable interrupt 30 (port F)

}

void PWMConfig(void)
{
    SYSCTL_RCGCPWM_R |= (1<<1);     // Enable PWM1 clock
    GPIO_PORTF_AFSEL_R |= (1<<2);   //Enable alternate function
    GPIO_PORTF_PCTL_R |= 0x500;     //Make PF2 as PWM output

    // Configure the PWM generator
    PWM1_3_CTL_R |= 0x00; // Disable PWM3 while configuring and select down count mode
    PWM1_3_GENA_R = 0x8C; // Set PWM3A to produce a symmetric down-counting PWM signal
    PWM1_3_LOAD_R = 160;
    PWM1_3_CMPA_R = (duty/100)*time_period - 1;
    PWM1_3_CTL_R |= 0x01; //Enable generator 3 counter
    PWM1_ENABLE_R |= 0x040; // Enable PWM1 channel 6 Output
}

void main(void)

{
    GPIO_PORTF_setup();
    PWMConfig();
    duty=50; //Initial duty cycle is 50%
    PWM1_3_CMPA_R = (time_period * duty) / 100; //50% duty cycle to PWM upon start up
    while(1){
        //do nothing
    }
}

void GPIOFHandler(void)
{
    dual_switch();

    int wait;
    for(wait = 0; wait <1600*1000/4; wait++){}           //Debounce Delay of 0.25seconds

    GPIO_PORTF_ICR_R = 0x11;
    GPIO_PORTF_IM_R |= 0x11;
}

void dual_switch(void)
{
    GPIO_PORTF_IM_R &= ~0x11;

        if(GPIO_PORTF_RIS_R & 0x10)    //SW 2
        {
            if (duty < 90)
                   {
                       duty = duty + del_duty;
                   }
            if (duty >= 90){
                duty = 90;
            }
        }
        if (GPIO_PORTF_RIS_R & 0x01)    //SW 1
        {
            if (duty > 5)
                   {
                       duty = duty - del_duty;
                   }
            if (duty <= 5){
                duty = 5;
            }
        }
        if (GPIO_PORTF_RIS_R & 0x11)    //Both switches pressed at a time
        {
            duty = duty;
        }
        PWM1_3_CMPA_R = (time_period * duty) / 100;
}
