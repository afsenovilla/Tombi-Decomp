// DATA 8011a4a0 9708 X018
// Data of X018 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8011CACC[];
extern char D_8011CAD4[];
extern char D_8011CB08[];
extern char D_8011CB34[];
extern char D_8011CB3C[];
extern char D_8011CB54[];
extern char D_8011CB68[];
extern char D_8011CB70[];
extern char D_8011CBA0[];
extern char D_8011CBC8[];
extern char D_8011CBD8[];
extern unsigned int data_8011A4A0[];
extern unsigned int data_8011A4A4[];
extern unsigned int data_8011A4A8[];
extern unsigned int data_8011A4AC[];
extern unsigned int data_8011A4B0[];
extern unsigned int data_8011A4B4[];
extern unsigned int data_8011A4B8[];
extern unsigned int data_8011A4C8[];
extern unsigned int data_8011A51C[];
extern unsigned int data_8011A520[];
extern unsigned int data_8011A558[];
extern unsigned int data_8011A55C[];
extern unsigned int data_8011A560[];
extern unsigned int data_8011A564[];
extern unsigned int data_8011A574[];
extern unsigned int data_8011A5AC[];
extern unsigned int data_8011A5F4[];
extern unsigned int data_8011A5FC[];
extern unsigned int data_8011A63C[];
extern unsigned int data_8011A648[];
extern unsigned int data_8011A678[];
extern unsigned int data_8011A6B0[];
extern unsigned int data_8011A6DC[];
extern unsigned int data_8011A718[];
extern unsigned int data_8011A744[];
extern unsigned int data_8011A758[];
extern unsigned int data_8011A784[];
extern unsigned int data_8011A7BC[];
extern unsigned int data_8011A7E0[];
extern unsigned int data_8011A7F0[];
extern unsigned int data_8011A7F4[];
extern unsigned int data_8011A840[];
extern unsigned int data_8011A888[];
extern unsigned int data_8011A890[];
extern unsigned int data_8011A8B4[];
extern unsigned int data_8011A8D0[];
extern unsigned int data_8011A914[];
extern unsigned int data_8011A964[];
extern unsigned int data_8011A9A4[];
extern unsigned int data_8011A9E0[];
extern unsigned int data_8011A9F4[];
extern unsigned int data_8011AA20[];
extern unsigned int data_8011AA3C[];
extern unsigned int data_8011AA50[];
extern unsigned int data_8011AA74[];
extern unsigned int data_8011AAB0[];
extern unsigned int data_8011AB0C[];
extern unsigned int data_8011AB10[];
extern unsigned int data_8011AB44[];
extern unsigned int data_8011AB90[];
extern unsigned int data_8011ABA0[];
extern unsigned int data_8011ABE4[];
extern unsigned int data_8011AC2C[];
extern unsigned int data_8011AC60[];
extern unsigned int data_8011AC98[];
extern unsigned int data_8011AC9C[];
extern unsigned int data_8011ACCC[];
extern unsigned int data_8011ACD0[];
extern unsigned int data_8011AD0C[];
extern unsigned int data_8011AD24[];
extern unsigned int data_8011AD40[];
extern unsigned int data_8011AD54[];
extern unsigned int data_8011AD78[];
extern unsigned int data_8011AD98[];
extern unsigned int data_8011ADBC[];
extern unsigned int data_8011AE10[];
extern unsigned int data_8011AE70[];
extern unsigned int data_8011AE9C[];
extern unsigned int data_8011AEB0[];
extern unsigned int data_8011AEDC[];
extern unsigned int data_8011AEE8[];
extern unsigned int data_8011AF04[];
extern unsigned int data_8011AF20[];
extern unsigned int data_8011AF34[];
extern unsigned int data_8011AF90[];
extern unsigned int data_8011AFC8[];
extern unsigned int data_8011AFD8[];
extern unsigned int data_8011AFE8[];
extern unsigned int data_8011B014[];
extern unsigned int data_8011B03C[];
extern unsigned int data_8011B040[];
extern unsigned int data_8011B07C[];
extern unsigned int data_8011B0C4[];
extern unsigned int data_8011B0D4[];
extern unsigned int data_8011B108[];
extern unsigned int data_8011B150[];
extern unsigned int data_8011B1AC[];
extern AnimWord anim_8011B1E0[];
extern unsigned int data_8011B1E8[];
extern unsigned int data_8011B1F8[];
extern unsigned int data_8011B224[];
extern unsigned int data_8011B270[];
extern unsigned int data_8011B2B0[];
extern unsigned int data_8011B2E8[];
extern unsigned int data_8011B310[];
extern unsigned int data_8011B354[];
extern unsigned int data_8011B360[];
extern unsigned int data_8011B398[];
extern unsigned int data_8011B3E4[];
extern unsigned int data_8011B41C[];
extern unsigned int data_8011B478[];
extern unsigned int data_8011B4A8[];
extern unsigned int data_8011B4E8[];
extern unsigned int data_8011B53C[];
extern unsigned int data_8011B588[];
extern unsigned int data_8011B5CC[];
extern unsigned int data_8011B5E8[];
extern unsigned int data_8011B608[];
extern unsigned int data_8011B648[];
extern unsigned int data_8011B698[];
extern unsigned int data_8011B6E0[];
extern unsigned int data_8011B6EC[];
extern unsigned int data_8011B730[];
extern unsigned int data_8011B780[];
extern unsigned int data_8011B7C8[];
extern unsigned int data_8011B7F0[];
extern unsigned int data_8011B80C[];
extern unsigned int data_8011B848[];
extern unsigned int data_8011B884[];
extern unsigned int data_8011B8A4[];
extern unsigned int data_8011B8BC[];
extern unsigned int data_8011B8C0[];
extern unsigned int data_8011B8E0[];
extern unsigned int data_8011B900[];
extern unsigned int data_8011B948[];
extern unsigned int data_8011B984[];
extern unsigned int data_8011B9D0[];
extern unsigned int data_8011BA18[];
extern unsigned int data_8011BA3C[];
extern unsigned int data_8011BA58[];
extern unsigned int data_8011BA98[];
extern unsigned int data_8011BADC[];
extern unsigned int data_8011BB0C[];
extern unsigned int data_8011BB10[];
extern unsigned int data_8011BB44[];
extern unsigned int data_8011BB88[];
extern unsigned int data_8011BBDC[];
extern unsigned int data_8011BC20[];
extern unsigned int data_8011BC5C[];
extern unsigned int data_8011BCA8[];
extern unsigned int data_8011BCD4[];
extern unsigned int data_8011BD30[];
extern unsigned int data_8011BD80[];
extern unsigned int data_8011BDC8[];
extern unsigned int data_8011BE20[];
extern unsigned int data_8011BE54[];
extern unsigned int data_8011BE88[];
extern unsigned int data_8011BED8[];
extern unsigned int data_8011BF0C[];
extern unsigned int data_8011BF10[];
extern unsigned int data_8011BF48[];
extern unsigned int data_8011BF68[];
extern unsigned int data_8011BF90[];
extern unsigned int data_8011BFE4[];
extern unsigned int data_8011C02C[];
extern unsigned int data_8011C068[];
extern unsigned int data_8011C08C[];
extern unsigned int data_8011C0AC[];
extern unsigned int data_8011C0F4[];
extern unsigned int data_8011C134[];
extern unsigned int data_8011C158[];
extern unsigned int data_8011C178[];
extern unsigned int data_8011C1B8[];
extern unsigned int data_8011C1C4[];
extern unsigned int data_8011C208[];
extern unsigned int data_8011C20C[];
extern unsigned int data_8011C248[];
extern unsigned int data_8011C274[];
extern unsigned int data_8011C298[];
extern unsigned int data_8011C2D4[];
extern unsigned int data_8011C2DC[];
extern unsigned int data_8011C328[];
extern unsigned int data_8011C354[];
extern unsigned int data_8011C370[];
extern unsigned int data_8011C3B8[];
extern unsigned int data_8011C3F8[];
extern unsigned int data_8011C434[];
extern unsigned int data_8011C468[];
extern unsigned int data_8011C4A8[];
extern unsigned int data_8011C4F4[];
extern unsigned int data_8011C540[];
extern unsigned int data_8011C548[];
extern unsigned int data_8011C574[];
extern unsigned int data_8011C598[];
extern unsigned int data_8011C5E0[];
extern unsigned int data_8011C610[];
extern unsigned int data_8011C628[];
extern unsigned int data_8011C630[];
extern unsigned int data_8011C678[];
extern unsigned int data_8011C6AC[];
extern unsigned int data_8011C6D0[];
extern unsigned int data_8011C6E8[];
extern unsigned int data_8011C708[];
extern void *ptrs_8011C724[];
extern void *ptrs_8011C890[];
extern void *ptrs_8011CA48[];
extern AnimWord anim_8011CA50[];
extern AnimWord anim_8011CA58[];
extern void *ptrs_8011CA60[];
extern void *ptrs_8011CA78[];

