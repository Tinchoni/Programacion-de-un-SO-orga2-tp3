/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "stdint.h"
#include "colors.h"
#include "defines.h"
#include "gameStructs.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    uint8_t c;
    uint8_t a;
} ca;

void get_exception(uint8_t ex_number);

void print(const char* text, uint32_t x, uint32_t y, uint8_t attr);
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint8_t attr);
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint8_t attr);

void screen_drawBox(uint32_t fInit, uint32_t cInit,
                    uint32_t fSize, uint32_t cSize,
                    uint8_t character, uint8_t attr );

#endif  /* !__SCREEN_H__ */
