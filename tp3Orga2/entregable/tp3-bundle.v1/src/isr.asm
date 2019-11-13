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

extern print_exception 

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;


%macro ISR 1
global _isr%1

_isr%1:
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

_isr32:
    pushad
    call nextClock
    ;cuando el clock cambie vamos a llamar al scheduler porque... para eso teniamos el clock y el scheduler, no?
    ;aca vamos a swappear de tarea 
    ;call sched_nextTask
    call pic_finish1
    popad
    iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33
_isr33:
    pushad ;preservamos TODES les registres
    in al, 0x60
    push eax
    call print_number
    add esp, 4
    .salir:
    call pic_finish1
    popad
    iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

global _isr47
_isr47:
    cmp eax, 0x80000003
    je .setHandler
    jmp .sigo1
    .setHandler:
        call setHandler
    
    .sigo1:
    cmp eax, 0x80000001
    je .talk
    jmp .sigo2
    .talk:
        call talk
    
    .sigo2:
    cmp eax, 0x80000002
    je .where
    jmp .sigo3
    .where:
        call where

    .sigo3:
    cmp eax, 0x8000FFFF
    je .informAction
    jmp .sigo4
    .informAction:
        call informAction

    .sigo4:
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
    pushad
    push ebx
    call setHandlerValue
    add esp, 4
    popad
    ret 

talk: 
    pushad
    ;COMPLETAR
    popad
    ret

where: 
    pushad
    ;COMPLETAR
    popad
    ret

informAction: 
    pushad
    
    ;call a game.c
    call actualizarMovimientoPendiente

    ;call a sched.c
    call saltarDeHandlerATarea

    popad
    ret


