// FUNC 8003c550 628 MAIN0
// MATCHING 8003c550 628
// Portado de psx_tomba (main/game/reward.c, func_8003F9A4); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

u_char D_8007D6D0_data[0x10] asm("D_8007D6D0") = {
    7, 7, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

void updateItemPickupAnim(GameObject* arg0);

extern int D_80077274;
extern int D_8013B104;
extern int D_80134D0C;
extern u8 D_8009CEFB;
extern u8 D_8009D2AE;
extern int D_80131D84[];
extern int D_8007728C;

typedef struct {
    int x;
    int y;
    int z;
} VEC3;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    s16 unk6;
    int* unk8;
} unk_8007D6E0;

extern unk_8007D6E0 D_8007D6E0[];

unk_8007D6E0 D_8007D6E0[14] = {
    { 0xA, 0x14, 0x10, 0x20, 1, 0, 2, (int*)0x80014CCC },
    { 0x10, 0x20, 8, 0x18, 0x13, 0x24, 5, (int*)0x8013A078 },
    { 0x10, 0x20, 0xA, 0x14, 8, 0, 3, (int*)0x8013D954 },
    { 0x10, 0x20, 0x10, 0x20, 1, 0x11, 2, (int*)0x8013EFE8 },
    { 0x10, 0x20, 0x10, 0x20, 2, 0xC, 8, (int*)0x8012DDA4 },
    { 0x10, 0x20, 0x10, 0x20, 1, 0, 8, (int*)0x8013E574 },
    { 8, 0x10, 8, 0x10, 9, 0, 3, (int*)0x8012D3F8 },
    { 0x10, 0x20, 0xE, 0x1C, 0xB, 6, 5, (int*)0x801388B4 },
    { 0xA, 0x14, 0x10, 0x20, 1, 0x1F, 2, (int*)0x80137AAC },
    { 0x10, 0x20, 8, 0x10, 1, 4, 5, (int*)0x801338C0 },
    { 8, 0x10, 8, 0x10, 1, 0x1E, 5, (int*)0x80139A4C },
    { 0x10, 0x20, 0x10, 0x20, 1, 0xB, 5, (int*)0x80139A4C },
    { 0x10, 0x20, 0x10, 0x20, 1, 4, 2, (int*)0x80131694 },
    { 0xA, 0x14, 0xA, 0x14, 5, 0, 5, (int*)0x80131D84 }
};

s16 D_8007D788[0x100] = {
    0, -70, -140, -210, -281, -350, -420, -490, -559, -628, -696, -764, -832, -899, -965, -1031,
    -1097, -1161, -1225, -1289, -1351, -1413, -1474, -1533, -1592, -1650, -1707, -1763, -1818, -1872, -1925, -1977,
    -2027, -2076, -2124, -2171, -2216, -2260, -2302, -2344, -2383, -2422, -2459, -2494, -2528, -2561, -2591, -2621,
    -2648, -2675, -2699, -2722, -2743, -2763, -2781, -2797, -2812, -2824, -2836, -2845, -2853, -2859, -2863, -2866,
    -2867, -2866, -2863, -2859, -2853, -2845, -2836, -2824, -2812, -2797, -2781, -2763, -2743, -2722, -2699, -2675,
    -2648, -2621, -2591, -2561, -2528, -2494, -2459, -2422, -2383, -2344, -2302, -2260, -2216, -2171, -2124, -2076,
    -2027, -1977, -1925, -1872, -1818, -1763, -1707, -1650, -1592, -1533, -1474, -1413, -1351, -1289, -1225, -1161,
    -1097, -1031, -965, -899, -832, -764, -696, -628, -559, -490, -420, -350, -281, -210, -140, -70,
    0, 70, 140, 210, 281, 350, 420, 490, 559, 628, 696, 764, 832, 899, 965, 1031,
    1097, 1161, 1225, 1289, 1351, 1413, 1474, 1533, 1592, 1650, 1707, 1763, 1818, 1872, 1925, 1977,
    2027, 2076, 2124, 2171, 2216, 2260, 2302, 2344, 2383, 2422, 2459, 2494, 2528, 2561, 2591, 2621,
    2648, 2675, 2699, 2722, 2743, 2763, 2781, 2797, 2812, 2824, 2836, 2845, 2853, 2859, 2863, 2866,
    2867, 2866, 2863, 2859, 2853, 2845, 2836, 2824, 2812, 2797, 2781, 2763, 2743, 2722, 2699, 2675,
    2648, 2621, 2591, 2561, 2528, 2494, 2459, 2422, 2383, 2344, 2302, 2260, 2216, 2171, 2124, 2076,
    2027, 1977, 1925, 1872, 1818, 1763, 1707, 1650, 1592, 1533, 1474, 1413, 1351, 1289, 1225, 1161,
    1097, 1031, 965, 899, 832, 764, 696, 628, 559, 490, 420, 350, 281, 210, 140, 70
};

