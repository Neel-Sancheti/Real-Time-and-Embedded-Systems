#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "system_init.h"
#include "led_init.h"
#include "timer2.h"
#include "UART.h"
#include <math.h>

static short post_success = 0;
static int count = 0;
static int read_all_values[101];
static uint16_t lower_limit = 950;
int post_count = 0;

char RxComByte = 0;
uint8_t buffer[BufferSize];
//int user_value = 0;
char temp_str;

void take_inputs(void);
int read(void);


int main(void)
{
	System_Init(); //Initialize system clock to 80MHz
	LED_Init();  //Initialize Red and Green LEDs 
	Green_LED_off(); //Turn off green led
	Red_LED_off(); //Turn off red led
	UART2_Init(); //Initialize UART2_Init
	TIMER2_Init(); //Initialize Timer2
	
	temp_str = sprintf((char *)buffer, "**********************************\r\n\tStarting POST\r\n**********************************\r\n");
	USART_Write(USART2, buffer, temp_str);  //Write on serial port
	
	for(int i=0;i<8000000;i++);
	
	while(1)
	{
		if(post_success == 1) //Read the period value only if POST is successful
		{
			if(TIM2->SR & TIM_SR_CC1IF && count<1001) //If timer flag is set and 1000 pulses have not been scanned
			{	
				TIM2->SR |= TIM_SR_CC1IF; //Set the timer flag
				TIM2->CNT &= 0x00000000; //reset the timer count register
				
				//Reading TIMER2 data automatically clears the CC1IF flag
				uint16_t read_timer = *(volatile uint16_t *) & TIM2->CCR1;
				
				count = count + 1;
				read_timer -= lower_limit;	
				
				if(read_timer <= 100) //If valid timer count? Increament the respective pulsewidth count value
				{
					read_all_values[read_timer]++;
				}	
			}
			if(count==1000) //If 1000 pulses have been scanned? Display the values on the serial port with non zero counts
			{
				TIM2->SR |= TIM_SR_CC1IF; //Set the timer flag
				TIM2->CNT &= 0x00000000; //reset the timer count register

				temp_str = sprintf((char *)buffer, "\r\n\n**********************************\r\n  Pulse Width Spread Table:\r\n**********************************\r\n"); //array with non zero elements;
			    USART_Write(USART2, buffer, temp_str); 
				for(int i = 0; i<=100; i++)
				{
					if(read_all_values[i] != 0)
					{
						temp_str = sprintf((char *)buffer, "%d\t", i + lower_limit);
						temp_str += sprintf((char *)buffer + temp_str, "%d\r\n", read_all_values[i]);
						USART_Write(USART2, buffer, temp_str);
					}						
				}
				temp_str = sprintf((char *)buffer, "**********************************\r\n"); //array with non zero elements;
   			    USART_Write(USART2, buffer, temp_str); 
				Red_LED_on(); //turn on red led
				take_inputs(); //take next operation commands from user
			}
		}
		else if(post_success == 0) //If POST is unsuccessful?
		{			
			if(TIM2->SR & TIM_SR_CC1IF && post_count < 10) //if timer flag is set and 10 pulses have not been scanned
			{	
				TIM2->SR |= TIM_SR_CC1IF; //Set the timer flag
				TIM2->CNT &= 0x00000000; //reset the timer count register

				//Reading TIMER2 data automatically clears the CC1IF flag
				uint32_t read_timer = *(volatile uint32_t *) & TIM2->CCR1;

				post_count++;

				if(read_timer <= 100000) //If counter value is less than 1msec.
				{
					count++; //increment count variable
				}	
			}
			else
			{
				if(post_count == 10 && (post_count - count) <= 2) //if 10 pulses have been scanned and the valid periods are more than 8?
				{
					count = -2;
					post_success = 1;
					post_count = 0;
					Green_LED_on(); //Green led on					
					temp_str = sprintf((char *)buffer, "**********************************\r\n\tPOST Successful\r\n**********************************\r\n");
					USART_Write(USART2, buffer, temp_str); // write on serial port
					
					//Displaying lower and upper limits
					temp_str = sprintf((char *)buffer, "\r\nLower Limit = %d\t", lower_limit);
					temp_str += sprintf((char *)buffer + temp_str, "Upper Limit = %d\r\n", (lower_limit + 100));
					USART_Write(USART2, buffer, temp_str);
					
					take_inputs(); //take next operation commands from user
				}
				else if(post_count == 10 && (post_count - count) > 2) //If number of valid periods are less than 8?
				{
					Red_LED_on(); //Red led on
					temp_str = sprintf((char *)buffer, "**********************************\r\n\tPOST Failed\r\n**********************************\r\n\r\n\r\nDo you want to try again? (Yes = 1; No = 0)\r\n");
					USART_Write(USART2, buffer, temp_str); ///serial write
					if(read() == 1) //if user entered 1
					{
						post_count = count = post_success = 0;
					}
					else 
					{
						temp_str = sprintf((char *)buffer, "\r\nTHANK YOU!\r\n"); //
						USART_Write(USART2, buffer, temp_str);
						break;
					}
				}
			}
		}
	}
}

