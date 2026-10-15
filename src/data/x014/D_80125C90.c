// DATA 80125c90 116 X014
// Data of X014 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void evilPigAttackBaccusVillage();
extern void evilPigAttackDeepJungle();
extern void evilPigAttackFlowerForest();
extern void evilPigAttackHauntedMansion();
extern void evilPigAttackLavaCaves();
extern void evilPigAttackPhoenixMountain();
extern void evilPigAttackRealEvilPig();
extern void evilPigAttackTrickVillage();
extern unsigned int data_80125C90[];
extern unsigned int data_80125C94[];
extern unsigned int data_80125C98[];
extern unsigned int data_80125C9C[];
extern unsigned int data_80125CA0[];
extern unsigned int data_80125CA8[];
extern unsigned int data_80125CB8[];
extern unsigned int data_80125CC4[];
extern unsigned int data_80125CC8[];
extern void *ptrs_80125CD8[];
extern void *ptrs_80125CE4[];
extern unsigned int data_80125CF8[];

unsigned int data_80125C90[1] = {
    0xFE2AFF13,
};

unsigned int data_80125C94[1] = {
    0xFE24FF05,
};

unsigned int data_80125C98[1] = {
    0xFE0CFF1A,
};

unsigned int data_80125C9C[1] = {
    0xF20204,
};

unsigned int data_80125CA0[2] = {
    0x600080, 0x66,
};

unsigned int data_80125CA8[4] = {
    0x10100, 0x1000001, 0x1000001, 0x10100,
};

unsigned int data_80125CB8[3] = {
    0x10001, 0x10101, 0x1000100,
};

unsigned int data_80125CC4[1] = {
    0x1010001,
};

unsigned int data_80125CC8[4] = {
    0x1010101, 0x1010100, 0x1000101, 0x1000101,
};

void *ptrs_80125CD8[3] = {
    evilPigAttackPhoenixMountain,
    evilPigAttackBaccusVillage,
    evilPigAttackTrickVillage,
};

void *ptrs_80125CE4[5] = {
    evilPigAttackLavaCaves,
    evilPigAttackFlowerForest,
    evilPigAttackDeepJungle,
    evilPigAttackHauntedMansion,
    evilPigAttackRealEvilPig,
};

unsigned int data_80125CF8[3] = {
    0x6, 0x7, 0xA,
};
