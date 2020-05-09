#include "usart2.h"

void USART2_Init(void)
{
// USART Pins: PD5 = USART2_TX, PD6 = USART2_RX

	//*************************** Initialize Timer Registers ***************************
	//Enable peripheral clock for USART2
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

  //Selct the clock source as System Clock (01) for USART2
	RCC->CCIPR &= 0b00<<(2*2);
	RCC->CCIPR |= 0b01<<(2*2);
	
//*************************** Initialize the GPIOs ***************************
	//Enable the Peripheral Clock of Port D 
  RCC->AHB2ENR |= 0b1<<(4);
	
  //Initialize UART2 GPIOs PD5 and PD6 as alternate functions (10)
	GPIOD->MODER &= 0b00<<(5*2); 
	GPIOD->MODER |= 0b10<<(5*2);
	
	GPIOD->MODER &= 0b00<<(6*2); 
	GPIOD->MODER |= 0b10<<(6*2);	
	
	//Set Output Speed as High (11)
	GPIOD->OSPEEDR &= 0b00<<(5*2);
	GPIOD->OSPEEDR |= 0b11<<(5*2);
	
	//Select the correct AFR for PD5 and PD6 (AFR7 = 0111 = UART2)
	GPIOD->AFR[0] &= 0b0000<<(5*4);
	GPIOD->AFR[0] |= 0b0111<<(5*4);
	
	GPIOD->AFR[0] &= 0b0000<<(6*4);
	GPIOD->AFR[0] |= 0b0111<<(6*4);
	
//*************************** Initialize Timer Registers ***************************
	//Disable USART2 (UE = 0)
	USART2->CR1 &= ~USART_CR1_UE;
	
	//Configure USART2 in 8 bit mode (M1 = 0, M0 = 0)
	//Word Length = 8, No Parity bit, 1 Start bit, 1 Stop bit
	USART2->CR1 &= ~USART_CR1_M0;
	USART2->CR1 &= ~USART_CR1_M1;

	//1 Stop bit (STOP0 = 0, STOP1 = 0)
	USART2->CR2 &= ~USART_CR2_STOP_0;
	USART2->CR2 &= ~USART_CR2_STOP_1;
	
	//Parity Control Disabled
	USART2->CR1 &= ~USART_CR1_PCE;
	
	//Configure USART2 Oversampling mode (by 16 = 0)
	USART2->CR1 &= ~USART_CR1_OVER8;
	
	//Parity Control Disabled
	USART2->CR1 &= ~USART_CR1_PCE;
	
	//Configure Baudrate = 9600 
	//Baudrate = FClk/ USARTDIV
	//USARTDIV =  80,000,000/9600 = 8333.333 = 8333 = 0x208D
  USART2->BRR = 0x208D;

	//UART2 Enable DMA (Direct Memory Access) for Transmittion (1) and Reception (1)
  USART2->CR3 = USART_CR3_DMAR;
  USART2->CR3 = USART_CR3_DMAT;	

	//UART Transmitter Enable
	USART2->CR1 |= USART_CR1_TE;
	
	//UART Receiver Enable
	USART2->CR1 |= USART_CR1_RE;
	
	//Clear Transmission Complete Flag (1)
	USART2->ICR |= USART_ICR_TCCF; 
	
	//Clear Parity Error Flag (1)
	USART2->ICR |= USART_ICR_PECF; 
	
	//Clear Overrun Error Flag (1)
	USART2->ICR |= USART_ICR_ORECF; 	
		
	//Enable USART2 (UE = 1)
	USART2->CR1 |= USART_CR1_UE;
	
  //Wait while USART2 Transmittion and Reception are ready
	while(~(USART2->ISR & USART_ISR_TEACK) & ~(USART2->ISR & USART_ISR_REACK));
}

void USART2_Write()
{
	
}

void USART2_Read(void)
{
	
}

void USART2_IRQ_Handler(void)
{
	
}
