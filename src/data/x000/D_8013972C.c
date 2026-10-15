// DATA 8013972c 2808 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013A278[];
extern char D_8013A2BC[];
extern char D_8013A300[];
extern char D_8013A344[];
extern char D_8013A354[];
extern char D_8013A36C[];
extern char D_8013A390[];
extern char D_8013A3B4[];
extern char D_8013A408[];
extern char D_8013A42C[];
extern char D_8013A434[];
extern char D_8013A478[];
extern char D_8013A4C0[];
extern char D_8013A540[];
extern char D_8013A5D0[];
extern char D_8013A660[];
extern char D_8013A688[];
extern char D_8013A6B0[];
extern char D_8013A6F0[];
extern char D_8013A740[];
extern char D_8013A7A4[];
extern char D_8013A7E4[];
extern char D_8013A804[];
extern char D_8013A80C[];
extern char D_8013A814[];
extern char D_8013A81C[];
extern char D_8013A824[];
extern char D_8013A858[];
extern char D_8013A89C[];
extern char D_8013A8D0[];
extern char D_8013A99C[];
extern char D_8013A9E8[];
extern char D_8013A9F8[];
extern char D_8013AA00[];
extern char D_8013AA18[];
extern char D_8013AA3C[];
extern char D_8013AAAC[];
extern char D_8013AAC4[];
extern char D_8013AAD4[];
extern char D_8013AAFC[];
extern char D_8013AB0C[];
extern char D_8013AB24[];
extern char D_8013AB34[];
extern char D_8013AB3C[];
extern char D_8013AB44[];
extern char D_8013AB4C[];
extern unsigned int data_8013972C[];
extern unsigned int data_80139774[];
extern unsigned int data_801397C8[];
extern unsigned int data_801397F8[];
extern unsigned int data_80139830[];
extern unsigned int data_8013985C[];
extern unsigned int data_80139898[];
extern unsigned int data_801398D8[];
extern unsigned int data_80139904[];
extern unsigned int data_80139914[];
extern unsigned int data_8013993C[];
extern unsigned int data_80139974[];
extern unsigned int data_801399C0[];
extern unsigned int data_80139A08[];
extern unsigned int data_80139A34[];
extern unsigned int data_80139A58[];
extern unsigned int data_80139AA0[];
extern unsigned int data_80139AD8[];
extern unsigned int data_80139B34[];
extern unsigned int data_80139B84[];
extern unsigned int data_80139BB8[];
extern unsigned int data_80139BF4[];
extern unsigned int data_80139C38[];
extern unsigned int data_80139C78[];
extern unsigned int data_80139CC8[];
extern unsigned int data_80139D04[];
extern void *ptrs_80139D24[];
extern unsigned int data_8013A048[];
extern unsigned int data_8013A0E8[];
extern void *ptrs_8013A160[];
extern void *ptrs_8013A178[];
extern void *ptrs_8013A17C[];
extern void *ptrs_8013A180[];
extern void *ptrs_8013A184[];
extern void *ptrs_8013A188[];
extern void *ptrs_8013A18C[];
extern void *ptrs_8013A194[];
extern void *ptrs_8013A198[];
extern void *ptrs_8013A19C[];
extern void *ptrs_8013A1A0[];
extern void *ptrs_8013A1A4[];
extern void *ptrs_8013A1A8[];
extern void *ptrs_8013A1AC[];
extern void *ptrs_8013A1B8[];
extern void *ptrs_8013A1BC[];
extern void *ptrs_8013A1C0[];
extern void *ptrs_8013A1C4[];
extern void *ptrs_8013A1D4[];
extern void *ptrs_8013A1D8[];
extern void *ptrs_8013A1DC[];
extern void *ptrs_8013A1E0[];
extern void *ptrs_8013A1E4[];
extern void *ptrs_8013A1E8[];
extern void *ptrs_8013A1EC[];
extern void *ptrs_8013A1F0[];
extern void *ptrs_8013A1F4[];
extern void *ptrs_8013A1F8[];
extern void *ptrs_8013A1FC[];
extern void *ptrs_8013A200[];
extern void *ptrs_8013A204[];
extern void *ptrs_8013A208[];
extern void *ptrs_8013A20C[];
extern void *ptrs_8013A210[];
extern void *ptrs_8013A214[];
extern void *ptrs_8013A21C[];
extern void *ptrs_8013A220[];

unsigned int data_8013972C[18] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0700, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_80139774[21] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0xF500, 0x100F700,
    0x100F900, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_801397C8[12] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FA00,
    0x8200FC00, 0x8300FF00, 0x84000100, 0x85000300,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_801397F8[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_80139830[11] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FE00, 0x83000000, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8013985C[15] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000600, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_80139898[16] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000100, 0x85000400, 0x3000600, 0x3000800,
    0x4000A00, 0x4000C00, 0x4000E00, 0xFF,
};

unsigned int data_801398D8[11] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FC00,
    0x8200FE00, 0x83000000, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_80139904[4] = {
    0xF200, 0xF400, 0xF600, 0x8000F800,
};

unsigned int data_80139914[10] = {
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000700, 0x3000900, 0x4000B00,
    0x4000D00, 0xFF,
};

unsigned int data_8013993C[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_80139974[19] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0xFF,
};

unsigned int data_801399C0[18] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000100, 0x2000300, 0x3000500,
    0x3000700, 0xFF,
};

