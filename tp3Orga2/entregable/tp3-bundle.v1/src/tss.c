/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_initial;
tss tss_idle;

tss tss_A1;	
tss tss_A2;	
tss tss_A3;	
tss tss_HANDLER_A1;	
tss tss_HANDLER_A2;	
tss tss_HANDLER_A3;	

tss tss_B1;	
tss tss_B2;	
tss tss_B3;	
tss tss_HANDLER_B1;	
tss tss_HANDLER_B2;	
tss tss_HANDLER_B3;	


void tss_init() {
	tss_idle.ptl = (uint16_t) 0;/* ;*/
    tss_idle.unused0 = (uint16_t) 0;/* ;*/
    tss_idle.esp0 = (uint32_t) 0x0002B000;/* ; es donde empieza el stack del kernel por consigna y justo es de 32 bits. Entonces, esp1 y esp2 valen cero;*/
    tss_idle.ss0 = (uint16_t) 16 << 3;/* ;*/
    tss_idle.unused1 = (uint16_t) 0;/* ;*/
    tss_idle.esp1 = (uint32_t) 0;/* ; ni lo usamos;*/
    tss_idle.ss1 = (uint16_t) 0;/* ; ni lo usamos;*/
    tss_idle.unused2 = (uint16_t) 0;/* ;*/
    tss_idle.esp2 = (uint32_t) 0;/* ;*/
    tss_idle.ss2 = (uint16_t) 0;/* ;*/
    tss_idle.unused3 = (uint16_t) 0;/* ;*/
    tss_idle.cr3 = (uint32_t) rcr3();/* rcr3() = cr3 ACTUAL; pero como esto se ejecuta en 
    					contexto de inicializacion del kernel; cr3 ACTUAL = cr3 del kernel;*/
    tss_idle.eip = (uint32_t) 0x0001C000;/* ;*/
    tss_idle.eflags = (uint32_t) 0;/* ;*/
    tss_idle.eax = (uint32_t) 0;/* ;*/
    tss_idle.ecx = (uint32_t) 0;/* ;*/
    tss_idle.edx = (uint32_t) 0;/* ;*/
    tss_idle.ebx = (uint32_t) 0;/* ;*/
    tss_idle.esp = (uint32_t) 0;/* ;*/
    tss_idle.ebp = (uint32_t) 0;/* ;*/
    tss_idle.esi = (uint32_t) 0;/* ;*/
    tss_idle.edi = (uint32_t) 0;/* ;*/
    tss_idle.es = (uint16_t) 0;/* ;*/
    tss_idle.unused4 = (uint16_t) 0;/* ;*/
    tss_idle.cs = (uint16_t) 14 << 3;/* ;*/
    tss_idle.unused5 = (uint16_t) 0;/* ;*/
    tss_idle.ss = (uint16_t) 0;/* ;*/
    tss_idle.unused6 = (uint16_t) 0;/* ;*/
    tss_idle.ds = (uint16_t) 0;/* ;*/
    tss_idle.unused7 = (uint16_t) 0;/* ;*/
    tss_idle.fs = (uint16_t) 0;/* ;*/
    tss_idle.unused8 = (uint16_t) 0;/* ;*/
    tss_idle.gs = (uint16_t) 0;/* ;*/
    tss_idle.unused9 = (uint16_t) 0;/* ;*/
    tss_idle.ldt = (uint16_t) 0;/* ;*/
    tss_idle.unused10 = (uint16_t) 0;/* ;*/
    tss_idle.dtrap = (uint16_t) 0;/* ;*/
    tss_idle.iomap = (uint16_t) 0;/* ;*/
}

