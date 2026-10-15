// DATA 8013c9c0 4444 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013DB30[];
extern char D_8013DB74[];
extern char D_8013DB98[];
extern char D_8013DBA8[];
extern char D_8013DBB0[];
extern char D_8013DBD0[];
extern char D_8013DBD8[];
extern char D_8013DBE0[];
extern char D_8013DBE8[];
extern char D_8013DC34[];
extern char D_8013DC80[];
extern char D_8013DC88[];
extern char D_8013DCA0[];
extern unsigned int data_8013C9C0[];
extern unsigned int data_8013C9DC[];
extern unsigned int data_8013C9EC[];
extern unsigned int data_8013C9F0[];
extern unsigned int data_8013CA00[];
extern unsigned int data_8013CA10[];
extern unsigned int data_8013CA20[];
extern unsigned int data_8013CA28[];
extern unsigned int data_8013CA70[];
extern unsigned int data_8013CA9C[];
extern unsigned int data_8013CAFC[];
extern unsigned int data_8013CB4C[];
extern unsigned int data_8013CB8C[];
extern unsigned int data_8013CBDC[];
extern unsigned int data_8013CC08[];
extern unsigned int data_8013CC38[];
extern unsigned int data_8013CC5C[];
extern unsigned int data_8013CC98[];
extern unsigned int data_8013CCF4[];
extern unsigned int data_8013CD2C[];
extern unsigned int data_8013CD78[];
extern unsigned int data_8013CDCC[];
extern unsigned int data_8013CE14[];
extern unsigned int data_8013CE48[];
extern unsigned int data_8013CE80[];
extern unsigned int data_8013CEBC[];
extern unsigned int data_8013CEF0[];
extern unsigned int data_8013CF28[];
extern unsigned int data_8013CF6C[];
extern unsigned int data_8013CF9C[];
extern unsigned int data_8013CFF8[];
extern unsigned int data_8013D050[];
extern unsigned int data_8013D07C[];
extern unsigned int data_8013D0A8[];
extern unsigned int data_8013D0F0[];
extern unsigned int data_8013D134[];
extern unsigned int data_8013D17C[];
extern unsigned int data_8013D1AC[];
extern unsigned int data_8013D1EC[];
extern unsigned int data_8013D238[];
extern unsigned int data_8013D26C[];
extern unsigned int data_8013D2C4[];
extern unsigned int data_8013D30C[];
extern unsigned int data_8013D348[];
extern unsigned int data_8013D37C[];
extern unsigned int data_8013D3BC[];
extern unsigned int data_8013D408[];
extern unsigned int data_8013D45C[];
extern unsigned int data_8013D4AC[];
extern unsigned int data_8013D4CC[];
extern unsigned int data_8013D514[];
extern unsigned int data_8013D548[];
extern void *ptrs_8013D584[];
extern unsigned int data_8013D8A8[];
extern unsigned int data_8013D948[];
extern unsigned int data_8013D998[];
extern unsigned int data_8013DA18[];
extern unsigned int data_8013DA78[];
extern void *ptrs_8013DAE8[];
extern void *ptrs_8013DAEC[];
extern void *ptrs_8013DAF0[];
extern void *ptrs_8013DAF4[];
extern void *ptrs_8013DAF8[];
extern void *ptrs_8013DAFC[];
extern void *ptrs_8013DB00[];
extern void *ptrs_8013DB04[];
extern void *ptrs_8013DB08[];
extern void *ptrs_8013DB0C[];
extern void *ptrs_8013DB10[];
extern void *ptrs_8013DB14[];
extern void *ptrs_8013DB18[];

unsigned int data_8013C9C0[7] = {
    0xFE000001, 0xFE000001, 0xFD000001, 0xFC000002,
    0xFD000000, 0xFE000000, 0xFE000000,
};

unsigned int data_8013C9DC[4] = {
    0x10000040, 0x18000080, 0x200000C0, 0x30000100,
};

unsigned int data_8013C9EC[1] = {
    0xFB2D04BA,
};

unsigned int data_8013C9F0[4] = {
    0x690105B, 0x105BFB50, 0xFC40079E, 0x84E2105B,
};

unsigned int data_8013CA00[4] = {
    0x168012C, 0x12C01A4, 0x12C01A4, 0xF00168,
};

unsigned int data_8013CA10[4] = {
    0xFE02020D, 0xFDBC02C6, 0xFD8A03AC, 0xFDC6046A,
};

