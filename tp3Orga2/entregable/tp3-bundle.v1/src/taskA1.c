/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 

void handler(void);

void task() {
    char* message = "Soy la tareita A1";
    syscall_talk(message);
    syscall_setHandler(handler);

    int i = 0;
    while(1) { 
        __asm __volatile("mov $1, %%eax":::"eax");
        
        if(i == 5000002){
            int x = i - i;
            char* a = (char*) x;
            a[0] = 'a';
        }


        i++;
    }
}

void handler() {
    syscall_informAction(Center);
}
