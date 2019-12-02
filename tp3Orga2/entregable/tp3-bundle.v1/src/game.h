/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"
#include "gameStructs.h"

typedef void (*f_handler_t)();

coordenadaPelota* dameCoordenadas();
void matarTarea(uint32_t slot);
void crearPelota(uint32_t esJugadorA, uint32_t tipoDePelota);
void atender_teclado(uint8_t tecla_presionada);
void moverJugador(uint32_t esJugadorA, e_action_t movimientoAEjecutar);
void game_init();
void atenderY();
void manejar_excepcion(uint32_t codigoDeError, 
uint32_t stack3,
uint32_t stack2,
uint32_t stack1,
uint32_t stack0,
uint32_t ebx,
uint32_t ecx,
uint32_t edx,
uint32_t esi,
uint32_t edi,
uint32_t ds,
uint32_t es,
uint32_t fs,
uint32_t gs,
uint32_t cr0,
uint32_t cr2,
uint32_t cr3,
uint32_t cr4,
uint32_t esp,
uint32_t ss,
uint32_t eflags,
uint32_t cs,
uint32_t eip,
uint32_t eax,
uint32_t ebp);

#endif  /* !__GAME_H__ */
