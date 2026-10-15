// DATA 8012a7b4 472 X009
// Data of X009 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_8012A7B4[];
extern unsigned int data_8012A7D4[];
extern unsigned int data_8012A7EC[];
extern unsigned int data_8012A7FC[];
extern unsigned int data_8012A80C[];
extern unsigned int data_8012A81C[];
extern unsigned int data_8012A82C[];
extern void *ptrs_8012A83C[];
extern unsigned int data_8012A858[];
extern unsigned int data_8012A878[];
extern unsigned int data_8012A87C[];
extern unsigned int data_8012A930[];
extern unsigned int data_8012A938[];

unsigned int data_8012A7B4[8] = {
    0xB400FB4, 0x10902, 0xB401040, 0x2030100,
    0xB401100, 0x2000301, 0x0, 0x5010402,
};

unsigned int data_8012A7D4[6] = {
    0x0, 0x4000902, 0x0, 0x40902,
    0x0, 0x50902,
};

unsigned int data_8012A7EC[4] = {
    0x0, 0x1050902, 0x0, 0x2,
};

unsigned int data_8012A7FC[4] = {
    0x0, 0x2000002, 0x0, 0x1040902,
};

unsigned int data_8012A80C[4] = {
    0x0, 0x1030902, 0x0, 0x1010902,
};

unsigned int data_8012A81C[4] = {
    0x0, 0x20902, 0x0, 0x2010902,
};

unsigned int data_8012A82C[4] = {
    0x0, 0x1010902, 0x0, 0x2000902,
};

void *ptrs_8012A83C[7] = {
    data_8012A7B4,
    data_8012A7D4,
    data_8012A7EC,
    data_8012A7FC,
    data_8012A80C,
    data_8012A81C,
    data_8012A82C,
};

unsigned int data_8012A858[8] = {
    0x33A0000, 0x7FFF027B, 0x425F01A1, 0x5E201C1,
    0xE260A04, 0x16891247, 0x1ECC1AAB, 0x2B3022EE,
};

unsigned int data_8012A878[1] = {
    0x202,
};

unsigned int data_8012A87C[45] = {
    0xFDEE080D, 0x303, 0xFD8008B0, 0x2,
    0xFD7F08C9, 0x101, 0xFD8708E0, 0x202,
    0xFD12093A, 0x10001, 0xFCA708D6, 0x10202,
    0xFC9E08EE, 0x10101, 0xFC300A19, 0x10001,
    0xFC930A8C, 0x10202, 0xFC980AA6, 0x101,
    0xFDC20B10, 0x303, 0xFDD80B5F, 0x10002,
    0xFCF70B14, 0x202, 0xFD6D0C64, 0x10202,
    0xFC470BAC, 0x10001, 0xFC450BC8, 0x10101,
    0xFC400BE4, 0x10303, 0xFC1D0C3B, 0x10202,
    0xFC1D0C56, 0x101, 0xFD960D3C, 0x10001,
    0xFD440DBE, 0x2, 0xFE3A0E74, 0x101,
    0xFE3E0E90,
};

unsigned int data_8012A930[2] = {
    0xFBE20870, 0xFCCC0E60,
};

unsigned int data_8012A938[21] = {
    0x2800050E, 0x50E0002, 0x22900, 0x320000A,
    0xFF9C, 0xC80000, 0xFF380000, 0x40000,
    0x0, 0xFE7005DC, 0x0, 0x0,
    0x2580004, 0x258FF6A, 0xFF6A, 0x0,
    0x40000, 0x96FDA8, 0x96FDA8, 0x0,
    0x0,
};
