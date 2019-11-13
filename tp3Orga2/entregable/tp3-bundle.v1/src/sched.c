/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

extern void saltarATarea(int16_t selectorDeSegmento);

uint32_t quantum = 0;

// handlers_activos[i] == 0 ===> El handler de la tarea-i esta inactivo.
// handlers_activos[i] != 0 ===> El handler de la tarea-i esta activo y es handlers_activos[i].

// handlers_activos[0]: Handler de la pelota del Jugador A Tipo 1
// handlers_activos[1]: Handler de la pelota del Jugador A Tipo 2
// handlers_activos[2]: Handler de la pelota del Jugador A Tipo 3
// handlers_activos[3]: Handler de la pelota del Jugador B Tipo 1
// handlers_activos[4]: Handler de la pelota del Jugador B Tipo 2
// handlers_activos[5]: Handler de la pelota del Jugador B Tipo 3
uint32_t handlers_activos[6];

// pelotas_vivas[i] == 1 ===> La pelota-i esta viva.
// pelotas_vivas[0]: pelota del Jugador A Tipo 1
// pelotas_vivas[1]: pelota del Jugador A Tipo 2
// pelotas_vivas[2]: pelota del Jugador A Tipo 3
// pelotas_vivas[3]: pelota del Jugador B Tipo 1
// pelotas_vivas[4]: pelota del Jugador B Tipo 2
// pelotas_vivas[5]: pelota del Jugador B Tipo 3
uint32_t pelotas_vivas[6];


uint32_t next_quantum(){
	//cada quantum contempla el handler de una tarea (+ sus llamadas al sistema) 
	//y la tarea en si (+ sus llamadas al sistema)
	quantum = ((quantum + 1) % 7);
	return quantum;
}

void setHandlerValue(uint32_t punteritoAlCodigoDelHandler){
	//como el quantum lo avanzamos justo al inicializar la tarea, 
	//en esta instancia quantum - 1 sera el tipo de tarea que 
	//se esta ejecutando en este momento
	handlers_activos[quantum - 1] = punteritoAlCodigoDelHandler;
}

void saltarDeHandlerATarea(){
	// quantum - 1 == tipo de tarea ejecutandose actualmente.
	saltarATarea(GDTEntryByType(quantum - 1) << 3);
}


void sched_init() {
	//aca vamos a armar las estructuras necesarias 
	//para poder saber que handlers estan seteados y 
	//que tareas estan activas.
	for(uint32_t i = 0; i < 7; i++){
		// todos los handlers empiezan inactivos.
		handlers_activos[i] = 0;
		// todas las pelotas empiezan muertas.
		pelotas_vivas[i] = 0;
	}
}

int16_t GDTHandlerEntryByType(uint32_t type){
	return GDT_TSS_HANDLER_A1 + type;
}

int16_t GDTEntryByType(uint32_t type){
	return GDT_TSS_A1 + type;
}

int16_t sched_nextTask() {
	if(quantum < 6){
		//estoy swappeando entre manejo de pelotas.
		//quantum == tipo de tarea que estoy procesando
		int16_t response = 0;
		if(handlers_activos[quantum] != 0){
			//reinicio el handler
			initUserTask(quantum, 1, handlers_activos[quantum]);
			response = GDTHandlerEntryByType(quantum) << 3;
		} else if(pelotas_vivas[quantum] != 0) {
			//hay una tarea viva que no tiene handler pero si esta viva
			//asi que le toca ejecutar a ella
			response = GDTEntryByType(quantum) << 3;
		} else {
			//no hay handler ni tampoco tarea viva, con lo cual
			//salto a la tarea idle
			response = GDT_TSS_IDLE << 3;
		}
	}
	//TENGO QUE EJECUTAR ESTO AHORA?????? KEEEEEEEEEEEEEEEEE
	next_quantum();
	return response;
}

