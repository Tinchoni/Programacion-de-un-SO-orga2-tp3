#ifndef __GAME_STRUCTS_H__
#define __GAME_STRUCTS_H__

//mrs quantum que rige la vida
int32_t quantum;


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

typedef struct str_coordenadaPelota {
    uint32_t  x;
    uint32_t  y;
    uint32_t  direccionX; //0 = izquierda, 1 = derecha.
    uint32_t  direccionY; //0 = no invertido, 1 = invertido
} coordenadaPelota;


typedef enum e_action {
    Up = 1,
    Center = 2,
    Down = 3,
} e_action_t;

char* mensajesJugadorA[3];
char* mensajesJugadorB[3];

coordenadaPelota coordsPelotasPorSlot[6];
uint32_t alturaJugadorA;
uint32_t alturaJugadorB;

e_action_t movimientosPendientesPorSlot[6];

uint32_t kernelLevelTasksStacks[6]; // array de esp0's
uint32_t userLevelTasksCodeAndStacks[6]; // array de CR3's

uint32_t pelotasDisponiblesA;
uint32_t pelotasDisponiblesB;

uint32_t puntajeA;
uint32_t puntajeB;



#endif  /* !__GAME_STRUCTS_H__ */