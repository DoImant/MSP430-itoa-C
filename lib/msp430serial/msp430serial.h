
#ifndef _MSP430SERIAL_H_
#define _MSP430SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

// ------------------ Definitions ----------------------------------------------

#define SEND_CHAR
#define DCO_01_MHZ
//#define DCO_16_MHZ

// ----------------- Gl. Variables --------------------------------------------- 
volatile unsigned char rcv;

//----------------- Forward declartions --------------------------------------------- 

void UART_init(void);
__interrupt void UART_receive_ISR(void);
#ifdef SEND_CHAR
void UART_send_char(const char);
#else
void UART_send_int(long val);                 // Send integer (converted to string) via UCA0
#endif
void UART_send_string(const char* str);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif