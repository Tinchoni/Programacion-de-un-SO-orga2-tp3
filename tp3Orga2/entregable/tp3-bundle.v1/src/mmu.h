/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024
#define KERNEL_PAGE_DIR 0x2B000 // por consigna, no me mires
#define KERNEL_PAGE_TABLE 0x2C000 //como el directorio empieza en 2B000 y ocupa 4k, es decir, ocupa 0x01000, la direccion de la tabla de paginas empieza 4k mas adelante, es decir, en 0x2C000
#define PAGE_SIZE 4096
#define INICIO_PAGINAS_LIBRES_KERNEL 0x100000
#define INICIO_PAGINAS_LIBRES_TASK 0x400000
#define CANT_PAGINAS_KERNEL 1024

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"


typedef struct str_pde {
    uint8_t   present:1;
    uint8_t   readwrite:1;
    uint8_t   usersupervisor:1;
    uint8_t   pwt:1;
    uint8_t   pcd:1;
    uint8_t   accesed:1;
    uint8_t   ignored:1;
    uint8_t   ps:1;
    uint8_t   g:1;
    uint8_t   disponible:3;
    uint32_t  direccion_tabla_de_descriptores_de_paginas:20;
} __attribute__((__packed__, aligned (4))) pde;

typedef struct str_pte {
    uint8_t   present:1;
    uint8_t   readwrite:1;
    uint8_t   usersupervisor:1;
    uint8_t   pwt:1;
    uint8_t   pcd:1;
    uint8_t   accesed:1;
    uint8_t   dirtybit:1;
    uint8_t   pat:1;
    uint8_t   g:1;
    uint8_t   disponible:3;
    uint32_t  direccion_pagina:20;
} __attribute__((__packed__, aligned (4))) pte;


void mmu_init();

uint32_t mmu_nextFreeKernelPage();

uint32_t mmu_nextFreeTaskPage();

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs);

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3);

uint32_t mmu_initKernelDir();

uint32_t obtenerPosicionDeMemoriaDeCodigo(uint32_t tipoDeTarea);

uint32_t mmu_initTaskDir(uint32_t tipoDeTarea);




#endif	/* !__MMU_H__ */