s16 D_8007D988[0x100] = {
    0, -100, -200, -301, -401, -501, -601, -700, -799, -897, -995, -1092, -1189, -1284, -1379, -1474,
    -1567, -1659, -1751, -1841, -1930, -2018, -2105, -2191, -2275, -2358, -2439, -2519, -2598, -2675, -2750, -2824,
    -2896, -2966, -3034, -3101, -3166, -3229, -3289, -3348, -3405, -3460, -3513, -3563, -3612, -3658, -3702, -3744,
    -3784, -3821, -3856, -3889, -3919, -3947, -3973, -3996, -4017, -4035, -4051, -4065, -4076, -4084, -4091, -4094,
    -4096, -4094, -4091, -4084, -4076, -4065, -4051, -4035, -4017, -3996, -3973, -3947, -3919, -3889, -3856, -3821,
    -3784, -3744, -3702, -3658, -3612, -3563, -3513, -3460, -3405, -3348, -3289, -3229, -3166, -3101, -3034, -2966,
    -2896, -2824, -2750, -2675, -2598, -2519, -2439, -2358, -2275, -2191, -2105, -2018, -1930, -1841, -1751, -1659,
    -1567, -1474, -1379, -1284, -1189, -1092, -995, -897, -799, -700, -601, -501, -401, -301, -200, -100,
    0, 100, 200, 301, 401, 501, 601, 700, 799, 897, 995, 1092, 1189, 1284, 1379, 1474,
    1567, 1659, 1751, 1841, 1930, 2018, 2105, 2191, 2275, 2358, 2439, 2519, 2598, 2675, 2750, 2824,
    2896, 2966, 3034, 3101, 3166, 3229, 3289, 3348, 3405, 3460, 3513, 3563, 3612, 3658, 3702, 3744,
    3784, 3821, 3856, 3889, 3919, 3947, 3973, 3996, 4017, 4035, 4051, 4065, 4076, 4084, 4091, 4094,
    4096, 4094, 4091, 4084, 4076, 4065, 4051, 4035, 4017, 3996, 3973, 3947, 3919, 3889, 3856, 3821,
    3784, 3744, 3702, 3658, 3612, 3563, 3513, 3460, 3405, 3348, 3289, 3229, 3166, 3101, 3034, 2966,
    2896, 2824, 2750, 2675, 2598, 2519, 2439, 2358, 2275, 2191, 2105, 2018, 1930, 1841, 1751, 1659,
    1567, 1474, 1379, 1284, 1189, 1092, 995, 897, 799, 700, 601, 501, 401, 301, 200, 100
};

s16 D_8007DB88[0x100] = {
    4096, 4094, 4091, 4084, 4076, 4065, 4051, 4035, 4017, 3996, 3973, 3947, 3919, 3889, 3856, 3821,
    3784, 3744, 3702, 3658, 3612, 3563, 3513, 3460, 3405, 3348, 3289, 3229, 3166, 3101, 3034, 2966,
    2896, 2824, 2750, 2675, 2598, 2519, 2439, 2358, 2275, 2191, 2105, 2018, 1930, 1841, 1751, 1659,
    1567, 1474, 1379, 1284, 1189, 1092, 995, 897, 799, 700, 601, 501, 401, 301, 200, 100,
    0, -100, -200, -301, -401, -501, -601, -700, -799, -897, -995, -1092, -1189, -1284, -1379, -1474,
    -1567, -1659, -1751, -1841, -1930, -2018, -2105, -2191, -2275, -2358, -2439, -2519, -2598, -2675, -2750, -2824,
    -2896, -2966, -3034, -3101, -3166, -3229, -3289, -3348, -3405, -3460, -3513, -3563, -3612, -3658, -3702, -3744,
    -3784, -3821, -3856, -3889, -3919, -3947, -3973, -3996, -4017, -4035, -4051, -4065, -4076, -4084, -4091, -4094,
    -4096, -4094, -4091, -4084, -4076, -4065, -4051, -4035, -4017, -3996, -3973, -3947, -3919, -3889, -3856, -3821,
    -3784, -3744, -3702, -3658, -3612, -3563, -3513, -3460, -3405, -3348, -3289, -3229, -3166, -3101, -3034, -2966,
    -2896, -2824, -2750, -2675, -2598, -2519, -2439, -2358, -2275, -2191, -2105, -2018, -1930, -1841, -1751, -1659,
    -1567, -1474, -1379, -1284, -1189, -1092, -995, -897, -799, -700, -601, -501, -401, -301, -200, -100,
    0, 100, 200, 301, 401, 501, 601, 700, 799, 897, 995, 1092, 1189, 1284, 1379, 1474,
    1567, 1659, 1751, 1841, 1930, 2018, 2105, 2191, 2275, 2358, 2439, 2519, 2598, 2675, 2750, 2824,
    2896, 2966, 3034, 3101, 3166, 3229, 3289, 3348, 3405, 3460, 3513, 3563, 3612, 3658, 3702, 3744,
    3784, 3821, 3856, 3889, 3919, 3947, 3973, 3996, 4017, 4035, 4051, 4065, 4076, 4084, 4091, 4094
};

u8 D_8007DD88[0xA0] = {
    2, 2, 3, 0x13, 0, 1, 1, 1, 0xC, 1, 0, 0, 1, 1, 0xB, 4,
    0xA, 0, 0x16, 5, 1, 7, 8, 9, 0, 0, 0, 0, 1, 0x17, 1, 1,
    0, 0xD, 0, 0, 0, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0x11,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    0, 0, 0x14, 0x14, 0x14, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1,
    0, 0, 0x10, 0, 1, 0, 0, 0xB, 0, 1, 1, 0, 1, 0, 0xF, 1,
    1, 1, 0x15, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0
};

