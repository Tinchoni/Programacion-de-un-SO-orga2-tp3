; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

extern print_number

;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask
extern setHandlerValue
extern actualizarMovimientoPendiente
extern saltarDeHandlerATarea
extern print_exception 
extern dibujarPantalla
extern atender_teclado
extern dameCoordenadas
extern write_message

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;


%macro ISR 1
global _isr%1

_isr%1:
    ;xchg bx, bx
    mov eax, %1
    push eax
    call print_exception
    add esp, 4
    jmp $

%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31


;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32

offset: DD 0
selector: DW 0

_isr32:
    pushad
    call pic_finish1
    call nextClock
    ;cuando el clock cambie vamos a llamar al scheduler porque... para eso teniamos el clock y el scheduler, no?
    ;aca vamos a swappear de tarea 
    call sched_nextTask
    ;xchg bx, bx
    .sigo:
    mov [selector], ax
    str cx
    cmp cx, ax
    je .fin
    cmp ax, 0
    je .dibujarPantalla

    ;else tengo que saltar, efectivamente, a la siguiente tarea
    ;xchg bx, bx
    ;jmp (21<<3):0
    jmp far [offset]
    jmp .fin
    .dibujarPantalla:
    call dibujarPantalla 
    str cx
    cmp cx, 20<<3
    je .fin
    jmp (20<<3):0 ;idle diabolico
    .fin:
    popad
    iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33
_isr33:
    pushad ;preservamos TODES les registres
    in al, 0x60
    push eax
    ;call print_number
    call atender_teclado
    add esp, 4

    .salir:
    call pic_finish1
    popad
    iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

whereX: DD 0
whereY: DD 0

global _isr47
_isr47:
    pushad ; preservo estado del sistema

    cmp eax, 0x80000003
    je .setHandler
    jmp .sigo1
    .setHandler:
        call setHandler
    jmp .popRegisters
    
    .sigo1:
    cmp eax, 0x80000001
    je .talk
    jmp .sigo2
    .talk:
        call talk
    jmp .popRegisters

    .sigo2:
    cmp eax, 0x80000002
    je .where
    jmp .sigo3
    .where:
        call where
    jmp .popWhere

    .sigo3:
    cmp eax, 0x8000FFFF
    je .informAction
    jmp .popRegisters
    .informAction:
        call informAction
    jmp .popRegisters

    .popRegisters:
    popad ; recupero estado previo a la interrupcion
    jmp .fin

    .popWhere:
    mov [whereX], ebx
    mov [whereY], ecx
    popad
    mov ebx, [whereX]
    mov ecx, [whereY]
    jmp .fin

    .fin:
    iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
        popad
        ret

;debe preservar eax
setHandler:
    push ebx
    call setHandlerValue
    add esp, 4
    ret 

talk: 
    push ebx
    call write_message
    add esp, 4
    ret

where: 
    call dameCoordenadas
    mov ebx, [eax + 0] ;coordPelota.x
    mov ecx, [eax + 4] ;coordPelota.y
    ret

informAction: 
    
    ;call a game.c
    push ebx
    call actualizarMovimientoPendiente
    add esp, 4

    ;call a sched.c
    call saltarDeHandlerATarea

    ret


