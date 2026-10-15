// DATA 80139124 316 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013B7D8[];
extern char D_8013B7E0[];
extern char D_8013B7E8[];
extern char D_8013B7F0[];
extern char D_8013B7F8[];
extern char D_8013B800[];
extern char D_8013B808[];
extern void FUN_8012f1b4();
extern void FUN_8012f380();
extern void FUN_8012fb3c();
extern void FUN_8012fe98();
extern void FUN_801302a0();
extern unsigned int data_80139124[];
extern unsigned int data_80139134[];
extern unsigned int data_8013913C[];
extern void *ptrs_80139154[];
extern unsigned int data_80139184[];
extern unsigned int data_8013919C[];
extern unsigned int data_801391B4[];
extern unsigned int data_801391CC[];
extern void *ptrs_801391E4[];
extern unsigned int data_80139208[];
extern unsigned int data_80139228[];
extern unsigned int data_8013922C[];
extern unsigned int data_80139230[];
extern void *ptrs_80139234[];
extern unsigned int data_80139240[];

unsigned int data_80139124[4] = {
    0x10000, 0x2020001, 0x1000100, 0x10201,
};

unsigned int data_80139134[2] = {
    0x20003, 0x10001,
};

unsigned int data_8013913C[6] = {
    0x280040, 0x200000, 0x0, 0x0,
    0x0, 0x0,
};

void *ptrs_80139154[12] = {
    D_8013B7D8,
    D_8013B7E0,
    D_8013B7E8,
    D_8013B7F0,
    D_8013B7F8,
    D_8013B7F8,
    D_8013B808,
    D_8013B808,
    D_8013B808,
    D_8013B800,
    D_8013B800,
    D_8013B800,
};

unsigned int data_80139184[6] = {
    0x4C0048, 0x4C0048, 0x4C0034, 0x4C0034,
    0x4C0022, 0x4C0022,
};

unsigned int data_8013919C[6] = {
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_801391B4[6] = {
    0x280020, 0x280020, 0x280020, 0x280020,
    0x280020, 0x280020,
};

unsigned int data_801391CC[6] = {
    0x3B0024, 0x430024, 0x3B0024, 0x430024,
    0x3B0024, 0x430024,
};

void *ptrs_801391E4[9] = {
    data_80139184,
    data_8013919C,
    data_801391B4,
    data_801391CC,
    FUN_8012f1b4,
    FUN_8012f380,
    FUN_8012fb3c,
    FUN_8012fe98,
    FUN_801302a0,
};

unsigned int data_80139208[8] = {
    0x18000C, 0x800000, 0x18000C, 0x200020,
    0x0, 0x0, 0x18000C, 0x800000,
};

unsigned int data_80139228[1] = {
    0x4F00310,
};

unsigned int data_8013922C[1] = {
    0x85C067C,
};

unsigned int data_80139230[1] = {
    0xCA80AC8,
};

void *ptrs_80139234[3] = {
    data_80139228,
    data_8013922C,
    data_80139230,
};

unsigned int data_80139240[8] = {
    0x0, 0x0, 0x10000, 0x10001,
    0x20002, 0x30002, 0x30003, 0x30003,
};
