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
#include "msp430serial.h"
#include "itoa16.h"
#include "itoa32.h"

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
#ifdef SEND_CHAR
      uitoa16nb(&UART_send_char, count++);
#else
      UART_send_int(count++);
#endif 
      UART_send_string("\n");
    } 
  } 
}
