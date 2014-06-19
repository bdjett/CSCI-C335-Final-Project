/**************
 * f3d_uart.c
 *
 * Created by: Brian Jett (bdjett) and Adam Combs (addcombs)
 * Created on: 14 February 2014
 * Last modified by: Brian Jett (bdjett)
 * Last modified on: 6 March 2014
 * Assignment: Lab 5
 */

#include <stm32f30x.h>
#include <f3d_led.h>
#include <f3d_uart.h>
#include <queue.h>

queue_t rxbuf;
queue_t txbuf;

void f3d_uart_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
	GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 ,&USART_InitStructure);
	USART_Cmd(USART1 , ENABLE);
	
	// Initialize the rx and tx queues
	init_queue(&rxbuf);
	init_queue(&txbuf);
	
	// Setup the NVIC priority and subpriority
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the RX interrupt 
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

void USART1_IRQHandler(void) {
	int ch;
	
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
		ch = USART_ReceiveData(USART1);
		while (!enqueue(&rxbuf,ch));
	}
	if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
		ch = dequeue(&txbuf);
		if (ch) {
			USART_SendData(USART1,ch);
		}
		else {
			USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
		}
	}
}

// Put chars on the UART
int putchar(int c) {
	if (!queue_empty(&txbuf)) {
		flush_uart();
	}
	while(!enqueue(&txbuf, c));	
	return 0;
} 

// Get chars from UART
int getchar(void) {
	int data;
	while (!(data = dequeue(&rxbuf)));
	return data;
}

void flush_uart(void) {
	USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
}

// get char from UART without waiting for input
int getchar_nb(void) {
	int c;
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET) {
		// no character received
		return 0;
	} else {
		// character received
		c = USART_ReceiveData(USART1);
		return c;
	}
}

// Sends string of data with putchar
void putstring(char *s) {
	// iterate through string and call putchar on each character
	while(*s != '\0') {
		putchar(*s);
		s++;
	}
}

/* f3d_uart.c ends here */
