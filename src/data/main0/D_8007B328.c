// DATA 8007b328 1472 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void dispatchAreaItemDraw();
extern void dispatchAreaItemInit();
extern void func_8003E960();
extern void func_8003EDB0();
extern unsigned int data_8007B328[];
extern unsigned int data_8007B35C[];
extern unsigned int data_8007B368[];
extern unsigned int data_8007B374[];
extern unsigned int data_8007B380[];
extern unsigned int data_8007B38C[];
extern unsigned int data_8007B3DC[];
extern unsigned int data_8007B3FC[];
extern unsigned int data_8007B414[];
extern unsigned int data_8007B42C[];
extern unsigned int data_8007B44C[];
extern unsigned int data_8007B450[];
extern void *ptrs_8007B460[];
extern void *ptrs_8007B46C[];
extern unsigned int data_8007B4B0[];
extern unsigned int data_8007B4B8[];
extern unsigned int data_8007B4C0[];
extern unsigned int data_8007B4C8[];
extern unsigned int data_8007B4D0[];
extern unsigned int data_8007B4E8[];
extern unsigned int data_8007B4EC[];
extern unsigned int data_8007B4F0[];
extern unsigned int data_8007B4F8[];
extern unsigned int data_8007B4FC[];
extern unsigned int data_8007B504[];
extern unsigned int data_8007B510[];
extern unsigned int data_8007B518[];
extern unsigned int data_8007B530[];
extern unsigned int data_8007B534[];
extern unsigned int data_8007B53C[];
extern unsigned int data_8007B540[];
extern unsigned int data_8007B548[];
extern unsigned int data_8007B554[];
extern unsigned int data_8007B558[];
extern void *ptrs_8007B55C[];
extern unsigned int data_8007B5AC[];
extern unsigned int data_8007B5DC[];
extern unsigned int data_8007B5E4[];
extern unsigned int data_8007B610[];
extern unsigned int data_8007B63C[];
extern unsigned int data_8007B6A4[];
extern unsigned int data_8007B720[];
extern unsigned int data_8007B73C[];
extern unsigned int data_8007B754[];
extern unsigned int data_8007B770[];
extern unsigned int data_8007B78C[];
extern unsigned int data_8007B7CC[];
extern unsigned int data_8007B7E0[];
extern unsigned int data_8007B7F0[];
extern unsigned int data_8007B804[];
extern unsigned int data_8007B828[];
extern unsigned int data_8007B850[];
extern unsigned int data_8007B868[];
extern unsigned int data_8007B874[];
extern unsigned int data_8007B898[];
extern unsigned int data_8007B8B8[];

unsigned int data_8007B328[13] = {
    0x80123B30, (unsigned int)dispatchAreaItemInit, 0x8003DD28, (unsigned int)func_8003E960,
    (unsigned int)func_8003EDB0, 0x801236F0, 0x80123A1C, 0x80123F24,
    0x8011F128, 0x80116B58, 0x8011BA3C, 0x80116D14,
    (unsigned int)dispatchAreaItemDraw,
};

unsigned int data_8007B35C[3] = {
    0xFF000080, 0xFF00FF80, 0xFF000000,
};

unsigned int data_8007B368[3] = {
    0x4010401, 0x4040401, 0x4,
};

unsigned int data_8007B374[3] = {
    0xFF000080, 0xFF00FF80, 0xFF000000,
};

unsigned int data_8007B380[3] = {
    0xFF000080, 0xFF00FF80, 0xFF000000,
};

