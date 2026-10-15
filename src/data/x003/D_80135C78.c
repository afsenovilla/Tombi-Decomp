// DATA 80135c78 244 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80135C78[];
extern unsigned int data_80135CB0[];
extern unsigned int data_80135D2C[];
extern unsigned int data_80135D3C[];
extern unsigned int data_80135D4C[];
extern unsigned int data_80135D54[];

unsigned int data_80135C78[14] = {
    0x300018, 0x80004, 0x280018, 0x140010,
    0x280018, 0x160012, 0x300020, 0x1C0018,
    0x300018, 0x140010, 0x280028, 0x340030,
    0x280028, 0x2C0028,
};

unsigned int data_80135CB0[31] = {
    0x1B0E2814, 0x1B0E2814, 0x2D202814, 0x2D202814,
    0x1B0E2814, 0x1B0E2814, 0x1B0E2814, 0x13063018,
    0x13063018, 0x13062814, 0x15082814, 0x1B0E2814,
    0x5B4E1008, 0x5B4E1008, 0x13063018, 0x13063018,
    0x5B4E1008, 0x4B3E1008, 0x43361008, 0x3B2E1008,
    0x2D201008, 0x13063018, 0x13063018, 0x4B3E1008,
    0x43361008, 0x3B2E1008, 0x2D201008, 0x1F122814,
    0x23162814, 0x291C2814, 0x1B0E2814,
};

unsigned int data_80135D2C[4] = {
    0xC800B4, 0xC800B4, 0xB400B4, 0xA000B4,
};

unsigned int data_80135D3C[4] = {
    0xB400F0, 0x12C00B4, 0x168012C, 0xB400F0,
};

unsigned int data_80135D4C[2] = {
    0x8C787864, 0x7864788C,
};

unsigned int data_80135D54[6] = {
    0x3C283C50, 0x3C503C28, 0x1918170C, 0x1A1A1A1A,
    0x15150E0E, 0x16161616,
};
