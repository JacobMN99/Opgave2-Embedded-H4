/*
 * ProjectDefines.h
 *
 * Created: 15-01-2018 14:56:37
 *  Author: ltpe
 */

#ifndef PROJECTDEFINES_H_
#define PROJECTDEFINES_H_

#define F_CPU 16000000UL  // Antager 16 MHz krystal (ret hvis din kører med f.eks. 8 MHz)

#include <avr/io.h>
#include <avr/interrupt.h>

#if defined (_AVR_IOM168_H_) || defined (_AVR_IOM328P_H_)
#define USR0_Vect_Num USART_RX_vect

#endif

#if defined (_AVR_IOM2560_H_) || defined (_AVR_ATMEGA328PB_H_INCLUDED)
#define USR0_Vect_Num USART0_RX_vect

#endif

#define GreenLedBit		PORTB3
#define YellowLedBit    PORTB2
#define RedLedBit		PORTB1

// LED'en som Timer 1 toggler hvert sekund.
// OBS: PB3 er TXD1 (UART1's sendeben) på ATmega328PB, så den grønne LED på PB3
// kan ikke bruges, når UART1 er slået til. Vi bruger derfor den gule LED på PB2.
#define TimerLedBit		YellowLedBit

typedef void (*Function_Pointer_With_One_CharPointer_Parameter)(char *);

#endif /* PROJECTDEFINES_H_ */
