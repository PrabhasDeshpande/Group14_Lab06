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

int clock = 16000000;       // global variable

void GPIO_PORTF_INIT(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;  //enable clock to portf

    //GPIO_PORTF_LOCK_R = 0X4C4F434B;
    //GPIO_PORTF_CR_R = 0X1F;

    GPIO_PORTF_DIR_R = 0x0E;    //set correct directions

    GPIO_PORTF_DEN_R = 0x1F;    //enable digital operation at pins
    GPIO_PORTF_PUR_R = 0x11;    //enable pullups for switches
    // GPIO_PORTF_DATA_R =  0x0;    // put no data on GPIO



    /* ..........................................below part for interrupts..................................................... */



      GPIO_PORTF_IM_R  = 0x00;

     GPIO_PORTF_IS_R  |= (1<<4) ;   // sw2 level detection only
     GPIO_PORTF_IBE_R &= ~(1<<4);   // int gen controlled by IEV



     GPIO_PORTF_IEV_R &= ~(1<<4); // low level or falling edge triggers int

    //GPIO_PORTF_ICR_R = 0x01;   // clear int // not needed for level sensitive app

     GPIO_PORTF_IM_R  |= (1<<4);  // send int to controller




     //GPIO_PORTF_RIS_R this is set when interrupt occurs.

     //GPIO_PORTF_MIS_R this is set when int has reached int ctrll

    // GPIO_PORTF_ICR_R for level detect this has no effect



}


void delay(float time, int clock)
{

    float val;
    val = (time*clock)/1000;
    NVIC_ST_RELOAD_R = val;        // reload value
    NVIC_ST_CURRENT_R  = 0x0;         // current value
    NVIC_ST_CTRL_R = 0x05;          // enable and choice of clk

    while((NVIC_ST_CTRL_R & (1<<16)) == 0)
      {
            //do nothing
      }
     NVIC_ST_CTRL_R = 0x0;


}

void GPIO_PORTF_Handler(void)
{



         int state1 = GPIO_PORTF_DATA_R & (1<<4);

         delay(25,clock);
         int state12 = GPIO_PORTF_DATA_R & (1<<4);

         delay(375,clock);
         int state2 = GPIO_PORTF_DATA_R & (1<<4);

             if(state1 == state2){

              GPIO_PORTF_DATA_R ^= (0x04);
              }


          //delay(25,clock);


              if(state1 == state12){

                  GPIO_PORTF_DATA_R ^= 0x02;

              }



}


void pwm_gen(float Fdes)
{

    /*                  for output: PF2 :  Motion Control Module 1 PWM 6. This signal is
                                    controlled by Module 1 PWM Generator 3.                             */


   /* PWMCTL
    PWMSYNC
    PWMSTATUS---------> related to fault
    PWMPP ------------> provides info of pwm module */



    /*  PWM1_CTL_R = 0x08; //updates load register on nxt cycle for gen3
        PWM1_SYNC_R = 0x08; //reset counter of gen3 */

}


void main(void)
{
    NVIC_EN0_R = (1<<30); // enabling int on PORTF
    GPIO_PORTF_INIT();
    //int state0, state1;

    pwm_gen(100000);

    while(1)
    {



    }

}
