/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


void mmu_init() {
}

uint32_t mmu_nextFreeKernelPage() {
    return 0;
}

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs) {
}

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3) {
    return 0;
}

uint32_t mmu_initKernelDir() {
    return 0;
}