u8 D_8007DE28[0x60] = {
    0x8C, 0, 4, 0x80, 0xE0, 0, 4, 0x80, 0x68, 2, 4, 0x80, 0x5C, 3, 4, 0x80,
    0xE4, 3, 4, 0x80, 0x68, 4, 4, 0x80, 0xE8, 4, 4, 0x80, 0x64, 5, 4, 0x80,
    0xC8, 5, 4, 0x80, 0x2C, 6, 4, 0x80, 0x90, 6, 4, 0x80, 0x18, 7, 4, 0x80,
    0xC, 8, 4, 0x80, 0x5C, 9, 4, 0x80, 0xCC, 9, 4, 0x80, 0x30, 0xA, 4, 0x80,
    0x28, 0xB, 4, 0x80, 0x98, 0xB, 4, 0x80, 0xC, 0xC, 4, 0x80, 0xA0, 0xD, 4, 0x80,
    0x24, 0xE, 4, 0x80, 0xD8, 0xF, 4, 0x80, 0x48, 0x10, 4, 0x80, 0xD8, 0x10, 4, 0x80
};

itemDef D_8007DE88 = { 0, 2, 0x15, 0xFB, 4, 1, 1, 0, 0x160, 0x1F5, 7, 0x10, 7, 0x10, 0x80012190 };

itemDef D_8007DE9C = { 0, 2, 0x15, 0xFB, 4, 1, 1, 0, 0x160, 0x1F5, 7, 0x10, 7, 0x10, 0x80012190 };

itemDef D_8007DEB0 = { 1, 3, 0x14, 0xF6, 4, 1, 2, 0, 0, 0, 8, 0x10, 8, 0x10, 0x800122DC };

itemDef D_8007DEC4 = { 0, 0, 0x15, 0xF6, 4, 0, 0, 0, 0, 0, 7, 0x10, 7, 0x10, 0x800122F8 };

itemDef D_8007DED8 = { 0, 0, 0x14, 0xF6, 4, 1, 1, 0, 0x160, 0x1FA, 8, 0x10, 8, 0x10, 0x8001230C };

itemDef D_8007DEEC = { 0, 1, 8, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8013A42C };

itemDef D_8007DF00 = { 0, 1, 0xB, 0xF6, 3, 1, 0, 0, 0xF0, 0x1E7, 8, 0x10, 8, 0x10, 0x8013A428 };

itemDef D_8007DF14 = { 0, 1, 0x14, 0, 4, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80012374 };

itemDef D_8007DF28 = { 0, 0xC, 0xC, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8013A580 };

itemDef D_8007DF3C = { 0, 1, 0xA, 0xEC, 3, 0, 0, 1, 0, 0, 8, 0x10, 8, 0x10, 0x8012D3B0 };

itemDef D_8007DF50 = { 0, 0, 0x14, 0xF6, 4, 1, 3, 0, 0x160, 0x1FA, 8, 0x10, 8, 0x10, 0x8001230C };

itemDef D_8007DF64 = { 0, 1, 9, 0, 3, 0, 0, 0, 0, 0, 0xA, 0x14, 8, 0x18, 0x8012D3D0 };

itemDef D_8007DF78 = { 0, 1, 9, 0, 3, 0, 0, 0, 0, 0, 0xA, 0x14, 8, 0x18, 0x8012D3D4 };

itemDef D_8007DF8C = { 0, 0xB, 0x14, 0xF6, 4, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80012320 };

itemDef D_8007DFA0 = { 0, 4, 7, 0xEC, 3, 1, 0, 0, 0x100, 0x1E5, 8, 0x10, 8, 0x10, 0x8013DAB4 };

itemDef D_8007DFB4 = { 0, 0xA, 0x15, 0xF6, 4, 1, 0, 0, 0x160, 0x1F0, 8, 0x10, 8, 0x10, 0x80012324 };

itemDef D_8007DFC8 = { 0, 0, 0x15, 0xF6, 4, 1, 0, 0, 0x170, 0x1EF, 8, 0x10, 8, 0x10, 0x80012328 };

itemDef D_8007DFDC = { 0, 5, 0xB, 0, 3, 0, 0, 0, 0, 0, 8, 0x10, 0x10, 0x20, 0x8013D950 };

itemDef D_8007DFF0 = { 0, 1, 0x14, 0xF6, 4, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80012308 };

itemDef D_8007E004 = { 0, 7, 0x15, 0xF6, 4, 0, 0, 1, 0, 0, 8, 0x10, 8, 0x10, 0x8001232C };

itemDef D_8007E018 = { 0, 8, 0x15, 0xF6, 4, 0, 0, 1, 0, 0, 8, 0x10, 8, 0x10, 0x80012330 };

itemDef D_8007E02C = { 0, 9, 0x15, 0xF6, 4, 0, 0, 1, 0, 0, 8, 0x10, 8, 0x10, 0x80012334 };

itemDef D_8007E040 = { 0, 0, 0x14, 0xF6, 4, 1, 3, 0, 0x160, 0x1FA, 8, 0x10, 8, 0x10, 0x8001230C };

itemDef D_8007E054 = { 0, 1, 0x14, 0xF6, 4, 1, 1, 0, 0x150, 0x1FC, 8, 0x10, 8, 0x10, 0x80012304 };

