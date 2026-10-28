// DATA 8012f2e8 10620 X010
// Data of X010 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80131DD0[];
extern char D_80131FCC[];
extern char D_80131FEC[];
extern char D_8013200C[];
extern char D_8013202C[];
extern char D_80132A20[];
extern char D_80132A34[];
extern void bitingPlantSway();
extern void bitingPlantLunge();
extern void area10Type07SwingChain();
extern void area10Type07StepDChain();
extern unsigned int data_8012F2E8[];
extern unsigned int data_8012F2F0[];
extern unsigned int data_8012F2F8[];
extern unsigned int data_8012F35C[];
extern unsigned int data_8012F378[];
extern unsigned int data_8012F398[];
extern void *ptrs_8012F3A8[];
extern unsigned int data_8012F3B0[];
extern unsigned int data_8012F3B4[];
extern unsigned int data_8012F3B8[];
extern unsigned int data_8012F3BC[];
extern unsigned int data_8012F3C0[];
extern unsigned int data_8012F3C4[];
extern unsigned int data_8012F3C8[];
extern unsigned int data_8012F3D4[];
extern unsigned int data_8012F3D8[];
extern unsigned int data_8012F3E0[];
extern unsigned int data_8012F3E4[];
extern unsigned int data_8012F3E8[];
extern unsigned int data_8012F3EC[];
extern unsigned int data_8012F3F0[];
extern unsigned int data_8012F3F4[];
extern unsigned int data_8012F3F8[];
extern unsigned int data_8012F400[];
extern unsigned int data_8012F404[];
extern unsigned int data_8012F408[];
extern unsigned int data_8012F410[];
extern unsigned int data_8012F414[];
extern unsigned int data_8012F418[];
extern unsigned int data_8012F420[];
extern unsigned int data_8012F42C[];
extern unsigned int data_8012F448[];
extern unsigned int data_8012F460[];
extern unsigned int data_8012F464[];
extern unsigned int data_8012F468[];
extern unsigned int data_8012F46C[];
extern unsigned int data_8012F474[];
extern unsigned int data_8012F47C[];
extern unsigned int data_8012F484[];
extern unsigned int data_8012F494[];
extern unsigned int data_8012F49C[];
extern unsigned int data_8012F4AC[];
extern unsigned int data_8012F4EC[];
extern unsigned int data_8012F4F4[];
extern unsigned int data_8012F524[];
extern unsigned int data_8012F52C[];
extern unsigned int data_8012F530[];
extern unsigned int data_8012F534[];
extern unsigned int data_8012F538[];
extern unsigned int data_8012F53C[];
extern unsigned int data_8012F574[];
extern unsigned int data_8012F578[];
extern unsigned int data_8012F58C[];
extern unsigned int data_8012F594[];
extern unsigned int data_8012F598[];
extern unsigned int data_8012F59C[];
extern unsigned int data_8012F5A0[];
extern unsigned int data_8012F5A8[];
extern void *ptrs_8012F5CC[];
extern unsigned int data_8012F5E0[];
extern unsigned int data_8012F628[];
extern unsigned int data_8012F67C[];
extern unsigned int data_8012F69C[];
extern unsigned int data_8012F6AC[];
extern unsigned int data_8012F6E4[];
extern unsigned int data_8012F710[];
extern unsigned int data_8012F74C[];
extern unsigned int data_8012F78C[];
extern unsigned int data_8012F7B8[];
extern unsigned int data_8012F7F0[];
extern unsigned int data_8012F828[];
extern unsigned int data_8012F874[];
extern unsigned int data_8012F8BC[];
extern unsigned int data_8012F8E8[];
extern unsigned int data_8012F910[];
extern unsigned int data_8012F948[];
extern unsigned int data_8012F968[];
extern unsigned int data_8012F988[];
extern unsigned int data_8012F998[];
extern unsigned int data_8012F9A0[];
extern unsigned int data_8012F9D8[];
extern unsigned int data_8012FA28[];
extern unsigned int data_8012FA54[];
extern unsigned int data_8012FA84[];
extern unsigned int data_8012FAA8[];
extern unsigned int data_8012FACC[];
extern unsigned int data_8012FAE4[];
extern unsigned int data_8012FB40[];
extern unsigned int data_8012FB78[];
extern unsigned int data_8012FBC4[];
extern unsigned int data_8012FC18[];
extern unsigned int data_8012FC60[];
extern unsigned int data_8012FC88[];
extern unsigned int data_8012FC94[];
extern unsigned int data_8012FCCC[];
extern unsigned int data_8012FD04[];
extern unsigned int data_8012FD40[];
extern unsigned int data_8012FD74[];
extern unsigned int data_8012FDAC[];
extern unsigned int data_8012FDF0[];
extern unsigned int data_8012FE44[];
extern unsigned int data_8012FE74[];
extern unsigned int data_8012FEA4[];
extern unsigned int data_8012FEE4[];
extern unsigned int data_8012FF38[];
extern unsigned int data_8012FF68[];
extern unsigned int data_8012FFC4[];
extern unsigned int data_8013001C[];
extern unsigned int data_80130048[];
extern unsigned int data_80130074[];
extern unsigned int data_801300B0[];
extern unsigned int data_801300F8[];
extern unsigned int data_8013013C[];
extern unsigned int data_80130150[];
extern unsigned int data_80130184[];
extern unsigned int data_801301E0[];
extern unsigned int data_80130218[];
extern unsigned int data_80130258[];
extern unsigned int data_801302A4[];
extern unsigned int data_801302E4[];
extern unsigned int data_8013031C[];
extern unsigned int data_80130344[];
extern unsigned int data_80130394[];
extern unsigned int data_801303CC[];
extern unsigned int data_80130418[];
extern unsigned int data_80130450[];
extern unsigned int data_801304AC[];
extern unsigned int data_801304B4[];
extern unsigned int data_801304DC[];
extern unsigned int data_8013051C[];
extern unsigned int data_80130570[];
extern unsigned int data_801305BC[];
extern unsigned int data_801305DC[];
extern unsigned int data_80130600[];
extern unsigned int data_8013061C[];
extern unsigned int data_8013063C[];
extern unsigned int data_8013067C[];
extern unsigned int data_801306CC[];
extern unsigned int data_80130720[];
extern unsigned int data_80130764[];
extern unsigned int data_801307B0[];
extern unsigned int data_801307B4[];
extern unsigned int data_801307FC[];
extern unsigned int data_80130840[];
extern unsigned int data_8013087C[];
extern unsigned int data_801308B8[];
extern unsigned int data_801308C0[];
extern unsigned int data_801308F0[];
extern unsigned int data_80130934[];
extern unsigned int data_8013097C[];
extern unsigned int data_80130990[];
extern unsigned int data_801309B8[];
extern unsigned int data_80130A04[];
extern unsigned int data_80130A4C[];
extern unsigned int data_80130A8C[];
extern unsigned int data_80130ACC[];
extern unsigned int data_80130B10[];
extern unsigned int data_80130B2C[];
extern unsigned int data_80130B44[];
extern unsigned int data_80130B78[];
extern unsigned int data_80130BBC[];
extern unsigned int data_80130C10[];
extern unsigned int data_80130C54[];
extern unsigned int data_80130C90[];
extern unsigned int data_80130CDC[];
extern unsigned int data_80130D08[];
extern unsigned int data_80130D64[];
extern unsigned int data_80130DB4[];
extern unsigned int data_80130DC0[];
extern unsigned int data_80130DFC[];
extern unsigned int data_80130E3C[];
extern unsigned int data_80130E54[];
extern unsigned int data_80130E88[];
extern unsigned int data_80130EBC[];
extern unsigned int data_80130F0C[];
extern unsigned int data_80130F40[];
extern unsigned int data_80130F7C[];
extern unsigned int data_80130FC4[];
extern unsigned int data_80131018[];
extern unsigned int data_80131060[];
extern unsigned int data_8013109C[];
extern unsigned int data_801310E0[];
extern unsigned int data_80131128[];
extern unsigned int data_80131168[];
extern unsigned int data_801311AC[];
extern unsigned int data_801311F8[];
extern unsigned int data_8013123C[];
extern unsigned int data_8013127C[];
extern unsigned int data_801312CC[];
extern unsigned int data_801312F8[];
extern unsigned int data_80131308[];
extern unsigned int data_8013135C[];
extern unsigned int data_801313A4[];
extern unsigned int data_801313EC[];
extern unsigned int data_8013142C[];
extern unsigned int data_80131468[];
extern unsigned int data_8013149C[];
extern unsigned int data_801314DC[];
extern unsigned int data_80131528[];
extern unsigned int data_8013157C[];
extern unsigned int data_801315B0[];
extern unsigned int data_801315CC[];
extern unsigned int data_801315DC[];
extern unsigned int data_80131614[];
extern unsigned int data_80131644[];
extern unsigned int data_80131664[];
extern unsigned int data_801316AC[];
extern unsigned int data_801316E0[];
extern unsigned int data_8013171C[];
extern void *ptrs_80131758[];
extern void *ptrs_80131904[];
extern void *ptrs_801319C8[];
extern unsigned int data_80131A7C[];
extern unsigned int data_80131B1C[];
extern unsigned int data_80131BA4[];
extern unsigned int data_80131BBC[];
extern unsigned int data_80131BF4[];
extern void *ptrs_80131C2C[];
extern AnimWord anim_80131C34[];
extern AnimWord anim_80131C3C[];
extern void *ptrs_80131C50[];
extern void *ptrs_80131C60[];

