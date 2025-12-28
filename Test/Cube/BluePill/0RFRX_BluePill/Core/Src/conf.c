#include "conf.h"

/*
PINOUT
M1 Left
M2 Right

A Peripherals

PA0 PWM2Ch1
PA1 PWM2Ch2


PA3 M1 Front
PA4 M1 Back

PA5 M2 Front
PA6 M2 Back

PB0 LEDs Left
PB1 LEDs Right

PB5 PWM SERVOMOTOR TIMER3

PA8 CSN
PA9 IRQ
PA10 CE

PB[13:15] SPI
PB13 SCK
PB14 MISO
PB15 MOSI

*/
#define LeftFront 3
#define LeftBack 4
#define RightFront 5
#define RightBack 6
#define PI	3.14159265358979

// Definir buzer y nrf




/*--- Clase Carro ----*/

    /*
    Valor minimo 200 -45
    Valor maximo 280 +45
    TIM3 servo motor direccion
    TIM2 mmo
    */


void CAR_Leds(void){
    GPIOB->ODR &= ~(3<<0);
    // Checar si es menor que 235
    if(TIM3->CCR1>115)
        GPIOB->ODR |= (1<<0);
    // Checar si es menor que 245
    if(TIM3->CCR1 < 125)
        GPIOB->ODR |= (1<<1);

}


/* Motor Functions */
void CAR_Speed(uint16_t speed){
    if(speed>100)speed=100;
    TIM2->CCR1= (65535*speed)/100; //DUTYCYCLE 50% CHANEL1
    TIM2->CCR2= (65535*speed)/100; //DUTYCYCLE 50% CHANEL2
}


void CAR_Front(void){
    GPIOA->ODR &=(0<<RightBack)|(0<<LeftBack);
    GPIOA->ODR |=(1<<RightFront)|(1<<LeftFront);
}
void CAR_Back(void){
    GPIOA->ODR &=(0<<RightFront)|(0<<LeftFront);
    GPIOA->ODR |=(1<<RightBack)|(1<<LeftBack);
}

void CAR_Direction(float angle){//HAY QUE LIMITAR EL RANGO DE ANGULOS CON UN IF
    if(((PI/4 > angle) ||(-PI/4 < angle)))
        return;
    uint8_t mangol =101 + (uint8_t)(40*angle+20*PI)/PI;
    TIM3->CCR2= mangol;
}

/*----- Configuration -----*/
void confRCC(void){
                /*    IOB,    IOA,   AF*/
    RCC->APB2ENR |= (1<<3)|(1<<2)|(1<<0); // GPIOA GPIOB

    RCC->APB1ENR |= (1<<0) | (1<<1); //TIM2 y TIM3
}
void confGPIO(void){

    GPIOA->CRL &= ~(0x0FFFF0FF);
    /*A0 A1 Outs pushpull MaxSpeed*/
    GPIOA->CRL |= (2<<6)|(3<<4)|(2<<2)|(3<<0);
    /*A3 A4 A5 A6 Output PushPull */
    GPIOA->CRL |= (3<<24)|(3<<20)|(3<<16)|(3<<12);
    // AF01 Se configuran

    //GPIOB
    GPIOB->CRL &= ~(0xF000FF);
    GPIOB->CRL |= (2<<22)|(3<<20)|(3<<4)|(3<<0);
    AFIO->MAPR |= (2<<10);
    //PB4 AF2 TIM3 CH1



}

void confTIMER(void)
{

    TIM2->PSC = 1;
                /* CH1      CH2*/
    TIM2->CCMR1 |=(6<<4)|(6<<12); // Enable PWM
    //TIM2->CCMR1 |=(1<<3)|(1<<11); // Preload
    TIM2->CCER  |=(1<<0) |(1<<4);//ENABLECHANEL1

    TIM2->ARR  =65535; /*8.2 mS*/

    TIM2->CCR1= 0;//DUTYCYCLE 50% CHANEL1
    TIM2->CCR2 =0;//pwm duty cycle 50% CHANEL 2
    TIM2->CR1 |= (1<<0);

	/*CH1    CH2*/

    TIM3->PSC = 99;
                /* CH1*/
    TIM3->CCMR1 |=(6<<12); // Enable PWM
    TIM3->CCER  |=(1<<4);//ENABLE CHANEL 2
    /*
    1.25 - 1.75 ms
    Servo
    2 ms 90
    1 ms -90

    20 ms  50hz PERIOD
    */
    TIM3->ARR  =1599; /*8k/50*/

    TIM3->CCR2= 120;//1600*1.5/20 => 120  7.5% 0°
    TIM3->CR1 |= (1<<0);

    /*
    Valor minimo 100 -45
    Valor maximo 140 +45
    */

}


void delay_ms(uint32_t delay){
	for(uint32_t i=0;i<delay;i++){
	for(uint32_t j=0;j<11500;j++);
	}
}


void config(void){
    confRCC();
    confGPIO();
    confTIMER();
    CAR_Speed(50);
}
