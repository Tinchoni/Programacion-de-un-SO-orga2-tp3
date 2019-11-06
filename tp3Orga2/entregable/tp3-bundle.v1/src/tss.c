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
	tss_idle = (tss) {
		(uint16_t) 0,/* ptl;*/
	    (uint16_t) 0,/* unused0;*/
	    (uint32_t) 0x0002B000,/*   esp0, es donde empieza el stack del kernel por consigna y justo es de 32 bits. Entonces, esp1 y esp2 valen cero;*/
	    (uint16_t) 16 << 3,/* ss0;*/
	    (uint16_t) 0,/* unused1;*/
	    (uint32_t) 0,/*   esp1, ni lo usamos;*/
	    (uint16_t) 0,/* ss1, ni lo usamos;*/
	    (uint16_t) 0,/* unused2;*/
	    (uint32_t) 0,/*   esp2;*/
	    (uint16_t) 0,/* ss2;*/
	    (uint16_t) 0,/* unused3;*/
	    (uint32_t) rcr3(),/*   rcr3() = cr3 ACTUAL, pero como esto se ejecuta en 
	    					contexto de inicializacion del kernel, cr3 ACTUAL = cr3 del kernel;*/
	    (uint32_t) 0x0001C000,/*   eip;*/
	    (uint32_t) 0,/*   eflags;*/
	    (uint32_t) 0,/*   eax;*/
	    (uint32_t) 0,/*   ecx;*/
	    (uint32_t) 0,/*   edx;*/
	    (uint32_t) 0,/*   ebx;*/
	    (uint32_t) 0,/*   esp;*/
	    (uint32_t) 0,/*   ebp;*/
	    (uint32_t) 0,/*   esi;*/
	    (uint32_t) 0,/*   edi;*/
	    (uint16_t) 0,/* es;*/
	    (uint16_t) 0,/* unused4;*/
	    (uint16_t) 14 << 3,/* cs;*/
	    (uint16_t) 0,/* unused5;*/
	    (uint16_t) 0,/* ss;*/
	    (uint16_t) 0,/* unused6;*/
	    (uint16_t) 0,/* ds;*/
	    (uint16_t) 0,/* unused7;*/
	    (uint16_t) 0,/* fs;*/
	    (uint16_t) 0,/* unused8;*/
	    (uint16_t) 0,/* gs;*/
	    (uint16_t) 0,/* unused9;*/
	    (uint16_t) 0,/* ldt;*/
	    (uint16_t) 0,/* unused10;*/
	    (uint16_t) 0,/* dtrap;*/
	    (uint16_t) 0/* iomap;*/
	}
}

