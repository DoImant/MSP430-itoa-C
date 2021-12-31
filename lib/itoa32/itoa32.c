
//////////////////////////////////////////////////////////////////////////////
/// @file itoa32.c
/// @author Kai R. 
/// @brief Hilfsfunktionen zum Ausgeben von Integerwerten über eine serielle
///        Schnittstelle (Terminal). Diese Funktionen wurden hinsichtlich des 
///        Speicherverbrauchs optimiert und funktionieren für Werte bis 32 Bit
/// 
/// @date 2021-12-24
/// @version 1.0
/// 
/// @copyright Copyright (c) 2021
/// 
//////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "itoa32.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (signed) Integer Zahl in einen String um
/// 
/// @param buffer    Zeiger auf einen Puffer zum Speichern der ASCII-Daten
/// @param val       Integer Wert der umgewandelt werden soll
//////////////////////////////////////////////////////////////////////////////

char* itoa32(char* const buffer, int32_t val) {
  char* wpBuffer = buffer;
  uint8_t negative = 0;
  
  if (!(val & (INT32_MAX))) {                   // Overflow 2147483648 ? Abfangen, sonst Zeichensalat!
    val = 0;                                    // INT32_MAX Konstante aus <stdint.h>
  }
  
  if (val < 0) {                                
    val = -val;                                 // Zweierkomplement wenn Wert negativ ist.
    negative = 1;
  }
  
  do {                                          // Integerzahl in einzelne ASCII-Ziffern umwandeln                                      
    *wpBuffer++ = (char)(val % BASE_L) + '0';     // Durch das wiederholte dividieren / 10 wird der 
  } while (val /=  BASE_L);                       // String verkehrt herum aufgebaut
  
  if (negative) {                               // Zahlen sind konvertiert nun das '-' Zeichen
    *wpBuffer++ = '-';                          // setzen wenn nötig
  }
  *wpBuffer = '\0';                             // String mit NULL abschließen
  strrev(buffer, (wpBuffer-buffer-1));  // Drehe String um. Berechne Stringlänge aus Zeigersubtraktion
  return buffer;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (unsigned) Integer Zahl in einen String um
/// 
/// @param buffer    Zeiger auf einen Puffer zum Speichern der ASCII-Daten
/// @param val       Integer Wert der umgewandelt werden soll
//////////////////////////////////////////////////////////////////////////////

char* uitoa32(char* const buffer, uint32_t val) {
  char* wpBuffer = buffer;

  do {                                          // gleiches Verfahren wie bei itos()                                      
    *wpBuffer++ = (char)(val % BASE_L) + '0';                         
  } while ((val /= BASE_L));
  *wpBuffer = '\0';                           
  strrev(buffer, (wpBuffer-buffer-1));  
  return buffer;                        
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (signed) 32 Bit Integer Zahl in einen String um und 
///        gibt das Zeichen mit Hilfe der übergebenen Funktion aus
/// 
/// @param putChar 
/// @param val 
//////////////////////////////////////////////////////////////////////////////

void itoa32nb( fP putChar, int32_t val) {
    uint32_t divisor = 1;

    if (val < 0) {              // ist die Zahl negativ?
      putChar('-');             // negatives Vorzeichen ausgeben
      val = -val;               // Zweierkomplement deine Lösung ist schöner
    }
    while (val/divisor > BASE_L) { divisor *= BASE_L; }

    do {                                        // Schleife über alle  Dez Stellen
     if(divisor == 1) {                         // wenn die 1er behandelt werden
        putChar('0' + (char)(val));  
     }
     else {                                    // bei allen Stellen ausser bei den 1ern
        putChar('0' + (char)(val / divisor));  // val/dec Der Quotient (digit) an die Output Funktion schicken
        val  %= divisor;                       // Ursprungswert auf den Rest der Division setzen
     }
     divisor /= BASE_L;
  } while (divisor);
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Wandelt eine (unsigned) 32 Bit Integer Zahl in einen String um und 
///        gibt das Zeichen mit Hilfe der übergebenen Funktion aus
/// 
/// @param putChar 
/// @param val 
//////////////////////////////////////////////////////////////////////////////

void uitoa32nb( fP putChar, uint32_t val) {
    uint32_t divisor = 1;

    while (val/divisor > BASE_L) { divisor *= BASE_L; }

    do {                                        // Schleife über alle  Dez Stellen
     if(divisor == 1) {                         // wenn die 1er behandelt werden
        putChar('0' + (char)(val));
     }
     else {                                    // bei allen Stellen ausser bei den 1ern
        putChar('0' + (char)(val / divisor));  // val/dec Der Quotient (digit) an die Output Funktion schicken
        val  %= divisor;                       // Ursprungswert auf den Rest der Division setzen
     }
     divisor /= BASE_L;
  } while (divisor);
}

#ifdef __cplusplus
}
#endif //__cplusplus