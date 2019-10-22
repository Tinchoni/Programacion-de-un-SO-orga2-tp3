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
extern idt_inicializar
extern idt_init
extern mmu_initKernelDir

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

LU db 'Martin Mamani Aleman 630 17 Uriel Chami 157 17 Augusto Gonzalez Omahen 496/ 17'
LU_size equ $ - LU

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

    ; Inicializar tablero e interfaz:
    ; ACA IRIA TODO LO DEL TABLERO Y BLABLABLABLALBALBALBLALAL INTERFAZ MASA MADRE

    ; ---------------------------- Fin Clase 1 --------------------------------



    ; ------------------------------ Clase 4 --------------------------------

    ; Inicializar el manejador de memoria
    ; aca va call mmu_init AGUANTE LA MMU VIEJITAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

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
    xchg bx, bx
    
    ; ---------------------------- Fin Clase 3 --------------------------------



    ; ------------------------------ Clase 5? --------------------------------
    
    ; Inicializar tss

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