void take_inputs(void) //take user input from serial port
{
	temp_str = sprintf((char *)buffer, "\r\nDo you want to run: \r\n0 - Yes, Same Specifications as previous \r\n1 - Yes, Different Specification \r\n2 - No\r\n"); //
	USART_Write(USART2, buffer, temp_str); //serial write

	int read_value = read();
	if(read_value == 1) //if user entered 1
	{	
		count = -2;
		memset(read_all_values, 0, sizeof(read_all_values)); //Clear the array "read_all_values[]"
		temp_str = sprintf((char *)buffer, "\r\nEnter the Lower Limit:\r\n"); //
		USART_Write(USART2, buffer, temp_str); //write on serial port
		lower_limit = read(); // read lower limit data from serial port
		
		//Displaying lower and upper limits
		temp_str = sprintf((char *)buffer, "\n\rLower Limit = %d\t", lower_limit);
		temp_str += sprintf((char *)buffer + temp_str, "Upper Limit = %d\r\n\nPress Enter to Continue", (lower_limit + 100));
		USART_Write(USART2, buffer, temp_str);
		
		//wait while 'Enter Key' not pressed
		while(1)
		{
			if(USART_Read(USART2) == '\r') 
				break;
		}	
	}
	else if(read_value == 0) //if user entered 0
	{
		count = -2;
		
		memset(read_all_values, 0, sizeof(read_all_values)); //Clear the array "read_all_values[]"
		
		//Displaying lower and upper limits
		temp_str = sprintf((char *)buffer, "\r\nLower Limit = %d\t", lower_limit);
		temp_str += sprintf((char *)buffer + temp_str, "Upper Limit = %d\r\nPress Enter to Continue", (lower_limit + 100));
		USART_Write(USART2, buffer, temp_str);
		
		//wait while 'Enter Key' not pressed
		while(1)
		{
			if(USART_Read(USART2) == '\r') 
				break;
		}	
	}
	else if(read_value >= 2) //if user entered 2 or more?
	{
		temp_str = sprintf((char *)buffer, "\r\nTHANK YOU!\r\n"); //
		USART_Write(USART2, buffer, temp_str);
		post_success = 2;
	}
}

int read (void)
{
	int rx_byte[10];
	int rx_value = 0;
	int i;
	int j;
	int temp;
	
	for(i = 0; i<=10; i++)
	{
		rx_byte[i] = USART_Read(USART2);
		if(rx_byte[i] == '\r') //If 'enter' key is not pressed
		{
			
			temp = 0;
			for(j = i-1; j>=0; j--) //logic to convert mulitple digit inputs to an single interger value
			{
				rx_value = rx_value + (rx_byte[temp] - 48)*pow(10,j);
				temp = temp +1;
			}
			memset(rx_byte, 0, sizeof(rx_byte)); //Clear the array "read_all_values[]"
			return(rx_value);
		}
		else
		{
			temp_str = sprintf((char *)buffer, "%d",(rx_byte[i] - 48)); //Echo the user entered value on the terminal
			USART_Write(USART2, buffer, temp_str);
		}
	}
	return(2);
}
