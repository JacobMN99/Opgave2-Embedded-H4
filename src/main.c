/**
 * @file main.c
 * @author jma
 * @date 2026-11-09
 * @brief Main function - Opgave 2: kommunikation på både UART0 og UART1
 */
#include "timer/timer.h"
#include "uart/uart.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Forskellen på et stort og et lille bogstav i ASCII er netop bit 5 (0x20),
// fx 'A' = 0x41 og 'a' = 0x61.
#define Upper_Lower_Bit_Value 0x20

// Kaldes fra UART0-modtage-interruptet (via callback) med hvert modtaget tegn.
// Intervallerne går til 0x5D/0x7D, så de danske 7-bit tegn [\] <-> {|}
// (Æ Ø Å <-> æ ø å) også byttes.
void ConvertReceivedChar(char *ReceivedChar)
{
	// Den smarte måde at få konverteret små bogstaver om til store bogstaver og
	// modsat er ved brug af Xor, som vi tidligere har set. Så kan vi klare det i
	// én linje kode.
	if ( ((*ReceivedChar >= 0x41) && (*ReceivedChar <= 0x5D)) ||
	     ((*ReceivedChar >= 0x61) && (*ReceivedChar <= 0x7D)))
	{
		*ReceivedChar = *ReceivedChar ^ Upper_Lower_Bit_Value;
	}
}

int main(void) {
    uint32_t SecondsSinceStartup = 0;
    char     TextBuffer[40];
    char     ch;

    DDRB |= (1 << TimerLedBit);  // Sæt LED-benet (PB2) som output

    // Timer 1 giver et interrupt hvert sekund
    Timer1_Init_ms(1000);

    // UART0 (mEDBG / PC): 9600 baud, 8N1. Modtage-interruptet konverterer
    // store/små bogstaver og lægger tegnene i en buffer.
    Uart0_Init();
    SetupUARTFunctionCallbackPointer(ConvertReceivedChar);
    Enable_UART_Receive_Interrupt();

    // UART1 (CP2102): 9600 baud, 8N1
    Uart1_Init();

    // Aktivér globale interrupts
    sei();

    printf("UART0 klar\r\n");
    Uart1_PutString("UART1 klar\r\n");

    while (1) {
        // 1) Timer-flaget er sat af Timer 1-interruptet -> et sekund er gået
        if (Timer1_Flag) {
            Timer1_Flag = 0;  // Husk at resette flaget!
            SecondsSinceStartup++;

            snprintf(TextBuffer, sizeof(TextBuffer),
                     "Sekunder efter opstart: %lu\r\n",
                     (unsigned long)SecondsSinceStartup);
            Uart1_PutString(TextBuffer);
        }

        // 2) Send alle konverterede tegn fra UART0 ud på både UART0 og UART1
        while (Uart0_GetReceivedChar(&ch)) {
            Uart0_PutChar(ch);
            Uart1_PutChar(ch);
        }
    }
}
