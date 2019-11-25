/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"


#define tasksAndHandlers_ptl 0
#define tasksAndHandlers_unused0 0

#define tasksAndHandlers_ss0 (GDT_DATOS_KERNEL << 3)

#define tasksAndHandlers_unused1 0
#define tasksAndHandlers_esp1 0
#define tasksAndHandlers_ss1 0
#define tasksAndHandlers_unused2 0
#define tasksAndHandlers_esp2 0
#define tasksAndHandlers_ss2 0
#define tasksAndHandlers_unused3 0

#define tasks_eip 0x08000000 /* porque todas las tareas estan mapeadas desde 0x08000000 mediante la funcion mmu_initTaskDir */

#define tasksAndHandlers_eflags 0x202
#define tasksAndHandlers_eax 0
#define tasksAndHandlers_ecx 0
#define tasksAndHandlers_edx 0
#define tasksAndHandlers_ebx 0

#define tasks_esp (0x08000000 + 7*1024) /*porque la pila de las tareas empieza en 0x08000000 + 7kb.*/
#define handlers_esp (0x08000000 + 8*1024) /* porque bla, analogo, ver figura 5*/

#define tasks_ebp (0x08000000 + 7*1024)
#define handlers_ebp (0x08000000 + 8*1024)


#define tasksAndHandlers_esi 0
#define tasksAndHandlers_edi 0
#define tasksAndHandlers_es (GDT_DATOS_USUARIO << 3) + 3
#define tasksAndHandlers_unused4 0
#define tasksAndHandlers_cs (GDT_CODIGO_USUARIO << 3) + 3
#define tasksAndHandlers_unused5 0
#define tasksAndHandlers_ss (GDT_DATOS_USUARIO << 3) + 3
#define tasksAndHandlers_unused6 0
#define tasksAndHandlers_ds (GDT_DATOS_USUARIO << 3) + 3
#define tasksAndHandlers_unused7 0
#define tasksAndHandlers_fs (GDT_DATOS_USUARIO << 3) + 3
#define tasksAndHandlers_unused8 0
#define tasksAndHandlers_gs (GDT_DATOS_USUARIO << 3) + 3
#define tasksAndHandlers_unused9 0
#define tasksAndHandlers_ldt 0
#define tasksAndHandlers_unused10 0
#define tasksAndHandlers_dtrap 0
#define tasksAndHandlers_iomap 0


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
	tss_idle.ptl = (uint16_t) 0;
    tss_idle.unused0 = (uint16_t) 0;
    tss_idle.esp0 = (uint32_t) 0;/* es donde empieza el stack del kernel por consigna y justo es de 32 bits. Entonces, esp1 y esp2 valen cero;*/
    tss_idle.ss0 = (uint16_t) 0;
    tss_idle.unused1 = (uint16_t) 0;
    tss_idle.esp1 = (uint32_t) 0;/* ni lo usamos*/
    tss_idle.ss1 = (uint16_t) 0;/* ni lo usamos;*/
    tss_idle.unused2 = (uint16_t) 0;
    tss_idle.esp2 = (uint32_t) 0;
    tss_idle.ss2 = (uint16_t) 0;
    tss_idle.unused3 = (uint16_t) 0;
    tss_idle.cr3 = (uint32_t) rcr3();/* rcr3() = cr3 ACTUAL; pero como esto se ejecuta en 
    					contexto de inicializacion del kernel; cr3 ACTUAL = cr3 del kernel;*/
    tss_idle.eip = (uint32_t) 0x0001C000;
    tss_idle.eflags = (uint32_t) 0x202;
    tss_idle.eax = (uint32_t) 0;
    tss_idle.ecx = (uint32_t) 0;
    tss_idle.edx = (uint32_t) 0;
    tss_idle.ebx = (uint32_t) 0;
    tss_idle.esp = (uint32_t) 0x2B000; /* stack de kernel */
    tss_idle.ebp = (uint32_t) 0x2B000;
    tss_idle.esi = (uint32_t) 0;
    tss_idle.edi = (uint32_t) 0;
    tss_idle.es = (uint16_t) GDT_DATOS_KERNEL << 3;
    tss_idle.unused4 = (uint16_t) 0;
    tss_idle.cs = (uint16_t) GDT_CODIGO_KERNEL << 3;
    tss_idle.unused5 = (uint16_t) 0;
    tss_idle.ss = (uint16_t) GDT_DATOS_KERNEL << 3;
    tss_idle.unused6 = (uint16_t) 0;
    tss_idle.ds = (uint16_t) GDT_DATOS_KERNEL << 3;
    tss_idle.unused7 = (uint16_t) 0;
    tss_idle.fs = (uint16_t) GDT_DATOS_KERNEL << 3;
    tss_idle.unused8 = (uint16_t) 0;
    tss_idle.gs = (uint16_t) GDT_DATOS_KERNEL << 3;
    tss_idle.unused9 = (uint16_t) 0;
    tss_idle.ldt = (uint16_t) 0;
    tss_idle.unused10 = (uint16_t) 0;
    tss_idle.dtrap = (uint16_t) 0;
    tss_idle.iomap = (uint16_t) 0;

    for(uint32_t i= 0; i < 6; i++){
    	kernelLevelTasksStacks[i] = mmu_nextFreeKernelPage();
    }
    // ahora ya esta seteado el arreglo de CR3's y el de ESP0's


    //como la gdt es global, modificamos las bases de los descriptores para que apunten a lo definido en este archivo
    
    // Definir la base del descriptor de la gdt de idle
    uint32_t diridle = (uint32_t) &tss_idle;

    gdt[GDT_TSS_IDLE].base_0_15 = diridle;
    gdt[GDT_TSS_IDLE].base_23_16 = (diridle >> 16);
    gdt[GDT_TSS_IDLE].base_31_24 = (diridle >> 24);


	// Definir la base del descriptor de la gdt de initial
    uint32_t dirinitial = (uint32_t) &tss_initial;

    gdt[GDT_TSS_TAREA_INICIAL].base_0_15 = dirinitial;
    gdt[GDT_TSS_TAREA_INICIAL].base_23_16 = (dirinitial >> 16);
    gdt[GDT_TSS_TAREA_INICIAL].base_31_24 = (dirinitial >> 24);



    // Definir la base del descriptor de la gdt de A1
    uint32_t dirA1 = (uint32_t) &tss_A1;

    gdt[GDT_TSS_A1].base_0_15 = dirA1;
    gdt[GDT_TSS_A1].base_23_16 = (dirA1 >> 16);
    gdt[GDT_TSS_A1].base_31_24 = (dirA1 >> 24);

    // Definir la base del descriptor de la gdt de HANDLER_A1
    uint32_t dirA1Handler = (uint32_t) &tss_HANDLER_A1;

    gdt[GDT_TSS_HANDLER_A1].base_0_15 = dirA1Handler;
    gdt[GDT_TSS_HANDLER_A1].base_23_16 = (dirA1Handler >> 16);
    gdt[GDT_TSS_HANDLER_A1].base_31_24 = (dirA1Handler >> 24);

	// Definir la base del descriptor de la gdt de A2
    uint32_t dirA2 = (uint32_t) &tss_A2;

    gdt[GDT_TSS_A2].base_0_15 = dirA2;
    gdt[GDT_TSS_A2].base_23_16 = (dirA2 >> 16);
    gdt[GDT_TSS_A2].base_31_24 = (dirA2 >> 24);

    // Definir la base del descriptor de la gdt de HANDLER_A2
    uint32_t dirA2Handler = (uint32_t) &tss_HANDLER_A2;

    gdt[GDT_TSS_HANDLER_A2].base_0_15 = dirA2Handler;
    gdt[GDT_TSS_HANDLER_A2].base_23_16 = (dirA2Handler >> 16);
    gdt[GDT_TSS_HANDLER_A2].base_31_24 = (dirA2Handler >> 24);

    // Definir la base del descriptor de la gdt de A3
    uint32_t dirA3 = (uint32_t) &tss_A3;

    gdt[GDT_TSS_A3].base_0_15 = dirA3;
    gdt[GDT_TSS_A3].base_23_16 = (dirA3 >> 16);
    gdt[GDT_TSS_A3].base_31_24 = (dirA3 >> 24);

    // Definir la base del descriptor de la gdt de HANDLER_A3
    uint32_t dirA3Handler = (uint32_t) &tss_HANDLER_A3;

    gdt[GDT_TSS_HANDLER_A3].base_0_15 = dirA3Handler;
    gdt[GDT_TSS_HANDLER_A3].base_23_16 = (dirA3Handler >> 16);
    gdt[GDT_TSS_HANDLER_A3].base_31_24 = (dirA3Handler >> 24);

    // Definir la base del descriptor de la gdt de B1
    uint32_t dirB1 = (uint32_t) &tss_B1;

    gdt[GDT_TSS_B1].base_0_15 = dirB1;
    gdt[GDT_TSS_B1].base_23_16 = (dirB1 >> 16);
    gdt[GDT_TSS_B1].base_31_24 = (dirB1 >> 24);

    // Definir la base del descriptor de la gdt de HANDLER_B1
    uint32_t dirB1Handler = (uint32_t) &tss_HANDLER_B1;

    gdt[GDT_TSS_HANDLER_B1].base_0_15 = dirB1Handler;
    gdt[GDT_TSS_HANDLER_B1].base_23_16 = (dirB1Handler >> 16);
    gdt[GDT_TSS_HANDLER_B1].base_31_24 = (dirB1Handler >> 24);

    // Definir la base del descriptor de la gdt de B2
    uint32_t dirB2 = (uint32_t) &tss_B2;

    gdt[GDT_TSS_B2].base_0_15 = dirB2;
    gdt[GDT_TSS_B2].base_23_16 = (dirB2 >> 16);
    gdt[GDT_TSS_B2].base_31_24 = (dirB2 >> 24);

    // Definir la base del descriptor de la gdt de HANDLER_B2
    uint32_t dirB2Handler = (uint32_t) &tss_HANDLER_B2;

    gdt[GDT_TSS_HANDLER_B2].base_0_15 = dirB2Handler;
    gdt[GDT_TSS_HANDLER_B2].base_23_16 = (dirB2Handler >> 16);
    gdt[GDT_TSS_HANDLER_B2].base_31_24 = (dirB2Handler >> 24);

    // Definir la base del descriptor de la gdt de B3
    uint32_t dirB3 = (uint32_t) &tss_B3;

    gdt[GDT_TSS_B3].base_0_15 = dirB3;
    gdt[GDT_TSS_B3].base_23_16 = (dirB3 >> 16);
    gdt[GDT_TSS_B3].base_31_24 = (dirB3 >> 24);

    // Definir la base del descriptor de la gdt de HANDLER_B3
    uint32_t dirB3Handler = (uint32_t) &tss_HANDLER_B3;

    gdt[GDT_TSS_HANDLER_B3].base_0_15 = dirB3Handler;
    gdt[GDT_TSS_HANDLER_B3].base_23_16 = (dirB3Handler >> 16);
    gdt[GDT_TSS_HANDLER_B3].base_31_24 = (dirB3Handler >> 24);
}



