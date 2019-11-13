/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

char* mensajesJugadorA[3];
char* mensajesJugadorB[3];

coordenadaPelota coordsPelotasPorTipo[6];

e_action_t movimientosPendientesPorTipo[6];

void game_init() {

}

void actualizarMovimientoPendiente(e_action_t action){
	movimientosPendientesPorTipo[quantum - 1] = action;
}

void write_message(uint16_t player, char* message){
	
	//TODO: chequear que message.length <= 20. (wont fix)

	// player 0 = A
	// player 1 = B
	if(player == 0){
		// "push" --> mensajesJugadorA.push(message);
		mensajesJugadorA[2] = mensajesJugadorA[1];
		mensajesJugadorA[1] = mensajesJugadorA[0];
		mensajesJugadorA[0] = message;
	} else {
		mensajesJugadorB[2] = mensajesJugadorB[1];
		mensajesJugadorB[1] = mensajesJugadorB[0];
		mensajesJugadorB[0] = message;
	}
}

coordenadaPelota where_is(uint32_t type){
	return coordsPelotasPorTipo[type];
} 

