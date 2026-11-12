/*
 * uart.h
 *
 * Created: 15-01-2018 09:45:07
 *  Author: ltpe
 *
 * Driver til begge UART'er i ATmega328PB:
 *   UART0 (PD0 = RXD0, PD1 = TXD0) -> PC via mEDBG's virtuelle COM-port
 *   UART1 (PB4 = RXD1, PB3 = TXD1) -> PC via CP2102 USB-til-seriel-modul
 */


#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <stdint.h>
#include "ProjectDefines.h"  // F_CPU og Function_Pointer_With_One_CharPointer_Parameter
#define BAUD 9600UL

// Størrelse på modtagebufferen til UART0. Skal være en potens af 2 (2, 4, 8, ..., 128),
// fordi indekset regnes rundt med "& (størrelse - 1)".
#define UART_RX_BUFFER_SIZE 64

#define UART_NORMAL_MODE
#ifdef UART_NORMAL_MODE
#define MyUBRR (unsigned int)(F_CPU/(16*BAUD) -1)   // normal mode
#else
#define MyUBRR (unsigned int)(F_CPU/(8*BAUD) -1)   // *2 mode
#endif

// ---------- UART0 ----------
extern void Uart0_Init(void);               // 9600 baud, 8N1. printf() skriver på UART0.
extern void Enable_UART_Receive_Interrupt(void);
extern void Disable_UART_Receive_Interupt(void);
extern void Uart0_PutChar(char ch);

// Henter næste modtagne (og konverterede) tegn fra UART0's modtagebuffer.
// Returnerer 1 hvis der var et tegn (lagt i *ch), ellers 0.
extern uint8_t Uart0_GetReceivedChar(char *ch);

// Bruges af printf()/scanf() (stdout/stdin = UART0)
extern int uart_getch(FILE* stream);
extern int uart_putch(char ch, FILE* stream);

// Registrerer en funktion, som UART0's modtage-interrupt kalder med hvert modtaget tegn,
// inden tegnet lægges i modtagebufferen. Funktionen må gerne ændre tegnet.
extern void SetupUARTFunctionCallbackPointer(Function_Pointer_With_One_CharPointer_Parameter
                                             Function_To_Callback);

// ---------- UART1 ----------
extern void Uart1_Init(void);               // 9600 baud, 8N1
extern void Uart1_PutChar(char ch);
extern void Uart1_PutString(const char *str);

#endif /* UART_H_ */
