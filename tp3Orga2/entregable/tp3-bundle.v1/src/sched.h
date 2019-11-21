/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "screen.h"
#include "tss.h"

int32_t quantum;

void sched_init();

int16_t sched_nextTask();

int16_t GDTHandlerEntryByType(uint32_t type);
int16_t GDTEntryByType(uint32_t type);
void saltarDeHandlerATarea();
uint32_t dameSlotLibre(uint32_t esJugadorA);



// handlers_activos[i] == 0 ===> El handler de la tarea-i esta inactivo.
// handlers_activos[i] != 0 ===> El handler de la tarea-i esta activo y es handlers_activos[i].

// handlers_activos[0]: Handler de la pelota del Jugador A Slot 1
// handlers_activos[1]: Handler de la pelota del Jugador A Slot 2
// handlers_activos[2]: Handler de la pelota del Jugador A Slot 3
// handlers_activos[3]: Handler de la pelota del Jugador B Slot 1
// handlers_activos[4]: Handler de la pelota del Jugador B Slot 2
// handlers_activos[5]: Handler de la pelota del Jugador B Slot 3
uint32_t handlers_activos[6];

// pelotas_vivas[i] == 1 ===> La pelota-i esta viva.
// pelotas_vivas[0]: pelota del Jugador A Slot 1
// pelotas_vivas[1]: pelota del Jugador A Slot 2
// pelotas_vivas[2]: pelota del Jugador A Slot 3
// pelotas_vivas[3]: pelota del Jugador B Slot 1
// pelotas_vivas[4]: pelota del Jugador B Slot 2
// pelotas_vivas[5]: pelota del Jugador B Slot 3
uint32_t pelotas_vivas[6];


#endif	/* !__SCHED_H__ */
