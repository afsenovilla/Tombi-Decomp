// DATA 801345cc 40 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80134738[];
extern char D_8013477C[];
extern char D_801347C0[];
extern char D_801347EC[];
extern char D_80134818[];
extern char D_80134824[];
extern char D_80134848[];
extern char D_8013486C[];
extern char D_80134888[];
extern char D_801348B4[];
extern void *ptrs_801345CC[];
extern void *ptrs_801345D0[];
extern void *ptrs_801345DC[];
extern void *ptrs_801345E0[];
extern void *ptrs_801345E8[];
extern void *ptrs_801345EC[];

void *ptrs_801345CC[1] = {
    D_80134738,
};

void *ptrs_801345D0[3] = {
    D_8013477C,
    D_801347C0,
    D_801347EC,
};

void *ptrs_801345DC[1] = {
    D_80134818,
};

void *ptrs_801345E0[2] = {
    D_80134824,
    D_80134848,
};

void *ptrs_801345E8[1] = {
    D_8013486C,
};

void *ptrs_801345EC[2] = {
    D_80134888,
    D_801348B4,
};
