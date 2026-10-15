// DATA 800794d4 2180 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80012024[];
extern char D_80012028[];
extern void FUN_8002a3d4();
extern void FUN_8002a4d0();
extern void dispatchAreaDialogInit();
extern void dispatchMsgBoxHandler();
extern void dispatchSectionClose();
extern void dispatchSectionConfirm();
extern void dispatchSectionDraw();
extern void dispatchSectionInit();
extern void dispatchSectionInput();
extern void dispatchSectionScroll();
extern void dispatchSectionSelect();
extern void dispatchSectionUpdate();
extern void func_80027810();
extern void func_80029F1C();
extern void func_8002A0FC();
extern void func_8002A258();
extern void func_8002B018();
extern void func_8002B0B4();
extern void func_8002B0BC();
extern void func_8002B8C4();
extern void func_8002BE80();
extern void func_8002C43C();
extern void func_8002C73C();
extern void func_8002C920();
extern void func_8002CA88();
extern void func_8002E4B4();
extern void func_8002E6C4();
extern void func_8002EB34();
extern void func_8002FE78();
extern void updateCameraFollow();
extern void updateInfoMessage();
extern void updateLookScroll();
extern unsigned int data_800794D4[];
extern unsigned int data_800794E0[];
extern unsigned int data_800794EC[];
extern unsigned int data_800794F8[];
extern unsigned int data_80079504[];
extern unsigned int data_80079530[];
extern unsigned int data_80079538[];
extern unsigned int data_80079540[];
extern unsigned int data_8007954C[];
extern unsigned int data_80079554[];
extern unsigned int data_80079564[];
extern unsigned int data_80079578[];
extern unsigned int data_80079584[];
extern unsigned int data_800795B0[];
extern unsigned int data_800795B4[];
extern unsigned int data_800795C4[];
extern unsigned int data_800795D4[];
extern unsigned int data_800795E4[];
extern unsigned int data_800795FC[];
extern unsigned int data_80079604[];
extern void *ptrs_8007960C[];
extern unsigned int data_8007965C[];
extern unsigned int data_80079660[];
extern unsigned int data_80079664[];
extern unsigned int data_80079668[];
extern unsigned int data_8007977C[];
extern unsigned int data_80079784[];
extern unsigned int data_8007978C[];
extern unsigned int data_800797B4[];
extern unsigned int data_800797CC[];
extern unsigned int data_800797DC[];
extern unsigned int data_800797EC[];
extern unsigned int data_800797F4[];
extern unsigned int data_8007981C[];
extern unsigned int data_80079824[];
extern void *ptrs_8007983C[];
extern unsigned int data_80079854[];
extern unsigned int data_8007986C[];
extern void *ptrs_8007987C[];
extern unsigned int data_8007988C[];
extern unsigned int data_8007989C[];
extern void *ptrs_800798A4[];
extern unsigned int data_800798B4[];
extern unsigned int data_800798C4[];
extern unsigned int data_800798D4[];
extern void *ptrs_800798DC[];
extern unsigned int data_800798F0[];
extern unsigned int data_80079908[];
extern unsigned int data_80079918[];
extern unsigned int data_80079928[];
extern void *ptrs_80079930[];
extern unsigned int data_8007994C[];
extern unsigned int data_8007995C[];
extern unsigned int data_8007996C[];
extern unsigned int data_8007998C[];
extern unsigned int data_80079994[];
extern unsigned int data_8007999C[];
extern void *ptrs_800799A4[];
extern unsigned int data_800799D0[];
extern unsigned int data_800799E0[];
extern unsigned int data_80079A00[];
extern void *ptrs_80079A08[];
extern unsigned int data_80079A14[];
extern unsigned int data_80079A24[];
extern unsigned int data_80079A34[];
extern void *ptrs_80079A3C[];
extern unsigned int data_80079A48[];
extern unsigned int data_80079AEC[];
extern unsigned int data_80079B3C[];
extern unsigned int data_80079B88[];
extern unsigned int data_80079BAC[];
extern void *ptrs_80079D34[];
extern unsigned int data_80079D3C[];
extern void *ptrs_80079D44[];
extern unsigned int data_80079D50[];
extern unsigned int data_80079D54[];

unsigned int data_800794D4[3] = {
    0x1000000, 0x3000300, 0x2000000,
};

unsigned int data_800794E0[3] = {
    0x10001, 0x3010201, 0x401,
};

unsigned int data_800794EC[3] = {
    0x1010101, 0x5010101, 0x7010601,
};

unsigned int data_800794F8[3] = {
    0x30003, 0x2030103, 0x30003,
};

unsigned int data_80079504[11] = {
    0x1050005, 0x3050205, 0x50005, 0x50005,
    0x50005, 0x50005, 0x50005, 0x4050005,
    0x50005, 0x50005, 0x50005,
};

