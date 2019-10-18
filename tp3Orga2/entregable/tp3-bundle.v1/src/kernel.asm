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
%define CARACTER_LIMPITO 32 << 8 | (0x2 << 4) | 0x5 

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
    mov ax, 16 << 3
    mov ds, ax    ;selector de segmento de la datos
    mov ss, ax    ;selector de segmento de la pila

    mov ax, 0x0090
    mov es, ax     ;selector de segmento para la pantalla

    ; Establecer la base de la pila
    mov esp, 0x0002B000
    
    ; Imprimir mensaje de bienvenida

    ; Inicializar pantalla
    
    mov eax, 0
    .limpiarPantalla:
        cmp eax, 8000
        je .finLimpiarPantalla
        mov word [es:eax], CARACTER_LIMPITO
        add eax, 2
        jmp .limpiarPantalla

    .finLimpiarPantalla:


    ; Inicializar el manejador de memoria
    

    ; Inicializar el directorio de paginas
    call mmu_initKernelDir

    ; Cargar directorio de paginas
    mov eax, KERNEL_PAGE_DIR
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

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
    ; Cargar tarea inicial

    jmp $
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
