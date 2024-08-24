#include "stm32f446xx.h"

#define DOUT 	1
#define LED_PIN 5
#define RCC ((RCC_TypeDef *) RCC_BASE)
#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)


void RCC_init()
{
	uint32_t tmp;
	tmp = RCC->CR;

	tmp |= ((uint32_t)RCC_CR_HSION);
	RCC->CR = tmp;

    /* wait until HSI is ready */
   	while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
/* Reset SWS bits and assign HSI as system clock source */
  	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    /* Wait till HSI is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS_HSI) != RCC_CFGR_SWS_HSI) {;} /* on one line! */
}


void GPIO_Init()
{
	//1
	RCC->AHB1ENR |= 1;
	//uint32_t bitmask = 0b11 << (LED_PIN * 2)

	GPIOA->MODER |= (1 << 10);
	GPIOA->MODER &= ~(1 << 11);

	GPIOA->OTYPER &= ~(1 << LED_PIN);

	GPIOA->OSPEEDR |= (1 << 10);
	GPIOA->OSPEEDR &= ~(1 << 11);

	GPIOA->PUPDR &= ~(1 << 10);
	GPIOA->PUPDR &= ~(1 << 11);
}


void toggle_led()
{
	//2
	GPIOA->ODR ^= (1 << LED_PIN);
}


void delay_msecs(uint32_t msec)
{
	uint32_t i;
	for (i = 0; i < 1000*msec ; i++) {
		asm("nop");
	}
}


int main()
{
	asm("nop");
	asm("nop");

	RCC_init();
	GPIO_Init();
	
	while(1){
		delay_msecs(1000);
		toggle_led();

	}

}
