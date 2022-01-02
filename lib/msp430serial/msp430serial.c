
#include <msp430.h>
#include "msp430serial.h"

//////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the UART interface
/// 
//////////////////////////////////////////////////////////////////////////////
/// UCA0MCTL Register 
/// Bits  7  6  5  4 | 3  2  1 | 0
///        UCBFRX      UCBRSX    UCOS16
///       0  0  0  0   0  0  1   0    ->   9600 Baud,8,N,1 / DCO  1 Mhz
///       1  0  1  1   0  0  0   1    -> 115200 Baud,8,N,1 / DCO 16 Mhz
//////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////
/// @brief Receive an unsigned char via interrupt via the UART module UCA0
/// 
//////////////////////////////////////////////////////////////////////////////
#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_receive_ISR(void)
{
  while (!(IFG2 & UCA0RXIFG));                // Wait until the character is received ...
  rcv = UCA0RXBUF;                            // and read it out
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Send a char via the UART module UCA0 
/// 
//////////////////////////////////////////////////////////////////////////////
#ifdef SEND_CHAR
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

//////////////////////////////////////////////////////////////////////////////
/// @brief Send a char via the UART module UCA0 
/// 
//////////////////////////////////////////////////////////////////////////////
void UART_send_string(const char* str)
{
  while (*str != 0)                           // Solange nicht das Terminiert-Zeichen kommt ...
  {
    P1OUT ^= BIT0;
    while (!(IFG2 & UCA0TXIFG));		          // Warten bis USART0 TX-Puffer frei (leer) ...
    UCA0TXBUF = *str++;                       // Zeichen dazufügen.-
  } 
}
