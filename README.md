# MSP430 itoa / uitoa function for 32 bit (long) integer

This PlatformIO project contains four itoa funktions in two (16/32 Bit) variants. The functions that begin with a "u"
are used to convert unsigned int variables.

The itoa16 / itoa32 functions have an output range from -32768 to 32767 / -2.147.483.648 to 2.147.483.647

The functions that begin with a "u" are used to convert unsigned int variables.  
Its output range ist 0 to 65535 / 0 to 4.294.967.295.

## Functions that require a character buffer

* char\* itoa16 (char* strBuf, int16_t val)
* char\* uitoa16 (char* strBuf, uint16_t val>)

* char\* itoa32 (char* strBuf, int32_t val)
* char\* uitoa32 (char* strBuf, uint32_t val>)

These functions expect a pointer to a string buffer as a parameter, which saves the integer value
that is also to be transferred as an ASCII character string. This can then be output to a serial terminal,
e.g. via a UART interface.

## Functions that do not require a character buffer

* void itoa16nb( fP putChar, int16_t val);
* void uitoa16nb( fP putChar, uint16_t val);

* void itoa32nb( fP putChar, int32_t val);
* void uitoa32nb( fP putChar, uint32_t val);

These functions expect a pointer to a function which processes a char character as a parameter.  
The integer value that is to be converted into ASCII characters must also be transferred as the second parameter.  
  
No character buffer is required with this variant. The transferred function can be used for the  
direct output / transfer of the ascertained ASCII digits. However, it can also be used  
independently of the two functions.  

## Required auxiliary function

* void strrev(char* pLeft, uint8_t r_len);

This function is required for the first four itoa functions that use a string buffer. Because the ASCII characters  
are generated in reverse order, the characters must be rearranged with the help of this function.  
  
## main.c
  
The main.c file contains a sample code to demonstrate the itoa32 function.
A serial console (9600,8,N,1) is required through which the letter "a" must be entered.
If the input is recognized, the program outputs a number converted by itoa32
into an ASCII string on the serial console.