unsigned int data_8007B38C[20] = {
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_8007B3DC[8] = {
    0x0, 0x0, 0x0, 0x0,
    0x4B00000, 0xA460000, 0x0, 0x0,
};

unsigned int data_8007B3FC[6] = {
    0x0, 0x0, 0x820000, 0x1700000,
    0x8800000, 0xB400000,
};

unsigned int data_8007B414[6] = {
    0xB400000, 0x0, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007B42C[8] = {
    0xFFC0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_8007B44C[1] = {
    0x1498,
};

unsigned int data_8007B450[4] = {
    0x1680000, 0x5460000, 0x1680000, 0x5A00000,
};

void *ptrs_8007B460[3] = {
    data_8007B3DC,
    data_8007B3FC,
    data_8007B38C,
};

void *ptrs_8007B46C[17] = {
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B414,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B42C,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
    data_8007B38C,
};

unsigned int data_8007B4B0[2] = {
    0x50505, 0x0,
};

unsigned int data_8007B4B8[2] = {
    0x0, 0x0,
};

unsigned int data_8007B4C0[2] = {
    0x100, 0x0,
};

unsigned int data_8007B4C8[2] = {
    0x10101, 0x101,
};

unsigned int data_8007B4D0[6] = {
    0x5030607, 0x0, 0x10000, 0x0,
    0x1000000, 0x0,
};

unsigned int data_8007B4E8[1] = {
    0x1000100,
};

unsigned int data_8007B4EC[1] = {
    0x0,
};

unsigned int data_8007B4F0[2] = {
    0x0, 0x0,
};

unsigned int data_8007B4F8[1] = {
    0x0,
};

unsigned int data_8007B4FC[2] = {
    0x1, 0x0,
};

unsigned int data_8007B504[3] = {
    0x20001, 0x20001, 0x0,
};

unsigned int data_8007B510[2] = {
    0x1000100, 0x0,
};

unsigned int data_8007B518[6] = {
    0x5030607, 0x0, 0x10000, 0x0,
    0x1000000, 0x0,
};

unsigned int data_8007B530[1] = {
    0x1,
};

unsigned int data_8007B534[2] = {
    0x0, 0x1,
};

unsigned int data_8007B53C[1] = {
    0x0,
};

unsigned int data_8007B540[2] = {
    0x0, 0x0,
};

unsigned int data_8007B548[3] = {
    0x0, 0x0, 0x0,
};

unsigned int data_8007B554[1] = {
    0x0,
};

unsigned int data_8007B558[1] = {
    0x100,
};

void *ptrs_8007B55C[20] = {
    data_8007B4B0,
    data_8007B4B8,
    data_8007B4C0,
    data_8007B4C8,
    data_8007B4D0,
    data_8007B4E8,
    data_8007B4EC,
    data_8007B4F0,
    data_8007B4F8,
    data_8007B4FC,
    data_8007B504,
    data_8007B510,
    data_8007B518,
    data_8007B530,
    data_8007B534,
    data_8007B53C,
    data_8007B540,
    data_8007B548,
    data_8007B554,
    data_8007B558,
};

unsigned int data_8007B5AC[12] = {
    0x8012F18C, 0x8012F3C8, 0x8012F58C, 0x8012F910,
    0x8012FACC, 0x8012FC88, 0x8012FE74, 0x80130150,
    0x801304B4, 0x801308C0, 0x80130B2C, 0x80130DC0,
};

unsigned int data_8007B5DC[2] = {
    0x0, 0x0,
};

unsigned int data_8007B5E4[11] = {
    0xFFFC0010, 0xFFFC0010, 0x0, 0x10,
    0x18, 0x1C, 0x10000A, 0x10,
    0x10, 0x0, 0x10,
};

unsigned int data_8007B610[11] = {
    0xFFFA0010, 0xFFFA0010, 0x0, 0xFFFE0010,
    0xFFFE0018, 0xFFFE001C, 0xFFFE000A, 0xFFFE0010,
    0xFFFE0010, 0x0, 0x10,
};

unsigned int data_8007B63C[26] = {
    0x1010001, 0x0, 0x10001, 0x10001,
    0x1000100, 0x10000, 0x1, 0x0,
    0x10100, 0x1000000, 0x0, 0x1,
    0x0, 0x0, 0x10001, 0x0,
    0x0, 0x1000000, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007B6A4[31] = {
    0x10000, 0x0, 0x10001, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x100, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x10101, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007B720[7] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370,
};

unsigned int data_8007B73C[6] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C,
};

unsigned int data_8007B754[7] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370,
};

unsigned int data_8007B770[7] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370,
};

unsigned int data_8007B78C[16] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370, 0x1F800374,
    0x1F800378, 0x1F80037C, 0x1F800380, 0x1F800384,
    0x1F800388, 0x1F80038C, 0x1F800390, 0x1F800394,
};

unsigned int data_8007B7CC[5] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368,
};

unsigned int data_8007B7E0[4] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
};

unsigned int data_8007B7F0[5] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368,
};

unsigned int data_8007B804[9] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370, 0x1F800374,
    0x1F800378,
};

unsigned int data_8007B828[10] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370, 0x1F800374,
    0x1F800378, 0x1F80037C,
};

unsigned int data_8007B850[6] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C,
};

unsigned int data_8007B868[3] = {
    0x1F800358, 0x1F80035C, 0x1F800360,
};

unsigned int data_8007B874[9] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370, 0x1F800374,
    0x1F800378,
};

unsigned int data_8007B898[8] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370, 0x1F800374,
};

unsigned int data_8007B8B8[12] = {
    0x1F800358, 0x1F80035C, 0x1F800360, 0x1F800364,
    0x1F800368, 0x1F80036C, 0x1F800370, 0x1F800374,
    0x1F800378, 0x1F80037C, 0x1F800380, 0x1F800384,
};
