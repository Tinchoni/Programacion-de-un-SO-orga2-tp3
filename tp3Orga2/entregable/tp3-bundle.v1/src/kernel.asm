; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================


;//void print(const char* text, uint32_t x, uint32_t y, uint8_t attr)

;PRINT CABEZA HECHO POR DAVID
;jmp chau
;texto: db "sarasa liquida"
;chau:
;push (0x2 << 4) | 0x5
;push 10
;push 10
;push texto
;call print
;add esp, 4*4

;jmp $

%include "print.mac"
%define CARACTER_LIMPITO 32 << 8 | (0x0 << 4) | 0x0
%define KERNEL_PAGE_DIR 0x2B000 

global start

extern pic_reset
extern pic_enable
extern IDT_DESC
extern GDT_DESC
extern screen_drawBox
extern idt_inicializar
extern idt_init
extern mmu_initKernelDir
extern mmu_init
extern mmu_initTaskDir
extern tss_init
extern print

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

LU db '630/17, 157/17, 496/17'
LU_size equ $ - LU

puntos_msg db 'Puntos:'
puntos_len equ $ - puntos_msg
pelotas_msg db 'Pelotas:'
pelotas_len equ $ - pelotas_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    ; ax = 1003h -> para poder tener 16 colores de background
    mov ax, 1003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0



    ; ---------------------------- Clase 1 --------------------------------

    ; Habilitar A20
    call A20_enable ;llamamos a una funcion implementada por la catedra.
    
    ; Cargar la GDT
    lgdt [GDT_DESC] ;la direccion del gdtr.

    ; Setear el bit PE del registro CR0
    mov eax,cr0
    or eax,1 
    mov cr0,eax ; ahora cr0 esta igual que antes salvo el bit menos significativo, que esta en 1.
    
    

    ; Saltar a modo protegido
    farJump: jmp 0x0070:modoProtegido ; 0x0070 = 000000001110|0|00 pues privilegio=00, ti=0 e indice=14=0000 1110 (se lo extiende con 5 ceros en la parte mas significativa). Era lo mismo que poner 14<<3.


    modoProtegido:

    BITS 32

    ; Establecer selectores de segmentos
    mov ax, 16 << 3 ; la entrada numero 16 de la gdt, que le corresponde al segmento GDT_DATOS_KERNEL.
    mov ds, ax    ;selector de segmento de la datos
    mov ss, ax    ;selector de segmento de la pila, comparten el mismo segmento.

    mov ax, 18 << 3 ; o sea 0x0090, el segmento numero 18, es decir GDT_PANTALLA_KERNEL
    mov es, ax     ;selector de segmento para la pantalla

    ; Establecer la base de la pila
    mov esp, 0x0002B000 ; por consigna, a mi no me mires. Haria falta el ebp tambien?
    
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 0, 0 ; uso la macro definida en print.mac, donde los parametros son un puntero al mensaje (start_pm_msg), la longitud del mensaje (start_pm_len), el color (0x07, es decir, C_BG_BLACK con C_FG_LIGHT_GREY) y la fila y columna (0,0).

    ; Inicializar pantalla
    mov eax, 0
    .limpiarPantalla:
        cmp eax, 8000 ; cuando llegamos a 80*50, ya procesamos todos los pixeles, so, chau
        je .finLimpiarPantalla
        mov word [es:eax], CARACTER_LIMPITO
        add eax, 2 ; porque cada pixel ocupa 2 bytes.
        jmp .limpiarPantalla
    .finLimpiarPantalla:
    
    ; quiero imprimir un area gris (C_FG_DARK_GREY y C_BG_DARK_GREY, o sea que el atributo es 0x88) en las primeras 40 filas, desde la columna 1 hasta la 79.
    push 0x88 ; attr
    push 0x32 ; character, cualquiera, total se pinta gris (?). es invisible.
    push 78   ; cSize
    push 40   ; fSize, termina en la fila 39.
    push 1    ; cInit
    push 0    ; fInit
    call screen_drawBox ; screen_drawBox(0, 1, 40, 78, 0x32, 0x88)

    ; ahora quiero imprimir unos marcos negros (atributo 0x00) para los puntajes y demas. Para eso, imprimo un solo rectangulo negro y despues piso lo necesario y ahi quedan los marcos negros.
    push 0x00 ; attr
    push 0x32 ; character
    push 80   ; cSize
    push 10   ; fSize
    push 0    ; cInit
    push 40    ; fInit, empieza en la fila 40
    call screen_drawBox ; screen_drawBox(40, 0, 10, 80, 0x32, 0x00)

    ; ahora imprimo dos rectangulos, uno rojo y otro azul (atributos 0x44 y 0x11)
    push 0xCC ; attr
    push 0x32 ; character, cualquiera, total se pinta gris (?). es invisible.
    push 38   ; cSize
    push 8   ; fSize
    push 1    ; cInit
    push 41    ; fInit
    call screen_drawBox ; screen_drawBox(41, 1, 8, 38, 0x32, 0xCC)

    push 0x99 ; attr
    push 0x32 ; character, cualquiera, total se pinta gris (?). es invisible.
    push 38   ; cSize
    push 8   ; fSize
    push 41    ; cInit
    push 41    ; fInit
    call screen_drawBox ; screen_drawBox(41, 41, 8, 38, 0x32, 0x99)

    ; ahora imprimo a los 2 jugadores (los jugadores van desde las filas 17 inclusive a 23 inclusive, tamaño 7 porque pinto).
    push 0xCC ; attr
    push 0x32 ; character, cualquiera, total se pinta gris (?). es invisible.
    push 1   ; cSize
    push 7   ; fSize
    push 0    ; cInit
    push 17    ; fInit
    call screen_drawBox ; screen_drawBox(17, 0, 7, 1, 0x32, 0xCC)

    push 0x99 ; attr
    push 0x32 ; character, cualquiera, total se pinta gris (?). es invisible.
    push 1   ; cSize
    push 7   ; fSize
    push 79    ; cInit
    push 17    ; fInit
    call screen_drawBox ; screen_drawBox(17, 0, 7, 1, 0x32, 0xCC)

    ; imprimo Puntos, Pelotas.
    print_text_pm puntos_msg, puntos_len, 0xF0, 42, 2 ; puntero al mensaje (puntos_msg), longitud del mensaje (puntos_len), color (0xF0, es decir, C_BG_WHITE y C_FG_BLACK), fila y columna (0,0).
    print_text_pm pelotas_msg, pelotas_len, 0xF0, 43, 2

    print_text_pm puntos_msg, puntos_len, 0xF0, 42, 42 
    print_text_pm pelotas_msg, pelotas_len, 0xF0, 43, 42

    ; el resto deberia ser hecho/actualizado por alguna tarea.
    ; de hecho, imprimir los puntos y las pelotas no es parte del ejercicio 1. Lo dejo porque despues va a servir como molde, pero va a volar

    ; xchg bx, bx

    ; ---------------------------- Fin Clase 1 --------------------------------



    ; ------------------------------ Clase 4 --------------------------------

    ; Inicializar el manejador de memoria
    call mmu_init
    ; ---------------------------- Fin Clase 4 --------------------------------



    ; ------------------------------ Clase 3 --------------------------------

    ; Inicializar el directorio de paginas
    call mmu_initKernelDir

    ; Imprimir numero de libreta de los integrantes:
    print_text_pm LU, LU_size, 0x07, 0, 0 ; los parametros son los explicados mas arriba al imprimir el mensaje de bienvenida.


    ; Cargar directorio de paginas
    mov eax, KERNEL_PAGE_DIR ; esto equivale a 0x2B000 por consigna.
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, (1 << 31) ; teniamos que modificar el bit mas significativo de los 32 bits de cr0.
    mov cr0, eax
    
    ; ---------------------------- Fin Clase 3 --------------------------------




    ; test de punto 5.d)

    ;mov ebx, cr3
    ;push 0
    ;call mmu_initTaskDir
    ;mov cr3,eax
    ; quiero imprimir un area gris (C_FG_DARK_GREY y C_BG_DARK_GREY, o sea que el atributo es 0x88) en las primeras 40 filas, desde la columna 1 hasta la 79.
    ;push 0x55 ; attr magenta.
    ;push 0x32 ; character, cualquiera, total se pinta gris (?). es invisible.
    ;push 78   ; cSize
    ;push 40   ; fSize, termina en la fila 39.
    ;push 1    ; cInit
    ;push 0    ; fInit
    ;call screen_drawBox ; screen_drawBox(0, 1, 40, 78, 0x32, 0x88)

    ;mov eax, ebx
    ;mov cr3, eax





    ; ------------------------------ Clase 5? --------------------------------
    

    ; Inicializar tss
    call tss_init

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; ---------------------------- Fin Clase 5 --------------------------------



    ; ------------------------------ Clase 2 ---------------------------------

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]
    
    ;test excepcion dividir por 0
    ;mov eax, 0
    ;div eax

    ; Configurar controlador de interrupciones
    call pic_reset ; remapeo el PIC
    call pic_enable ;prendo el PIC
    sti ;habilito de vuelta interrupciones

    ;TEST

    push 0
    call mmu_initTaskDir
    mov cr3, eax


    ; ---------------------------- Fin Clase 2 --------------------------------

    ; Cargar tarea inicial

    jmp $ ; por que esta aca? que onda, pa mi hay que volarlo o ponerlo mas abajo (?)

    ; Habilitar interrupciones
    sti 

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
