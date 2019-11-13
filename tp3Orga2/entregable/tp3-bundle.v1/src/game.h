/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

typedef void (*f_handler_t)();

typedef struct str_coordenadaPelota {
    uint32_t  x;
    uint32_t  y;
} coordenadaPelota;


typedef enum e_action {
    Up = 1,
    Center = 2,
    Down = 3,
} e_action_t;

void game_init();

#endif  /* !__GAME_H__ */