unsigned int data_8011A4A0[1] = {
    0xFFCCFF84,
};

unsigned int data_8011A4A4[1] = {
    0xFFCC0086,
};

unsigned int data_8011A4A8[1] = {
    0x2BFF84,
};

unsigned int data_8011A4AC[1] = {
    0x2B0086,
};

unsigned int data_8011A4B0[1] = {
    0x10082010,
};

unsigned int data_8011A4B4[1] = {
    0x3000D,
};

unsigned int data_8011A4B8[4] = {
    (unsigned int)ptrs_8011CA60, 0x10082010, 0x3000E, (unsigned int)ptrs_8011CA78,
};

unsigned int data_8011A4C8[21] = {
    0x10000, 0x0, 0x0, 0x0,
    0xD00000, 0xFFC6, 0x0, 0x10000,
    0x0, 0x0, 0x10000, 0x1100000,
    0xFFC6, 0x0, 0xFF, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0,
};

unsigned int data_8011A51C[1] = {
    (unsigned int)data_8011A4C8,
};

unsigned int data_8011A520[14] = {
    0x10000, 0x0, 0x0, 0x0,
    0xDA0000, 0xFFCF, 0x280000, 0xFF,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8011A558[1] = {
    (unsigned int)data_8011A520,
};

unsigned int data_8011A55C[1] = {
    0x2010180C,
};

unsigned int data_8011A560[1] = {
    0x30006,
};

unsigned int data_8011A564[4] = {
    0x8011CA8C, 0x2010180C, 0x30007, 0x8011CA8C,
};

unsigned int data_8011A574[14] = {
    0x82, 0x0, 0x1000100, 0x1FF0080,
    0x840000, 0x100, 0x400000, 0x800100,
    0x1FF, 0x7FFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8011A5AC[18] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0700, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8011A5F4[2] = {
    0xFEF500, 0x1FEF700,
};

unsigned int data_8011A5FC[16] = {
    0x80FEF900, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0300, 0x85FE0500, 0x3FE0700, 0x3FE0900,
    0xF500, 0x100F700, 0x100F900, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x3000500,
};

unsigned int data_8011A63C[3] = {
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011A648[12] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FA00,
    0x8200FC00, 0x8300FF00, 0x84000100, 0x85000300,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8011A678[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8011A6B0[11] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FE00, 0x83000000, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011A6DC[15] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000600, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8011A718[11] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000100, 0x85000400, 0x3000600,
};

unsigned int data_8011A744[5] = {
    0x3000800, 0x4000A00, 0x4000C00, 0x4000E00,
    0xFF,
};

unsigned int data_8011A758[11] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FC00,
    0x8200FE00, 0x83000000, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011A784[14] = {
    0xF200, 0xF400, 0xF600, 0x8000F800,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000700, 0x3000900, 0x4000B00,
    0x4000D00, 0xFF,
};

unsigned int data_8011A7BC[9] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
    0x85000400,
};

unsigned int data_8011A7E0[4] = {
    0x3000600, 0x3000800, 0x4000A00, 0x4000C00,
};

unsigned int data_8011A7F0[1] = {
    0xFF,
};

unsigned int data_8011A7F4[19] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0xFF,
};

unsigned int data_8011A840[18] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000100, 0x2000300, 0x3000500,
    0x3000700, 0xFF,
};

