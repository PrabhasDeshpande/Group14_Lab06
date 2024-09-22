//Lab 6 part 2

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"


int duty;

void GPIO_PORTF_setup(void);
void PWM_setup(void);
void single_switch(void);

volatile int i=0;



void GPIO_PORTF_setup(void)
{
    SYSCTL_RCGCGPIO_R |= (1<<5);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_DIR_R = 0x0E;

    GPIO_PORTF_IM_R &= ~0x11;
    GPIO_PORTF_IS_R &= ~0x11;
    GPIO_PORTF_IBE_R &= ~0x11;
    GPIO_PORTF_IEV_R &= ~0x11;


    GPIO_PORTF_ICR_R |= 0x11;
    GPIO_PORTF_IM_R |= 0x11;

    NVIC_PRI7_R &= 0xFF3FFFFF;

    NVIC_EN0_R |= 1 << 30;
}

void PWM_setup(void)
{
    SYSCTL_RCGCPWM_R |= (1<<1);
    GPIO_PORTF_AFSEL_R |= (1<<2);
    GPIO_PORTF_PCTL_R |= 0x500;


    PWM1_3_CTL_R |= 0x00;
    PWM1_3_GENA_R = 0x8C;
    PWM1_3_LOAD_R = 160;
    PWM1_3_CMPA_R = (duty/100)*160 - 1;
    PWM1_3_CTL_R |= 0x01;
    PWM1_ENABLE_R |= 0x040;
}

void main(void)

{
    GPIO_PORTF_setup();
    PWM_setup();
    duty=50;
    PWM1_3_CMPA_R = (160 * duty) / 100;
    while(1){
        //do nothing
    }
}

void Portf_interrupt_handler(void)
{
    i=0;
    while((GPIO_PORTF_DATA_R & 0x01)==0){
     i++;
    }
    if (GPIO_PORTF_MIS_R & 0x10)    //switch 1
        {

        }
        else if (GPIO_PORTF_MIS_R & 0x01)    //switch 2
        {
            if (i >= 900000)
            {
                if (duty > 5)
                {
                    duty = duty - 5;
                }
                else
                {
                    duty = 5;
                }
            }
            else if (i >= 0 && i <= 900000)
            {
                if (duty < 100 - 5)
                {
                    duty = duty + 5;
                }
                else
                {
                    duty = 100;
                }
            }
        }
        PWM1_3_CMPA_R = (160 * duty) / 100;

    int j;

    for(j = 0; j <1600*100/4; j++){}
    GPIO_PORTF_ICR_R = 0x11;

}
