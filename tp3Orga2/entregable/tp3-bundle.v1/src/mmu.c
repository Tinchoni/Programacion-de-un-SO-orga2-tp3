/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

//Estas variables son parte de los section de mmu.c
// mmu.c pertenece al compilado final del tp que es kernel.bin
// la catedra se ocupa de meter kernel.bin en memoria y nosotros
// lo mapeamos con identity mapping, con lo cual, todo piola.
uint32_t proxima_pagina_libre_kernel;
uint32_t proxima_pagina_libre_task;

void mmu_init() {
	proxima_pagina_libre_kernel = INICIO_PAGINAS_LIBRES_KERNEL;
	proxima_pagina_libre_task = INICIO_PAGINAS_LIBRES_TASK;
}
uint32_t mmu_nextFreeTaskPage() {
	uint32_t pagina_libre = proxima_pagina_libre_task;
	proxima_pagina_libre_task += PAGE_SIZE;
	return pagina_libre;
}

uint32_t mmu_nextFreeKernelPage() {
	uint32_t pagina_libre = proxima_pagina_libre_kernel;
	proxima_pagina_libre_kernel += PAGE_SIZE;
	return pagina_libre;
}

// bit 1 de attrs == 1 => read/write
// bit 1 de attrs == 0 => read only
// bit 0 de attrs == 1 => user
// bit 0 de attrs == 0 => supervisor
void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs) {
	pde* dir = (pde*) ((cr3 >> 12) << 12); // cr3 = direccion base del directorio | ignored | ... |. En el tp toda la parte que no es direccion blabla esta en cero, pero en la vida real puede no serlo. So, limpiamos los primeros 12 bits.
	uint32_t offset_pd = virtual >> 22; // la direccion virtual tiene, del bit 22 a 31, a offset_page_directory 
	uint32_t offset_pt = (virtual << 10) >> 22; // la dir virtual tiene, del bit 12 a 21, a offset_page_table
	//obtengo de attrs los bits 0 y 1 (los que necesito).
	//bit 1
	uint32_t attrs_readwrite = (attrs << 30) >> 31;
	//bit 0
	uint32_t attrs_usersupervisor = (attrs << 31) >> 31;


	//si la pagina no esta mapeada aun, la mapeamos.
	if( !dir[offset_pd].present){
		pte *nuevaTabla = (pte*) mmu_nextFreeKernelPage();
		//inicializo una pt con todas sus entradas no presentes.
		for(int i = 0; i < PAGE_TABLE_SIZE; i++) {
			nuevaTabla[i].present = 0;
		}
		
		//Actualizo la pde que se condice con esta pagina
		//poniendola presente y con el puntero a la pt que acabo de crear.
		dir[offset_pd].present = 1;
	    dir[offset_pd].readwrite = attrs_readwrite;
	    dir[offset_pd].usersupervisor = attrs_usersupervisor;
	    dir[offset_pd].pwt = 0;
	    dir[offset_pd].pcd = 0;
	    dir[offset_pd].accesed = 0;
	    dir[offset_pd].ignored = 0;
	    dir[offset_pd].ps = 0;
	    dir[offset_pd].g = 0;
	    dir[offset_pd].disponible = 0;
	    dir[offset_pd].direccion_tabla_de_descriptores_de_paginas = ((uint32_t)nuevaTabla) >> 12;
	}

	uint32_t ptPointer = dir[offset_pd].direccion_tabla_de_descriptores_de_paginas << 12;
	pte *pt = (pte*) ptPointer;

	//lleno la pte de la pagina que queria mapear con lo que necesitaba
	//o sea, los atributos y la direccion fisica pedida (phy)
	pt[offset_pt].present = 1;
	pt[offset_pt].readwrite = attrs_readwrite;
	pt[offset_pt].usersupervisor = attrs_usersupervisor;
	pt[offset_pt].pwt = 0;
	pt[offset_pt].pcd = 0;
	pt[offset_pt].accesed = 0;
	pt[offset_pt].dirtybit = 0;
	pt[offset_pt].pat = 0;
	pt[offset_pt].g = 0;
	pt[offset_pt].disponible = 0;
	pt[offset_pt].direccion_pagina = (phy >> 12);  // phy puede no ser multiplo de 4k, pero para que caiga en donde yo quiero la "divido" por 4k
	
	tlbflush(); // esto dice la catedra que lo hagamos. no se
}

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3) {
    pde* dir = (pde*) ((cr3 >> 12) << 12); // cr3 = direccion base del directorio | ignored | ... |. En el tp toda la parte que no es direccion blabla esta en cero, pero en la vida real puede no serlo. So, limpiamos los primeros 12 bits.
	uint32_t offset_pd = virtual >> 22; // la direccion virtual tiene, del bit 22 a 31, a offset_page_directory 
	uint32_t offset_pt = (virtual << 10) >> 22; // la dir virtual tiene, del bit 12 a 21, a offset_page_table
	pde pd_entry = dir[offset_pd];

    pte *pt = (pte*) (pd_entry.direccion_tabla_de_descriptores_de_paginas << 12);

	//seteamos en 0 el bit present de la pte que apunta a la pagina que queremos desmapear
	pt[offset_pt].present = 0;

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

uint32_t obtenerPosicionDeMemoriaDeCodigo(uint32_t tipoDeTarea){
	switch(tipoDeTarea){
		case 0:
			//Jugador A Tipo 1
			return 0x10000;
			break;
		case 1:
			//Jugador A Tipo 2
			return 0x12000;
			break;
		case 2:
			//Jugador A Tipo 3
			return 0x14000;
			break;
		case 3:
			//Jugador B Tipo 1
			return 0x16000;
			break;
		case 4:
			//Jugador B Tipo 2
			return 0x18000;
			break;
		case 5:
			//Jugador B Tipo 3
			return 0x1A000;
			break;
	}
	//para que siempre retorne algo esta funcion.
	return 0;
}

//por ahora tipo de tarea es un numero de 0-5 donde 0 = Jugador A Tipo 1 ... 5 = Jugador B Tipo 3 (como en figura 2 de la consigna)
uint32_t mmu_initTaskDir(uint32_t tipoDeTarea){
	//NO MIRO LA CONSIGNA PORQUE NO SE ENTIENDE QUIEN ES EL JUGADOR.

	//vamos a hacer identity mapping de la memoria desde 0x00000 hasta 0x3FFFFF
	//esta es la memoria del kernel
	//y vamos a mapear la 0x08000000 y la 0x08001000 a una pagina libre del area de tareas correspondientemente
	
	uint32_t cr3Task = mmu_nextFreeKernelPage();  // cada tarea tiene su propio page directory y sus propios pde, pte, bla.

	uint32_t attrs_kernel = 0x1; //queremos ser supervisor y tener permiso read/write.
	uint32_t attrs_user = 0x3; // queremos los bits userSupervisor y read/write en 1. 
	//hay CANT_PAGINAS_KERNEL paginas en total, voy una por una
	for(int i= 0; i < CANT_PAGINAS_KERNEL; i++){
		mmu_mapPage(i << 12, cr3Task, i << 12, attrs_kernel); // i es el numero de pagina. i << 12 es la posicion de memoria del primer elemento de la i-esima pagina, mapeo esa, a esa misma, o sea, identity mapping.
	}

	// los directorios de las tareas viven en el area del kernel, por eso le pido memoria a kernel mediante mmu_nextFreeKernelPage().
	// en cr3Task vamos a inicializar el directorio de esta nueva tarea que llego por parametro (tipoDeTarea).

	//ahora mapeo el codigo y la pila de la tarea.
	uint32_t cr3Actual = (uint32_t)rcr3(); // "read cr3"

	uint32_t nuevaPaginaDelAreaDeTareaCodigo = mmu_nextFreeTaskPage(); // nos da una paginita del area libre de tareas
	// mapeo nuevaPaginaDelAreaDeTareaCodigo con los dos cr3 (actual y task)
	//mapeo la pagina a nivel kernel para poder escribirla con el codigo de la tarea en cuestion
	mmu_mapPage(0x08002000, cr3Actual, nuevaPaginaDelAreaDeTareaCodigo, attrs_user); // es auxiliar porque sino no puedo escribir ni leer nuevaPaginaDelAreaDeTareaCodigo porque no la tengo en mi directorio. Por eso uso cr3Actual
	//aca hay que usar tipoDeTarea y escribir nuevaPaginaDelAreaDeTareaCodigo con el codigo en cuestion
	mmu_mapPage(0x08000000,cr3Task, nuevaPaginaDelAreaDeTareaCodigo, attrs_user);
	

	// ahora hago lo mismo con la otra pagina (para la pila) con los dos cr3
	uint32_t nuevaPaginaDelAreaDeTareaPila = mmu_nextFreeTaskPage();
	mmu_mapPage(0x08003000, cr3Actual, nuevaPaginaDelAreaDeTareaPila, attrs_user); // es auxiliar porque sino no puedo escribir ni leer nuevaPaginaDelAreaDeTareaPila porque no la tengo en mi directorio. Por eso uso cr3Actual
	mmu_mapPage(0x08001000,cr3Task, nuevaPaginaDelAreaDeTareaPila, attrs_user);


	// nos contaron "hablado" que hay que copiar el codigo de la tarea (que esta en el kernel) a la posicion mapeada 0x08002000 (que es la posicion que mira cr3Actual) pero en el fondo estamos copiando en la direccion virtual 0x08000000 de cr3Task
	//copio el codigo de la tarea en cuestion y su pila, al codigo y pila que acabo de mapear.

	// paginaALeer = un puntero a donde empieza la pagina de codigo de la tarea en cuestion. en la pagina siguiente va a estar la pagina de pila de esa tarea.

	uint32_t *paginaALeer = (uint32_t*) obtenerPosicionDeMemoriaDeCodigo(tipoDeTarea); // es un pedacito del area de memoria del kernel. Es una direccion virtual que resulta igual a la fisica por identity mapping del area del kernel.
	uint32_t dosPaginas = 2048; //porque cada pagina tiene 1024 enteros de 32 bits, o lo que es lo mismo: 1024*4 = 4096 bytes.
	uint32_t *paginaAEscribir = (uint32_t*) 0x08002000; // el area donde empieza lo mappeado "auxiliarmente", en posicion fisica es nuevaPaginaDelAreaDeTareaCodigo
	for(int i=0; i < dosPaginas; i++){
		paginaAEscribir[i] = paginaALeer[i]; // esto copia de a 32 bits (de a dwords) desde la parte auxiliar hacia el area de tarea posta
	}

	//desmapeo las direcciones "aux" que use para poder escribir en nuevaPaginaDelAreaDeTareaPila/nuevaPaginaDelAreaDeTareaCodigo
	mmu_unmapPage(0x08002000, cr3Actual);
	mmu_unmapPage(0x08003000, cr3Actual);

	return cr3Task;
}








