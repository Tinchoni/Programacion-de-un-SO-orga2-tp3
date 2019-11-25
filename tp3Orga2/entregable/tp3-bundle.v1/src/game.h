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
#include "gameStructs.h"

typedef void (*f_handler_t)();

coordenadaPelota* dameCoordenadas();
void matarTarea(uint32_t murioDelLadoDerecho, uint32_t slot);
void crearPelota(uint32_t esJugadorA, uint32_t tipoDePelota);
void atender_teclado(uint8_t tecla_presionada);
void moverJugador(uint32_t esJugadorA, e_action_t movimientoAEjecutar);
void game_init();

#endif  /* !__GAME_H__ */
