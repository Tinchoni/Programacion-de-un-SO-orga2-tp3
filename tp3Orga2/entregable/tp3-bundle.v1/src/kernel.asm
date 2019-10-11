; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"
%define C_BG_LIGHT_MAGENTA 0x0D0D

global start

extern GDT_DESC

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

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
    
    ; Habilitar A20
    
    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax,cr0
    or eax,1
    mov cr0,eax
    
    

    ; Saltar a modo protegido
    farJump: jmp 0x0070:modoProtegido


    modoProtegido:
        BITS 32
    ; Establecer selectores de segmentos
    mov ax, 0x0080
    mov ss, ax    ;selector de segmento de la pila

    mov ax, 0x0090
    mov es, ax     ;selector de segmento para la pantalla

    ; Establecer la base de la pila
    mov esp, 0x0002B000
    
    ; Imprimir mensaje de bienvenida

    ; Inicializar pantalla
    
    xchg bx, bx
    mov eax, 0
    .limpiarPantalla:
        cmp eax, 8000
        je .finLimpiarPantalla
        mov word [es:eax], C_BG_LIGHT_MAGENTA
        add eax, 2
        jmp .limpiarPantalla

    .finLimpiarPantalla:


    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    
    ; Cargar IDT
 
    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
