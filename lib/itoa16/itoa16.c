
//////////////////////////////////////////////////////////////////////////////
/// @file itoa16.c
/// @author Kai R. 
/// @brief Hilfsfunktionen zum Ausgeben von Integerwerten über eine serielle
///        Schnittstelle (Terminal). Diese Funktionen wurden hinsichtlich des 
///        Speicherverbrauchs optimiert und funktioniert für Werte bis 16 Bit
/// 
/// @date 2021-12-31
/// @version 1.0
/// 
/// @copyright Copyright (c) 2021
/// 
//////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "itoa16.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (signed) Integer Zahl in einen String um
/// 
/// @param buffer    Zeiger auf einen Puffer zum Speichern der ASCII-Daten
/// @param val       Integer Wert der umgewandelt werden soll
//////////////////////////////////////////////////////////////////////////////

char* itoa16(char* const buffer, int16_t val) {
  char* wpBuffer = buffer;
  uint8_t negative = 0;
  
  if (!(val & (INT16_MAX))) {                   // Overflow 65536? Abfangen, sonst Zeichensalat!
    val = 0;                                    // INT32_MAX Konstante aus <stdint.h>
  }
  
  if (val < 0) {                                
    val = -val;                                 // Zweierkomplement wenn Wert negativ ist.
    negative = 1;
  }
  
  do {                                          // Integerzahl in einzelne ASCII-Ziffern umwandeln                                      
    *wpBuffer++ = (char)(val % BASE) + '0';     // Durch das wiederholte dividieren / 10 wird der 
  } while (val /=  BASE);                       // String verkehrt herum aufgebaut
  
  if (negative) {                               // Zahlen sind konvertiert nun das '-' Zeichen
    *wpBuffer++ = '-';                          // setzen wenn nötig
  }
  *wpBuffer = '\0';                             // String mit NULL abschließen
  strrev(buffer, (wpBuffer-buffer-1));          // Drehe String um. Berechne Stringlänge aus Zeigersubtraktion
  return buffer;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (unsigned) Integer Zahl in einen String um
/// 
/// @param buffer    Zeiger auf einen Puffer zum Speichern der ASCII-Daten
/// @param val       Integer Wert der umgewandelt werden soll
//////////////////////////////////////////////////////////////////////////////

char* uitoa16(char* const buffer, uint16_t val) {
  char* wpBuffer = buffer;

  do {                                          // gleiches Verfahren wie bei itos()                                      
    *wpBuffer++ = (char)(val % BASE) + '0';                         
  } while ((val /= BASE));
  *wpBuffer = '\0';                           
  strrev(buffer, (wpBuffer-buffer-1));  
  return buffer;                        
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (signed) 16 Bit Integer Zahl in einen String um und 
///        gibt das Zeichen mit Hilfe der übergebenen Funktion aus
/// 
/// @param putChar 
/// @param val 
//////////////////////////////////////////////////////////////////////////////

void itoa16nb( fP putChar, int16_t val) {
  static const int16_t divisor[5] = {1,10,100,1000,10000};
  uint8_t divIdx = 0;

  if (val < 0) {               // ist die Zahl negativ?
     putChar('-');             // negatives Vorzeichen ausgeben
     val = -val;               // Zweierkomplement a
  }
  while( (val >= (divisor[divIdx+1])) && (divIdx < MAX_DIVIDX)) { divIdx++; }
     
  do {                               // Schleife über alle  Dez Stellen
     if(!divIdx) {                   // wenn die 1er behandelt werden
        putChar('0' + (char)(val));  
     }
     else {                          // bei allen Stellen ausser bei den 1ern
        putChar('0' + (char)(val / divisor[divIdx]));  // val/dec Der Quotient (digit) an die Output Funktion schicken
        val  %= divisor[divIdx];     // Ursprungswert auf den Rest der Division setzen
     }
  } while (divIdx--);                // Weiter mit der nächsten Dez Stelle 
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (unsigned) 16 Bit Integer Zahl in einen String um und 
///        gibt das Zeichen mit Hilfe der übergebenen Funktion aus
/// 
/// @param putChar 
/// @param val 
//////////////////////////////////////////////////////////////////////////////

void uitoa16nb( fP putChar, uint16_t val) {
    uint16_t divisor = 1;

    while (val/divisor > BASE) { divisor *= BASE; }

    do {                                        // Schleife über alle  Dez Stellen
     if(divisor == 1) {                         // wenn die 1er behandelt werden
        putChar('0' + (char)(val));
     }
     else {                                    // bei allen Stellen ausser bei den 1ern
        putChar('0' + (char)(val / divisor));  // val/dec Der Quotient (digit) an die Output Funktion schicken
        val  %= divisor;                       // Ursprungswert auf den Rest der Division setzen
     }
     divisor /= BASE;
  } while (divisor);
}
#ifdef __cplusplus
}
#endif //__cplusplus