//funciones que llenan la TSS de cada tarea
void llenarTSSA1(uint32_t slotDeTarea){
	tss_A1.ptl = tasksAndHandlers_ptl;
    tss_A1.unused0 = tasksAndHandlers_unused0;
    
    // esp0 para tarea es (por figura 5) 
    tss_A1.esp0 = kernelLevelTasksStacks[slotDeTarea] + 2 * 1024 - 1;
    tss_A1.ss0 = tasksAndHandlers_ss0;
    tss_A1.unused1 = tasksAndHandlers_unused1;
    tss_A1.esp1 = tasksAndHandlers_esp1;
    tss_A1.ss1 = tasksAndHandlers_ss1;
    tss_A1.unused2 = tasksAndHandlers_unused2;
    tss_A1.esp2 = tasksAndHandlers_esp2;
    tss_A1.ss2 = tasksAndHandlers_ss2;
    tss_A1.unused3 = tasksAndHandlers_unused3;
    tss_A1.cr3 = userLevelTasksCodeAndStacks[slotDeTarea];
    tss_A1.eip = tasks_eip;
    tss_A1.eflags = tasksAndHandlers_eflags;
    tss_A1.eax = tasksAndHandlers_eax;
    tss_A1.ecx = tasksAndHandlers_ecx;
    tss_A1.edx = tasksAndHandlers_edx;
    tss_A1.ebx = tasksAndHandlers_ebx;
    tss_A1.esp = tasks_esp;
    tss_A1.ebp = tasks_ebp;
    tss_A1.esi = tasksAndHandlers_esi;
    tss_A1.edi = tasksAndHandlers_edi;
    tss_A1.es = tasksAndHandlers_es;
    tss_A1.unused4 = tasksAndHandlers_unused4;
    tss_A1.cs = tasksAndHandlers_cs;
    tss_A1.unused5 = tasksAndHandlers_unused5;
    tss_A1.ss = tasksAndHandlers_ss;
    tss_A1.unused6 = tasksAndHandlers_unused6;
    tss_A1.ds = tasksAndHandlers_ds;
    tss_A1.unused7 = tasksAndHandlers_unused7;
    tss_A1.fs = tasksAndHandlers_fs;
    tss_A1.unused8 = tasksAndHandlers_unused8;
    tss_A1.gs = tasksAndHandlers_gs;
    tss_A1.unused9 = tasksAndHandlers_unused9;
    tss_A1.ldt = tasksAndHandlers_ldt;
    tss_A1.unused10 = tasksAndHandlers_unused10;
    tss_A1.dtrap = tasksAndHandlers_dtrap;
    tss_A1.iomap = tasksAndHandlers_iomap;
}
void llenarTSSA1_Handler(uint32_t slotDeTarea, uint32_t eip){
	tss_HANDLER_A1.ptl = tasksAndHandlers_ptl;
    tss_HANDLER_A1.unused0 = tasksAndHandlers_unused0;
    
    //esp0 para handler es (por figura 5)
    tss_HANDLER_A1.esp0 = kernelLevelTasksStacks[slotDeTarea] + 4 * 1024 - 1;

    tss_HANDLER_A1.ss0 = tasksAndHandlers_ss0;
    tss_HANDLER_A1.unused1 = tasksAndHandlers_unused1;
    tss_HANDLER_A1.esp1 = tasksAndHandlers_esp1;
    tss_HANDLER_A1.ss1 = tasksAndHandlers_ss1;
    tss_HANDLER_A1.unused2 = tasksAndHandlers_unused2;
    tss_HANDLER_A1.esp2 = tasksAndHandlers_esp2;
    tss_HANDLER_A1.ss2 = tasksAndHandlers_ss2;
    tss_HANDLER_A1.unused3 = tasksAndHandlers_unused3;
    tss_HANDLER_A1.cr3 = tss_A1.cr3;
    tss_HANDLER_A1.eip = eip;
    tss_HANDLER_A1.eflags = tasksAndHandlers_eflags;
    tss_HANDLER_A1.eax = tasksAndHandlers_eax;
    tss_HANDLER_A1.ecx = tasksAndHandlers_ecx;
    tss_HANDLER_A1.edx = tasksAndHandlers_edx;
    tss_HANDLER_A1.ebx = tasksAndHandlers_ebx;
    tss_HANDLER_A1.esp = handlers_esp;
    tss_HANDLER_A1.ebp = handlers_ebp;
    tss_HANDLER_A1.esi = tasksAndHandlers_esi;
    tss_HANDLER_A1.edi = tasksAndHandlers_edi;
    tss_HANDLER_A1.es = tasksAndHandlers_es;
    tss_HANDLER_A1.unused4 = tasksAndHandlers_unused4;
    tss_HANDLER_A1.cs = tasksAndHandlers_cs;
    tss_HANDLER_A1.unused5 = tasksAndHandlers_unused5;
    tss_HANDLER_A1.ss = tasksAndHandlers_ss;
    tss_HANDLER_A1.unused6 = tasksAndHandlers_unused6;
    tss_HANDLER_A1.ds = tasksAndHandlers_ds;
    tss_HANDLER_A1.unused7 = tasksAndHandlers_unused7;
    tss_HANDLER_A1.fs = tasksAndHandlers_fs;
    tss_HANDLER_A1.unused8 = tasksAndHandlers_unused8;
    tss_HANDLER_A1.gs = tasksAndHandlers_gs;
    tss_HANDLER_A1.unused9 = tasksAndHandlers_unused9;
    tss_HANDLER_A1.ldt = tasksAndHandlers_ldt;
    tss_HANDLER_A1.unused10 = tasksAndHandlers_unused10;
    tss_HANDLER_A1.dtrap = tasksAndHandlers_dtrap;
    tss_HANDLER_A1.iomap = tasksAndHandlers_iomap;
}

