//////////////////////////////////////////////////////////////////////////////
/// @file strrev.c
/// @author Kai R. (you@domain.com)
/// @brief Zeichen in Pufferspeicher von rechts nach links tauschen
/// 
/// @date 2021-12-31
/// @version 0.1
/// 
/// @copyright Copyright (c) 2021
/// 
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////////
/// @brief Zeichen in Pufferspeicher von rechts nach links tauschen
/// 
/// @param pLeft       Zeiger auf Pufferspeicheranfang
/// @param r_len       Index auf rechtes Speicherende 
//////////////////////////////////////////////////////////////////////////////

void strrev(char* pLeft, uint8_t r_len) {
  char* pRight = pLeft + r_len;
  char tmp;
  while(pRight > pLeft) {
    tmp = *pRight;
    *pRight-- = *pLeft;
    *pLeft++ = tmp;
  }
}

#ifdef __cplusplus
}
#endif //__cplusplus