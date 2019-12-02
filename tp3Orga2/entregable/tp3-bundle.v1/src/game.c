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
#define CODE_y 0x15

extern void saltarATarea(int16_t selectorDeSegmento);

void game_init() {
	alturaJugadorA = 20;
	alturaJugadorB = 20;
	for(int i = 0; i <= 5; i++){
		movimientosPendientesPorSlot[i] = Center;
	}
	for(int i=0; i < 3; i++){
		mensajesJugadorA[i][0] = '\0';
		mensajesJugadorB[i][0] = '\0';
	}

	pelotasDisponiblesA = 15;
	pelotasDisponiblesB = 15;
	modoDebug = 0;
	enPausa = 0;
	//crearPelota(1, 0);
	//crearPelota(1, 0);
}

void iniciarTarea(uint32_t slotLibre, uint32_t tipoDePelota){
	//pongo como viva la pelota
	pelotas_vivas[slotLibre] = 1;
	userLevelTasksCodeAndStacks[slotLibre] = mmu_initTaskDir(tipoDePelota);
	//lleno la tss de la tarea
	initUserTask(slotLibre, 0, 0);
}

void crearPelota(uint32_t esJugadorA, uint32_t tipoDePelota){
	uint32_t slotLibre = dameSlotLibre(esJugadorA);
	if(slotLibre < 6){
		if( esJugadorA && pelotasDisponiblesA > 0){
			//la pelota es del jugador A
			pelotasDisponiblesA--;
			coordsPelotasPorSlot[slotLibre].x = 0; // arranca justo adelante del jugador
			coordsPelotasPorSlot[slotLibre].y = alturaJugadorA; // arranca en el centro de la posicion del jugador
			coordsPelotasPorSlot[slotLibre].direccionX = 1; // 1 = derecha
			coordsPelotasPorSlot[slotLibre].direccionY = 0; // 0 = no invertida
			
			iniciarTarea(slotLibre, tipoDePelota);
		} else if(!esJugadorA && pelotasDisponiblesB > 0) {
			//pelota del jugador B
			pelotasDisponiblesB--;
			coordsPelotasPorSlot[slotLibre].x = 79; 
			coordsPelotasPorSlot[slotLibre].y = alturaJugadorB;
			coordsPelotasPorSlot[slotLibre].direccionX = 0; // 0 = izquierda
			coordsPelotasPorSlot[slotLibre].direccionY = 0; // 0 = no invertida
			
			iniciarTarea(slotLibre, tipoDePelota);
		}
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
	//TODO: Que onda con el enunciado que nos dice "limitar una accion cada 30 ciclos"????
	switch(tecla_presionada){
		case CODE_w:
			//mover hacia arriba jugador A
			moverJugador(1, Up);
			break;
		case CODE_s:
			//mover hacia abajo jugador A
			moverJugador(1, Down);
			break;
		case CODE_z:
			//nueva pelota tipo 1 jugador A
			crearPelota(1, 0);
			break;
		case CODE_x:
			//nueva pelota tipo 2 jugador A
			crearPelota(1, 1);
			break;
		case CODE_c:
			//nueva pelota tipo 3 jugador A
			crearPelota(1, 2);
			break;
		case CODE_i:
			//mover hacia arriba jugador B
			moverJugador(0, Up);
			break;
		case CODE_k:
			//mover hacia abajo jugador B
			moverJugador(0, Down);
			break;
		case CODE_b:
			//nueva pelota tipo 1 jugador B
			crearPelota(0, 3);
			break;
		case CODE_n:
			//nueva pelota tipo 2 jugador B
			crearPelota(0, 4);
			break;
		case CODE_m:
			//nueva pelota tipo 3 jugador B
			crearPelota(0, 5);
			break;
		case CODE_y:
			atenderY();
			break; 
	}
}

void atenderY() {
	if(enPausa){
		enPausa = 0;
		quantum = 0;
	} else {
		modoDebug = !modoDebug;
	}
}

void copiarString(uint32_t index, char* src, uint32_t esJugadorA){
	// TODO: Los mensajes claramente andan mal. Creemos que se debe a 
	// que la copia no es deep, es decir, 
	// no estamos copiando realmente si no pasando punteritos de aca pa alla
	int i;
	for(i=0; i < 20 && src[i] != 0; i++){
		if(esJugadorA){
			mensajesJugadorA[index][i] = src[i];
		} else {
			mensajesJugadorB[index][i] = src[i];
		}
	}
	if(esJugadorA){
		mensajesJugadorA[index][i] = '\0';
	} else {
		mensajesJugadorB[index][i] = '\0';
	}
}

void moverJugador(uint32_t esJugadorA, e_action_t movimientoAEjecutar){
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
		alturaJugadorA = altura;
	} else {
		alturaJugadorB = altura;
	}
}