void llenarTSSA2(uint32_t slotDeTarea){
	tss_A2.ptl = tasksAndHandlers_ptl;
    tss_A2.unused0 = tasksAndHandlers_unused0;
    
    // esp0 para tarea es (por figura 5) 
    tss_A2.esp0 = kernelLevelTasksStacks[slotDeTarea] + 2 * 1024 - 1;
    tss_A2.ss0 = tasksAndHandlers_ss0;
    tss_A2.unused1 = tasksAndHandlers_unused1;
    tss_A2.esp1 = tasksAndHandlers_esp1;
    tss_A2.ss1 = tasksAndHandlers_ss1;
    tss_A2.unused2 = tasksAndHandlers_unused2;
    tss_A2.esp2 = tasksAndHandlers_esp2;
    tss_A2.ss2 = tasksAndHandlers_ss2;
    tss_A2.unused3 = tasksAndHandlers_unused3;
    tss_A2.cr3 = userLevelTasksCodeAndStacks[slotDeTarea];
    tss_A2.eip = tasks_eip;
    tss_A2.eflags = tasksAndHandlers_eflags;
    tss_A2.eax = tasksAndHandlers_eax;
    tss_A2.ecx = tasksAndHandlers_ecx;
    tss_A2.edx = tasksAndHandlers_edx;
    tss_A2.ebx = tasksAndHandlers_ebx;
    tss_A2.esp = tasks_esp;
    tss_A2.ebp = tasks_ebp;
    tss_A2.esi = tasksAndHandlers_esi;
    tss_A2.edi = tasksAndHandlers_edi;
    tss_A2.es = tasksAndHandlers_es;
    tss_A2.unused4 = tasksAndHandlers_unused4;
    tss_A2.cs = tasksAndHandlers_cs;
    tss_A2.unused5 = tasksAndHandlers_unused5;
    tss_A2.ss = tasksAndHandlers_ss;
    tss_A2.unused6 = tasksAndHandlers_unused6;
    tss_A2.ds = tasksAndHandlers_ds;
    tss_A2.unused7 = tasksAndHandlers_unused7;
    tss_A2.fs = tasksAndHandlers_fs;
    tss_A2.unused8 = tasksAndHandlers_unused8;
    tss_A2.gs = tasksAndHandlers_gs;
    tss_A2.unused9 = tasksAndHandlers_unused9;
    tss_A2.ldt = tasksAndHandlers_ldt;
    tss_A2.unused10 = tasksAndHandlers_unused10;
    tss_A2.dtrap = tasksAndHandlers_dtrap;
    tss_A2.iomap = tasksAndHandlers_iomap;
}
void llenarTSSA2_Handler(uint32_t slotDeTarea, uint32_t eip){
	tss_HANDLER_A2.ptl = tasksAndHandlers_ptl;
    tss_HANDLER_A2.unused0 = tasksAndHandlers_unused0;
    
    //esp0 para handler es (por figura 5)
    tss_HANDLER_A2.esp0 = kernelLevelTasksStacks[slotDeTarea] + 4 * 1024 - 1;

    tss_HANDLER_A2.ss0 = tasksAndHandlers_ss0;
    tss_HANDLER_A2.unused1 = tasksAndHandlers_unused1;
    tss_HANDLER_A2.esp1 = tasksAndHandlers_esp1;
    tss_HANDLER_A2.ss1 = tasksAndHandlers_ss1;
    tss_HANDLER_A2.unused2 = tasksAndHandlers_unused2;
    tss_HANDLER_A2.esp2 = tasksAndHandlers_esp2;
    tss_HANDLER_A2.ss2 = tasksAndHandlers_ss2;
    tss_HANDLER_A2.unused3 = tasksAndHandlers_unused3;
    tss_HANDLER_A2.cr3 = tss_A2.cr3;
    tss_HANDLER_A2.eip = eip;
    tss_HANDLER_A2.eflags = tasksAndHandlers_eflags;
    tss_HANDLER_A2.eax = tasksAndHandlers_eax;
    tss_HANDLER_A2.ecx = tasksAndHandlers_ecx;
    tss_HANDLER_A2.edx = tasksAndHandlers_edx;
    tss_HANDLER_A2.ebx = tasksAndHandlers_ebx;
    tss_HANDLER_A2.esp = handlers_esp;
    tss_HANDLER_A2.ebp = handlers_ebp;
    tss_HANDLER_A2.esi = tasksAndHandlers_esi;
    tss_HANDLER_A2.edi = tasksAndHandlers_edi;
    tss_HANDLER_A2.es = tasksAndHandlers_es;
    tss_HANDLER_A2.unused4 = tasksAndHandlers_unused4;
    tss_HANDLER_A2.cs = tasksAndHandlers_cs;
    tss_HANDLER_A2.unused5 = tasksAndHandlers_unused5;
    tss_HANDLER_A2.ss = tasksAndHandlers_ss;
    tss_HANDLER_A2.unused6 = tasksAndHandlers_unused6;
    tss_HANDLER_A2.ds = tasksAndHandlers_ds;
    tss_HANDLER_A2.unused7 = tasksAndHandlers_unused7;
    tss_HANDLER_A2.fs = tasksAndHandlers_fs;
    tss_HANDLER_A2.unused8 = tasksAndHandlers_unused8;
    tss_HANDLER_A2.gs = tasksAndHandlers_gs;
    tss_HANDLER_A2.unused9 = tasksAndHandlers_unused9;
    tss_HANDLER_A2.ldt = tasksAndHandlers_ldt;
    tss_HANDLER_A2.unused10 = tasksAndHandlers_unused10;
    tss_HANDLER_A2.dtrap = tasksAndHandlers_dtrap;
    tss_HANDLER_A2.iomap = tasksAndHandlers_iomap;
}