unsigned int data_80139A08[11] = {
    0xF600, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_80139A34[9] = {
    0x100F700, 0x8000F900, 0x8100FB00, 0x8200FE00,
    0x83000000, 0x84000300, 0x85000500, 0x3000700,
    0xFF,
};

unsigned int data_80139A58[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_80139AA0[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_80139AD8[23] = {
    0xFEF100, 0xFEF300, 0xFEF500, 0x1FEF700,
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x4FE0A00, 0x4FE0C00, 0x4FE0E00, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_80139B34[20] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F900, 0x100FB00, 0x200FD00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_80139B84[13] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_80139BB8[15] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FB00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_80139BF4[17] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFF00,
    0x83FE0100, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_80139C38[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_80139C78[20] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_80139CC8[15] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0600,
    0x3FE0800, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0xFF,
};

unsigned int data_80139D04[8] = {
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000200, 0x85000400, 0x3000600, 0xFF,
};

void *ptrs_80139D24[201] = {
    data_8013972C,
    data_80139774,
    data_801397C8,
    data_80139830,
    data_80139898,
    data_801398D8,
    data_8013972C,
    data_80139904,
    data_8013993C,
    data_80139974,
    data_801399C0,
    data_80139B34,
    data_80139A08,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139A34,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139AD8,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139B84,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139BF4,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139C78,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139BB8,
    data_80139C38,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139CC8,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139D04,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_801397F8,
    data_8013985C,
    data_8013972C,
    data_80139A58,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_80139AA0,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    data_8013972C,
    ptrs_80139D24,
};

unsigned int data_8013A048[40] = {
    0x6050403, 0xA090807, 0xFFFF0C0B, 0xFF0DFFFF,
    0xFFFFFFFF, 0x11100F0E, 0x13FFFF12, 0x17161514,
    0x1B1A1918, 0xFFFFFF1C, 0xFFFFFFFF, 0xFFFFFFFF,
    0x1DFFFFFF, 0x1EFFFFFF, 0xFFFFFF1F, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x2221FF20, 0xFFFFFF23, 0xFFFFFFFF, 0x2524FFFF,
    0x29282726, 0xFFFFFF2A, 0xFFFFFF2B, 0x2E2DFF2C,
    0xFF31302F, 0x35343332, 0x36FFFFFF, 0xFFFFFFFF,
    0xFF37FFFF, 0x38FFFFFF, 0x3B3AFF39, 0xFF3E3D3C,
};

unsigned int data_8013A0E8[30] = {
    0x4, 0x64, 0x1010004, 0x50,
    0x2020004, 0x64, 0x3030004, 0x50,
    0x4040004, 0x50, 0x5050004, 0x50,
    0x6060004, 0x7F, 0x7070004, 0x64,
    0x8080004, 0x64, 0x9090004, 0x64,
    0xA0A0004, 0x64, 0xB0B0004, 0x64,
    0xC0C0004, 0x64, 0xD0D0004, 0x64,
    0xE0E0004, 0x64,
};

void *ptrs_8013A160[6] = {
    D_8013A4C0,
    D_8013A540,
    D_8013A5D0,
    D_8013A660,
    D_8013A688,
    D_8013A6B0,
};

void *ptrs_8013A178[1] = {
    D_8013A6F0,
};

void *ptrs_8013A17C[1] = {
    D_8013A740,
};

void *ptrs_8013A180[1] = {
    D_8013A278,
};

void *ptrs_8013A184[1] = {
    D_8013A2BC,
};

void *ptrs_8013A188[1] = {
    D_8013A36C,
};

void *ptrs_8013A18C[2] = {
    D_8013A390,
    D_8013A3B4,
};

void *ptrs_8013A194[1] = {
    D_8013A7E4,
};

void *ptrs_8013A198[1] = {
    D_8013A99C,
};

void *ptrs_8013A19C[1] = {
    D_8013A408,
};

void *ptrs_8013A1A0[1] = {
    D_8013A42C,
};

void *ptrs_8013A1A4[1] = {
    D_8013A300,
};

void *ptrs_8013A1A8[1] = {
    D_8013A344,
};

void *ptrs_8013A1AC[3] = {
    D_8013A354,
    D_8013AAAC,
    D_8013AAC4,
};

void *ptrs_8013A1B8[1] = {
    D_8013AAD4,
};

void *ptrs_8013A1BC[1] = {
    D_8013AAFC,
};

void *ptrs_8013A1C0[1] = {
    D_8013AB0C,
};

void *ptrs_8013A1C4[4] = {
    D_8013AB24,
    D_8013AB34,
    D_8013AB34,
    D_8013AB34,
};

void *ptrs_8013A1D4[1] = {
    D_8013AB34,
};

void *ptrs_8013A1D8[1] = {
    D_8013AB3C,
};

void *ptrs_8013A1DC[1] = {
    D_8013AB44,
};

void *ptrs_8013A1E0[1] = {
    D_8013A9E8,
};

void *ptrs_8013A1E4[1] = {
    D_8013A9F8,
};

void *ptrs_8013A1E8[1] = {
    D_8013A804,
};

void *ptrs_8013A1EC[1] = {
    D_8013A80C,
};

void *ptrs_8013A1F0[1] = {
    D_8013A814,
};

void *ptrs_8013A1F4[1] = {
    D_8013A81C,
};

void *ptrs_8013A1F8[1] = {
    D_8013A7A4,
};

void *ptrs_8013A1FC[1] = {
    D_8013AB4C,
};

void *ptrs_8013A200[1] = {
    D_8013A434,
};

void *ptrs_8013A204[1] = {
    D_8013A478,
};

void *ptrs_8013A208[1] = {
    D_8013AA00,
};

void *ptrs_8013A20C[1] = {
    D_8013AA18,
};

void *ptrs_8013A210[1] = {
    D_8013AA3C,
};

void *ptrs_8013A214[2] = {
    D_8013A824,
    D_8013A858,
};

void *ptrs_8013A21C[1] = {
    D_8013A89C,
};

void *ptrs_8013A220[1] = {
    D_8013A8D0,
};