unsigned int data_8012F2E8[2] = {
    0x0, 0x0,
};

unsigned int data_8012F2F0[2] = {
    0x80008000, 0x40406020,
};

unsigned int data_8012F2F8[25] = {
    0x10001, 0x10001, 0x20002, 0x30002,
    0x30003, 0x40004, 0x50005, 0x70006,
    0x90008, 0xB000A, 0xD000C, 0xE000E,
    0xF000F, 0x100010, 0x110011, 0x110011,
    0x120012, 0x120012, 0x140013, 0x160015,
    0x180017, 0x1A0019, 0x1B001B, 0x1B001B,
    0x1C001B,
};

unsigned int data_8012F35C[7] = {
    0x1C001C, 0x1C001C, 0x1C001C, (unsigned int)bitingPlantSway,
    (unsigned int)bitingPlantLunge, (unsigned int)area10Type07SwingChain, (unsigned int)area10Type07StepDChain,
};

unsigned int data_8012F378[8] = {
    0x18000C, 0x800000, 0x18000C, 0x200020,
    0x0, 0x0, 0x18000C, 0x800000,
};

unsigned int data_8012F398[4] = {
    0x10000040, 0x18000080, 0x200000C0, 0x30000100,
};

void *ptrs_8012F3A8[2] = {
    D_80132A20,
    D_80132A34,
};