void llenarTSSA3(uint32_t slotDeTarea){
	tss_A3.ptl = tasksAndHandlers_ptl;
    tss_A3.unused0 = tasksAndHandlers_unused0;
    
    // esp0 para tarea es (por figura 5) 
    tss_A3.esp0 = kernelLevelTasksStacks[slotDeTarea] + 2 * 1024 - 1;
    tss_A3.ss0 = tasksAndHandlers_ss0;
    tss_A3.unused1 = tasksAndHandlers_unused1;
    tss_A3.esp1 = tasksAndHandlers_esp1;
    tss_A3.ss1 = tasksAndHandlers_ss1;
    tss_A3.unused2 = tasksAndHandlers_unused2;
    tss_A3.esp2 = tasksAndHandlers_esp2;
    tss_A3.ss2 = tasksAndHandlers_ss2;
    tss_A3.unused3 = tasksAndHandlers_unused3;
    tss_A3.cr3 = userLevelTasksCodeAndStacks[slotDeTarea];
    tss_A3.eip = tasks_eip;
    tss_A3.eflags = tasksAndHandlers_eflags;
    tss_A3.eax = tasksAndHandlers_eax;
    tss_A3.ecx = tasksAndHandlers_ecx;
    tss_A3.edx = tasksAndHandlers_edx;
    tss_A3.ebx = tasksAndHandlers_ebx;
    tss_A3.esp = tasks_esp;
    tss_A3.ebp = tasks_ebp;
    tss_A3.esi = tasksAndHandlers_esi;
    tss_A3.edi = tasksAndHandlers_edi;
    tss_A3.es = tasksAndHandlers_es;
    tss_A3.unused4 = tasksAndHandlers_unused4;
    tss_A3.cs = tasksAndHandlers_cs;
    tss_A3.unused5 = tasksAndHandlers_unused5;
    tss_A3.ss = tasksAndHandlers_ss;
    tss_A3.unused6 = tasksAndHandlers_unused6;
    tss_A3.ds = tasksAndHandlers_ds;
    tss_A3.unused7 = tasksAndHandlers_unused7;
    tss_A3.fs = tasksAndHandlers_fs;
    tss_A3.unused8 = tasksAndHandlers_unused8;
    tss_A3.gs = tasksAndHandlers_gs;
    tss_A3.unused9 = tasksAndHandlers_unused9;
    tss_A3.ldt = tasksAndHandlers_ldt;
    tss_A3.unused10 = tasksAndHandlers_unused10;
    tss_A3.dtrap = tasksAndHandlers_dtrap;
    tss_A3.iomap = tasksAndHandlers_iomap;
}
void llenarTSSA3_Handler(uint32_t slotDeTarea, uint32_t eip){
	tss_HANDLER_A3.ptl = tasksAndHandlers_ptl;
    tss_HANDLER_A3.unused0 = tasksAndHandlers_unused0;
    
    //esp0 para handler es (por figura 5)
    tss_HANDLER_A3.esp0 = kernelLevelTasksStacks[slotDeTarea] + 4 * 1024 - 1;

    tss_HANDLER_A3.ss0 = tasksAndHandlers_ss0;
    tss_HANDLER_A3.unused1 = tasksAndHandlers_unused1;
    tss_HANDLER_A3.esp1 = tasksAndHandlers_esp1;
    tss_HANDLER_A3.ss1 = tasksAndHandlers_ss1;
    tss_HANDLER_A3.unused2 = tasksAndHandlers_unused2;
    tss_HANDLER_A3.esp2 = tasksAndHandlers_esp2;
    tss_HANDLER_A3.ss2 = tasksAndHandlers_ss2;
    tss_HANDLER_A3.unused3 = tasksAndHandlers_unused3;
    tss_HANDLER_A3.cr3 = tss_A3.cr3;
    tss_HANDLER_A3.eip = eip;
    tss_HANDLER_A3.eflags = tasksAndHandlers_eflags;
    tss_HANDLER_A3.eax = tasksAndHandlers_eax;
    tss_HANDLER_A3.ecx = tasksAndHandlers_ecx;
    tss_HANDLER_A3.edx = tasksAndHandlers_edx;
    tss_HANDLER_A3.ebx = tasksAndHandlers_ebx;
    tss_HANDLER_A3.esp = handlers_esp;
    tss_HANDLER_A3.ebp = handlers_ebp;
    tss_HANDLER_A3.esi = tasksAndHandlers_esi;
    tss_HANDLER_A3.edi = tasksAndHandlers_edi;
    tss_HANDLER_A3.es = tasksAndHandlers_es;
    tss_HANDLER_A3.unused4 = tasksAndHandlers_unused4;
    tss_HANDLER_A3.cs = tasksAndHandlers_cs;
    tss_HANDLER_A3.unused5 = tasksAndHandlers_unused5;
    tss_HANDLER_A3.ss = tasksAndHandlers_ss;
    tss_HANDLER_A3.unused6 = tasksAndHandlers_unused6;
    tss_HANDLER_A3.ds = tasksAndHandlers_ds;
    tss_HANDLER_A3.unused7 = tasksAndHandlers_unused7;
    tss_HANDLER_A3.fs = tasksAndHandlers_fs;
    tss_HANDLER_A3.unused8 = tasksAndHandlers_unused8;
    tss_HANDLER_A3.gs = tasksAndHandlers_gs;
    tss_HANDLER_A3.unused9 = tasksAndHandlers_unused9;
    tss_HANDLER_A3.ldt = tasksAndHandlers_ldt;
    tss_HANDLER_A3.unused10 = tasksAndHandlers_unused10;
    tss_HANDLER_A3.dtrap = tasksAndHandlers_dtrap;
    tss_HANDLER_A3.iomap = tasksAndHandlers_iomap;
}




