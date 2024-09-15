/*
 *
 *
 * Part 1:

Create a PWM waveform with frequency = 100KHz and variable duty cycle.

The program should begin with d = 50%.

On pressing one switch the duty should be increased by 5% and on pressing other switch it should be decreased by 5%.


Part 2:

Implement the same but using only 1 switch (SW1 OR SW2) – short press for d increase and long press for decrease.
 *
 *
 *
 *
 * */



#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>


void GPIO_PORTF_INIT(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;  //enable clock to portf

    //GPIO_PORTF_LOCK_R = 0X4C4F434B;
    //GPIO_PORTF_CR_R = 0X1F;

    GPIO_PORTF_DIR_R = 0x0E;    //set correct directions

    GPIO_PORTF_DEN_R = 0x1F;    //enable digital operation at pins
    GPIO_PORTF_PUR_R = 0x11;    //enable pullups for switches



}


void systick_pwm_gen(float d, int clock, float Ttot,int state)
{

    float T = (d*Ttot)/100;        // calculating on time for diven duty cycle

    float reload_val = T*clock;    // calculating reload value for timer

    NVIC_ST_RELOAD_R = reload_val;       // reload value
    NVIC_ST_CURRENT_R  = 0x0;     // current value
    NVIC_ST_CTRL_R = 0x05;       // enable and choice of clk



    while((NVIC_ST_CTRL_R & (1<<16)) == 0)
      {

          GPIO_PORTF_DATA_R = state;
      }



     NVIC_ST_CTRL_R = 0x0;


}



void main(void)
{
    GPIO_PORTF_INIT();

    int clock = 16000000;   // on chip clock @ 16Mhz

    int Fdes = 1; //00000;       // Desired frequency

    float Ttot = 1/Fdes;    // total time period of wave

    float D = 50;           // default duty cycle of 50%

    while(1)
    {

        systick_pwm_gen(D,clock,Ttot,0x02);
        systick_pwm_gen(D,clock,Ttot,0x00);

    }




    //return 0;
}
