/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#define CODE_w 0x11
#define CODE_s 0x1f
#define CODE_z 0x2c 
#define CODE_x 0x2d
#define CODE_c 0x2e 
#define CODE_i 0x17 
#define CODE_k 0x25 
#define CODE_b 0x30 
#define CODE_n 0x31 
#define CODE_m 0x32 


char* mensajesJugadorA[3];
char* mensajesJugadorB[3];

coordenadaPelota coordsPelotasPorSlot[6];
uint32_t alturaJugadorA;
uint32_t alturaJugadorB;

e_action_t movimientosPendientesPorSlot[6];

void game_init() {
	alturaJugadorA = 20;
	alturaJugadorB = 20;
}

void crearPelota(uint32_t slot){
	if( slot < 3 ){
		//la pelota es del jugador A
		coordsPelotasPorSlot[slot].x = 2; // arranca justo adelante del jugador
		coordsPelotasPorSlot[slot].y = alturaJugadorA; // arranca en el centro de la posicion del jugador
		coordsPelotasPorSlot[slot].direccionX = 1; // 1 = derecha
		coordsPelotasPorSlot[slot].direccionY = 0; // 0 = no invertida
	} else {
		//pelota del jugador B
		coordsPelotasPorSlot[slot].x = 78; 
		coordsPelotasPorSlot[slot].y = alturaJugadorB;
		coordsPelotasPorSlot[slot].direccionX = 0; // 0 = izquierda
		coordsPelotasPorSlot[slot].direccionY = 0; // 0 = no invertida
	}
}

e_action_t invertir(e_action_t accion, uint32_t invierte) {
	e_action_t res = accion;
	if(invierte) {
		if(accion == Up) {
			res = Down;
		} else if(accion == Center) {
			res = Center;
		} else {
			// el movimiento es Down
			res = Up;
		}
	}
	return res;
}

void atender_teclado(uint8_t tecla_presionada){
	switch(tecla_presionada){
		
	case CODE_w:
		//mover hacia arriba jugador A

		break;
	case CODE_s:
		//mover hacia abajo jugador A
		break;
	case CODE_z:
		//nueva pelota tipo 1 jugador A
		break;
	case CODE_x:
		//nueva pelota tipo 2 jugador A
		break;
	case CODE_c:
		//nueva pelota tipo 3 jugador A
		break;
	case CODE_i:
		//mover hacia arriba jugador B
		break;
	case CODE_k:
		//mover hacia abajo jugador B
		break;
	case CODE_b:
		//nueva pelota tipo 1 jugador B
		break;
	case CODE_n:
		//nueva pelota tipo 2 jugador B
		break;
	case CODE_m:
		//nueva pelota tipo 3 jugador B
		break;
	}
}

void moverJugadorEnVertical(uint32_t esJugadorA, e_action_t movimientoAEjecutar){
	uint32_t altura; 
	if(esJugadorA){
		altura = alturaJugadorA;
	} else {
		altura = alturaJugadorB;
	}

	if(movimientoAEjecutar == Up) {
		if(altura > 3){
			altura--;
		}
	} else if(movimientoAEjecutar == Down) {
		if(altura < 36){
			altura++;
		}
	}
	
	if(esJugadorA){
		//cambiar variables de altura
		alturaJugadorA = altura;
		//dibujar jugador
		
	} else {
		alturaJugadorB = altura;
	}	



}

coordenadaPelota moverEnVertical(coordenadaPelota coordActual, e_action_t movimientoAEjecutar){
	if(movimientoAEjecutar == Up) {
		if(coordActual.y == 0){
			coordActual = 0;
			coordActual.direccionY = !coordActual.direccionY;
		} else {
			coordActual.y--;
		}

	} else if(movimientoAEjecutar == Down) {
		if(coordActual.y == 39){
			coordActual = 39;
			coordActual.direccionY = !coordActual.direccionY;
		} else {
			coordActual.y++;
		}
	}
	return coordActual;
}

uint32_t laAtaja(uint32_t alturaPelota, uint32_t alturaJugador){
	//TODO: Mirar aca si el atajado anda mal
	if(alturaJugador - 3 <= alturaPelota  && alturaPelota <= alturaJugador + 3){
		return 1;
	} else {
		return 0;
	}
}

coordenadaPelota moverEnHorizontal(coordenadaPelota coordActual) {
	if(coordActual.direccionX) {
		//derecha
		if(coordActual.x == 79){
			//TODO: matar tarea y borrarla de la pantalla pq esta en la columna 79
		} else if( coordActual.x == 78 && laAtaja(coordActual.y, alturaJugadorB) ){
			coordActual.direccionX = !coordActual.direccionX;
		} else {
			coordActual.x++;
		}
	} else {
		//izquierda
		if(coordActual.x == 0){
			//TODO: matar tarea y borrarla de la pantalla pq esta en la columna 0
		} else if( coordActual.x == 1 && laAtaja(coordActual.y, alturaJugadorA) ){
			coordActual.direccionX = !coordActual.direccionX;
		} else {
			coordActual.x--;
		}
	}
	return coordActual;
}

void dibujarPantalla(){
	screen_drawBox(0, 1, 40, 78, 0x32, 0x88); // limpio el tablero (en particular, las pelotas)
	for(uint32_t i = 0; i < 7; i++){
		coordenadaPelota coordActual = coordsPelotasPorSlot[i];
		e_action_t movimientoAEjecutar = invertir(movimientosPendientesPorSlot[i], coordActual.direccionY);
		
		coordActual = moverEnVertical(coordActual, movimientoAEjecutar);
		
		coordActual = moverEnHorizontal(coordActual);

		if(pelotas_vivas[i]) {
			uint8_t atributos;
			if(0 <= i && i <= 3) {
				atributos = 0xCC; // la pelota era del jugador A. So, la pelota es del color de A
			} else {
				atributos = 0x99
			}
			print("*", coordActual.x, coordActual.y, atributos);
		}
}

void actualizarMovimientoPendiente(e_action_t action){
	movimientosPendientesPorSlot[quantum] = action;
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
	return coordsPelotasPorSlot[type];
} 