void llenarTSSB1(uint32_t slotDeTarea){
	tss_B1.ptl = tasksAndHandlers_ptl;
    tss_B1.unused0 = tasksAndHandlers_unused0;
    
    // esp0 para tarea es (por figura 5) 
    tss_B1.esp0 = kernelLevelTasksStacks[slotDeTarea] + 2 * 1024 - 1;
    tss_B1.ss0 = tasksAndHandlers_ss0;
    tss_B1.unused1 = tasksAndHandlers_unused1;
    tss_B1.esp1 = tasksAndHandlers_esp1;
    tss_B1.ss1 = tasksAndHandlers_ss1;
    tss_B1.unused2 = tasksAndHandlers_unused2;
    tss_B1.esp2 = tasksAndHandlers_esp2;
    tss_B1.ss2 = tasksAndHandlers_ss2;
    tss_B1.unused3 = tasksAndHandlers_unused3;
    tss_B1.cr3 = userLevelTasksCodeAndStacks[slotDeTarea];
    tss_B1.eip = tasks_eip;
    tss_B1.eflags = tasksAndHandlers_eflags;
    tss_B1.eax = tasksAndHandlers_eax;
    tss_B1.ecx = tasksAndHandlers_ecx;
    tss_B1.edx = tasksAndHandlers_edx;
    tss_B1.ebx = tasksAndHandlers_ebx;
    tss_B1.esp = tasks_esp;
    tss_B1.ebp = tasks_ebp;
    tss_B1.esi = tasksAndHandlers_esi;
    tss_B1.edi = tasksAndHandlers_edi;
    tss_B1.es = tasksAndHandlers_es;
    tss_B1.unused4 = tasksAndHandlers_unused4;
    tss_B1.cs = tasksAndHandlers_cs;
    tss_B1.unused5 = tasksAndHandlers_unused5;
    tss_B1.ss = tasksAndHandlers_ss;
    tss_B1.unused6 = tasksAndHandlers_unused6;
    tss_B1.ds = tasksAndHandlers_ds;
    tss_B1.unused7 = tasksAndHandlers_unused7;
    tss_B1.fs = tasksAndHandlers_fs;
    tss_B1.unused8 = tasksAndHandlers_unused8;
    tss_B1.gs = tasksAndHandlers_gs;
    tss_B1.unused9 = tasksAndHandlers_unused9;
    tss_B1.ldt = tasksAndHandlers_ldt;
    tss_B1.unused10 = tasksAndHandlers_unused10;
    tss_B1.dtrap = tasksAndHandlers_dtrap;
    tss_B1.iomap = tasksAndHandlers_iomap;
}
void llenarTSSB1_Handler(uint32_t slotDeTarea, uint32_t eip){
	tss_HANDLER_B1.ptl = tasksAndHandlers_ptl;
    tss_HANDLER_B1.unused0 = tasksAndHandlers_unused0;
    
    //esp0 para handler es (por figura 5)
    tss_HANDLER_B1.esp0 = kernelLevelTasksStacks[slotDeTarea] + 4 * 1024 - 1;

    tss_HANDLER_B1.ss0 = tasksAndHandlers_ss0;
    tss_HANDLER_B1.unused1 = tasksAndHandlers_unused1;
    tss_HANDLER_B1.esp1 = tasksAndHandlers_esp1;
    tss_HANDLER_B1.ss1 = tasksAndHandlers_ss1;
    tss_HANDLER_B1.unused2 = tasksAndHandlers_unused2;
    tss_HANDLER_B1.esp2 = tasksAndHandlers_esp2;
    tss_HANDLER_B1.ss2 = tasksAndHandlers_ss2;
    tss_HANDLER_B1.unused3 = tasksAndHandlers_unused3;
    tss_HANDLER_B1.cr3 = tss_B1.cr3;
    tss_HANDLER_B1.eip = eip;
    tss_HANDLER_B1.eflags = tasksAndHandlers_eflags;
    tss_HANDLER_B1.eax = tasksAndHandlers_eax;
    tss_HANDLER_B1.ecx = tasksAndHandlers_ecx;
    tss_HANDLER_B1.edx = tasksAndHandlers_edx;
    tss_HANDLER_B1.ebx = tasksAndHandlers_ebx;
    tss_HANDLER_B1.esp = handlers_esp;
    tss_HANDLER_B1.ebp = handlers_ebp;
    tss_HANDLER_B1.esi = tasksAndHandlers_esi;
    tss_HANDLER_B1.edi = tasksAndHandlers_edi;
    tss_HANDLER_B1.es = tasksAndHandlers_es;
    tss_HANDLER_B1.unused4 = tasksAndHandlers_unused4;
    tss_HANDLER_B1.cs = tasksAndHandlers_cs;
    tss_HANDLER_B1.unused5 = tasksAndHandlers_unused5;
    tss_HANDLER_B1.ss = tasksAndHandlers_ss;
    tss_HANDLER_B1.unused6 = tasksAndHandlers_unused6;
    tss_HANDLER_B1.ds = tasksAndHandlers_ds;
    tss_HANDLER_B1.unused7 = tasksAndHandlers_unused7;
    tss_HANDLER_B1.fs = tasksAndHandlers_fs;
    tss_HANDLER_B1.unused8 = tasksAndHandlers_unused8;
    tss_HANDLER_B1.gs = tasksAndHandlers_gs;
    tss_HANDLER_B1.unused9 = tasksAndHandlers_unused9;
    tss_HANDLER_B1.ldt = tasksAndHandlers_ldt;
    tss_HANDLER_B1.unused10 = tasksAndHandlers_unused10;
    tss_HANDLER_B1.dtrap = tasksAndHandlers_dtrap;
    tss_HANDLER_B1.iomap = tasksAndHandlers_iomap;
}