itemDef D_8007E068 = { 0, 1, 0x14, 0xF6, 4, 1, 1, 0, 0x150, 0x1FD, 8, 0x10, 8, 0x10, 0x80012304 };

itemDef D_8007E07C = { 0, 0xD, 0x14, 0xF6, 4, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80012310 };

itemDef D_8007E090 = { 0, 0, 0xB, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x801340D0 };

itemDef D_8007E0A4 = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80134070 };

itemDef D_8007E0B8 = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80134074 };

itemDef D_8007E0CC = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80134078 };

itemDef D_8007E0E0 = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8013407C };

itemDef D_8007E0F4 = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80134080 };

itemDef D_8007E108 = { 0, 1, 8, 0, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80134088 };

itemDef D_8007E11C = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x150, 0x1F9, 8, 0x10, 8, 0x10, 0x80012384 };

itemDef D_8007E130 = { 0, 1, 8, 0, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8013D96C };

itemDef D_8007E144 = { 0, 1, 9, 0, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x801340A4 };

itemDef D_8007E158 = { 0, 1, 9, 0, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x801340A8 };

itemDef D_8007E16C = { 0, 0, 0xB, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8012D408 };

itemDef D_8007E180 = { 0, 0, 0xB, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8012D3AC };

itemDef D_8007E194 = { 0, 0, 0xB, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8012D3C4 };

itemDef D_8007E1A8 = { 0, 1, 8, 0, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8013DA58 };

itemDef D_8007E1BC = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8012D3C0 };

itemDef D_8007E1D0 = { 0, 1, 0xB, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8012D3C8 };

itemDef D_8007E1E4 = { 0, 1, 6, 0xF6, 0xC, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80138320 };

itemDef D_8007E1F8 = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x801340B8 };

itemDef D_8007E20C = { 0, 1, 8, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80138288 };

itemDef D_8007E220 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x80131094 };

itemDef D_8007E234 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x80131098 };

itemDef D_8007E248 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x8013109C };

itemDef D_8007E25C = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x801310A0 };

itemDef D_8007E270 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x801310A4 };

itemDef D_8007E284 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x801310A8 };

itemDef D_8007E298 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x801310AC };

itemDef D_8007E2AC = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x801310B0 };

itemDef D_8007E2C0 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x801310B4 };

itemDef D_8007E2D4 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E7, 0xC, 0xC, 0xC, 0xC, 0x801310B8 };

itemDef D_8007E2E8 = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x120, 0x1F3, 0xC, 0xC, 0xC, 0xC, 0x800122EC };

itemDef D_8007E2FC = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x130, 0x1F1, 0xC, 0xC, 0xC, 0xC, 0x800122E4 };

itemDef D_8007E310 = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x130, 0x1F0, 0xC, 0xC, 0xC, 0xC, 0x800122E0 };

itemDef D_8007E324 = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x130, 0x1F2, 0xC, 0xC, 0xC, 0xC, 0x800122E8 };

itemDef D_8007E338 = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x130, 0x1F5, 0xC, 0xC, 0xC, 0xC, 0x800122F4 };

itemDef D_8007E34C = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x120, 0x1F4, 0xC, 0xC, 0xC, 0xC, 0x800122F0 };

itemDef D_8007E360 = { 0, 1, 8, 0xF6, 3, 1, 0, 0, 0xF0, 0x1E0, 0xC, 0xC, 0xC, 0xC, 0x8011BE74 };

itemDef D_8007E374 = { 0, 1, 8, 0xF6, 3, 1, 0, 0, 0xF0, 0x1E1, 0xC, 0xC, 0xC, 0xC, 0x8011BE74 };

itemDef D_8007E388 = { 0, 1, 8, 0xF6, 3, 1, 0, 0, 0xF0, 0x1E2, 0xC, 0xC, 0xC, 0xC, 0x8011BE74 };

itemDef D_8007E39C = { 0, 1, 8, 0xF6, 3, 1, 0, 0, 0xF0, 0x1E3, 0xC, 0xC, 0xC, 0xC, 0x8011BE74 };

itemDef D_8007E3B0 = { 0, 1, 8, 0xF6, 3, 1, 0, 0, 0xF0, 0x1E4, 0xC, 0xC, 0xC, 0xC, 0x8011BE74 };

itemDef D_8007E3C4 = { 0, 1, 8, 0xF6, 3, 1, 0, 0, 0xF0, 0x1E5, 0xC, 0xC, 0xC, 0xC, 0x8011BE74 };

itemDef D_8007E3D8 = { 0, 1, 8, 0xF6, 3, 0, 0, 0, 0, 0, 0xC, 0xC, 0xC, 0xC, 0x80134084 };

itemDef D_8007E3EC = { 0, 1, 0xC, 0xF6, 3, 0, 0, 0, 0, 0, 0xC, 0xC, 0xC, 0xC, 0x80131070 };

itemDef D_8007E400 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E2, 0xC, 0xC, 0xC, 0xC, 0x80131088 };

itemDef D_8007E414 = { 0, 1, 0xC, 0xF6, 3, 0, 0, 0, 0, 0, 0xC, 0xC, 0xC, 0xC, 0x8013105C };

itemDef D_8007E428 = { 0, 1, 0xC, 0xF6, 3, 0, 0, 0, 0, 0, 0xC, 0xC, 0xC, 0xC, 0x8013107C };

