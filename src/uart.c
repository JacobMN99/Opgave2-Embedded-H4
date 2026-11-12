/*
 * uart.c
 *
 * Created: 15-01-2018 09:44:48
 *  Author: ltpe
 */


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ProjectDefines.h"
#include "uart/uart.h"

// Funktion som UART0's modtage-interrupt kalder med hvert modtaget tegn (NULL = ingen)
static Function_Pointer_With_One_CharPointer_Parameter Callback_Function_Pointer = NULL;

// Ringbuffer til tegn modtaget på UART0.
// Interruptet skriver ved Head, main-løkken læser ved Tail. Indeksene er 8 bit,
// så de læses/skrives i én instruktion og kan deles sikkert mellem ISR og main.
static volatile char    RxBuffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t RxHead = 0;
static volatile uint8_t RxTail = 0;

// =====================================================================
// UART0
// =====================================================================

 void Uart0_Init(void)
 {
	 // Set baud rate
	 UBRR0L = (unsigned char)(MyUBRR & 0xff);
	 UBRR0H = (unsigned char)(MyUBRR>>8);
	 #ifdef UART_NORMAL_MODE
	 UCSR0A = 0x00;
	 #else
	 UCSR0A = 1<<U2X0; // Set *2 mode
	 #endif
	 UCSR0B = (1<<RXEN0) | (1<<TXEN0);  //0x18 , enable receive and transmit
	 UCSR0C =  (1<<UCSZ01) | (1<<UCSZ00); // 8 data bit 1 stop bit
	 __init_stdout(uart_putch); // printf() skriver via uart_putch (UART0)
	 __init_stdin(uart_getch);  // scanf() læser via uart_getch (UART0)
 }

 void Enable_UART_Receive_Interrupt(void)
 {
	 UCSR0B |= (1 << RXCIE0); // Enable the USART Recieve Complete interrupt (USART_RXC)
 }

 void Disable_UART_Receive_Interupt(void)
 {
	UCSR0B &= ~(1 << RXCIE0); // Disable the USART Recieve Complete interrupt (USART_RXC)
 }

 void Uart0_PutChar(char ch)
 {
	 while( !(UCSR0A & (1<<UDRE0))) ; // Vent til senderegisteret er tomt
	 UDR0 = ch;
 }

 int uart_getch(FILE* stream)
 {
	 while( !(UCSR0A & (1<<RXC0)))  ; // do nothing but wait
	 return( UDR0);
 }

 int uart_putch(char ch, FILE* stream)
 {
	 Uart0_PutChar(ch);
	 return 0;
 }

 uint8_t Uart0_GetReceivedChar(char *ch)
 {
	 if (RxHead == RxTail)
	 {
		 return 0; // Bufferen er tom
	 }
	 *ch = RxBuffer[RxTail];
	 RxTail = (RxTail + 1) & (UART_RX_BUFFER_SIZE - 1);
	 return 1;
 }

 ISR(USART0_RX_vect)
 {
	 char ReceivedByte;
	 ReceivedByte = UDR0;
	 if (Callback_Function_Pointer != NULL)
	 {
		 Callback_Function_Pointer(&ReceivedByte); // fx ConvertReceivedChar() i main.c
	 }

	 // Læg tegnet i ringbufferen. Main-løkken sender det videre på UART0 og UART1.
	 // Er bufferen fuld, smides tegnet væk.
	 uint8_t NextHead = (RxHead + 1) & (UART_RX_BUFFER_SIZE - 1);
	 if (NextHead != RxTail)
	 {
		 RxBuffer[RxHead] = ReceivedByte;
		 RxHead = NextHead;
	 }
}

void SetupUARTFunctionCallbackPointer(Function_Pointer_With_One_CharPointer_Parameter
                                      Function_To_Callback)
{
	Callback_Function_Pointer = Function_To_Callback;
}

// =====================================================================
// UART1  (TXD1 = PB3, RXD1 = PB4 -> CP2102)
// =====================================================================

void Uart1_Init(void)
{
	UBRR1L = (unsigned char)(MyUBRR & 0xff);
	UBRR1H = (unsigned char)(MyUBRR>>8);
	#ifdef UART_NORMAL_MODE
	UCSR1A = 0x00;
	#else
	UCSR1A = 1<<U2X1; // Set *2 mode
	#endif
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);     // Enable receive and transmit
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);   // 8 data bit 1 stop bit
}

void Uart1_PutChar(char ch)
{
	while( !(UCSR1A & (1<<UDRE1))) ; // Vent til senderegisteret er tomt
	UDR1 = ch;
}

void Uart1_PutString(const char *str)
{
	while (*str != '\0')
	{
		Uart1_PutChar(*str++);
	}
}
