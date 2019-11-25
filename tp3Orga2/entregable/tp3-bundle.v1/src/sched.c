/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

extern void saltarATarea(int16_t selectorDeSegmento);


int16_t GDTHandlerEntryBySlot(uint32_t slot){
	int16_t res;
	switch(slot){
		case 0:
			res = GDT_TSS_HANDLER_A1;
			break;
		case 1:
			res = GDT_TSS_HANDLER_A2;
			break;
		case 2:
			res = GDT_TSS_HANDLER_A3;
			break;
		case 3:
			res = GDT_TSS_HANDLER_B1;
			break;
		case 4:
			res = GDT_TSS_HANDLER_B2;
			break;
		case 5:
			res = GDT_TSS_HANDLER_B3;
			break;
	}
	return res;
}

int16_t GDTEntryBySlot(uint32_t slot){
	int16_t res;
	switch(slot){
		case 0:
			res = GDT_TSS_A1;
			break;
		case 1:
			res = GDT_TSS_A2;
			break;
		case 2:
			res = GDT_TSS_A3;
			break;
		case 3:
			res = GDT_TSS_B1;
			break;
		case 4:
			res = GDT_TSS_B2;
			break;
		case 5:
			res = GDT_TSS_B3;
			break;
	}
	return res;
}

void next_quantum(){
	//cada quantum contempla el handler de una tarea (+ sus llamadas al sistema) 
	//y la tarea en si (+ sus llamadas al sistema)
	quantum = ((quantum + 1) % 7);
}

void setHandlerValue(uint32_t punteritoAlCodigoDelHandler){
	//en esta instancia quantum sera el slot de tarea que 
	//se esta ejecutando en este momento
	handlers_activos[quantum] = punteritoAlCodigoDelHandler;
}

void saltarDeHandlerATarea(){
	// quantum == tipo de tarea ejecutandose actualmente.
	saltarATarea(GDTEntryBySlot(quantum) << 3);
}

uint32_t dameSlotLibre(uint32_t esJugadorA){
	uint32_t response = 9999;
	if(esJugadorA){
		uint32_t laEncontre = 0;
		for(uint32_t i = 0; i < 3 && !laEncontre; i++){
			if(!pelotas_vivas[i]){
				laEncontre = 1;
				response = i;
			}
		}	
	} else {
		uint32_t laEncontre = 0;
		for(uint32_t i = 3; i < 6 && !laEncontre; i++){
			if(!pelotas_vivas[i]){
				laEncontre = 1;
				response = i;
			}
		}
	}
	return response;
}


void sched_init() {
	//aca vamos a armar las estructuras necesarias 
	//para poder saber que handlers estan seteados y 
	//que tareas estan activas.
	quantum = -1;
	for(uint32_t i = 0; i <= 5; i++){
		// todos los handlers empiezan inactivos.
		handlers_activos[i] = 0;
		// todas las pelotas empiezan muertas.
		pelotas_vivas[i] = 0;
	}
}



int16_t sched_nextTask() {
	int16_t response = 0;
	next_quantum();
	if(quantum < 6){
		//estoy swappeando entre manejo de pelotas.
		//quantum == slot de pelota que estoy procesando
		if(handlers_activos[quantum] != 0){
			//reinicio el handler
			initUserTask(quantum, 1, handlers_activos[quantum]);
			response = GDTHandlerEntryBySlot(quantum) << 3;
		} else if(pelotas_vivas[quantum] != 0) {
			//hay una tarea viva que no tiene handler pero si esta viva
			//asi que le toca ejecutar a ella
			response = (GDTEntryBySlot(quantum) << 3);
		} else {
			//no hay handler ni tampoco tarea viva, con lo cual
			//salto a la tarea idle
			response = GDT_TSS_IDLE << 3;
		}
	} else {
		// tendriamos que orquestar las respuestas de las tareas y dibujar las pantallas
		
		//por ahora saltamos a idle.
		response = 0;
	}
	return response;
}