void llenarTSSB2(uint32_t slotDeTarea){
	tss_B2.ptl = tasksAndHandlers_ptl;
    tss_B2.unused0 = tasksAndHandlers_unused0;
    
    // esp0 para tarea es (por figura 5) 
    tss_B2.esp0 = kernelLevelTasksStacks[slotDeTarea] + 2 * 1024 - 1;
    tss_B2.ss0 = tasksAndHandlers_ss0;
    tss_B2.unused1 = tasksAndHandlers_unused1;
    tss_B2.esp1 = tasksAndHandlers_esp1;
    tss_B2.ss1 = tasksAndHandlers_ss1;
    tss_B2.unused2 = tasksAndHandlers_unused2;
    tss_B2.esp2 = tasksAndHandlers_esp2;
    tss_B2.ss2 = tasksAndHandlers_ss2;
    tss_B2.unused3 = tasksAndHandlers_unused3;
    tss_B2.cr3 = userLevelTasksCodeAndStacks[slotDeTarea];
    tss_B2.eip = tasks_eip;
    tss_B2.eflags = tasksAndHandlers_eflags;
    tss_B2.eax = tasksAndHandlers_eax;
    tss_B2.ecx = tasksAndHandlers_ecx;
    tss_B2.edx = tasksAndHandlers_edx;
    tss_B2.ebx = tasksAndHandlers_ebx;
    tss_B2.esp = tasks_esp;
    tss_B2.ebp = tasks_ebp;
    tss_B2.esi = tasksAndHandlers_esi;
    tss_B2.edi = tasksAndHandlers_edi;
    tss_B2.es = tasksAndHandlers_es;
    tss_B2.unused4 = tasksAndHandlers_unused4;
    tss_B2.cs = tasksAndHandlers_cs;
    tss_B2.unused5 = tasksAndHandlers_unused5;
    tss_B2.ss = tasksAndHandlers_ss;
    tss_B2.unused6 = tasksAndHandlers_unused6;
    tss_B2.ds = tasksAndHandlers_ds;
    tss_B2.unused7 = tasksAndHandlers_unused7;
    tss_B2.fs = tasksAndHandlers_fs;
    tss_B2.unused8 = tasksAndHandlers_unused8;
    tss_B2.gs = tasksAndHandlers_gs;
    tss_B2.unused9 = tasksAndHandlers_unused9;
    tss_B2.ldt = tasksAndHandlers_ldt;
    tss_B2.unused10 = tasksAndHandlers_unused10;
    tss_B2.dtrap = tasksAndHandlers_dtrap;
    tss_B2.iomap = tasksAndHandlers_iomap;
}
void llenarTSSB2_Handler(uint32_t slotDeTarea, uint32_t eip){
	tss_HANDLER_B2.ptl = tasksAndHandlers_ptl;
    tss_HANDLER_B2.unused0 = tasksAndHandlers_unused0;
    
    //esp0 para handler es (por figura 5)
    tss_HANDLER_B2.esp0 = kernelLevelTasksStacks[slotDeTarea] + 4 * 1024 - 1;

    tss_HANDLER_B2.ss0 = tasksAndHandlers_ss0;
    tss_HANDLER_B2.unused1 = tasksAndHandlers_unused1;
    tss_HANDLER_B2.esp1 = tasksAndHandlers_esp1;
    tss_HANDLER_B2.ss1 = tasksAndHandlers_ss1;
    tss_HANDLER_B2.unused2 = tasksAndHandlers_unused2;
    tss_HANDLER_B2.esp2 = tasksAndHandlers_esp2;
    tss_HANDLER_B2.ss2 = tasksAndHandlers_ss2;
    tss_HANDLER_B2.unused3 = tasksAndHandlers_unused3;
    tss_HANDLER_B2.cr3 = tss_B2.cr3;
    tss_HANDLER_B2.eip = eip;
    tss_HANDLER_B2.eflags = tasksAndHandlers_eflags;
    tss_HANDLER_B2.eax = tasksAndHandlers_eax;
    tss_HANDLER_B2.ecx = tasksAndHandlers_ecx;
    tss_HANDLER_B2.edx = tasksAndHandlers_edx;
    tss_HANDLER_B2.ebx = tasksAndHandlers_ebx;
    tss_HANDLER_B2.esp = handlers_esp;
    tss_HANDLER_B2.ebp = handlers_ebp;
    tss_HANDLER_B2.esi = tasksAndHandlers_esi;
    tss_HANDLER_B2.edi = tasksAndHandlers_edi;
    tss_HANDLER_B2.es = tasksAndHandlers_es;
    tss_HANDLER_B2.unused4 = tasksAndHandlers_unused4;
    tss_HANDLER_B2.cs = tasksAndHandlers_cs;
    tss_HANDLER_B2.unused5 = tasksAndHandlers_unused5;
    tss_HANDLER_B2.ss = tasksAndHandlers_ss;
    tss_HANDLER_B2.unused6 = tasksAndHandlers_unused6;
    tss_HANDLER_B2.ds = tasksAndHandlers_ds;
    tss_HANDLER_B2.unused7 = tasksAndHandlers_unused7;
    tss_HANDLER_B2.fs = tasksAndHandlers_fs;
    tss_HANDLER_B2.unused8 = tasksAndHandlers_unused8;
    tss_HANDLER_B2.gs = tasksAndHandlers_gs;
    tss_HANDLER_B2.unused9 = tasksAndHandlers_unused9;
    tss_HANDLER_B2.ldt = tasksAndHandlers_ldt;
    tss_HANDLER_B2.unused10 = tasksAndHandlers_unused10;
    tss_HANDLER_B2.dtrap = tasksAndHandlers_dtrap;
    tss_HANDLER_B2.iomap = tasksAndHandlers_iomap;
}