unsigned int data_8011A888[2] = {
    0xF600, 0x100F800,
};

unsigned int data_8011A890[9] = {
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000100,
    0x84000300, 0x85000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_8011A8B4[7] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200,
};

unsigned int data_8011A8D0[17] = {
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0xF300,
    0xF500, 0x100F700, 0x100F900, 0x100FB00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0x4000A00, 0x4000C00,
    0xFF,
};

unsigned int data_8011A914[20] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8011A964[16] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000600, 0x3000800,
    0x4000A00, 0x4000C00, 0x4000E00, 0xFF,
};

unsigned int data_8011A9A4[15] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00,
};

unsigned int data_8011A9E0[5] = {
    0x2000100, 0x2000400, 0x3000600, 0x3000800,
    0xFF,
};

unsigned int data_8011A9F4[11] = {
    0xF500, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011AA20[7] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FD00, 0x8300FF00, 0x84000200,
};

unsigned int data_8011AA3C[5] = {
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0xFF,
};

unsigned int data_8011AA50[9] = {
    0x100F700, 0x8000F900, 0x8100FB00, 0x8200FE00,
    0x83000000, 0x84000300, 0x85000500, 0x3000700,
    0xFF,
};

unsigned int data_8011AA74[15] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011AAB0[23] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0xF200, 0xF400, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8011AB0C[1] = {
    0xF200,
};

