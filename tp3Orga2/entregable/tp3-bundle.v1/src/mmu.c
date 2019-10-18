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
	pde *dir = (pde *) KERNEL_PAGE_DIR;
	for(int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
		//inicializo cada entrada de la page directory con un page directory entry nulo
		//dir[i] = (pde)(0);
		dir[i].present = 0;
	}	
	dir[0].present = 1;
    dir[0].readwrite = 1;
    dir[0].usersupervisor = 0;
    dir[0].pwt = 0;
    dir[0].pcd = 0;
    dir[0].accesed = 0;
    dir[0].ignored = 0;
    dir[0].ps = 0;
    dir[0].g = 0;
    dir[0].disponible = 0;
    dir[0].direccion_tabla_de_descriptores_de_paginas = KERNEL_PAGE_TABLE >> 12;
    
    //no me hace falta inicializar la page table, porque la voy llenar toda (smirk)
    //porque necesito exactamente 4MB y justito 1 page directory entry = 1024 page table entry = 1024 * 4kb = 4MB

    pte *table = (pte *) KERNEL_PAGE_TABLE;
    for(int i = 0; i < PAGE_TABLE_SIZE; i++) {
		table[i].present = 1;
		table[i].readwrite = 1;
		table[i].usersupervisor = 0;
		table[i].pwt = 0;
		table[i].pcd = 0;
		table[i].accesed = 0;
		table[i].dirtybit = 0;
		table[i].pat = 0;
		table[i].g = 0;
		table[i].disponible = 0;
		table[i].direccion_pagina = i;
	}

	/*
	
	direccion lineal:
	0000 0000 1100 0001 0010 << 12 + 0x345 = 0x00C12345
	offset_page: 0x0345
	offset_table: 0x012
	offset_directory: 0x003
	
	UNA LECTURA:

	pte *miTablita = (pte *) (dir[offset_directory].direccion_tabla_de_descriptores_de_paginas << 12);
	page *miPaginita = (miTablita[offset_table].direccion_pagina << 12) 0000 0000 11-00 0001 0010 = 0x00C12
	EL DATO! = miPaginita[offset_page] 0X00012000 + 0X345 = 0X00C12345



	b -> direccion_pagina << 12 + 0x00F

	direccion fisica deseada:
	0x00C12345	
	
	*/


    return 0;
}