itemDef D_8007E43C = { 0, 1, 0xD, 0xF6, 3, 0, 0, 0, 0, 0, 0xC, 0xC, 0xC, 0xC, 0x801310C8 };

itemDef D_8007E450 = { 0, 1, 0xC, 0xF6, 3, 0, 0, 0, 0, 0, 0xC, 0xC, 0xC, 0xC, 0x80131080 };

itemDef D_8007E464 = { 0, 1, 0xC, 0xF6, 3, 0, 0, 0, 0, 0, 0xC, 0xC, 0xC, 0xC, 0x80131074 };

itemDef D_8007E478 = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E3, 0xC, 0xC, 0xC, 0xC, 0x801310C4 };

itemDef D_8007E48C = { 0, 1, 0xD, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E6, 0xC, 0xC, 0xC, 0xC, 0x801310CC };

itemDef D_8007E4A0 = { 0, 1, 0xF, 0xF6, 5, 1, 0, 0, 0x130, 0x1E6, 8, 0x10, 8, 0x10, 0x80119484 };

itemDef D_8007E4B4 = { 0, 1, 9, 0xF6, 3, 1, 0, 0, 0xB0, 0x1E4, 8, 0x10, 8, 0x10, 0x801382A0 };

itemDef D_8007E4C8 = { 0, 1, 1, 0xF6, 3, 1, 0, 0, 0xC0, 0x1E3, 8, 0x10, 8, 0x10, 0x8012CF4C };

itemDef D_8007E4DC = { 0, 1, 0xF, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8011787C };

itemDef D_8007E4F0 = { 0, 1, 9, 0xF6, 3, 1, 0, 0, 0x110, 0x1E2, 8, 0x10, 8, 0x10, 0x801382A8 };

itemDef D_8007E504 = { 0, 0, 0x14, 0xF6, 4, 1, 3, 0, 0x160, 0x1FA, 8, 0x10, 8, 0x10, 0x8001230C };

itemDef D_8007E518 = { 0, 1, 9, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x801340AC };

itemDef D_8007E52C = { 0, 1, 9, 0xF6, 3, 1, 0, 0, 0xE0, 0x1E4, 8, 0x10, 8, 0x10, 0x801382A4 };

itemDef D_8007E540 = { 0, 1, 8, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80134098 };

itemDef D_8007E554 = { 0, 1, 0xB, 0xF6, 3, 0, 0, 0, 0, 0, 0xA, 0x14, 8, 0x18, 0x8012D404 };

itemDef D_8007E568 = { 0, 1, 0xC, 0xF6, 0xB, 1, 0, 0, 0xA0, 0x1ED, 8, 0x10, 8, 0x10, 0x8011E4CC };

itemDef D_8007E57C = { 0, 6, 0x14, 0xF6, 4, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x8001231C };

itemDef D_8007E590 = { 0, 1, 0xE, 0xF6, 0xB, 1, 0, 0, 0x130, 0x1ED, 8, 0x10, 8, 0x20, 0x8011B60C };

itemDef D_8007E5A4 = { 0, 1, 8, 0xF6, 3, 1, 0, 0, 0x130, 0x1EA, 8, 0x10, 8, 0x20, 0x8013A430 };

itemDef D_8007E5B8 = { 0, 1, 0xB, 0xF6, 3, 0, 0, 0, 0xF0, 0x1E9, 8, 0x10, 8, 0x20, 0x8013D8BC };

itemDef D_8007E5CC = { 0, 1, 8, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x801340BC };

itemDef D_8007E5E0 = { 0, 1, 0x14, 0xF6, 4, 1, 0, 0, 0x120, 0x1F4, 8, 0x10, 8, 0x10, 0x800122F0 };

itemDef D_8007E5F4 = { 0, 1, 9, 0xF6, 3, 1, 0, 0, 0x110, 0x1E2, 8, 0x10, 8, 0x10, 0x801382A8 };

itemDef D_8007E608 = { 0, 1, 7, 0xF6, 3, 0, 0, 0, 0, 0, 8, 0x10, 8, 0x10, 0x80119C84 };

