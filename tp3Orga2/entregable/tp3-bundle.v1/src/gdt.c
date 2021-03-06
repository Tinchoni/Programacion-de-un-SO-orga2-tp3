/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = { //la tabla gdt
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) { //forma de acceder a la tabla en el indice GDT_IDX_NULL_DESC
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    //primer descriptor de segmento de codigo nivel 0
    [GDT_CODIGO_KERNEL] = (gdt_entry) {
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x08,           /* type = codigo execute-only = (basura de 4 bits) + 1000          */
        (uint8_t)     0x01,           /* s  = 1 SIEMPRE         */
        (uint8_t)     0x00,           /* dpl = 0, soy kernel          */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl       */
        (uint8_t)     0x00,           /* l = 0 , estamos en 32b          */
        (uint8_t)     0x01,           /* db = 1, voy a usar este segmento cuando pase a modo protegido.          */
        (uint8_t)     0x01,           /* g = 1, son 163mb, necesito apuntar de a 4kb            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [GDT_CODIGO_USUARIO] = (gdt_entry) {
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x08,           /* type = codigo execute-only = (basura de 4 bits) + 1000          */
        (uint8_t)     0x01,           /* s  = 1 SIEMPRE         */
        (uint8_t)     0x03,           /* dpl = 3, soy usuario, alto gil          */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l = 0 , estamos en 32b          */
        (uint8_t)     0x01,           /* db = 1, voy a usar este segmento cuando pase a modo protegido.          */
        (uint8_t)     0x01,           /* g = 1, son 163mb, necesito apuntar de a 4kb            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [GDT_DATOS_KERNEL] = (gdt_entry) {
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x02,      /* type = data read/write = (basura de 4 bits) + 0010          */
        (uint8_t)     0x01,           /* s  = 1 SIEMPRE         */
        (uint8_t)     0x00,           /* dpl = 0, soy kernel, alto no gil          */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l = 0 , estamos en 32b          */
        (uint8_t)     0x01,           /* db = 1, voy a usar este segmento cuando pase a modo protegido.          */
        (uint8_t)     0x01,           /* g = 1, son 163mb, necesito apuntar de a 4kb            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [GDT_DATOS_USUARIO] = (gdt_entry) {
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x02,      /* type = data read/write, accesed = (basura de 4 bits) + 0011          */
        (uint8_t)     0x01,           /* s  = 1 SIEMPRE         */
        (uint8_t)     0x03,           /* dpl = 3, soy usuario, alto  gil          */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l = 0 , estamos en 32b          */
        (uint8_t)     0x01,           /* db = 1, voy a usar este segmento cuando pase a modo protegido.          */
        (uint8_t)     0x01,           /* g = 1, son 163mb, necesito apuntar de a 4kb            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [GDT_PANTALLA_KERNEL] = (gdt_entry) {
        (uint16_t)    0x0001,         /* limit[0:15]  */
        (uint16_t)    0x8000,         /* base[0:15]   */
        (uint8_t)     0x0B,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type = data read/write = (basura de 4 bits) + 0010          */
        (uint8_t)     0x01,           /* s  = 1 SIEMPRE         */
        (uint8_t)     0x00,           /* dpl = 0, soy kernel          */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l = 0 , estamos en 32b          */
        (uint8_t)     0x01,           /* db = 1, voy a usar este segmento cuando pase a modo protegido.          */
        (uint8_t)     0x01,           /* g = 1, son 8kb, necesito apuntar de a 4kb            */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_TAREA_INICIAL] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15], vamos a llenarla desde la funcion tss_init, que esta en tss.c ya que la gdt es global.   */ 
        (uint8_t)     0x0B,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_IDLE] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_A1] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_A2] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_A3] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_HANDLER_A1] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_HANDLER_A2] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_HANDLER_A3] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },

    [GDT_TSS_B1] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_B2] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_B3] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_HANDLER_B1] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_HANDLER_B2] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    },
    [GDT_TSS_HANDLER_B3] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x00,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type = TSS not busy (b=0)*/
        (uint8_t)     0x00,           /* 0         */
        (uint8_t)     0x00,           /* dpl = 0, un usuario (una tarea) no puede ejecutar otra tarea         */
        (uint8_t)     0x01,           /* p = 1 SIEMPRE           */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* 0           */
        (uint8_t)     0x00,           /* 0    */
        (uint8_t)     0x00,           /* g = 0, usamos solo 100 bytes = 0x67 bytes.             */
        (uint8_t)     0x00,           /* base[31:24]  */  
    }
};

gdt_descriptor GDT_DESC = { // este es el gdtr de mierda 
    sizeof(gdt) - 1, //el fkin LIMIT
    (uint32_t) &gdt //la BASE 
};


// el limite es un puntero a la memoria al ultimo byte legible del segmento. Como Granularity = 1 => si limit = 00001010001100000000 (163mb / 4kb) el puntero sera 00001010001100000000111111111110 (espero)
