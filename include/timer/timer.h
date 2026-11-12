/**
 * @file timer.h
 * @author jma
 * @date 2026-11-05
 * @brief Timer 1 - initialisering og interrupt
 */
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "ProjectDefines.h"  // F_CPU og TimerLedBit

// Sættes til 1 af Timer 1-interruptet ved hvert timeout.
// Hovedløkken skal selv nulstille flaget igen, når den har reageret på det.
// "volatile" fordi variablen ændres i et interrupt og læses i main.
extern volatile uint8_t Timer1_Flag;

// Initialiseringsfunktion til Timer 1 med dynamisk millisekund-beregning
// (maks. ca. 1048 ms ved 16 MHz og prescaler 256)
void Timer1_Init_ms(uint16_t timeout_ms);

#endif // TIMER_H
