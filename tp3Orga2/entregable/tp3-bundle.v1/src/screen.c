/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

void print(const char* text, uint32_t x, uint32_t y, uint8_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_number (uint8_t code){ // la funcion utilizada para imprimir por pantalla las teclas 0 a 9
    if(code > 0x0b || code < 0x02){
        return;
    }
    if(code == 0x0b) {
        print_dec((uint32_t)0, 1, 10, 10, ((0x0 << 4) | 0x7) );
    } else {
        print_dec((uint32_t)code - 1, 1, 10, 10, ((0x0 << 4) | 0x7) );
    }
}

void print_exception(uint8_t ex_number){
    char* excepciones_msgs[34];
    excepciones_msgs[0]= "Divide-by-zero Error";
    excepciones_msgs[1]= "Debug";
    excepciones_msgs[2]= "Non-maskable Interrupt";
    excepciones_msgs[3]= "Breakpoint";
    excepciones_msgs[4]= "Overflow";
    excepciones_msgs[5]= "Bound Range Exceeded";
    excepciones_msgs[6]= "Invalid Opcode";
    excepciones_msgs[7]= "Device Not Available";
    excepciones_msgs[8]= "Double Fault, estas en las malas papa";
    excepciones_msgs[9]= "Coprocessor Segment Overrun";
    excepciones_msgs[10]= "Invalid TSS";
    excepciones_msgs[11]= "Segment Not Present";
    excepciones_msgs[12]= "Stack-Segment Fault";
    excepciones_msgs[13]= "General Protection Fault";
    excepciones_msgs[14]= "Page Fault";
    excepciones_msgs[15]= "Reserved, no tocar";
    excepciones_msgs[16]= "x87 Floating-Point Exception";
    excepciones_msgs[17]= "Alignment Check";
    excepciones_msgs[18]= "Machine Check";
    excepciones_msgs[19]= "SIMD Floating-Point Exception";
    excepciones_msgs[20]= "Virtualization Exception";
    excepciones_msgs[21]= "Reserved";
    excepciones_msgs[22]= "Reserved";
    excepciones_msgs[23]= "Reserved";
    excepciones_msgs[24]= "Reserved";
    excepciones_msgs[25]= "Reserved";
    excepciones_msgs[26]= "Reserved";
    excepciones_msgs[27]= "Reserved";
    excepciones_msgs[28]= "Reserved";
    excepciones_msgs[29]= "Reserved, puto";
    excepciones_msgs[30]= "Security Exception";
    excepciones_msgs[31]= "Reserved, tipeas vos?";
    
    print(excepciones_msgs[ex_number], 10, 10, ((0x0 << 4) | 0x7) );
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint8_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint8_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    uint8_t hexa[8];
    uint8_t letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_drawBox(uint32_t fInit,
                    uint32_t cInit,
                    uint32_t fSize,
                    uint32_t cSize,
                    uint8_t character,
                    uint8_t attr ) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    uint32_t f;
    uint32_t c;
    for (f = fInit; f < fInit+fSize; f++) {
    for (c = cInit; c < cInit+cSize; c++) {
          p[f][c].c = character;
          p[f][c].a = attr;
    }}
}




