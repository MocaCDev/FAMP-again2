#ifndef FAMP_PROTOCOL_SECOND_STAGE_H
#define FAMP_PROTOCOL_SECOND_STAGE_H
#define OS_RELATED
//#define BIT32_PROGRAM
#include <common.hpp>

extern uint8 filesystem[];

extern "C" void __load_gdt();
extern "C" void _init_pm();

#endif