void llenarTSSB3(uint32_t slotDeTarea){
	tss_B3.ptl = tasksAndHandlers_ptl;
    tss_B3.unused0 = tasksAndHandlers_unused0;
    
    // esp0 para tarea es (por figura 5) 
    tss_B3.esp0 = kernelLevelTasksStacks[slotDeTarea] + 2 * 1024 - 1;
    tss_B3.ss0 = tasksAndHandlers_ss0;
    tss_B3.unused1 = tasksAndHandlers_unused1;
    tss_B3.esp1 = tasksAndHandlers_esp1;
    tss_B3.ss1 = tasksAndHandlers_ss1;
    tss_B3.unused2 = tasksAndHandlers_unused2;
    tss_B3.esp2 = tasksAndHandlers_esp2;
    tss_B3.ss2 = tasksAndHandlers_ss2;
    tss_B3.unused3 = tasksAndHandlers_unused3;
    tss_B3.cr3 = userLevelTasksCodeAndStacks[slotDeTarea];
    tss_B3.eip = tasks_eip;
    tss_B3.eflags = tasksAndHandlers_eflags;
    tss_B3.eax = tasksAndHandlers_eax;
    tss_B3.ecx = tasksAndHandlers_ecx;
    tss_B3.edx = tasksAndHandlers_edx;
    tss_B3.ebx = tasksAndHandlers_ebx;
    tss_B3.esp = tasks_esp;
    tss_B3.ebp = tasks_ebp;
    tss_B3.esi = tasksAndHandlers_esi;
    tss_B3.edi = tasksAndHandlers_edi;
    tss_B3.es = tasksAndHandlers_es;
    tss_B3.unused4 = tasksAndHandlers_unused4;
    tss_B3.cs = tasksAndHandlers_cs;
    tss_B3.unused5 = tasksAndHandlers_unused5;
    tss_B3.ss = tasksAndHandlers_ss;
    tss_B3.unused6 = tasksAndHandlers_unused6;
    tss_B3.ds = tasksAndHandlers_ds;
    tss_B3.unused7 = tasksAndHandlers_unused7;
    tss_B3.fs = tasksAndHandlers_fs;
    tss_B3.unused8 = tasksAndHandlers_unused8;
    tss_B3.gs = tasksAndHandlers_gs;
    tss_B3.unused9 = tasksAndHandlers_unused9;
    tss_B3.ldt = tasksAndHandlers_ldt;
    tss_B3.unused10 = tasksAndHandlers_unused10;
    tss_B3.dtrap = tasksAndHandlers_dtrap;
    tss_B3.iomap = tasksAndHandlers_iomap;
}
void llenarTSSB3_Handler(uint32_t slotDeTarea, uint32_t eip){
	tss_HANDLER_B3.ptl = tasksAndHandlers_ptl;
    tss_HANDLER_B3.unused0 = tasksAndHandlers_unused0;
    
    //esp0 para handler es (por figura 5)
    tss_HANDLER_B3.esp0 = kernelLevelTasksStacks[slotDeTarea] + 4 * 1024 - 1;

    tss_HANDLER_B3.ss0 = tasksAndHandlers_ss0;
    tss_HANDLER_B3.unused1 = tasksAndHandlers_unused1;
    tss_HANDLER_B3.esp1 = tasksAndHandlers_esp1;
    tss_HANDLER_B3.ss1 = tasksAndHandlers_ss1;
    tss_HANDLER_B3.unused2 = tasksAndHandlers_unused2;
    tss_HANDLER_B3.esp2 = tasksAndHandlers_esp2;
    tss_HANDLER_B3.ss2 = tasksAndHandlers_ss2;
    tss_HANDLER_B3.unused3 = tasksAndHandlers_unused3;
    tss_HANDLER_B3.cr3 = tss_B3.cr3;
    tss_HANDLER_B3.eip = eip;
    tss_HANDLER_B3.eflags = tasksAndHandlers_eflags;
    tss_HANDLER_B3.eax = tasksAndHandlers_eax;
    tss_HANDLER_B3.ecx = tasksAndHandlers_ecx;
    tss_HANDLER_B3.edx = tasksAndHandlers_edx;
    tss_HANDLER_B3.ebx = tasksAndHandlers_ebx;
    tss_HANDLER_B3.esp = handlers_esp;
    tss_HANDLER_B3.ebp = handlers_ebp;
    tss_HANDLER_B3.esi = tasksAndHandlers_esi;
    tss_HANDLER_B3.edi = tasksAndHandlers_edi;
    tss_HANDLER_B3.es = tasksAndHandlers_es;
    tss_HANDLER_B3.unused4 = tasksAndHandlers_unused4;
    tss_HANDLER_B3.cs = tasksAndHandlers_cs;
    tss_HANDLER_B3.unused5 = tasksAndHandlers_unused5;
    tss_HANDLER_B3.ss = tasksAndHandlers_ss;
    tss_HANDLER_B3.unused6 = tasksAndHandlers_unused6;
    tss_HANDLER_B3.ds = tasksAndHandlers_ds;
    tss_HANDLER_B3.unused7 = tasksAndHandlers_unused7;
    tss_HANDLER_B3.fs = tasksAndHandlers_fs;
    tss_HANDLER_B3.unused8 = tasksAndHandlers_unused8;
    tss_HANDLER_B3.gs = tasksAndHandlers_gs;
    tss_HANDLER_B3.unused9 = tasksAndHandlers_unused9;
    tss_HANDLER_B3.ldt = tasksAndHandlers_ldt;
    tss_HANDLER_B3.unused10 = tasksAndHandlers_unused10;
    tss_HANDLER_B3.dtrap = tasksAndHandlers_dtrap;
    tss_HANDLER_B3.iomap = tasksAndHandlers_iomap;
}