u_char D_8007E61C[0xC8] = {
    [ITEM_CHICK]               = 0,
    [ITEM_FROG]                = 1,
    [ITEM_LOSTDWARF]           = 2,
    [ITEM_BANANAS]             = 3,
    [ITEM_FURIOUSTORNADO]      = 4,
    [ITEM_100YEAROLDBELL]      = 5,
    [ITEM_100YEAROLDKEY]       = 6,
    [ITEM_CHARITYWINGS]        = 7,
    [ITEM_BITINGPLANTFLOWER]   = 8,
    [ITEM_HEALINGMUSHROOM]     = 9,
    [ITEM_BUCKET]              = 0xA,
    [ITEM_TELESCOPE]           = 0,
    [ITEM_TEARJAR]             = 0xB,
    [ITEM_FLOWERTEARS]         = 0xC,
    [ITEM_BARON]               = 0xD,
    [ITEM_BAKEDYAM]            = 0xE,
    [ITEM_LEAFBUTTERFLY]       = 0xF,
    [ITEM_TORCH]               = 0,
    [ITEM_BUCKETOFWATER]       = 0x10,
    [ITEM_DIRTYMIRROR]         = 0x11,
    [ITEM_FUNKYPARASOL]        = 0x12,
    [ITEM_WOODBOOMERANG]       = 0x13,
    [ITEM_STONEBOOMERANG]      = 0x14,
    [ITEM_IRONBOOMERANG]       = 0x15,
    [ITEM_DASHINGPANTS]        = 0,
    [ITEM_MAP]                 = 0,
    [ITEM_BROKENVASE]          = 0,
    [ITEM_BLACKJACK]           = 0,
    [ITEM_FLASHPANTS]          = 0x53,
    [ITEM_JUMPINGPANTS]        = 0x16,
    [ITEM_LUNCHBOX]            = 0x17,
    [ITEM_LARGELUNCHBOX]       = 0x18,
    [ITEM_NORMALPANTS]         = 0,
    [ITEM_GRAPPLE]             = 0x19,
    [ITEM_GRAPPLEJACK]         = 0,
    [ITEM_BABYPIG]             = 0,
    [ITEM_1000YEAROLDKEY]      = 0x1A,
    [ITEM_REDPIGBAG]           = 0,
    [ITEM_ORANGEPIGBAG]        = 0x4E,
    [ITEM_YELLOWPIGBAG]        = 0x46,
    [ITEM_GREENPIGBAG]         = 0x2B,
    [ITEM_BLUEEVILPIGBAG]      = 0,
    [ITEM_NAVYPIGBAG]          = 0x45,
    [ITEM_PINKPIGBAG]          = 0x44,
    [ITEM_10000YEAROLDKEY]     = 0,
    [ITEM_1000000YEAROLDKEY]   = 0,
    [ITEM_LARGEKEYPANEL_1]     = 0x1B,
    [ITEM_LARGEKEYPANEL_2]     = 0x1C,
    [ITEM_LARGEKEYPANEL_3]     = 0x1D,
    [ITEM_LARGEKEYPANEL_4]     = 0x1E,
    [ITEM_LARGEKEYPANEL_5]     = 0x1F,
    [ITEM_FUELBARREL]          = 0x5A,
    [ITEM_RAINESSENCE]         = 0x20,
    [ITEM_BIGKEY]              = 0,
    [ITEM_SMALLKEY]            = 0x2C,
    [ITEM_CHEESE]              = 0x21,
    [ITEM_MAGICMIRROR]         = 0,
    [ITEM_TORNMAP1]            = 0,
    [ITEM_TORNMAP2]            = 0,
    [ITEM_RUBBERGLOVES]        = 0,
    [ITEM_BOMB]                = 0x38,
    [ITEM_IRONHAMMER]          = 0,
    [ITEM_IRONWHEEL]           = 0,
    [ITEM_FLOWERSEEDS]         = 0x22,
    [ITEM_PIPE]                = 0x51,
    [ITEM_WINE]                = 0,
    [ITEM_BUNKFLOWER]          = 0x2D,
    [ITEM_MATHBEADD1]          = 0x2E,
    [ITEM_MATHBEADD2]          = 0x2F,
    [ITEM_MATHBEADD3]          = 0x30,
    [ITEM_MATHBEADD4]          = 0x31,
    [ITEM_MATHBEADD5]          = 0x32,
    [ITEM_MATHBEADD6]          = 0x33,
    [ITEM_MATHBEADD7]          = 0x34,
    [ITEM_MATHBEADD8]          = 0x35,
    [ITEM_MATHBEADD9]          = 0x36,
    [ITEM_MATHBEADD10]         = 0x37,
    [ITEM_MATHBEADD11]         = 0x59,
    [ITEM_MATHBEADD12]         = 0,
    [ITEM_MATHBEADD13]         = 0,
    [ITEM_MATHBEADD14]         = 0,
    [ITEM_MATHBEADD15]         = 0,
    [ITEM_CRYSTAL]             = 0,
    [ITEM_BELL]                = 0,
    [ITEM_CAKE]                = 0,
    [ITEM_HAT]                 = 0,
    [ITEM_SHIPPARTS]           = 0,
    [ITEM_BRONZEMEDAL]         = 0,
    [ITEM_SILVERMEDAL]         = 0,
    [ITEM_GOLDMEDAL]           = 0,
    [ITEM_LETTER]              = 0,
    [ITEM_WOOD]                = 0x4B,
    [ITEM_RAFT]                = 0,
    [ITEM_GOLDENLEAFBUTTERFLY] = 0,
    [ITEM_GOLDENFRUIT]         = 0,
    [ITEM_GOLDENFLOWER]        = 0x58,
    [ITEM_PSYCHICFISH]         = 0,
    [ITEM_SHOVEL]              = 0,
    [ITEM_JEWELOFFIRE]         = 0x54,
    [ITEM_JEWELOFWATER]        = 0x4C,
    [ITEM_JEWELOFWIND]         = 0x55,
    [ITEM_MIGHTYFISH]          = 0,
    [ITEM_SILVERPOWDER]        = 0x23,
    [ITEM_MOLASSES]            = 0x24,
    [ITEM_KOKKACLAW]           = 0x39,
    [ITEM_BUTAMUSHITHORN]      = 0x3A,
    [ITEM_NEEDLEGATORTEETH]    = 0x3B,
    [ITEM_FLOWER]              = 0,
    [ITEM_ELECTRIC_EEL]        = 0,
    [ITEM_BLACKWATER]          = 0,
    [ITEM_REDCANDY]            = 0x3E,
    [ITEM_BLUECANDY]           = 0x3F,
    [ITEM_GREENCANDY]          = 0x40,
    [ITEM_BLACKCANDY]          = 0x41,
    [ITEM_SILVERCANDY]         = 0x42,
    [ITEM_GOLDENCANDY]         = 0x43,
    [ITEM_FORBIDDENMUSHROOM]   = 0x29,
    [ITEM_BLUEPOWDER]          = 0x25,
    [ITEM_COCONUTS]            = 0,
    [ITEM_FUNGALEATHER]        = 0,
    [ITEM_GRANDPASBRACELET]    = 0x3C,
    [ITEM_WEEDKILLER]          = 0,
    [ITEM_FUNGATREE]           = 0x49,
    [ITEM_FUNGASAP]            = 0x48,
    [ITEM_1000YEAROLDBELL]     = 0x26,
    [ITEM_FUNGADRUM]           = 0x5E,
    [ITEM_MIGHTYFISHFOOD]      = 0x56,
    [ITEM_UNUSUALKEY]          = 0x2C,
    [ITEM_CHUCKLINGMUSHROOM]   = 0,
    [ITEM_WEEPINGMUSHROOM]     = 0,
    [ITEM_MYSTERIOUSMUSHROOM]  = 0x27,
    [ITEM_OVENBAKEDMUSHROOM]   = 0,
    [ITEM_SACREDFISH]          = 0x60,
    [ITEM_CHICK2]              = 0,
    [ITEM_CHICK3]              = 0,
    [ITEM_GOLDENBOWL]          = 0,
    [ITEM_FLOWERTEARS2]        = 0,
    [ITEM_SMALLDRUM]           = 0x3D,
    [ITEM_RISEANDSHINEPOWDER]  = 0x57,
    [ITEM_BANANAJUICE]         = 0,
    [ITEM_CHIEFSPEAR]          = 0x4A,
    [ITEM_CHARLESPANTS]        = 0,
    [ITEM_THREECRYSTALBALLS]   = 0x28,
    [ITEM_WHATTHETHIEFLOST]    = 0x52,
    [ITEM_WHATTHETHIEFFORGOT]  = 0x5F,
    [ITEM_BOSSJEWEL]           = 0x5D,
    [ITEM_ORDINARYMUSHROOM]    = 0x2A,
    [ITEM_TRASHCAN]            = 0,
    [ITEM_SEASHELLNECKLACE]    = 0x4D,
    [ITEM_THIEFSWIRE]          = 0,
    [ITEM_STRONGWIRE]          = 0,
    [ITEM_10000YEAROLDBELL]    = 0x5B,
    [ITEM_1000000YEAROLDBELL]  = 0x5C,
    [ITEM_COLDMEDICINE]        = 0,
    [ITEM_YANSLUNCHBOX]        = 0,
    [ITEM_KEYTOOLPOND]         = 0,
    [ITEM_HEALINGHERBS]        = 0x4F,
    [ITEM_KNOWLEDGEFRUIT]      = 0x47,
    [ITEM_SEAWEED]             = 0x50,
    [ITEM_MINERSHAT]           = 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int D_8007E6E4[97] = {
    (int)&D_8007DE88, (int)&D_8007DE9C, (int)&D_8007DEB0, (int)&D_8007DEC4,
    (int)&D_8007DED8, (int)&D_8007DEEC, (int)&D_8007DF00, (int)&D_8007DF14,
    (int)&D_8007DF28, (int)&D_8007DF3C, (int)&D_8007DF50, (int)&D_8007DF64,
    (int)&D_8007DF78, (int)&D_8007DF8C, (int)&D_8007DFA0, (int)&D_8007DFB4,
    (int)&D_8007DFC8, (int)&D_8007DFDC, (int)&D_8007DFF0, (int)&D_8007E004,
    (int)&D_8007E018, (int)&D_8007E02C, (int)&D_8007E040, (int)&D_8007E054,
    (int)&D_8007E068, (int)&D_8007E07C, (int)&D_8007E090, (int)&D_8007E0A4,
    (int)&D_8007E0B8, (int)&D_8007E0CC, (int)&D_8007E0E0, (int)&D_8007E0F4,
    (int)&D_8007E108, (int)&D_8007E11C, (int)&D_8007E130, (int)&D_8007E144,
    (int)&D_8007E158, (int)&D_8007E16C, (int)&D_8007E180, (int)&D_8007E194,
    (int)&D_8007E1A8, (int)&D_8007E1BC, (int)&D_8007E1D0, (int)&D_8007E1E4,
    (int)&D_8007E1F8, (int)&D_8007E20C, (int)&D_8007E220, (int)&D_8007E234,
    (int)&D_8007E248, (int)&D_8007E25C, (int)&D_8007E270, (int)&D_8007E284,
    (int)&D_8007E298, (int)&D_8007E2AC, (int)&D_8007E2C0, (int)&D_8007E2D4,
    (int)&D_8007E2E8, (int)&D_8007E2FC, (int)&D_8007E310, (int)&D_8007E324,
    (int)&D_8007E338, (int)&D_8007E34C, (int)&D_8007E360, (int)&D_8007E374,
    (int)&D_8007E388, (int)&D_8007E39C, (int)&D_8007E3B0, (int)&D_8007E3C4,
    (int)&D_8007E3D8, (int)&D_8007E3EC, (int)&D_8007E400, (int)&D_8007E414,
    (int)&D_8007E428, (int)&D_8007E43C, (int)&D_8007E450, (int)&D_8007E464,
    (int)&D_8007E478, (int)&D_8007E48C, (int)&D_8007E4A0, (int)&D_8007E4B4,
    (int)&D_8007E4C8, (int)&D_8007E4DC, (int)&D_8007E4F0, (int)&D_8007E504,
    (int)&D_8007E518, (int)&D_8007E52C, (int)&D_8007E540, (int)&D_8007E554,
    (int)&D_8007E568, (int)&D_8007E57C, (int)&D_8007E590, (int)&D_8007E5A4,
    (int)&D_8007E5B8, (int)&D_8007E5CC, (int)&D_8007E5E0, (int)&D_8007E5F4,
    (int)&D_8007E608
};

typedef struct {
    int unk0;
    s16 unk4;
    s16 unk6;
} unk_8007E868;

unk_8007E868 D_8007E868_data[8] asm("D_8007E868") = {
    { 500, 0x150, 0x1E3 },
    { 1000, 0x150, 0x1E0 },
    { 2000, 0x150, 0x1E1 },
    { 5000, 0x150, 0x1E2 },
    { 10000, 0x150, 0x1E3 },
    { 20000, 0x150, 0x1E0 },
    { 100000, 0x150, 0x1E1 },
    { 500000, 0x150, 0x1E2 }
};

asm(".globl D_8007E86C\nD_8007E86C = D_8007E868 + 4");
asm(".globl D_8007E86E\nD_8007E86E = D_8007E868 + 6");

void func_8003F3D4(u8* self);

void func_8003F78C(u8* self);

void func_8003F9A4(u8* self)
{
    u8* obj;
    int sfx;

    switch (self[4]) {
    case 0:
        self[0] = 2;
        self[0xA] = 2;
        self[0xA5] = 1;
        self[0xD] = 0;
        *(int*)(self + 0x8C) = 0;
        *(s16*)(self + 0x6C) = 10;
        *(s16*)(self + 0x6E) = 0x14;
        *(s16*)(self + 0x70) = 0x10;
        *(s16*)(self + 0x72) = 0x20;
        self[4]++;
        *(int*)(self + 0x3C) = *(int*)0x1F8002D4;
        if (GAME.selectedArea == 0) {
            *(s16*)(self + 0x1E) = 10;
            *(int*)(self + 0x24) = D_8013B104;
        } else {
            *(s16*)(self + 0x1E) = 8;
            *(int*)(self + 0x24) = D_80134D0C;
        }
        readAnimFrameCount(self);
        func_80022E44(self);
        break;
    case 1:
        if (func_80022E44(self) != 0) {
            func_8003F78C(self);
        }
        break;
    case 2:
        if (func_80022E44(self) != 0) {
            if (GAME.selectedArea == 4) {
                D_8009CEFB++;
                func_80126760(self[3]);
                if (D_8009BCCA < 4) {
                    sfx = 0xF7;
                } else {
                    sfx = 0xF9;
                }
            } else {
                sfx = 0x34;
            }
            playSFX(sfx);
            func_800E98A4(self, *(s16*)(self + 0x12), *(s16*)(self + 0x16), *(s16*)(self + 0x1A));
            if (GAME.selectedArea == 0) {
                D_8009D2AE |= 1 << self[3];
                obj = allocObjectLayer2();
                if (obj != NULL) {
                    obj[0] = 1;
                    obj[2] = 3;
                    obj[3] = self[3];
                    obj[0xC] = self[0xC];
                    *(VEC3*)(obj + 0x10) = *(VEC3*)(self + 0x10);
                    obj[0x6B] = self[0x6B];
                    obj[0x1D] = self[0x1D];
                    *(s16*)(obj + 0x7A) = 0;
                    obj[4] = 0;
                    obj[5] = 0;
                    obj[6] = 0;
                }
            }
            self[4] = 3;
        }
        break;
    case 3:
        freeObjectLayer1(self);
        break;
    }
}

void applyItemEffect(GameObject* arg0, int arg1, short arg2, short arg3, int arg4);

void initItemObject(GameObject* arg0);

void rewardNone(GameObject* arg0);

void rewardItem(GameObject* arg0);

void rewardHeart(GameObject* arg0);

void rewardEffectOnly(GameObject* arg0);

void rewardBakedYam(GameObject* arg0);

void rewardDirtyMirror(GameObject* arg0);

void rewardVitalityMaxUp(GameObject* arg0);

void rewardWoodBoomerang(GameObject* arg0);

void rewardStoneBoomerang(GameObject* arg0);

void rewardIronBoomerang(GameObject* arg0);

void rewardOneUp(GameObject* arg0);

void rewardGoldenBowl(GameObject* arg0);

void rewardBitingPlantFlower(GameObject* arg0);

void rewardGrapple(GameObject* arg0);

void rewardGrappleJack(GameObject* arg0);

void rewardCrystalBalls(GameObject* arg0);

void rewardMysteriousMushroom(GameObject* arg0);

void rewardFlowerSeeds(GameObject* arg0);

void rewardPigBag(GameObject* arg0);

void rewardConditionalItem(GameObject* arg0);

void rewardJewel(GameObject* arg0);

void rewardSafeMushroom(GameObject* arg0);

void rewardAnimalDash(GameObject* arg0);

void rewardPants(GameObject* arg0);