coordenadaPelota moverEnVertical(coordenadaPelota coordActual, e_action_t movimientoAEjecutar){
	if(movimientoAEjecutar == Up) {
		if(coordActual.y == 0){
			coordActual.y = 0;
			coordActual.direccionY = !coordActual.direccionY;
		} else {
			coordActual.y--;
		}

	} else if(movimientoAEjecutar == Down) {
		if(coordActual.y == 39){
			coordActual.y = 39;
			coordActual.direccionY = !coordActual.direccionY;
		} else {
			coordActual.y++;
		}
	}
	return coordActual;
}

uint32_t laAtaja(uint32_t alturaPelota, uint32_t alturaJugador){
	if(alturaJugador - 3 <= alturaPelota  && alturaPelota <= alturaJugador + 3){
		return 1;
	} else {
		return 0;
	}
}

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
uint32_t ebp){
	//print_exception(codigoDeError);
	if(quantum < 6){
		//significa que la exepcion sucedio dentro de una tarea.
		//en este caso deberia matarla.
		//ahora, si estoy en modoDebug. Deberia printear los valores de esa tarea en pantalla
		
		if(modoDebug){
			get_exception(codigoDeError);
			enPausa = 1;
			//limpio tablero
			screen_drawBox(0, 1, 40, 78, 0x32, 0x88); 
			//dibujo una caja loca
			screen_drawBox(1, 19, 38, 40, 0x32, 0x00); // ancho 19-59. Alto 1-39 
			char* string_task;
			switch(quantum){
				case 0:
					string_task = "la pelota 1 del Jugador A";
					break;
				case 1:
					string_task = "la pelota 2 del Jugador A";
					break;
				case 2:
					string_task = "la pelota 3 del Jugador A";
					break;
				case 3:
					string_task = "la pelota 1 del Jugador B";
					break;
				case 4:
					string_task = "la pelota 2 del Jugador B";
					break;
				case 5:
					string_task = "la pelota 3 del Jugador B";
					break;
			}
			int base = 20;
			print("ERROR ", base, 2, 0x0F);
			print(exception_msg, base + 6,2,0x0F);

			int altura = 4;
			int ancho = base;
			print("En ", ancho , altura, 0x0F);
			print(string_task, ancho + 3, altura, 0x0F);

			int columnaIzquierda = 20;
			int columnaDerecha = 40;
			int alturaBase = 7;
			int offsetAltura = 0;

			//columna izquierda

			print("eax: ", columnaIzquierda ,alturaBase, 0x0F);
			print_hex(eax, 8, columnaIzquierda + 5, alturaBase, 0x0F);

			offsetAltura += 2;
			print("ebx: ", columnaIzquierda ,alturaBase + 2, 0x0F);
			print_hex(ebx, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("ecx: ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(ecx, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("edx: ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(edx, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("esi: ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(esi, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("edi: ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(edi, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("ebp: ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(ebp, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("esp: ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(esp, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("eip: ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(eip, 8, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F);

			offsetAltura += 2;
			print("cs : ", columnaIzquierda ,alturaBase + offsetAltura, 0x0F);
			print_hex(cs, 4, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F );

			offsetAltura += 2;
			print("ds : ", columnaIzquierda, alturaBase + offsetAltura, 0x0F);
			print_hex(ds, 4, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F );

			offsetAltura += 2;
			print("es : ", columnaIzquierda, alturaBase + offsetAltura, 0x0F);
			print_hex(es, 4, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F );

			offsetAltura += 2;
			print("fs : ", columnaIzquierda, alturaBase + offsetAltura, 0x0F);
			print_hex(fs, 4, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F );

			offsetAltura += 2;
			print("gs : ", columnaIzquierda, alturaBase + offsetAltura, 0x0F);
			print_hex(gs, 4, columnaIzquierda + 5, alturaBase + offsetAltura, 0x0F );

			//columna derecha

			offsetAltura = 0;
			print("ss : ", columnaDerecha, alturaBase, 0x0F);
			print_hex(ss, 4, columnaDerecha + 5, alturaBase, 0x0F );
			
			offsetAltura += 2;
			print("cr0: ", columnaDerecha, alturaBase + 2, 0x0F);
			print_hex(cr0, 8, columnaDerecha + 5, alturaBase + offsetAltura, 0x0F );
			
			offsetAltura += 2;
			print("cr2: ", columnaDerecha, alturaBase + offsetAltura, 0x0F);
			print_hex(cr2, 8, columnaDerecha + 5, alturaBase + offsetAltura, 0x0F );
			
			offsetAltura += 2;
			print("cr3: ", columnaDerecha, alturaBase + offsetAltura, 0x0F);
			print_hex(cr3, 8, columnaDerecha + 5, alturaBase + offsetAltura, 0x0F );
			
			offsetAltura += 2;
			print("cr4: ", columnaDerecha, alturaBase + offsetAltura, 0x0F);
			print_hex(cr4, 8, columnaDerecha + 5, alturaBase + offsetAltura, 0x0F );

			offsetAltura += 2;
			print("eflags: ", columnaDerecha, alturaBase + offsetAltura, 0x0F);
			print_hex(eflags, 8, columnaDerecha + 8, alturaBase + offsetAltura, 0x0F );

			offsetAltura += 6;
			print("stack: ", columnaDerecha, alturaBase + offsetAltura, 0x0F);
			offsetAltura += 2;
			print_hex(stack0, 8, columnaDerecha, alturaBase + offsetAltura, 0x0F );
			offsetAltura += 2;
			print_hex(stack1, 8, columnaDerecha, alturaBase + offsetAltura, 0x0F );
			offsetAltura += 2;
			print_hex(stack2, 8, columnaDerecha, alturaBase + offsetAltura, 0x0F );
			offsetAltura += 2;
			print_hex(stack3, 8, columnaDerecha, alturaBase + offsetAltura, 0x0F );
			
		}
		matarTarea(quantum);
		saltarATarea(GDT_TSS_IDLE << 3);
	}
}

void matarTarea(uint32_t slot){
	pelotas_vivas[slot] = 0;
	handlers_activos[slot] = 0;
	ejecutando_handler[slot] = 0;
	movimientosPendientesPorSlot[slot] = Center;
}

coordenadaPelota moverEnHorizontal(coordenadaPelota coordActual, uint32_t slot) {
	if(coordActual.direccionX) {
		//derecha
		if(coordActual.x == 79){
			//mato la tarea
			matarTarea(slot);
			puntajeA += 100;
		} else if( coordActual.x == 78 && laAtaja(coordActual.y, alturaJugadorB) ){
			coordActual.direccionX = !coordActual.direccionX;
		} else {
			coordActual.x++;
		}
	} else {
		//izquierda
		if(coordActual.x == 0){
			//mato la tarea
			matarTarea(slot);
			puntajeB += 100;
			
		} else if( coordActual.x == 1 && laAtaja(coordActual.y, alturaJugadorA) ){
			coordActual.direccionX = !coordActual.direccionX;
		} else {
			coordActual.x--;
		}
	}
	return coordActual;
}

void dibujarPantalla(){
	//TODO: MODO DEBUG! (atender tecla Y, poner cartelito, cortar las tareas, reestablecer)

	//TODO: Matar tareas cuando se tomaron mas de un ciclo de clock en el handler
	//TODO: Matar tareas cuando tienen una excepcion fuera del modo debug (es llamar a matarTare(quantum))
	if(!enPausa){

		// limpio el tablero (en particular, las pelotas)
		screen_drawBox(0, 1, 40, 78, 0x32, 0x88); 
		for(uint32_t i = 0; i <= 5; i++){
			if(pelotas_vivas[i] != 0) {
				coordenadaPelota coordActual = coordsPelotasPorSlot[i];
				e_action_t movimientoAEjecutar = invertir(movimientosPendientesPorSlot[i], coordActual.direccionY);


				//aca calculamos como va a ser el movimiento en vertical en el siguiente tick del juego		
				coordActual = moverEnVertical(coordActual, movimientoAEjecutar);
				
				// same para horizontal
				coordActual = moverEnHorizontal(coordActual, i);

				coordsPelotasPorSlot[i] = coordActual;
				uint8_t atributos;
				if(0 <= i && i <= 2) {
					atributos = C_BG_DARK_GREY + C_FG_LIGHT_RED; // la pelota era del jugador A. So, la pelota es del color de A
				} else {
					atributos = C_BG_DARK_GREY + C_FG_LIGHT_BLUE;
				}
				// aca dibujamos en pantalla ese resultado.
				print("*", coordActual.x, coordActual.y, atributos);
			}
		}

		// imprimo jugador A
		// limpio borde izquierdo
		screen_drawBox(0, 0, 40, 1, 0x32, C_BG_GREEN + C_FG_GREEN);

		// ahora dibujo al jugador A
		screen_drawBox(alturaJugadorA - 3, 0, 7, 1, 0x32, 0xCC);

		// imprimo jugador B
		// limpio borde derecho
		screen_drawBox(0, 79, 40, 1, 0x32, C_BG_GREEN + C_FG_GREEN);

		// ahora dibujo al jugador B
		screen_drawBox(alturaJugadorB - 3, 79, 7, 1, 0x32, 0x99);
		


		//imprimo mensajes
		screen_drawBox(45, 1, 4, 38, 0x32, 0xCC);
		screen_drawBox(45, 41, 4, 38, 0x32, 0x99);
		for(int i=0; i < 3; i++){
			print(mensajesJugadorA[i], 4, (45 + i), 0xF0);
			print(mensajesJugadorB[i], 44, (45 + i), 0xF0);
		}

		//imprimo "puntos"
		print("Puntos:", 2, 42, 0xF0);
		print_dec(puntajeA, 4, 10, 42, 0xF0);
		print("Puntos:", 42, 42, 0xF0);
		print_dec(puntajeB, 4, 50, 42, 0xF0);

		//imprimo "pelotas"
		print("Pelotas:", 2, 43, 0xF0);
		print_dec(pelotasDisponiblesA, 2, 11, 43, 0xF0);
		print("Pelotas:", 42, 43, 0xF0);
		print_dec(pelotasDisponiblesB, 2, 51, 43, 0xF0);

		//imprimo slots con pelotas activas (o no)
		for(int i= 0; i < 3; i++){
			//i esimo slot de pelotas de A
			char* taVivo = pelotas_vivas[i] ? "O" : "X";
			print(taVivo, 32 + 2*i, 42, 0xF0);

			//i esimo slot de pelotas de B
			taVivo = pelotas_vivas[i+3] ? "O" : "X";
			print(taVivo, 72 + 2*i, 42, 0xF0);
		}

		// imprimo Cartelito de "Modo debug"
		if(modoDebug){
			print("Modo debug", 0, 49, 0x0F);
		} else {
			print("Modo debug", 0, 49, (C_FG_BLACK + C_BG_BLACK));
		}
	}
}

void actualizarMovimientoPendiente(e_action_t action){
	movimientosPendientesPorSlot[quantum] = action;
}
coordenadaPelota* dameCoordenadas(){
	//aca quantum = slot.
	return &(coordsPelotasPorSlot[quantum]);
}
void write_message(char* message){
	uint32_t esJugadorA = 0;
	if(quantum < 3){	
		esJugadorA = 1;
	} else {
		esJugadorA = 0;
	}


	if(esJugadorA){
		// "push" --> mensajesJugadorA.push(message);
		copiarString(2, mensajesJugadorA[1],1); // mensajesJugadorA[2] = deepCopy(mensajesJugadorA[1])
		copiarString(1, mensajesJugadorA[0],1);
		copiarString(0, message,1);
		
	} else {
		copiarString(2, mensajesJugadorB[1],0);
		copiarString(1, mensajesJugadorB[0],0);
		copiarString(0, message,0);
	}
}