unsigned int data_8011AB10[13] = {
    0xF400, 0xF600, 0x8000F900, 0x8100FB00,
    0x8200FD00, 0x8300FF00, 0x84000200, 0x85000400,
    0x3000600, 0x3000900, 0x4000B00, 0x4000D00,
    0xFF,
};

unsigned int data_8011AB44[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011AB90[4] = {
    0x1FEF700, 0x80FEF900, 0x81FEFC00, 0x82FEFE00,
};

unsigned int data_8011ABA0[17] = {
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0xF500, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_8011ABE4[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_8011AC2C[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8011AC60[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8011AC98[1] = {
    0xF200,
};

unsigned int data_8011AC9C[12] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FB00,
    0x8200FD00, 0x8300FF00, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0x4000C00,
};

unsigned int data_8011ACCC[1] = {
    0xFF,
};

unsigned int data_8011ACD0[15] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFF00,
    0x83FE0100, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011AD0C[6] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00,
};

unsigned int data_8011AD24[7] = {
    0x83000000, 0x84000300, 0x85000500, 0x3000700,
    0x3000900, 0x4000B00, 0xFF,
};

unsigned int data_8011AD40[5] = {
    0xF200, 0xF400, 0xF600, 0x8000F800,
    0x8100FA00,
};

unsigned int data_8011AD54[9] = {
    0x8200FC00, 0x8300FF00, 0x84000100, 0x85000300,
    0x3000500, 0x3000800, 0x4000A00, 0x4000C00,
    0xFF,
};

unsigned int data_8011AD78[8] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0300, 0x85FE0500,
};

unsigned int data_8011AD98[9] = {
    0x3FE0700, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8011ADBC[21] = {
    0xFEF400, 0xFEF600, 0x1FEF800, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0x3000700,
    0xFF,
};

unsigned int data_8011AE10[24] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0700, 0xF200, 0xF400, 0xF600,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x3000500, 0x3000700,
    0x3000900, 0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8011AE70[11] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0500, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00,
};

unsigned int data_8011AE9C[5] = {
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_8011AEB0[11] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0xF600,
};

unsigned int data_8011AEDC[3] = {
    0x100F800, 0x100FA00, 0x200FC00,
};

unsigned int data_8011AEE8[7] = {
    0x200FF00, 0x2000100, 0x2000300, 0x3000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011AF04[7] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FA00,
    0x8200FD00, 0x8300FF00, 0x84000200,
};

unsigned int data_8011AF20[5] = {
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0xFF,
};

unsigned int data_8011AF34[23] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0800, 0x4FE0A00, 0xF500,
    0x100F700, 0x100F900, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8011AF90[14] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100F800,
};

unsigned int data_8011AFC8[4] = {
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
};

unsigned int data_8011AFD8[4] = {
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8011AFE8[11] = {
    0xF600, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FF00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011B014[10] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FB00,
    0x8200FE00, 0x83000000, 0x84000200, 0x85000400,
    0x3000700, 0x4000A00,
};

unsigned int data_8011B03C[1] = {
    0xFF,
};

unsigned int data_8011B040[15] = {
    0xF100, 0xF400, 0xF600, 0x100F800,
    0x8000FA00, 0x8100FD00, 0x8200FF00, 0x83000100,
    0x84000300, 0x85000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8011B07C[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_8011B0C4[4] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
};

unsigned int data_8011B0D4[13] = {
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_8011B108[18] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8011B150[23] = {
    0xFEF100, 0xFEF300, 0xFEF500, 0x1FEF700,
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x4FE0A00, 0x4FE0C00, 0x4FE0E00, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011B1AC[13] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x200FD00, 0x200FF00,
    0x2000100,
};

AnimWord anim_8011B1E0[2] = {
    ANIM_STOP(255, 0, -1792, 254),
};

unsigned int data_8011B1E8[4] = {
    0x81FEFB00, 0x82FEFD00, 0x83FEFF00, 0x84FE0100,
};

unsigned int data_8011B1F8[11] = {
    0x85FE0300, 0x3FE0500, 0x100F800, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8011B224[19] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFE00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0800, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0xFF,
};

unsigned int data_8011B270[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011B2B0[14] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x200FD00, 0x200FF00,
    0x2000100, 0xFF,
};

unsigned int data_8011B2E8[10] = {
    0x100F700, 0x8000F900, 0x8100FB00, 0x8200FD00,
    0x83000000, 0x84000200, 0x85000400, 0x3000600,
    0x3000800, 0xFF,
};

unsigned int data_8011B310[17] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0800, 0x4FE0A00, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300,
};

unsigned int data_8011B354[3] = {
    0x3000500, 0x3000700, 0xFF,
};

unsigned int data_8011B360[14] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0600, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_8011B398[19] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFA00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011B3E4[14] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000100,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8011B41C[23] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0xF200, 0xF400, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8011B478[12] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FD00, 0x8300FF00, 0x84000100, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8011B4A8[16] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8011B4E8[21] = {
    0xFEF400, 0xFEF600, 0x1FEF800, 0x80FEFB00,
    0x81FEFD00, 0x82FEFF00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_8011B53C[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFB00, 0x82FEFE00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011B588[17] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8011B5CC[7] = {
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0xFF,
};

unsigned int data_8011B5E8[8] = {
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x83000000,
    0x84000200, 0x85000400, 0x3000600, 0xFF,
};

unsigned int data_8011B608[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011B648[20] = {
    0xFEF200, 0xFEF400, 0xFEF600, 0x80FEF900,
    0x81FEFB00, 0x82FEFD00, 0x83FE0000, 0x84FE0200,
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011B698[18] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500,
};

unsigned int data_8011B6E0[3] = {
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011B6EC[17] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8011B730[20] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F900, 0x100FB00, 0x200FD00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8011B780[18] = {
    0xFEF600, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8011B7C8[10] = {
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0300, 0x85FE0500, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00,
};

unsigned int data_8011B7F0[7] = {
    0x200FE00, 0x2000000, 0x2000300, 0x3000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011B80C[15] = {
    0xFEF600, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8011B848[15] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x200FD00,
    0x200FF00, 0x2000100, 0xFF,
};

unsigned int data_8011B884[8] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
};

unsigned int data_8011B8A4[6] = {
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8011B8BC[1] = {
    0xFEF600,
};

unsigned int data_8011B8C0[8] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFD00, 0x82FEFF00,
    0x83FE0100, 0x84FE0300, 0x85FE0500, 0x3FE0700,
};

unsigned int data_8011B8E0[8] = {
    0x3FE0900, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011B900[18] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFD00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0x3000500, 0xFF,
};

unsigned int data_8011B948[15] = {
    0xFEF600, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8011B984[19] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFA00,
    0x82FEFC00, 0x83FEFE00, 0x84FE0000, 0x85FE0200,
    0x3FE0500, 0x3FE0700, 0x4FE0A00, 0x100F900,
    0x100FB00, 0x200FD00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8011B9D0[18] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100F700, 0x100F900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0xFF,
};

unsigned int data_8011BA18[9] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800,
};

unsigned int data_8011BA3C[7] = {
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011BA58[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011BA98[17] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000400, 0x3000600, 0x3000800,
    0xFF,
};

unsigned int data_8011BADC[12] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
};

unsigned int data_8011BB0C[1] = {
    0xFF,
};

unsigned int data_8011BB10[13] = {
    0xF400, 0xF600, 0x100F800, 0x8000FA00,
    0x8100FD00, 0x8200FF00, 0x83000100, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8011BB44[17] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_8011BB88[21] = {
    0x80FCFA00, 0x81FCFC00, 0x82FCFE00, 0x83FC0000,
    0x84FC0300, 0x85FC0500, 0x1FEF700, 0x1FEF900,
    0x1FEFB00, 0x2FEFD00, 0x2FEFF00, 0x2FE0100,
    0x2FE0300, 0x3FE0500, 0x3FE0700, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_8011BBDC[17] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0100, 0x84FE0400, 0x85FE0600,
    0x3FE0800, 0x4FE0A00, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8011BC20[15] = {
    0xF100, 0xF300, 0xF600, 0x100F800,
    0x8000FA00, 0x8100FC00, 0x8200FF00, 0x83000100,
    0x84000300, 0x85000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8011BC5C[19] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0xF600, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000100, 0x2000300, 0x3000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8011BCA8[11] = {
    0xF500, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000400, 0x85000600,
    0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8011BCD4[23] = {
    0xFEF100, 0xFEF300, 0xFEF500, 0x1FEF800,
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x4FE0A00, 0x4FE0C00, 0x4FE0E00, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011BD30[20] = {
    0xFEF200, 0xFEF400, 0xFEF600, 0x80FEF900,
    0x81FEFB00, 0x82FEFD00, 0x83FEFF00, 0x84FE0100,
    0x85FE0300, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011BD80[18] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FF00, 0x2000100, 0x2000300, 0x3000500,
    0x3000700, 0xFF,
};

unsigned int data_8011BDC8[22] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0xF600,
    0x100F800, 0x100FA00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0x3000700,
    0x3000900, 0xFF,
};

unsigned int data_8011BE20[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8011BE54[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000FA00,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8011BE88[20] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_8011BED8[13] = {
    0xF400, 0xF600, 0x100F800, 0x8000FA00,
    0x8100FC00, 0x8200FF00, 0x83000100, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8011BF0C[1] = {
    0xFEF600,
};

unsigned int data_8011BF10[14] = {
    0x80FEF800, 0x81FEFA00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0xFF,
};

unsigned int data_8011BF48[8] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
};

unsigned int data_8011BF68[10] = {
    0x3FE0700, 0x3FE0900, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0x3000500, 0xFF,
};

unsigned int data_8011BF90[21] = {
    0xFEF400, 0xFEF600, 0x1FEF800, 0x80FEFB00,
    0x81FEFD00, 0x82FEFF00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_8011BFE4[18] = {
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0xF500, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0x3000700,
    0x3000900, 0xFF,
};

unsigned int data_8011C02C[15] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FB00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8011C068[9] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700,
};

unsigned int data_8011C08C[8] = {
    0x3FE0900, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011C0AC[18] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0500, 0x3FE0700,
    0x100F700, 0x100F900, 0x100FB00, 0x200FD00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0xFF,
};

unsigned int data_8011C0F4[16] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8011C134[9] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600,
};

unsigned int data_8011C158[8] = {
    0x3FE0800, 0x4FE0A00, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8011C178[16] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
};

unsigned int data_8011C1B8[3] = {
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011C1C4[17] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFF00,
    0x83FE0100, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_8011C208[1] = {
    0x1FEF700,
};

unsigned int data_8011C20C[15] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8011C248[11] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900,
};

unsigned int data_8011C274[9] = {
    0x4FE0B00, 0x100F900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000300, 0x3000500,
    0xFF,
};

unsigned int data_8011C298[15] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0600,
    0x3FE0800, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8011C2D4[2] = {
    0x1FEF700, 0x80FEF900,
};

unsigned int data_8011C2DC[19] = {
    0x81FEFB00, 0x82FEFD00, 0x83FEFF00, 0x84FE0100,
    0x85FE0300, 0x3FE0500, 0x3FE0700, 0xF500,
    0x100F700, 0x100F900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000400, 0x3000600,
    0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8011C328[11] = {
    0x80FEF900, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0500, 0xF500, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00,
};

unsigned int data_8011C354[7] = {
    0x200FF00, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8011C370[18] = {
    0x80FEF900, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0500, 0xF500, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000200, 0x2000400, 0x3000600, 0x3000800,
    0x4000A00, 0xFF,
};

unsigned int data_8011C3B8[16] = {
    0x80FEF900, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0500, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0x3000800, 0xFF,
};

unsigned int data_8011C3F8[15] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFD00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0xFF,
};

unsigned int data_8011C434[13] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100FB00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8011C468[16] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0xFF,
};

unsigned int data_8011C4A8[19] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100F900,
    0x100FB00, 0x200FD00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8011C4F4[19] = {
    0x80FEF800, 0x81FEFA00, 0x82FEFC00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0500, 0xF300,
    0xF500, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0x4000A00,
};

unsigned int data_8011C540[2] = {
    0x4000C00, 0xFF,
};

unsigned int data_8011C548[11] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900,
};

unsigned int data_8011C574[9] = {
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_8011C598[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_8011C5E0[12] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FB00,
    0x8200FD00, 0x83000000, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8011C610[6] = {
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000200, 0x85000400,
};

unsigned int data_8011C628[2] = {
    0x3000600, 0xFF,
};

unsigned int data_8011C630[18] = {
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0100,
    0x84FE0300, 0x85FE0500, 0xF400, 0xF600,
    0x100F800, 0x100FA00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000400, 0x3000600, 0x3000800,
    0x4000A00, 0xFF,
};

unsigned int data_8011C678[13] = {
    0xF400, 0xF600, 0x100F800, 0x8000FA00,
    0x8100FC00, 0x8200FF00, 0x83000100, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8011C6AC[9] = {
    0xF100, 0xF300, 0xF500, 0x100F800,
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000100,
    0x84000300,
};

unsigned int data_8011C6D0[6] = {
    0x85000500, 0x3000700, 0x4000A00, 0x4000C00,
    0x4000E00, 0xFF,
};

unsigned int data_8011C6E8[8] = {
    0xF100, 0xF300, 0xF500, 0x100F800,
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000000,
};

unsigned int data_8011C708[7] = {
    0x84000200, 0x85000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

void *ptrs_8011C724[91] = {
    data_8011A5AC,
    data_8011A5F4,
    data_8011A648,
    data_8011A6B0,
    data_8011A718,
    data_8011A758,
    data_8011A5AC,
    data_8011A784,
    data_8011A7BC,
    data_8011A7F4,
    data_8011A840,
    data_8011B730,
    data_8011A888,
    data_8011A8B4,
    data_8011A9F4,
    data_8011AA20,
    data_8011AA50,
    data_8011AA74,
    data_8011A5AC,
    data_8011AC2C,
    data_8011AC98,
    data_8011ACD0,
    data_8011AD0C,
    data_8011AD78,
    data_8011AF04,
    data_8011AF34,
    data_8011AB90,
    data_8011B108,
    data_8011B150,
    data_8011B1AC,
    &anim_8011B1E0[1],
    data_8011B224,
    data_8011A5AC,
    data_8011B270,
    data_8011B2E8,
    data_8011B310,
    data_8011B360,
    data_8011B478,
    data_8011B4A8,
    data_8011B53C,
    data_8011B588,
    data_8011B5CC,
    data_8011B5E8,
    data_8011B608,
    data_8011B648,
    data_8011A5AC,
    data_8011B698,
    data_8011B6EC,
    data_8011A5AC,
    data_8011B780,
    data_8011B7C8,
    data_8011B80C,
    data_8011A5AC,
    data_8011B848,
    data_8011B900,
    data_8011B948,
    data_8011B984,
    data_8011B9D0,
    data_8011BA18,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011BB88,
    data_8011BADC,
    data_8011BB44,
    data_8011BBDC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011BC20,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011BCD4,
    data_8011BB10,
    data_8011BDC8,
    data_8011AB44,
    data_8011C1C4,
    data_8011C5E0,
    data_8011A5AC,
    data_8011A5AC,
    data_8011BA98,
    data_8011ADBC,
    data_8011AE10,
    data_8011AEB0,
    data_8011A5AC,
    data_8011C068,
};

void *ptrs_8011C890[110] = {
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011BC5C,
    data_8011BE54,
    data_8011BE20,
    data_8011BCA8,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011B884,
    data_8011BF90,
    data_8011AE70,
    data_8011C248,
    data_8011B040,
    data_8011BF48,
    data_8011A5AC,
    data_8011BD30,
    data_8011C02C,
    data_8011C208,
    data_8011C2D4,
    data_8011A5AC,
    data_8011BE88,
    data_8011BED8,
    data_8011C3F8,
    data_8011BF0C,
    data_8011C0F4,
    data_8011C468,
    data_8011C598,
    data_8011A5AC,
    data_8011A5AC,
    data_8011C298,
    data_8011C328,
    data_8011C370,
    data_8011C3B8,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011C4A8,
    data_8011C434,
    data_8011C4F4,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011BD80,
    data_8011C610,
    data_8011A5AC,
    data_8011BFE4,
    data_8011A5AC,
    data_8011A5AC,
    data_8011C0AC,
    data_8011C678,
    data_8011A5AC,
    data_8011C630,
    data_8011C6AC,
    data_8011C548,
    data_8011A5AC,
    data_8011A5AC,
    data_8011C6E8,
    data_8011A5AC,
    data_8011A678,
    data_8011A6DC,
    data_8011AB0C,
    data_8011ABE4,
    data_8011B398,
    data_8011B4E8,
    data_8011B2B0,
    data_8011B3E4,
    data_8011B8BC,
    data_8011B07C,
    data_8011B0C4,
    data_8011A914,
    data_8011A964,
    data_8011A9A4,
    data_8011AAB0,
    data_8011AD40,
    data_8011AC60,
    data_8011BA58,
    data_8011A5AC,
    data_8011B41C,
    data_8011A5AC,
    data_8011AF90,
    data_8011AFE8,
    data_8011B014,
    data_8011C134,
    data_8011C178,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    data_8011A5AC,
    ptrs_8011C724,
};

void *ptrs_8011CA48[2] = {
    anim_8011CA50,
    anim_8011CA58,
};

AnimWord anim_8011CA50[2] = {
    ANIM_STOP(64, 0, 0, 60),
};

AnimWord anim_8011CA58[2] = {
    ANIM_STOP(67, 0, 0, 60),
};

void *ptrs_8011CA60[6] = {
    D_8011CACC,
    D_8011CAD4,
    D_8011CB08,
    D_8011CB34,
    D_8011CB3C,
    D_8011CB54,
};

void *ptrs_8011CA78[5] = {
    D_8011CB68,
    D_8011CB70,
    D_8011CBA0,
    D_8011CBC8,
    D_8011CBD8,
};
