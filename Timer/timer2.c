#include "timer2.h"

uint16_t read_timer = 0;

void TIMER2_Init(void)
{
	//Enable the Peripheral Clock of Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; 		//Set bit 1 :: Port A
	
	// Enable the Clock for TIMER2
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	//Capture/Compare 1 output disable
	TIM2->CCER &= ~TIM_CCER_CC1E;
	
	// Prescaller for Timer2 Clock :: fCK_PSC / (PSC[15:0] + 1)
	// 1 MHz = 80 MHz / (PSC + 1)
	// PSC = 79
	TIM2->PSC = 79;
	
	//Reinitialize the counter register
	TIM2->EGR |= TIM_EGR_UG;
	
	//Wait while the registers are being updated
	while((TIM2->SR & TIM_SR_UIF) != 1);
	
	//Reset the update interrupt flag
	TIM2->SR &= ~TIM_SR_UIF;
	
	//Timer2 Auto Reload Register
	TIM2->ARR = 0xFFFFFFFF;
	
	//Timer Count Direction (Up = 0)
	TIM2->CR1 &= ~TIM_CR1_DIR;
	
	//Make PA0 as Input (00)
	GPIOA->MODER &= ~GPIO_MODER_MODER0_1;
	GPIOA->MODER &= ~GPIO_MODER_MODER0_0;
	
	//Make PA0 as Alternate Function (10)
	GPIOA->MODER |= GPIO_MODER_MODER0_1;
	GPIOA->MODER &= ~GPIO_MODER_MODER0_0;
	
	// Select Alternate function of PA0 as Timer2 Channel1
	GPIOA->AFR[0] |= 0x01;	
	
	//Capture/Compare 1 selection
  //CC1 channel is configured as input, IC1 is mapped on TI1 (01)	
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S_1;
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;
		
	//Disable Input Capture Filter
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1F;

  //CC1 channel configured as input for trigger or capture operations. 
  //noninverted/rising edge	(00)
	TIM2->CCER &= ~TIM_CCER_CC1NP;
	TIM2->CCER &= ~TIM_CCER_CC1P;

  //Update Interrupt Enable (1)
	TIM2->DIER |= TIM_DIER_UIE;
	
	//Capture/Compare 1 interrupt enable (1)
	TIM2->DIER |= TIM_DIER_CC1IE;
	
	//Capture/Compare 1 output enable (1)
	TIM2->CCER |= TIM_CCER_CC1E;
	
	//Enable the Counter (1)	
	TIM2->CR1 |= TIM_CR1_CEN;
}
