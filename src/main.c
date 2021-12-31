//////////////////////////////////////////////////////////////////////////////
/// @file main.c
/// @author Kai R. 
/// @brief Example program for a UART transmission, the auxiliary functions 
///        itoaxx() or uitoaxx() can be used to output numbers.
/// 
/// @date 2021-12-23
/// @version 0.1
/// 
/// @copyright Copyright (c) 2021
/// 
//////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "msp430g2553.h"
#include "itoa16.h"
#include "itoa32.h"

// ------------------ Definitions ----------------------------------------------

#define VARIANTE3
#define DCO_01_MHZ
//#define DCO_16_MHZ

// ----------------- Gl. Variables --------------------------------------------- 
volatile unsigned char rcv;

// ------------------ PROTOTYPES -----------------------------------------------

void UART_init(void);                         // Initialize UART-Modul UCA0
void UART_send_string(const char* str);       // Send String via UCA0
#ifdef VARIANTE3
void UART_send_char(const char);
#else
void UART_send_int(long val);                 // Send integer (converted to string) via UCA0
#endif

// ------------------ PROGRAM --------------------------------------------------

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  P1DIR |= BIT0 + BIT6;     // Set LED Pins P1.0, P1.6 to output

#ifdef DCO_01_MHZ  
  BCSCTL1 = CALBC1_1MHZ;              // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;               // Set DCO to 1MHz
#elif defined DCO_16_MHZ
  BCSCTL1 = CALBC1_16MHZ;             // Set DCO to 16MHz
  DCOCTL = CALDCO_16MHZ;              // Set DCO to 16MHz
#endif

  //long count = 0x7FFFFFFA;
  //long count = 0x80000000;
  int count = 32750;

  UART_init();
   _EINT();                           // Enable Interrupts 

  while(1)                            
  { 
    if (rcv == 'a')                   // Have received an a?
    {
      rcv = 0x00;                     // Reset received data
      UART_send_string("Zahl: ");     // Send number
#ifdef VARIANTE3
      uitoa16nb(&UART_send_char, count++);
#else
      UART_send_int(count++);
#endif 
      UART_send_string("\n");
    } 
  } 
}

// UCA0MCTL Register 
// Bits  7  6  5  4 | 3  2  1 | 0
//        UCBFRX      UCBRSX    UCOS16
//       0  0  0  0   0  0  1   0    ->   9600 Baud,8,N,1 / DCO  1 Mhz
//       1  0  1  1   0  0  0   1    -> 115200 Baud,8,N,1 / DCO 16 Mhz

void UART_init(void) {
  P1SEL  = BIT1 + BIT2;               // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2;               // P1.1 = RXD, P1.2=TXD

  UCA0CTL1 |= UCSWRST;
  UCA0CTL1 |= UCSSEL_2;               // Use SMCLK

#ifdef DCO_01_MHZ
  UCA0BR0 = 104;          // Set baud rate to 9600 with 1MHz clock (MSP430x2xx Family-User_Guide 15.3.13)
  UCA0BR1 = 0;            // Set baud rate to 9600 with 1MHz clock
  UCA0MCTL = UCBRS0;      // Modulation UCBRFX = 0; UCBRSx = 1, UCOS16 = 0 
#elif defined DCO_16_MHZ
  UCA0BR0 = 8;            // Set baud rate to 115200 with 16MHz clock and UCOS16 Oversampling
  UCA0BR1 = 0;            // Set baud rate to 115200 with 16MHz clock (MSP430x2xx Family-User_Guide 15.3.13)
  UCA0MCTL = UCBRF_11 | UCBRS_0 | UCOS16;     // Modulationstage for 16Mhz, Oversampling (UCOS16=1)
                                              // UCBRFX only in Oversamplingmode else 0
#endif
  UCA0CTL1 &= ~UCSWRST;   // Initialize USCI state machine
  IE2 |= UCA0RXIE;        // Enable USCI_A0 RX interrupt
}

//
// Receive an unsigned char via interrupt via the UART module UCA0 des
// MSP430G2553.
//
#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_receive_ISR(void)
{
  while (!(IFG2 & UCA0RXIFG));                // Wait until the character is received ...
  rcv = UCA0RXBUF;                            // and read it out
}

//
// Send a string via the UART module UCA0 of the MSP430G2553 
//
#ifdef VARIANTE3
void UART_send_char(const char digit)
{
    while (!(IFG2 & UCA0TXIFG));              // Warten bis USART0 TX-Puffer frei (leer) ...
    UCA0TXBUF = digit;
}
#else
void UART_send_int(long int val)
{
  char strBuf[12];
  
  //char* wpStrBuf = itos(strBuf, val);
  char* wpStrBuf = uitoa16(strBuf,val);
  while (*wpStrBuf > 0) {
    while (!(IFG2 & UCA0TXIFG));              // Warten bis USART0 TX-Puffer frei (leer) ...
    UCA0TXBUF = *wpStrBuf++;
  }
}
#endif

//
// Sende einen String über UART-Modul UCA0 des MSP430G2553. 
//
void UART_send_string(const char* str)
{
  while (*str != 0)                           // Solange nicht das Terminiert-Zeichen kommt ...
  {
    P1OUT ^= BIT0;
    while (!(IFG2 & UCA0TXIFG));		          // Warten bis USART0 TX-Puffer frei (leer) ...
    UCA0TXBUF = *str++;                       // Zeichen dazufügen.-
  } 
}
