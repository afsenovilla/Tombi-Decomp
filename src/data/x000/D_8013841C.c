// DATA 8013841c 460 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void cameraEventScrollHandler();
extern void cameraModeHandler2();
extern void cameraModeHandler3();
extern void cameraModeHandler5();
extern void a00IntroEventScroll();
extern void updateCameraFollow();
extern void updateLookScroll();
extern void *ptrs_8013841C[];
extern unsigned int data_80138438[];
extern unsigned int data_80138450[];
extern unsigned int data_80138460[];
extern unsigned int data_80138478[];
extern unsigned int data_80138488[];
extern unsigned int data_80138498[];
extern void *ptrs_801384A8[];
extern unsigned int data_801384C0[];
extern unsigned int data_80138510[];
extern unsigned int data_80138558[];
extern unsigned int data_801385A0[];

void *ptrs_8013841C[7] = {
    updateCameraFollow,
    a00IntroEventScroll,
    cameraEventScrollHandler,
    cameraModeHandler2,
    cameraModeHandler3,
    updateLookScroll,
    cameraModeHandler5,
};

unsigned int data_80138438[6] = {
    0xA004B0, 0x40000, 0xA004B0, 0x40000,
    0x0, 0x21202,
};

unsigned int data_80138450[4] = {
    0xA00A46, 0x2050000, 0x0, 0x5030202,
};

unsigned int data_80138460[6] = {
    0xA00A46, 0x0, 0x0, 0x30002,
    0x0, 0x30002,
};

unsigned int data_80138478[4] = {
    0x0, 0x2020002, 0x0, 0x1000102,
};

unsigned int data_80138488[4] = {
    0x4B0, 0x1000001, 0x0, 0x602,
};

unsigned int data_80138498[4] = {
    0xA28, 0x1010001, 0x0, 0x1030A02,
};

void *ptrs_801384A8[6] = {
    data_80138438,
    data_80138450,
    data_80138460,
    data_80138478,
    data_80138488,
    data_80138498,
};

unsigned int data_801384C0[20] = {
    0x4060504, 0x4040A09, 0x320000A, 0xFF9C,
    0xC80000, 0xFF380000, 0x40000, 0x0,
    0xFE7005DC, 0x0, 0x0, 0x2580004,
    0x258FF6A, 0xFF6A, 0x0, 0x40000,
    0x96FDA8, 0x96FDA8, 0x0, 0x0,
};

unsigned int data_80138510[18] = {
    0xFF9C, 0xFF9C, 0xFF9C, 0xFF9CFF9C,
    0xFF9C, 0xFF9CFF9C, 0xFF9C, 0xFF9CFF9C,
    0xFF9C, 0xFF9CFF9C, 0xFF9C, 0xFF9CFF9C,
    0xFF9C, 0xFF9CFF9C, 0xFF38FF38, 0xFF38FED4,
    0xFF38FE70, 0x19001F4,
};

unsigned int data_80138558[18] = {
    0xFF9C, 0xFF9C, 0xFF9C, 0xFF9CFF9C,
    0xFF9C, 0xFF9CFF9C, 0xFF9C, 0xFF9CFF9C,
    0xFF9C, 0xFF9CFF9C, 0xFF38, 0xFF9CFF38,
    0xFF38, 0xFF9CFF38, 0xFF38FF38, 0xFF38FED4,
    0xFF38FE70, 0x19001F4,
};

unsigned int data_801385A0[18] = {
    0xFF9C, 0xFF9C, 0xFF9C, 0xFF9CFF9C,
    0xFF9C, 0xFF9CFF9C, 0xFF9C, 0xFF9CFF9C,
    0xFF38, 0xFF9CFF38, 0xFF38, 0xFF9CFF38,
    0xFF38, 0xFF9CFF38, 0xFF38FED4, 0xFF38FE70,
    0xFF38FE70, 0x19001F4,
};