unsigned int data_80079530[2] = {
    0x40004, 0x40004,
};

unsigned int data_80079538[2] = {
    0xC000A, 0xC000C,
};

unsigned int data_80079540[3] = {
    0x10001, 0x3010201, 0x401,
};

unsigned int data_8007954C[2] = {
    0x40004, 0x40004,
};

unsigned int data_80079554[4] = {
    0x2, 0x0, 0x0, 0x2,
};

unsigned int data_80079564[5] = {
    0x60006, 0x80106, 0x60006, 0x80106,
    0x108,
};

unsigned int data_80079578[3] = {
    0x70007, 0x70007, 0x70007,
};

unsigned int data_80079584[11] = {
    0x1050005, 0x3050205, 0x50005, 0x50005,
    0x50005, 0x50005, 0x50005, 0x4050005,
    0x50005, 0x50005, 0x50005,
};

unsigned int data_800795B0[1] = {
    0xD,
};

unsigned int data_800795B4[4] = {
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_800795C4[4] = {
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_800795D4[4] = {
    0x70007, 0x70007, 0x70007, 0x70007,
};

unsigned int data_800795E4[6] = {
    0x70007, 0x70007, 0x70007, 0x70007,
    0x70007, 0x70007,
};

unsigned int data_800795FC[2] = {
    0x70007, 0x9,
};

unsigned int data_80079604[2] = {
    0x10001, 0xB,
};

void *ptrs_8007960C[20] = {
    data_800794D4,
    data_800794E0,
    data_800794EC,
    data_800794F8,
    data_80079504,
    data_80079530,
    data_80079538,
    data_80079540,
    data_8007954C,
    data_80079554,
    data_80079564,
    data_80079578,
    data_80079584,
    data_800795B0,
    data_800795B4,
    data_800795C4,
    data_800795D4,
    data_800795E4,
    data_800795FC,
    data_80079604,
};

unsigned int data_8007965C[1] = {
    0x3900000F,
};

unsigned int data_80079660[1] = {
    0x39000034,
};

unsigned int data_80079664[1] = {
    0x3900000C,
};

unsigned int data_80079668[69] = {
    0x3900000C, 0x39000E57, 0x39002E97, 0x3900000C,
    0x3900000C, 0x39000C77, 0x39000666, 0x3900000C,
    0x3900000C, 0x390041C0, 0x3930F3FF, 0x390041BF,
    0x390052E1, 0x3930F3FF, 0x3900278A, 0x39005CE7,
    0x39002586, 0x39001D76, 0x39000015, 0x39001B73,
    0x39000012, 0x3900001A, 0x3900002A, 0x39000666,
    0x39000B75, 0x3900004A, 0x3900000C, 0x39002D95,
    0x39000C55, 0x3900000C, 0x3900000C, 0x39000033,
    0x3900000F, 0x390041C0, 0x3930F3FF, 0x390040BC,
    0x39004FDC, 0x3930F3FF, 0x39002586, 0x390059E7,
    0x39002280, 0x39001B73, 0x39000014, 0x3900186E,
    0x3900000F, 0x39000018, 0x39000028, 0x39000769,
    0x39000C77, 0x39000047, 0x3900000C, 0x39002E97,
    0x39000E58, 0x3900000C, 0x3900000C, 0x39000034,
    0x39000011, 0x390044C6, 0x3930F3FF, 0x39004CD6,
    0x390053E3, 0x3930F3FF, 0x3900278B, 0x39005DE7,
    0x39003CB5, 0x39001D77, 0x3900001A, 0x39002E9A,
    0x3900105E,
};

unsigned int data_8007977C[2] = {
    0x640062, 0x63,
};

unsigned int data_80079784[2] = {
    0x30001, 0x2,
};

unsigned int data_8007978C[10] = {
    0x7530, 0xC350, 0x186A0, 0x30D40,
    0x7A120, 0xF4240, 0x1E8480, 0x4C4B40,
    0x989680, 0xFFFFFFFF,
};

unsigned int data_800797B4[6] = {
    0x63036300, 0x63086307, 0x63106309, 0x631F631E,
    0x63426333, 0xFFFF,
};

unsigned int data_800797CC[4] = {
    0x0, 0x2010102, 0x0, 0x10202,
};

unsigned int data_800797DC[4] = {
    0x0, 0x1000202, 0x0, 0x20202,
};

unsigned int data_800797EC[2] = {
    0x0, 0x1010202,
};

unsigned int data_800797F4[10] = {
    0x0, 0x40202, 0x0, 0x2050202,
    0x0, 0x2010002, 0x0, 0x4030102,
    0x0, 0x10B0402,
};

unsigned int data_8007981C[2] = {
    0x0, 0x3030202,
};

unsigned int data_80079824[6] = {
    0x0, 0x4030202, 0x0, 0x4030202,
    0x0, 0x70E02,
};

void *ptrs_8007983C[6] = {
    data_800797CC,
    data_800797DC,
    data_800797EC,
    data_800797F4,
    data_8007981C,
    data_80079824,
};

unsigned int data_80079854[6] = {
    0x0, 0x1000302, 0x0, 0x7020402,
    0x0, 0x10502,
};

unsigned int data_8007986C[4] = {
    0x0, 0x3000502, 0x0, 0x3000502,
};

void *ptrs_8007987C[4] = {
    data_80079854,
    data_8007986C,
    data_80079854,
    data_8007986C,
};

unsigned int data_8007988C[4] = {
    0x0, 0x10402, 0x0, 0x10802,
};

unsigned int data_8007989C[2] = {
    0x0, 0x1000802,
};

void *ptrs_800798A4[4] = {
    data_8007988C,
    data_8007989C,
    data_8007988C,
    data_8007989C,
};

unsigned int data_800798B4[4] = {
    0x0, 0x1011202, 0x0, 0x10B02,
};

unsigned int data_800798C4[4] = {
    0x0, 0x1011202, 0x0, 0x20B02,
};

unsigned int data_800798D4[2] = {
    0x0, 0x1010B02,
};

void *ptrs_800798DC[5] = {
    data_800798B4,
    data_800798C4,
    data_800798D4,
    data_800798C4,
    data_800798D4,
};

unsigned int data_800798F0[6] = {
    0x0, 0x3011202, 0x0, 0x20B02,
    0x0, 0x11002,
};

unsigned int data_80079908[4] = {
    0x0, 0x51002, 0x0, 0x3011202,
};

unsigned int data_80079918[4] = {
    0x0, 0x1041002, 0x0, 0x61002,
};

unsigned int data_80079928[2] = {
    0x0, 0x1051002,
};

void *ptrs_80079930[7] = {
    data_800798F0,
    data_80079908,
    data_80079918,
    data_80079928,
    data_80079908,
    data_80079918,
    data_80079928,
};

unsigned int data_8007994C[4] = {
    0x0, 0x2011202, 0x0, 0x11102,
};

unsigned int data_8007995C[4] = {
    0x0, 0x2011202, 0x0, 0x21102,
};

unsigned int data_8007996C[8] = {
    0x0, 0x1011102, 0x0, 0x31102,
    0x0, 0x51102, 0x0, 0x41102,
};

unsigned int data_8007998C[2] = {
    0x0, 0x1021102,
};

unsigned int data_80079994[2] = {
    0x0, 0x3021102,
};

unsigned int data_8007999C[2] = {
    0x0, 0x2021102,
};

void *ptrs_800799A4[11] = {
    data_8007994C,
    data_8007995C,
    data_8007996C,
    data_8007998C,
    data_80079994,
    data_8007999C,
    data_8007995C,
    data_8007996C,
    data_8007998C,
    data_80079994,
    data_8007999C,
};

unsigned int data_800799D0[4] = {
    0x0, 0x2050102, 0x0, 0x10302,
};

unsigned int data_800799E0[8] = {
    0x0, 0x10B02, 0x0, 0x11102,
    0x0, 0x41002, 0x0, 0x2000A02,
};

unsigned int data_80079A00[2] = {
    0x0, 0x3000002,
};

void *ptrs_80079A08[3] = {
    data_800799D0,
    data_800799E0,
    data_80079A00,
};

unsigned int data_80079A14[4] = {
    0x0, 0x2010102, 0x0, 0x11302,
};

unsigned int data_80079A24[4] = {
    0x0, 0x1001302, 0x0, 0x20202,
};

unsigned int data_80079A34[2] = {
    0x0, 0x3020302,
};

void *ptrs_80079A3C[3] = {
    data_80079A14,
    data_80079A24,
    data_80079A34,
};

unsigned int data_80079A48[41] = {
    0x801384A8, 0x8013C234, (unsigned int)ptrs_8007983C, 0x80134FC4,
    0x801305DC, (unsigned int)ptrs_8007987C, 0x8011EFC4, 0x8013C234,
    (unsigned int)ptrs_800798A4, 0x8012A83C, 0x8012F064, (unsigned int)ptrs_800798DC,
    0x801305DC, 0x80118100, 0x80125664, 0x80125664,
    (unsigned int)ptrs_80079930, (unsigned int)ptrs_800799A4, (unsigned int)ptrs_80079A08, (unsigned int)ptrs_80079A3C,
    (unsigned int)updateCameraFollow, (unsigned int)FUN_8002a4d0, 0x80116934, (unsigned int)func_80029F1C,
    (unsigned int)func_8002A0FC, (unsigned int)updateLookScroll, (unsigned int)func_8002A258, (unsigned int)FUN_8002a3d4,
    (unsigned int)FUN_8002a4d0, 0x80116934, (unsigned int)func_80029F1C, (unsigned int)func_8002A0FC,
    (unsigned int)updateLookScroll, (unsigned int)func_8002A258, (unsigned int)func_80027810, (unsigned int)FUN_8002a4d0,
    0x80116934, (unsigned int)func_80029F1C, (unsigned int)func_8002A0FC, (unsigned int)updateLookScroll,
    (unsigned int)func_8002A258,
};

unsigned int data_80079AEC[20] = {
    0x801175A4, 0x80117490, (unsigned int)dispatchSectionInit, 0x80117DC4,
    0x80118700, (unsigned int)dispatchSectionUpdate, 0x80116764, 0x80117490,
    (unsigned int)dispatchSectionDraw, 0x80116DD0, 0x80116AE8, (unsigned int)dispatchSectionInput,
    0x80118700, 0x801162D0, 0x801168A0, (unsigned int)updateCameraFollow,
    (unsigned int)dispatchSectionScroll, (unsigned int)dispatchSectionSelect, (unsigned int)dispatchSectionClose, (unsigned int)dispatchSectionConfirm,
};

unsigned int data_80079B3C[19] = {
    0x801176D8, 0x801174B0, 0x800E88E4, 0x80117DE4,
    0x80118BFC, (unsigned int)func_8002B018, 0x801167A0, 0x801174B0,
    (unsigned int)func_8002B018, 0x80116E4C, 0x80116B08, (unsigned int)func_8002B018,
    0x80118BFC, 0x801166E0, 0x80116A10, (unsigned int)func_8002B018,
    (unsigned int)func_8002B018, (unsigned int)func_8002B018, 0x8011657C,
};

unsigned int data_80079B88[9] = {
    0x800E9DC0, (unsigned int)func_8002B0BC, 0x8011759C, 0x80119114,
    0x80117BD4, 0x801319C8, 0x80117DE0, (unsigned int)func_8002B0B4,
    0x80117364,
};

unsigned int data_80079BAC[98] = {
    0x800E9E98, 0x80118438, 0x80118BC0, 0x80118F90,
    0x800EA168, 0x800EA284, 0x8011926C, 0x8011935C,
    0x80119468, 0x801195F4, 0x8011A63C, 0x8011A744,
    0x800EA67C, 0x8002B9B0, 0x80118724, (unsigned int)func_8002BE80,
    0x800EB0C8, 0x8011AB10, (unsigned int)func_8002C43C, (unsigned int)dispatchMsgBoxHandler,
    0x8011AD98, 0x80116D1C, 0x8011B0D4, 0x8011B1E0,
    0x80118AC0, 0x80118C1C, 0x8002CF40, 0x800EB394,
    (unsigned int)updateInfoMessage, (unsigned int)dispatchAreaDialogInit, 0x800EB6AC, 0x80118DA8,
    (unsigned int)func_8002E4B4, 0x80132760, (unsigned int)func_8002E6C4, 0x80117114,
    0x8011B2E8, 0x801172DC, 0x80118838, 0x801191B0,
    0x80118CF4, 0x80118F3C, 0x8011AA3C, 0x8011ABA0,
    0x801165DC, 0x80119268, 0x800E8420, 0x801287F8,
    0x80115FE0, 0x8002E83C, (unsigned int)func_8002EB34, 0x8011976C,
    0x801198DC, 0x8011910C, 0x801196F8, 0x80119BAC,
    0x8011D07C, 0x80119838, 0x800E94AC, 0x80117148,
    0x8011AD24, 0x801193E4, 0x8011992C, 0x801180B4,
    0x8011A4A8, 0x8011AE9C, 0x8011A7E0, 0x8012FBC4,
    0x800EB9F0, 0x8011617C, 0x801162D0, 0x80116D18,
    0x8012B6E8, 0x80118188, 0x80117468, 0x801167BC,
    0x801169C0, 0x801165A8, (unsigned int)func_8002FE78, 0x80117490,
    0x801175A0, 0x80117710, 0x801177C4, 0x80119874,
    0x80119BF4, 0x801176B0, 0x80117DDC, 0x80119EEC,
    0x801166F8, 0x8011B6E0, 0x8011B7F0, 0x800EBB48,
    0x800EC420, (unsigned int)func_8002B8C4, 0x801166F0, 0x800EC9C0,
    0x80116EC0, 0x80118760,
};

void *ptrs_80079D34[2] = {
    D_80012028,
    D_80012024,
};

unsigned int data_80079D3C[2] = {
    0x5080507, 0x5,
};

void *ptrs_80079D44[3] = {
    func_8002C73C,
    func_8002C920,
    func_8002CA88,
};

unsigned int data_80079D50[1] = {
    0x13,
};

unsigned int data_80079D54[1] = {
    0x4,
};
