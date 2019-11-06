/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004

/* Misc */
/* -------------------------------------------------------------------------- */

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 40

#define GDT_IDX_NULL_DESC           0
#define GDT_CODIGO_KERNEL 			14
#define GDT_CODIGO_USUARIO 			15
#define GDT_DATOS_KERNEL 			16
#define GDT_DATOS_USUARIO 			17
#define GDT_PANTALLA_KERNEL  		18
#define GDT_TSS_TAREA_INICIAL		19
#define GDT_TSS_IDLE				20

#define GDT_TSS_A1  				21
#define GDT_TSS_A2  				22
#define GDT_TSS_A3  				23
#define GDT_TSS_HANDLER_A1  		24
#define GDT_TSS_HANDLER_A2  		25
#define GDT_TSS_HANDLER_A3  		26

#define GDT_TSS_B1  				27
#define GDT_TSS_B2  				28
#define GDT_TSS_B3  				29
#define GDT_TSS_HANDLER_B1  		30
#define GDT_TSS_HANDLER_B2  		31
#define GDT_TSS_HANDLER_B3  		32



/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC << 3)


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE             0x08000000 /* direccion virtual del codigo */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
//#define KERNEL_PAGE_DIR          0x00027000
//#define KERNEL_PAGE_TABLE_0      0x00028000

#endif  /* !__DEFINES_H__ */