unsigned int data_8012F3B0[1] = {
    0x20102814,
};

unsigned int data_8012F3B4[1] = {
    0x7000A,
};

unsigned int data_8012F3B8[1] = {
    0x80132964,
};

unsigned int data_8012F3BC[1] = {
    0x28142814,
};

unsigned int data_8012F3C0[1] = {
    0x50007,
};

unsigned int data_8012F3C4[1] = {
    0x80132C84,
};

unsigned int data_8012F3C8[3] = {
    0x1C001B, 0x1E001D, 0x20001F,
};

unsigned int data_8012F3D4[1] = {
    0x341A1C0C,
};

unsigned int data_8012F3D8[2] = {
    0x341A140A, 0x341A140A,
};

unsigned int data_8012F3E0[1] = {
    0x240A140A,
};

unsigned int data_8012F3E4[1] = {
    0x240A140A,
};

unsigned int data_8012F3E8[1] = {
    0x2E141C0C,
};

unsigned int data_8012F3EC[1] = {
    0x240A140A,
};

unsigned int data_8012F3F0[1] = {
    0x240A140A,
};

unsigned int data_8012F3F4[1] = {
    0x240A140A,
};

unsigned int data_8012F3F8[2] = {
    0x341A200A, 0x341A200A,
};

unsigned int data_8012F400[1] = {
    0x260C1E0A,
};

unsigned int data_8012F404[1] = {
    0x341A1E0A,
};

unsigned int data_8012F408[2] = {
    0x260C200A, 0x341A200A,
};

unsigned int data_8012F410[1] = {
    0x260C140A,
};

unsigned int data_8012F414[1] = {
    0x341A140A,
};

unsigned int data_8012F418[2] = {
    0x260C140A, 0x341A140A,
};

unsigned int data_8012F420[3] = {
    0x3218200A, 0x341A200A, 0x341A200A,
};

unsigned int data_8012F42C[7] = {
    0x341A1C0C, 0x341A200A, 0x341A200A, 0x341A200A,
    0x341A200A, 0x260C140A, 0x341A200A,
};

unsigned int data_8012F448[6] = {
    0x32181C0C, 0x341A200A, 0x341A200A, 0x341A200A,
    0x341A200A, 0x341A1C0C,
};

unsigned int data_8012F460[1] = {
    0x341A200A,
};

unsigned int data_8012F464[1] = {
    0x341A1C0C,
};

unsigned int data_8012F468[1] = {
    0x341A140A,
};

unsigned int data_8012F46C[2] = {
    0x341A200A, 0x341A200A,
};

unsigned int data_8012F474[2] = {
    0x783C78, 0x3C3CB478,
};

unsigned int data_8012F47C[2] = {
    0x10000, 0x1000100,
};

unsigned int data_8012F484[4] = {
    0x3C5A1E1E, 0x3C1E003C, 0x1E5A3C00, 0x1E3C001E,
};

unsigned int data_8012F494[2] = {
    0x10000, 0x100,
};

unsigned int data_8012F49C[4] = {
    0x1E0000, 0xF, 0x140028, 0x1E0000,
};

unsigned int data_8012F4AC[16] = {
    0xFF100068, 0x0, 0xFF36, 0x0,
    0xFFA8, 0x5A, 0x30, 0x5A,
    0x78, 0x5A, 0x98, 0x0,
    0xFF84, 0xFFA6, 0x58, 0xFFA6,
};

unsigned int data_8012F4EC[2] = {
    0x78003C, 0xF000B4,
};

