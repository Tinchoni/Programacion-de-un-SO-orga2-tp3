/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "gdt.h"
#include "mmu.h"

uint32_t kernelLevelTasksStacks[6]; // array de esp0's
uint32_t userLevelTasksCodeAndStacks[6]; // array de CR3's

typedef struct str_tss {
    uint16_t  ptl;
    uint16_t  unused0;
    uint32_t    esp0;
    uint16_t  ss0;
    uint16_t  unused1;
    uint32_t    esp1;
    uint16_t  ss1;
    uint16_t  unused2;
    uint32_t    esp2;
    uint16_t  ss2;
    uint16_t  unused3;
    uint32_t    cr3;
    uint32_t    eip;
    uint32_t    eflags;
    uint32_t    eax;
    uint32_t    ecx;
    uint32_t    edx;
    uint32_t    ebx;
    uint32_t    esp;
    uint32_t    ebp;
    uint32_t    esi;
    uint32_t    edi;
    uint16_t  es;
    uint16_t  unused4;
    uint16_t  cs;
    uint16_t  unused5;
    uint16_t  ss;
    uint16_t  unused6;
    uint16_t  ds;
    uint16_t  unused7;
    uint16_t  fs;
    uint16_t  unused8;
    uint16_t  gs;
    uint16_t  unused9;
    uint16_t  ldt;
    uint16_t  unused10;
    uint16_t  dtrap;
    uint16_t  iomap;
} __attribute__((__packed__, aligned (8))) tss;

extern tss tss_initial;
extern tss tss_idle;

void tss_init();

void initUserTask(uint32_t tipoDeTarea, uint32_t esHandler, uint32_t punteroARutinaHandler);
void llenarTSSA1(uint32_t tipoDeTarea); 
void llenarTSSA2(uint32_t tipoDeTarea); 
void llenarTSSA3(uint32_t tipoDeTarea); 
void llenarTSSA1_HANDLER(uint32_t tipoDeTarea, uint32_t eip); 
void llenarTSSA2_HANDLER(uint32_t tipoDeTarea, uint32_t eip); 
void llenarTSSA3_HANDLER(uint32_t tipoDeTarea, uint32_t eip); 

void llenarTSSB1(uint32_t tipoDeTarea); 
void llenarTSSB2(uint32_t tipoDeTarea); 
void llenarTSSB3(uint32_t tipoDeTarea); 
void llenarTSSB1_HANDLER(uint32_t tipoDeTarea, uint32_t eip); 
void llenarTSSB2_HANDLER(uint32_t tipoDeTarea, uint32_t eip); 
void llenarTSSB3_HANDLER(uint32_t tipoDeTarea, uint32_t eip);

#endif  /* !__TSS_H__ */