unsigned int data_8013CA20[2] = {
    0x25801E0, 0x1B80168,
};

unsigned int data_8013CA28[18] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0700, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8013CA70[11] = {
    0xF600, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8013CA9C[24] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0xF300, 0xF500,
    0x100F700, 0x100F900, 0x100FB00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0x4000A00, 0x4000C00, 0xFF,
};

unsigned int data_8013CAFC[20] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8013CB4C[16] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000600, 0x3000800,
    0x4000A00, 0x4000C00, 0x4000E00, 0xFF,
};

unsigned int data_8013CB8C[20] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000400, 0x3000600, 0x3000800, 0xFF,
};

unsigned int data_8013CBDC[11] = {
    0xF500, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8013CC08[12] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FD00, 0x8300FF00, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8013CC38[9] = {
    0x100F700, 0x8000F900, 0x8100FB00, 0x8200FE00,
    0x83000000, 0x84000300, 0x85000500, 0x3000700,
    0xFF,
};

unsigned int data_8013CC5C[15] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8013CC98[23] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0xF200, 0xF400, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8013CCF4[14] = {
    0xF200, 0xF400, 0xF600, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000600, 0x3000900, 0x4000B00,
    0x4000D00, 0xFF,
};

unsigned int data_8013CD2C[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8013CD78[21] = {
    0x1FEF700, 0x80FEF900, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0xF500, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_8013CDCC[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_8013CE14[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8013CE48[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8013CE80[15] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFF00,
    0x83FE0100, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8013CEBC[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8013CEF0[14] = {
    0xF200, 0xF400, 0xF600, 0x8000F800,
    0x8100FA00, 0x8200FC00, 0x8300FF00, 0x84000100,
    0x85000300, 0x3000500, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8013CF28[17] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8013CF6C[12] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FA00,
    0x8200FD00, 0x8300FF00, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8013CF9C[23] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0800, 0x4FE0A00, 0xF500,
    0x100F700, 0x100F900, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8013CFF8[22] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8013D050[11] = {
    0xF600, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FF00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8013D07C[11] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FB00,
    0x8200FE00, 0x83000000, 0x84000200, 0x85000400,
    0x3000700, 0x4000A00, 0xFF,
};

unsigned int data_8013D0A8[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_8013D0F0[17] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_8013D134[18] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8013D17C[12] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FD00, 0x8300FF00, 0x84000100, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8013D1AC[16] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8013D1EC[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFB00, 0x82FEFE00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8013D238[13] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_8013D26C[22] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0xF600,
    0x100F800, 0x100FA00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0x3000700,
    0x3000900, 0xFF,
};

unsigned int data_8013D2C4[18] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0500, 0x3FE0700,
    0x100F700, 0x100F900, 0x100FB00, 0x200FD00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0xFF,
};

unsigned int data_8013D30C[15] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFD00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0xFF,
};

unsigned int data_8013D348[13] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100FB00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8013D37C[16] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0xFF,
};

unsigned int data_8013D3BC[19] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100F900,
    0x100FB00, 0x200FD00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8013D408[21] = {
    0x80FEF800, 0x81FEFA00, 0x82FEFC00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0500, 0xF300,
    0xF500, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0x4000A00, 0x4000C00,
    0xFF,
};

unsigned int data_8013D45C[20] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8013D4AC[8] = {
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000200, 0x85000400, 0x3000600, 0xFF,
};

unsigned int data_8013D4CC[18] = {
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0100,
    0x84FE0300, 0x85FE0500, 0xF400, 0xF600,
    0x100F800, 0x100FA00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000400, 0x3000600, 0x3000800,
    0x4000A00, 0xFF,
};

unsigned int data_8013D514[13] = {
    0xF400, 0xF600, 0x100F800, 0x8000FA00,
    0x8100FC00, 0x8200FF00, 0x83000100, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8013D548[15] = {
    0xF100, 0xF300, 0xF500, 0x100F800,
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

void *ptrs_8013D584[201] = {
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA70,
    data_8013CA9C,
    data_8013CBDC,
    data_8013CC08,
    data_8013CC38,
    data_8013CC5C,
    data_8013CA28,
    data_8013CE14,
    data_8013CA28,
    data_8013CE80,
    data_8013CEBC,
    data_8013CF28,
    data_8013CF6C,
    data_8013CF9C,
    data_8013CD78,
    data_8013D134,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D17C,
    data_8013D1AC,
    data_8013D1EC,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D238,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D26C,
    data_8013CD2C,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D30C,
    data_8013CA28,
    data_8013CA28,
    data_8013D37C,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D3BC,
    data_8013D348,
    data_8013D408,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D4AC,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D2C4,
    data_8013D514,
    data_8013CA28,
    data_8013D4CC,
    data_8013CA28,
    data_8013D45C,
    data_8013CA28,
    data_8013CA28,
    data_8013D548,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CCF4,
    data_8013CDCC,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013D0A8,
    data_8013D0F0,
    data_8013CAFC,
    data_8013CB4C,
    data_8013CB8C,
    data_8013CC98,
    data_8013CEF0,
    data_8013CE48,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CFF8,
    data_8013D050,
    data_8013D07C,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    data_8013CA28,
    ptrs_8013D584,
};

unsigned int data_8013D8A8[40] = {
    0x6050403, 0xA090807, 0xE0D0C0B, 0x1211100F,
    0x16151413, 0x1A191817, 0x1E1D1C1B, 0x2221201F,
    0x26252423, 0x2A292827, 0x2E2D2C2B, 0x3231302F,
    0x36353433, 0x3A393837, 0x3E3D3C3B, 0x4241403F,
    0x46454443, 0x4A494847, 0x4E4D4C4B, 0x5251504F,
    0x56555453, 0x5A595857, 0x5E5D5C5B, 0x6261605F,
    0x66656463, 0x6A696867, 0x6E6D6C6B, 0x7271706F,
    0x76757473, 0x7A797877, 0x7E7D7C7B, 0x8281807F,
    0x86858483, 0x8A898887, 0x8E8D8C8B, 0x9291908F,
    0x96959493, 0x9A999897, 0x9E9D9C9B, 0xA2A1A09F,
};

unsigned int data_8013D948[20] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x78, 0x3030004, 0x64,
    0x4040004, 0x64, 0x5050004, 0x78,
    0x6060004, 0x64, 0x7070004, 0x64,
    0x8080004, 0x64, 0x9090004, 0x64,
};

unsigned int data_8013D998[32] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x64, 0x3030004, 0x64,
    0x4040004, 0x64, 0x5050004, 0x64,
    0x6060004, 0x64, 0x7070004, 0x64,
    0x8080004, 0x64, 0x9090004, 0x64,
    0xA0A0004, 0x64, 0xB0B0004, 0x64,
    0xC0C0004, 0x64, 0xD0D0004, 0x64,
    0xE0E0004, 0x64, 0xF0F0004, 0x64,
};

unsigned int data_8013DA18[24] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x78, 0x3030004, 0x64,
    0x4040004, 0x64, 0x5050004, 0x78,
    0x6060004, 0x64, 0x7070004, 0x64,
    0x8080004, 0x64, 0x9090004, 0x64,
    0xA0A0004, 0x64, 0xB0B0004, 0x64,
};

unsigned int data_8013DA78[28] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x78, 0x3030004, 0x64,
    0x4040004, 0x64, 0x5050004, 0x78,
    0x6060004, 0x64, 0x7070004, 0x64,
    0x8080004, 0x64, 0x9090004, 0x64,
    0xA0A0004, 0x64, 0xB0B0004, 0x64,
    0xC0C0004, 0x64, 0xD0D0004, 0x64,
};

void *ptrs_8013DAE8[1] = {
    D_8013DB30,
};

void *ptrs_8013DAEC[1] = {
    D_8013DB74,
};

void *ptrs_8013DAF0[1] = {
    D_8013DBB0,
};

void *ptrs_8013DAF4[1] = {
    D_8013DC34,
};

void *ptrs_8013DAF8[1] = {
    D_8013DC80,
};

void *ptrs_8013DAFC[1] = {
    D_8013DBD0,
};

void *ptrs_8013DB00[1] = {
    D_8013DBD8,
};

void *ptrs_8013DB04[1] = {
    D_8013DBE0,
};

void *ptrs_8013DB08[1] = {
    D_8013DBE8,
};

void *ptrs_8013DB0C[1] = {
    D_8013DBA8,
};

void *ptrs_8013DB10[1] = {
    D_8013DB98,
};

void *ptrs_8013DB14[1] = {
    D_8013DC88,
};

void *ptrs_8013DB18[1] = {
    D_8013DCA0,
};