unsigned int data_8012F4F4[12] = {
    0x10000, 0x0, 0x0, 0x0,
    0x4500000, 0x5AFF04, 0x0, 0xFF,
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_8012F524[2] = {
    0x0, 0x0,
};

unsigned int data_8012F52C[1] = {
    (unsigned int)data_8012F4F4,
};

unsigned int data_8012F530[1] = {
    0x10081008,
};

unsigned int data_8012F534[1] = {
    0x80006,
};

unsigned int data_8012F538[1] = {
    (unsigned int)ptrs_80131C2C,
};

unsigned int data_8012F53C[14] = {
    0x0, 0x0, 0x0, 0x0,
    0x5A00000, 0x5AFFB4, 0x280000, 0xFF,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8012F574[1] = {
    (unsigned int)data_8012F53C,
};

unsigned int data_8012F578[5] = {
    0xFF, 0x0, 0x0, 0x0,
    0x0,
};

unsigned int data_8012F58C[2] = {
    0x0, 0x0,
};

unsigned int data_8012F594[1] = {
    (unsigned int)data_8012F578,
};

unsigned int data_8012F598[1] = {
    0x1,
};

unsigned int data_8012F59C[1] = {
    0xC90000,
};

unsigned int data_8012F5A0[2] = {
    0xFFC6, 0x0,
};

unsigned int data_8012F5A8[9] = {
    0x200082, 0x0, 0xE00100, 0x1FF0080,
    0x7FFF0000, 0x0, 0x0, 0x0,
    0x0,
};

void *ptrs_8012F5CC[5] = {
    ptrs_8012F5CC,
    ptrs_8012F5CC,
    ptrs_8012F5CC,
    ptrs_8012F5CC,
    ptrs_8012F5CC,
};

unsigned int data_8012F5E0[18] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0700, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8012F628[21] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0xF500, 0x100F700,
    0x100F900, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_8012F67C[8] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FA00,
    0x8200FC00, 0x8300FF00, 0x84000100, 0x85000300,
};

unsigned int data_8012F69C[4] = {
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8012F6AC[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8012F6E4[11] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FE00, 0x83000000, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8012F710[15] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000600, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8012F74C[16] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000100, 0x85000400, 0x3000600, 0x3000800,
    0x4000A00, 0x4000C00, 0x4000E00, 0xFF,
};

unsigned int data_8012F78C[11] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FC00,
    0x8200FE00, 0x83000000, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8012F7B8[14] = {
    0xF200, 0xF400, 0xF600, 0x8000F800,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000700, 0x3000900, 0x4000B00,
    0x4000D00, 0xFF,
};

unsigned int data_8012F7F0[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8012F828[19] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0xFF,
};

unsigned int data_8012F874[18] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000100, 0x2000300, 0x3000500,
    0x3000700, 0xFF,
};

unsigned int data_8012F8BC[11] = {
    0xF600, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8012F8E8[10] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800,
};

unsigned int data_8012F910[14] = {
    0xF300, 0xF500, 0x100F700, 0x100F900,
    0x100FB00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8012F948[8] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
};

unsigned int data_8012F968[8] = {
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
};

unsigned int data_8012F988[4] = {
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8012F998[2] = {
    0xF100, 0xF300,
};

unsigned int data_8012F9A0[14] = {
    0xF500, 0x100F700, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000000, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0x4000C00,
    0x4000E00, 0xFF,
};

unsigned int data_8012F9D8[20] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000400, 0x3000600, 0x3000800, 0xFF,
};

unsigned int data_8012FA28[11] = {
    0xF500, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_8012FA54[12] = {
    0xF500, 0x100F700, 0x8000F900, 0x8100FB00,
    0x8200FD00, 0x8300FF00, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8012FA84[9] = {
    0x100F700, 0x8000F900, 0x8100FB00, 0x8200FE00,
    0x83000000, 0x84000300, 0x85000500, 0x3000700,
    0xFF,
};

unsigned int data_8012FAA8[9] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100F900,
    0x100FB00,
};

unsigned int data_8012FACC[6] = {
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0x3000500, 0xFF,
};

unsigned int data_8012FAE4[23] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0xF200, 0xF400, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8012FB40[14] = {
    0xF200, 0xF400, 0xF600, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000600, 0x3000900, 0x4000B00,
    0x4000D00, 0xFF,
};

unsigned int data_8012FB78[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8012FBC4[21] = {
    0x1FEF700, 0x80FEF900, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0xF500, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_8012FC18[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_8012FC60[10] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700,
};

unsigned int data_8012FC88[3] = {
    0x3000900, 0x4000B00, 0xFF,
};

unsigned int data_8012FC94[14] = {
    0xF200, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x83000000, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8012FCCC[14] = {
    0xF200, 0xF400, 0xF600, 0x8000F800,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000200,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8012FD04[15] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFF00,
    0x83FE0100, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8012FD40[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_8012FD74[14] = {
    0xF200, 0xF400, 0xF600, 0x8000F800,
    0x8100FA00, 0x8200FC00, 0x8300FF00, 0x84000100,
    0x85000300, 0x3000500, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_8012FDAC[17] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8012FDF0[21] = {
    0xFEF400, 0xFEF600, 0x1FEF800, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0x3000700,
    0xFF,
};

unsigned int data_8012FE44[12] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0700, 0xF200, 0xF400, 0xF600,
};

unsigned int data_8012FE74[12] = {
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x3000500, 0x3000700,
    0x3000900, 0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8012FEA4[16] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0500, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_8012FEE4[21] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_8012FF38[12] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FA00,
    0x8200FD00, 0x8300FF00, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8012FF68[23] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0800, 0x4FE0A00, 0xF500,
    0x100F700, 0x100F900, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_8012FFC4[22] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_8013001C[11] = {
    0xF600, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FF00, 0x83000100, 0x84000300, 0x85000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_80130048[11] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FB00,
    0x8200FE00, 0x83000000, 0x84000200, 0x85000400,
    0x3000700, 0x4000A00, 0xFF,
};

unsigned int data_80130074[15] = {
    0xF100, 0xF400, 0xF600, 0x100F800,
    0x8000FA00, 0x8100FD00, 0x8200FF00, 0x83000100,
    0x84000300, 0x85000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_801300B0[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_801300F8[17] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_8013013C[5] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000,
};

unsigned int data_80130150[13] = {
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_80130184[23] = {
    0xFEF100, 0xFEF300, 0xFEF500, 0x1FEF700,
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x4FE0A00, 0x4FE0C00, 0x4FE0E00, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_801301E0[14] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x200FD00, 0x200FF00,
    0x2000100, 0xFF,
};

unsigned int data_80130218[16] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0500, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600, 0xFF,
};

unsigned int data_80130258[19] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFE00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0800, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0xFF,
};

unsigned int data_801302A4[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_801302E4[14] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x200FD00, 0x200FF00,
    0x2000100, 0xFF,
};

unsigned int data_8013031C[10] = {
    0x100F700, 0x8000F900, 0x8100FB00, 0x8200FD00,
    0x83000000, 0x84000200, 0x85000400, 0x3000600,
    0x3000800, 0xFF,
};

unsigned int data_80130344[20] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0800, 0x4FE0A00, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0xFF,
};

unsigned int data_80130394[14] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0600, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_801303CC[19] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFA00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_80130418[14] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FB00, 0x8200FD00, 0x8300FF00, 0x84000100,
    0x85000400, 0x3000600, 0x3000800, 0x4000A00,
    0x4000C00, 0xFF,
};

unsigned int data_80130450[23] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0xF200, 0xF400, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_801304AC[2] = {
    0xF500, 0x100F700,
};

unsigned int data_801304B4[10] = {
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000100, 0x85000400, 0x3000600, 0x3000800,
    0x4000A00, 0xFF,
};

unsigned int data_801304DC[16] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8013051C[21] = {
    0xFEF400, 0xFEF600, 0x1FEF800, 0x80FEFB00,
    0x81FEFD00, 0x82FEFF00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_80130570[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFB00, 0x82FEFE00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_801305BC[8] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
};

unsigned int data_801305DC[9] = {
    0x3FE0600, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_80130600[7] = {
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0xFF,
};

unsigned int data_8013061C[8] = {
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x83000000,
    0x84000200, 0x85000400, 0x3000600, 0xFF,
};

unsigned int data_8013063C[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8013067C[20] = {
    0xFEF200, 0xFEF400, 0xFEF600, 0x80FEF900,
    0x81FEFB00, 0x82FEFD00, 0x83FE0000, 0x84FE0200,
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_801306CC[21] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_80130720[17] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_80130764[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F900, 0x100FB00, 0x200FD00, 0x2000000,
    0x2000200, 0x2000400, 0x3000600,
};

unsigned int data_801307B0[1] = {
    0xFF,
};

unsigned int data_801307B4[18] = {
    0xFEF600, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0x3000600, 0xFF,
};

unsigned int data_801307FC[17] = {
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0300, 0x85FE0500, 0xF600, 0x100F800,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000300, 0x3000500, 0x3000700, 0x3000900,
    0xFF,
};

unsigned int data_80130840[15] = {
    0xFEF600, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0xFF,
};

unsigned int data_8013087C[15] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x200FD00,
    0x200FF00, 0x2000100, 0xFF,
};

unsigned int data_801308B8[2] = {
    0xF300, 0xF500,
};

unsigned int data_801308C0[12] = {
    0x100F700, 0x8000F900, 0x8100FB00, 0x8200FD00,
    0x83000000, 0x84000200, 0x85000400, 0x3000600,
    0x3000800, 0x4000A00, 0x4000C00, 0xFF,
};

unsigned int data_801308F0[17] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFD00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000100, 0x2000300, 0x3000500,
    0xFF,
};

unsigned int data_80130934[18] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFD00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0x3000500, 0xFF,
};

unsigned int data_8013097C[5] = {
    0xFEF600, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00,
};

unsigned int data_80130990[10] = {
    0x84FE0100, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0xFF,
};

unsigned int data_801309B8[19] = {
    0xFEF400, 0xFEF600, 0x80FEF800, 0x81FEFA00,
    0x82FEFC00, 0x83FEFE00, 0x84FE0000, 0x85FE0200,
    0x3FE0500, 0x3FE0700, 0x4FE0A00, 0x100F900,
    0x100FB00, 0x200FD00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0xFF,
};

unsigned int data_80130A04[18] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100F700, 0x100F900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0xFF,
};

unsigned int data_80130A4C[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_80130A8C[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_80130ACC[17] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000400, 0x3000600, 0x3000800,
    0xFF,
};

unsigned int data_80130B10[7] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600,
};

unsigned int data_80130B2C[6] = {
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0xFF,
};

unsigned int data_80130B44[13] = {
    0xF400, 0xF600, 0x100F800, 0x8000FA00,
    0x8100FD00, 0x8200FF00, 0x83000100, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_80130B78[17] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_80130BBC[21] = {
    0x80FCFA00, 0x81FCFC00, 0x82FCFE00, 0x83FC0000,
    0x84FC0300, 0x85FC0500, 0x1FEF700, 0x1FEF900,
    0x1FEFB00, 0x2FEFD00, 0x2FEFF00, 0x2FE0100,
    0x2FE0300, 0x3FE0500, 0x3FE0700, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_80130C10[17] = {
    0xFEF500, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFE00, 0x83FE0100, 0x84FE0400, 0x85FE0600,
    0x3FE0800, 0x4FE0A00, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_80130C54[15] = {
    0xF100, 0xF300, 0xF600, 0x100F800,
    0x8000FA00, 0x8100FC00, 0x8200FF00, 0x83000100,
    0x84000300, 0x85000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_80130C90[19] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0xF600, 0x100F800, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000100, 0x2000300, 0x3000500,
    0x3000700, 0x3000900, 0xFF,
};

unsigned int data_80130CDC[11] = {
    0xF500, 0x100F800, 0x8000FA00, 0x8100FC00,
    0x8200FE00, 0x83000100, 0x84000400, 0x85000600,
    0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_80130D08[23] = {
    0xFEF100, 0xFEF300, 0xFEF500, 0x1FEF800,
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x3FE0800,
    0x4FE0A00, 0x4FE0C00, 0x4FE0E00, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000300, 0x3000500, 0xFF,
};

unsigned int data_80130D64[20] = {
    0xFEF200, 0xFEF400, 0xFEF600, 0x80FEF900,
    0x81FEFB00, 0x82FEFD00, 0x83FEFF00, 0x84FE0100,
    0x85FE0300, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_80130DB4[3] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00,
};

unsigned int data_80130DC0[15] = {
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x100F800, 0x100FA00,
    0x200FC00, 0x200FF00, 0x2000100, 0x2000300,
    0x3000500, 0x3000700, 0xFF,
};

unsigned int data_80130DFC[16] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0xF600,
    0x100F800, 0x100FA00, 0x200FD00, 0x200FF00,
};

unsigned int data_80130E3C[6] = {
    0x2000100, 0x2000300, 0x3000500, 0x3000700,
    0x3000900, 0xFF,
};

unsigned int data_80130E54[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000F900,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_80130E88[13] = {
    0xF300, 0xF500, 0x100F700, 0x8000FA00,
    0x8100FC00, 0x8200FE00, 0x83000000, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_80130EBC[20] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEFA00,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0200,
    0x85FE0400, 0x3FE0600, 0x3FE0800, 0x4FE0A00,
    0x4FE0C00, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_80130F0C[13] = {
    0xF400, 0xF600, 0x100F800, 0x8000FA00,
    0x8100FC00, 0x8200FF00, 0x83000100, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_80130F40[15] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0xFF,
};

unsigned int data_80130F7C[18] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FE0000, 0x84FE0200, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0x3000500, 0xFF,
};

unsigned int data_80130FC4[21] = {
    0xFEF400, 0xFEF600, 0x1FEF800, 0x80FEFB00,
    0x81FEFD00, 0x82FEFF00, 0x83FE0100, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_80131018[18] = {
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0xF500, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0x3000700,
    0x3000900, 0xFF,
};

unsigned int data_80131060[15] = {
    0xF100, 0xF300, 0xF500, 0x100F700,
    0x8000F900, 0x8100FB00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000400, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

unsigned int data_8013109C[17] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFC00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FA00, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_801310E0[18] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFE00,
    0x83FE0000, 0x84FE0200, 0x85FE0500, 0x3FE0700,
    0x100F700, 0x100F900, 0x100FB00, 0x200FD00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0x3000800, 0xFF,
};

unsigned int data_80131128[16] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x100FB00, 0x200FD00,
    0x200FF00, 0x2000100, 0x2000300, 0xFF,
};

unsigned int data_80131168[17] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFC00,
    0x82FEFE00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000100, 0x2000300,
    0xFF,
};

unsigned int data_801311AC[19] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100FA00, 0x200FC00, 0x200FE00, 0x2000000,
    0x2000200, 0x2000400, 0xFF,
};

unsigned int data_801311F8[17] = {
    0x1FEF800, 0x80FEFA00, 0x81FEFC00, 0x82FEFF00,
    0x83FE0100, 0x84FE0300, 0x85FE0500, 0x3FE0700,
    0x100F800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0x3000600,
    0xFF,
};

unsigned int data_8013123C[16] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0200, 0x85FE0400, 0x3FE0600,
    0x3FE0800, 0x100FA00, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0x2000400, 0xFF,
};

unsigned int data_8013127C[20] = {
    0xFEF300, 0xFEF500, 0x1FEF700, 0x80FEF900,
    0x81FEFC00, 0x82FEFE00, 0x83FE0000, 0x84FE0300,
    0x85FE0500, 0x3FE0700, 0x3FE0900, 0x4FE0B00,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000300, 0x3000500, 0xFF,
};

unsigned int data_801312CC[11] = {
    0xFEF600, 0x80FEF800, 0x81FEFA00, 0x82FEFC00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0600,
    0x3FE0800, 0x100FB00, 0x200FD00,
};

unsigned int data_801312F8[4] = {
    0x200FF00, 0x2000100, 0x2000300, 0xFF,
};

unsigned int data_80131308[21] = {
    0x1FEF700, 0x80FEF900, 0x81FEFB00, 0x82FEFD00,
    0x83FEFF00, 0x84FE0100, 0x85FE0300, 0x3FE0500,
    0x3FE0700, 0xF500, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x200FF00, 0x2000100,
    0x2000400, 0x3000600, 0x3000800, 0x4000A00,
    0xFF,
};

unsigned int data_8013135C[18] = {
    0x80FEF900, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0500, 0xF500, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000200, 0x2000400, 0x3000600, 0x3000800,
    0x4000A00, 0xFF,
};

unsigned int data_801313A4[18] = {
    0x80FEF900, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0500, 0xF500, 0x100F700,
    0x100F900, 0x100FB00, 0x200FD00, 0x200FF00,
    0x2000200, 0x2000400, 0x3000600, 0x3000800,
    0x4000A00, 0xFF,
};

unsigned int data_801313EC[16] = {
    0x80FEF900, 0x81FEFC00, 0x82FEFE00, 0x83FE0000,
    0x84FE0200, 0x85FE0500, 0x100F700, 0x100F900,
    0x100FB00, 0x200FD00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0x3000800, 0xFF,
};

unsigned int data_8013142C[15] = {
    0xFEF600, 0x1FEF800, 0x80FEFA00, 0x81FEFD00,
    0x82FEFF00, 0x83FE0100, 0x84FE0300, 0x85FE0500,
    0x3FE0700, 0x3FE0900, 0x200FC00, 0x200FE00,
    0x2000000, 0x2000200, 0xFF,
};

unsigned int data_80131468[13] = {
    0x80FEF900, 0x81FEFB00, 0x82FEFD00, 0x83FE0000,
    0x84FE0200, 0x85FE0400, 0x3FE0600, 0x100FB00,
    0x200FE00, 0x2000000, 0x2000200, 0x2000400,
    0xFF,
};

unsigned int data_8013149C[16] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x200FC00,
    0x200FE00, 0x2000000, 0x2000200, 0xFF,
};

unsigned int data_801314DC[19] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100F900,
    0x100FB00, 0x200FD00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0xFF,
};

unsigned int data_80131528[21] = {
    0x80FEF800, 0x81FEFA00, 0x82FEFC00, 0x83FEFF00,
    0x84FE0100, 0x85FE0300, 0x3FE0500, 0xF300,
    0xF500, 0x100F700, 0x100F900, 0x100FB00,
    0x200FD00, 0x200FF00, 0x2000200, 0x2000400,
    0x3000600, 0x3000800, 0x4000A00, 0x4000C00,
    0xFF,
};

unsigned int data_8013157C[13] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
    0x82FEFD00, 0x83FEFF00, 0x84FE0100, 0x85FE0300,
    0x3FE0500, 0x3FE0700, 0x3FE0900, 0x100F800,
    0x100FA00,
};

unsigned int data_801315B0[7] = {
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0x3000600, 0xFF,
};

unsigned int data_801315CC[4] = {
    0xFEF500, 0x1FEF700, 0x80FEF900, 0x81FEFB00,
};

unsigned int data_801315DC[14] = {
    0x82FEFD00, 0x83FE0000, 0x84FE0200, 0x85FE0400,
    0x3FE0600, 0x3FE0800, 0x4FE0A00, 0x100FA00,
    0x200FC00, 0x200FE00, 0x2000000, 0x2000200,
    0x2000400, 0xFF,
};

unsigned int data_80131614[12] = {
    0xF400, 0xF600, 0x8000F800, 0x8100FB00,
    0x8200FD00, 0x83000000, 0x84000200, 0x85000400,
    0x3000600, 0x3000800, 0x4000A00, 0xFF,
};

unsigned int data_80131644[8] = {
    0x8000F900, 0x8100FB00, 0x8200FD00, 0x8300FF00,
    0x84000200, 0x85000400, 0x3000600, 0xFF,
};

unsigned int data_80131664[18] = {
    0x80FEFA00, 0x81FEFC00, 0x82FEFE00, 0x83FE0100,
    0x84FE0300, 0x85FE0500, 0xF400, 0xF600,
    0x100F800, 0x100FA00, 0x200FD00, 0x200FF00,
    0x2000100, 0x2000400, 0x3000600, 0x3000800,
    0x4000A00, 0xFF,
};

unsigned int data_801316AC[13] = {
    0xF400, 0xF600, 0x100F800, 0x8000FA00,
    0x8100FC00, 0x8200FF00, 0x83000100, 0x84000300,
    0x85000500, 0x3000700, 0x3000900, 0x4000B00,
    0xFF,
};

unsigned int data_801316E0[15] = {
    0xF100, 0xF300, 0xF500, 0x100F800,
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000100,
    0x84000300, 0x85000500, 0x3000700, 0x4000A00,
    0x4000C00, 0x4000E00, 0xFF,
};

unsigned int data_8013171C[15] = {
    0xF100, 0xF300, 0xF500, 0x100F800,
    0x8000FA00, 0x8100FC00, 0x8200FE00, 0x83000000,
    0x84000200, 0x85000500, 0x3000700, 0x3000900,
    0x4000B00, 0x4000D00, 0xFF,
};

void *ptrs_80131758[107] = {
    data_8012F5E0,
    data_8012F628,
    data_8012F67C,
    data_8012F6E4,
    data_8012F74C,
    data_8012F78C,
    data_8012F5E0,
    data_8012F7B8,
    data_8012F7F0,
    data_8012F828,
    data_8012F874,
    data_80130764,
    data_8012F8BC,
    data_8012F8E8,
    data_8012FA28,
    data_8012FA54,
    data_8012FA84,
    data_8012FAA8,
    data_8012F5E0,
    data_8012FC60,
    data_8012FCCC,
    data_8012FD04,
    data_8012FD40,
    data_8012FDAC,
    data_8012FF38,
    data_8012FF68,
    data_8012FBC4,
    data_8013013C,
    data_80130184,
    data_801301E0,
    data_80130218,
    data_80130258,
    data_8012F5E0,
    data_801302A4,
    data_8013031C,
    data_80130344,
    data_80130394,
    data_801304AC,
    data_801304DC,
    data_80130570,
    data_801305BC,
    data_80130600,
    data_8013061C,
    data_8013063C,
    data_8013067C,
    data_8012F5E0,
    data_801306CC,
    data_80130720,
    data_8012F5E0,
    data_801307B4,
    data_801307FC,
    data_80130840,
    data_8012F5E0,
    data_8013087C,
    data_80130934,
    data_8013097C,
    data_801309B8,
    data_80130A04,
    data_80130A4C,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_80130BBC,
    data_80130B10,
    data_80130B78,
    data_80130C10,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_80130C54,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_80130D08,
    data_80130B44,
    data_80130DFC,
    data_8012FB78,
    data_801311F8,
    data_80131614,
    data_8012F5E0,
    data_8012F5E0,
    data_80130ACC,
    data_8012FDF0,
    data_8012FE44,
    data_8012FEE4,
    data_8012F5E0,
    data_8013109C,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_80130C90,
    data_80130E88,
    data_80130E54,
    data_80130CDC,
    data_8012F5E0,
};

void *ptrs_80131904[49] = {
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_801308B8,
    data_80130FC4,
    data_8012FEA4,
    data_8013127C,
    data_80130074,
    data_80130F7C,
    data_8012F5E0,
    data_80130D64,
    data_80131060,
    data_8013123C,
    data_80131308,
    data_8012F5E0,
    data_80130EBC,
    data_80130F0C,
    data_8013142C,
    data_80130F40,
    data_80131128,
    data_8013149C,
    data_801315CC,
    data_8012F5E0,
    data_8012F5E0,
    data_801312CC,
    data_8013135C,
    data_801313A4,
    data_801313EC,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_801314DC,
    data_80131468,
    data_80131528,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_80130DB4,
    data_80131644,
    data_8012F5E0,
    data_80131018,
    data_8012F5E0,
    data_8012F5E0,
};

void *ptrs_801319C8[45] = {
    data_801310E0,
    data_801316AC,
    data_8012F5E0,
    data_80131664,
    data_801316E0,
    data_8013157C,
    data_8012F5E0,
    data_8012F5E0,
    data_8013171C,
    data_8012F5E0,
    data_8012F6AC,
    data_8012F710,
    data_8012FB40,
    data_8012FC18,
    data_801303CC,
    data_8013051C,
    data_801302E4,
    data_80130418,
    data_801308F0,
    data_801300B0,
    data_801300F8,
    data_8012F948,
    data_8012F998,
    data_8012F9D8,
    data_8012FAE4,
    data_8012FD74,
    data_8012FC94,
    data_80130A8C,
    data_8012F5E0,
    data_80130450,
    data_8012F5E0,
    data_8012FFC4,
    data_8013001C,
    data_80130048,
    data_80131168,
    data_801311AC,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    data_8012F5E0,
    ptrs_80131758,
};

unsigned int data_80131A7C[40] = {
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

unsigned int data_80131B1C[34] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x64, 0x3030004, 0x64,
    0x4040004, 0x64, 0x5050004, 0x64,
    0x6060004, 0x64, 0x7070004, 0x64,
    0x8080004, 0x64, 0x9090004, 0x64,
    0xA0A0004, 0x64, 0xB0B0004, 0x64,
    0xC0C0004, 0x64, 0xD0D0004, 0x64,
    0xE0E0004, 0x64, 0xF0F0004, 0x64,
    0x10000104, 0x64,
};

unsigned int data_80131BA4[6] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x64,
};

unsigned int data_80131BBC[14] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x64, 0x3030004, 0x64,
    0x4040004, 0x64, 0x5050004, 0x64,
    0x6060004, 0x64,
};

unsigned int data_80131BF4[14] = {
    0x4, 0x64, 0x1010004, 0x64,
    0x2020004, 0x64, 0x3030004, 0x64,
    0x4040004, 0x64, 0x5050004, 0x64,
    0x6060004, 0x64,
};

void *ptrs_80131C2C[2] = {
    anim_80131C34,
    anim_80131C3C,
};

AnimWord anim_80131C34[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80131C3C[5] = {
    ANIM(1, 0, 0, 16),
    ANIM_JUMP(2, 0, 0, 16, anim_80131C3C),
};

void *ptrs_80131C50[4] = {
    D_80131FCC,
    D_80131FEC,
    D_8013200C,
    D_8013202C,
};

void *ptrs_80131C60[1] = {
    D_80131DD0,
};
