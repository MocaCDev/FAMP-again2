#ifndef FAMP_PROTOCOL_ENTER_RMODE_H

extern "C" void rmode_print();
extern "C" void enter_rmode();
extern "C" void enter_pmode();
extern "C" void enter_rmode_and_stay();
extern "C" void enter_rmode_and_raise_err(uint16 err_code);
extern "C" void enter_rmode_and_hlt();
extern "C" void enter_rmode_n(uint8 *msg);

#endif