void initUserTask(uint32_t slotDeTarea, uint32_t esHandler, uint32_t punteroARutinaHandler) {

	// con el slot de tare recibido hago el "case-switch magico", que justamente es lo que hace la funcion mmu_initTaskDir.
	switch(slotDeTarea){
		case 0:
			//Jugador A slot 1
			//Tengo que llenar la tss en cuestion para este slot de tarea y jugador:
			if(esHandler == 1) {
				llenarTSSA1_Handler(slotDeTarea, punteroARutinaHandler);
			} else {
				llenarTSSA1(slotDeTarea);
			}
			break;
		case 1:
			//Jugador A slot 2
			if(esHandler == 1) {
				llenarTSSA2_Handler(slotDeTarea, punteroARutinaHandler);
			} else {
				llenarTSSA2(slotDeTarea);
                // print_hex((uint32_t)&tss_A2, 8, 20,22,0xC);
                // breakpoint();
			}
			break;
		case 2:
			//Jugador A slot 3
			if(esHandler == 1) {
				llenarTSSA3_Handler(slotDeTarea, punteroARutinaHandler);
			} else {
				llenarTSSA3(slotDeTarea);
			}
			break;
		case 3:
			//Jugador B slot 1
			if(esHandler == 1) {
				llenarTSSB1_Handler(slotDeTarea, punteroARutinaHandler);
			} else {
				llenarTSSB1(slotDeTarea);
			}
			break;
		case 4:
			//Jugador B slot 2
			if(esHandler == 1) {
				llenarTSSB2_Handler(slotDeTarea, punteroARutinaHandler);
			} else {
				llenarTSSB2(slotDeTarea);
			}
			break;
		case 5:
			//Jugador B slot 3
			if(esHandler == 1) {
				llenarTSSB3_Handler(slotDeTarea, punteroARutinaHandler);
			} else {
				llenarTSSB3(slotDeTarea);
			}
			break;
	}
}


