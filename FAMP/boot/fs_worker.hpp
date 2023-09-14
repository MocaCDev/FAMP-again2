#ifndef FAMP_PROTOCOL_FS_WORKER_H
#define FAMP_PROTOCOL_FS_WORKER_H
#define OS_RELATED
#define BIT32_PROGRAM
#include <common.hpp>

extern uint8 filesystem[];

extern "C" void enter_rmode();
extern "C" void enter_pmode();
extern "C" void rmode_print();

#endif