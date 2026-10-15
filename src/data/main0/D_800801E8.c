// DATA 800801e8 1228 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void dispatchUnlayeredByVariant();
extern void updateSectionFadeController();
extern void updateSectionSpawnManager();
extern unsigned int data_8007B958[];
extern unsigned int data_8007B970[];
extern unsigned int data_8007B97C[];
extern unsigned int data_8007B98C[];
extern unsigned int data_8007B9A0[];
extern unsigned int data_8007B9B0[];
extern unsigned int data_8007B9C4[];
extern unsigned int data_8007B9D4[];
extern unsigned int data_8007B9E4[];
extern unsigned int data_8007B9F0[];
extern unsigned int data_8007BA00[];
extern unsigned int data_8007BA10[];
extern unsigned int data_8007BA20[];
extern unsigned int data_8007BA48[];
extern unsigned int data_8007BA6C[];
extern unsigned int data_8007BA84[];
extern unsigned int data_8007BAD0[];
extern unsigned int data_8007BAE0[];
extern unsigned int data_8007BAFC[];
extern unsigned int data_8007BB0C[];
extern unsigned int data_8007BB30[];
extern unsigned int data_8007BB4C[];
extern unsigned int data_8007BB5C[];
extern unsigned int data_8007BB6C[];
extern unsigned int data_8007BB7C[];
extern unsigned int data_8007BB8C[];
extern unsigned int data_8007BB9C[];
extern unsigned int data_8007BBAC[];
extern unsigned int data_8007BBCC[];
extern unsigned int data_8007BBDC[];
extern unsigned int data_8007BBEC[];
extern unsigned int data_8007BBFC[];
extern unsigned int data_8007BC0C[];
extern unsigned int data_8007BC1C[];
extern unsigned int data_8007BC2C[];
extern unsigned int data_8007BC48[];
extern unsigned int data_8007BC58[];
extern unsigned int data_8007BC68[];
extern unsigned int data_8007BC78[];
extern unsigned int data_8007BC88[];
extern unsigned int data_8007BC98[];
extern unsigned int data_8007BCA8[];
extern unsigned int data_8007BCB8[];
extern unsigned int data_8007BCC8[];
extern unsigned int data_8007BCD8[];
extern unsigned int data_8007BCE8[];
extern void *ptrs_8007BCF8[];
extern void *ptrs_8007BD10[];
extern void *ptrs_8007BD24[];
extern void *ptrs_8007BD3C[];
extern void *ptrs_8007BD58[];
extern void *ptrs_8007BDA8[];
extern void *ptrs_8007BDB8[];
extern void *ptrs_8007BDC4[];
extern void *ptrs_8007BDD4[];
extern void *ptrs_8007BDF0[];
extern void *ptrs_8007BE14[];
extern void *ptrs_8007BE48[];
extern void *ptrs_8007BE5C[];
extern void *ptrs_8007BE64[];
extern void *ptrs_8007BE84[];
extern void *ptrs_8007BEA0[];
extern void *ptrs_8007BECC[];
extern void *ptrs_8007BED8[];
extern void *ptrs_8007BEE4[];
extern unsigned int data_8007BF34[];
extern unsigned int data_8007BF6C[];
extern unsigned int data_8007BF84[];
extern unsigned int data_8007BF98[];
extern unsigned int data_8007BFA8[];
extern unsigned int data_8007BFC0[];
extern unsigned int data_8007C010[];
extern unsigned int data_8007C020[];
extern unsigned int data_8007C02C[];
extern unsigned int data_8007C044[];
extern unsigned int data_8007C054[];
extern unsigned int data_8007C070[];
extern unsigned int data_8007C090[];
extern unsigned int data_8007C0A4[];
extern unsigned int data_8007C0F4[];
extern unsigned int data_8007C0FC[];
extern unsigned int data_8007C11C[];
extern unsigned int data_8007C138[];
extern unsigned int data_8007C164[];
extern unsigned int data_8007C16C[];
extern void *ptrs_8007C178[];
extern void *ptrs_8007C1A8[];
extern unsigned int data_8007C1C8[];
extern unsigned int data_8007C2B0[];
extern unsigned int data_8007C2D8[];
extern unsigned int data_8007C300[];
extern unsigned int data_8007C328[];
extern unsigned int data_8007C350[];
extern unsigned int data_8007C378[];
extern unsigned int data_8007C3A0[];
extern unsigned int data_8007C3C8[];
extern unsigned int data_8007C3F0[];
extern unsigned int data_8007C418[];
extern unsigned int data_8007C440[];
extern unsigned int data_8007C468[];
extern unsigned int data_8007C490[];
extern unsigned int data_8007C4B8[];
extern unsigned int data_8007C4E0[];
extern unsigned int data_8007C508[];
extern unsigned int data_8007C530[];
extern unsigned int data_8007C558[];
extern unsigned int data_8007C580[];
extern unsigned int data_8007C5A8[];
extern unsigned int data_8007C5D0[];
extern void *ptrs_8007C5F8[];
extern void *ptrs_8007C5FC[];
extern unsigned int data_8007C64C[];
extern unsigned int data_8007C674[];
extern unsigned int data_8007C69C[];
extern unsigned int data_8007C6D8[];
extern unsigned int data_8007C714[];
extern unsigned int data_8007C750[];
extern unsigned int data_8007C78C[];
extern unsigned int data_8007C7C8[];
extern unsigned int data_8007C804[];
extern unsigned int data_8007C840[];
extern unsigned int data_8007C87C[];
extern unsigned int data_8007C8B8[];
extern unsigned int data_8007C8F4[];
extern unsigned int data_8007C930[];
extern unsigned int data_8007C96C[];
extern unsigned int data_8007C9A8[];
extern unsigned int data_8007C9E4[];
extern unsigned int data_8007CA20[];
extern unsigned int data_8007CA5C[];
extern unsigned int data_8007CA98[];
extern unsigned int data_8007CAD4[];
extern unsigned int data_8007CB10[];
extern unsigned int data_8007CB4C[];
extern unsigned int data_8007CB88[];
extern unsigned int data_8007CBC4[];
extern unsigned int data_8007CC00[];
extern unsigned int data_8007CC3C[];
extern unsigned int data_8007CC78[];
extern unsigned int data_8007CCB4[];
extern unsigned int data_8007CCF0[];
extern unsigned int data_8007CD2C[];
extern unsigned int data_8007CD68[];
extern unsigned int data_8007CDA4[];
extern unsigned int data_8007CDE0[];
extern unsigned int data_8007CE1C[];
extern unsigned int data_8007CE58[];
extern unsigned int data_8007CE94[];
extern unsigned int data_8007CED0[];
extern unsigned int data_8007CF0C[];
extern unsigned int data_8007CF48[];
extern unsigned int data_8007CF84[];
extern unsigned int data_8007CFC0[];
extern unsigned int data_8007CFFC[];
extern unsigned int data_8007D038[];
extern unsigned int data_8007D074[];
extern unsigned int data_8007D09C[];
extern unsigned int data_8007D0C4[];
extern unsigned int data_8007D0EC[];
extern unsigned int data_8007D114[];
extern unsigned int data_8007D13C[];
extern unsigned int data_8007D164[];
extern unsigned int data_8007D18C[];
extern void *ptrs_8007D1B4[];
extern unsigned int data_8007D284[];
extern unsigned int data_8007D2B0[];
extern unsigned int data_8007D2C4[];
extern unsigned int data_8007D2EC[];
extern unsigned int data_8007D314[];
extern unsigned int data_8007D33C[];
extern unsigned int data_8007D364[];
extern unsigned int data_8007D38C[];
extern unsigned int data_8007D3B4[];
extern unsigned int data_8007D3DC[];
extern unsigned int data_8007D404[];
extern void *ptrs_8007D42C[];
extern void *ptrs_8007D430[];
extern unsigned int data_8007D454[];
extern unsigned int data_8007D5A4[];
extern unsigned int data_8007D61C[];
extern unsigned int data_8007D6BC[];
extern unsigned int data_8007D75C[];
extern unsigned int data_8007D7FC[];
extern unsigned int data_8007D89C[];
extern unsigned int data_8007D93C[];
extern unsigned int data_8007D95C[];
extern unsigned int data_8007D968[];
extern unsigned int data_8007D974[];
extern unsigned int data_8007D980[];
extern unsigned int data_8007D98C[];
extern unsigned int data_8007D998[];
extern unsigned int data_8007D9A4[];
extern unsigned int data_8007D9B0[];
extern unsigned int data_8007D9BC[];
extern unsigned int data_8007D9C8[];
extern unsigned int data_8007D9D4[];
extern unsigned int data_8007D9E0[];
extern unsigned int data_8007D9F4[];
extern unsigned int data_8007DA00[];
extern unsigned int data_8007DA0C[];
extern unsigned int data_8007DA18[];
extern unsigned int data_8007DA24[];
extern unsigned int data_8007DA30[];
extern unsigned int data_8007DA3C[];
extern unsigned int data_8007DA48[];
extern unsigned int data_8007DA68[];
extern unsigned int data_8007DA90[];
extern unsigned int data_8007DA9C[];
extern unsigned int data_8007DAA8[];
extern unsigned int data_8007DAD0[];
extern unsigned int data_8007DAF8[];
extern unsigned int data_8007DB20[];
extern unsigned int data_8007DB34[];
extern unsigned int data_8007DB48[];
extern unsigned int data_8007DB5C[];
extern unsigned int data_8007DB70[];
extern unsigned int data_8007DB80[];
extern unsigned int data_8007DB98[];
extern unsigned int data_8007DBA0[];
extern unsigned int data_8007DBB4[];
extern unsigned int data_8007DBC8[];
extern unsigned int data_8007DBDC[];
extern unsigned int data_8007DBF0[];
extern unsigned int data_8007DC04[];
extern unsigned int data_8007DC1C[];
extern unsigned int data_8007DC44[];
extern unsigned int data_8007DC6C[];
extern unsigned int data_8007DC94[];
extern unsigned int data_8007DCBC[];
extern unsigned int data_8007DCE4[];
extern unsigned int data_8007DD0C[];
extern void *ptrs_8007DD34[];
extern unsigned int data_8007DE24[];
extern unsigned int data_8007DE38[];
extern unsigned int data_8007DE88[];
extern unsigned int data_8007DE98[];
extern unsigned int data_8007DEA0[];
extern unsigned int data_8007DEF0[];
extern unsigned int data_8007DEF4[];
extern unsigned int data_80080000[];
extern unsigned int data_800801E8[];
extern unsigned int data_800801EC[];
extern unsigned int data_800801F0[];
extern unsigned int data_800801F8[];
extern unsigned int data_800801FC[];
extern unsigned int data_80080218[];
extern unsigned int data_8008021C[];
extern unsigned int data_80080258[];
extern unsigned int data_800802D4[];
extern unsigned int data_800802DC[];
extern unsigned int data_800802E4[];
extern AnimWord anim_800802EC[];
extern unsigned int data_80080304[];
extern unsigned int data_80080330[];
extern unsigned int data_80080338[];
extern unsigned int data_80080344[];
extern unsigned int data_80080350[];
extern unsigned int data_8008035C[];
extern unsigned int data_80080360[];
extern unsigned int data_80080368[];
extern unsigned int data_80080370[];
extern unsigned int data_80080378[];
extern AnimWord anim_8008037C[];
extern unsigned int data_80080394[];
extern AnimWord anim_8008039C[];
extern unsigned int data_800803B4[];
extern AnimWord anim_800803BC[];
extern unsigned int data_800803D4[];
extern AnimWord anim_800803DC[];
extern unsigned int data_800803F4[];
extern unsigned int data_80080404[];
extern unsigned int data_80080410[];
extern unsigned int data_800804E0[];
extern unsigned int data_800804EC[];
extern unsigned int data_80080504[];
extern unsigned int data_80080524[];
extern unsigned int data_800805EC[];

unsigned int data_800801E8[1] = {
    0x100010,
};

unsigned int data_800801EC[1] = {
    0xFFF8FFF8,
};

unsigned int data_800801F0[2] = {
    0x80A880A0, 0x88A888A0,
};

unsigned int data_800801F8[1] = {
    0x80B780B0,
};

unsigned int data_800801FC[7] = {
    0x88B788B0, 0x80BF80B8, 0x88BF88B8, 0x88B888B0,
    0x8FB88FB0, 0x88C088B8, 0x8FC08FB8,
};

unsigned int data_80080218[1] = {
    0x8000C0,
};

unsigned int data_8008021C[15] = {
    0x100008, 0x8000DF, 0x10FFF8, 0x8000D7,
    0x8FFF0, 0x8F00DF, 0xFFF0FFF8, 0x8F00C7,
    0xFFF0FFF8, 0x8F00D8, 0xFFF00008, 0x8000C8,
    0x80010, 0x8000D8, 0x100008,
};

unsigned int data_80080258[31] = {
    0x801396D0, 0x8011CB7C, 0x8011CBFC, 0x80117AE0,
    0x8012B33C, 0x80115F8C, 0x8012B428, 0x8012B484,
    0x80118D80, 0x80118E10, 0x80119A5C, 0x80120398,
    0x8012B4BC, 0x8012B4F4, 0x80118E48, 0x80118E80,
    0x80119D00, 0x80119D38, 0x80119AB8, 0x80119B48,
    0x80119B80, 0x80119BB8, 0x8012F5A8, 0x8011CC34,
    0x8011CC6C, 0x801203D0, 0x8011A574, 0x801182F8,
    0xF00030, 0xF00088, 0xF000E0,
};

unsigned int data_800802D4[2] = {
    0x30001, 0x2,
};

unsigned int data_800802DC[2] = {
    0x640062, 0x63,
};

unsigned int data_800802E4[2] = {
    0x30001, 0x2,
};

AnimWord anim_800802EC[6] = {
    ANIM(192, 64, 176, 96),
    ANIM(208, 0, 0, 160),
    ANIM_STOP(208, 0, 0, 4096),
};

unsigned int data_80080304[11] = {
    0x90109008, 0x90209018, 0x90309028, 0x90409038,
    0x90509048, 0x90609058, 0x90709068, 0x90809078,
    0x90909088, 0x90A09098, 0x90B090A8,
};

unsigned int data_80080330[2] = {
    0x13150B07, 0xFF0D,
};

unsigned int data_80080338[3] = {
    0x140E1701, 0xF0B0200, 0xFF0D,
};

unsigned int data_80080344[3] = {
    0xC0B1104, 0xF0B0200, 0xFF0D,
};

unsigned int data_80080350[3] = {
    0x130D09, 0x6000000, 0xFF11,
};

unsigned int data_8008035C[1] = {
    0xFF0A,
};

unsigned int data_80080360[2] = {
    0xF0D1403, 0xFF,
};

unsigned int data_80080368[2] = {
    0x100D1601, 0xFF14,
};

unsigned int data_80080370[2] = {
    0x140B1408, 0xFF1315,
};

unsigned int data_80080378[1] = {
    0xFF120B05,
};

AnimWord anim_8008037C[6] = {
    ANIM_STOP(136, 88, 816, 8),
    ANIM_STOP(120, 104, 824, 8),
    ANIM_STOP(120, 120, 836, 8),
};

unsigned int data_80080394[2] = {
    0xFFFFFFFF, 0x0,
};

AnimWord anim_8008039C[6] = {
    ANIM_STOP(120, 96, 824, 8),
    ANIM_STOP(192, 96, 860, 8),
    ANIM_STOP(120, 120, 848, 8),
};

unsigned int data_800803B4[2] = {
    0xFFFFFFFF, 0x0,
};

AnimWord anim_800803BC[6] = {
    ANIM_STOP(120, 96, 836, 8),
    ANIM_STOP(192, 96, 860, 8),
    ANIM_STOP(120, 120, 848, 8),
};

unsigned int data_800803D4[2] = {
    0xFFFFFFFF, 0x0,
};

AnimWord anim_800803DC[6] = {
    ANIM_STOP(144, 88, 864, 8),
    ANIM_STOP(140, 104, 872, 8),
    ANIM_STOP(136, 120, 880, 8),
};

unsigned int data_800803F4[4] = {
    0x880094, (unsigned int)data_80080378, 0xFFFFFFFF, 0x0,
};

unsigned int data_80080404[3] = {
    0x70009C, 0x7800A0, 0x7400A0,
};

unsigned int data_80080410[52] = {
    0x6040302, 0xAFF0908, 0x480D0C0B, 0x14130F0E,
    0x1CFF1615, 0x22201F1E, 0x2F1A2827, 0x33323130,
    0x38373534, 0x403E3D39, 0x44434241, 0x4746FF45,
    0x4B4A49FF, 0x504F4CFF, 0xFF535251, 0x5659FFFF,
    0xFFFF5B58, 0xFFFFFFFF, 0xFFFF5CFF, 0x62575FFF,
    0xFF817119, 0x242355FF, 0xFF6CFF26, 0xFFFFFFFF,
    0xFFFFFFFF, 0x645DFFFF, 0xFFFF5E63, 0x4DFFFFFF,
    0x2C732569, 0x6B60FF68, 0x65FF7572, 0x6E677966,
    0xFFFF807B, 0x78777674, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFF7D7A7C, 0x8261FFFF, 0xFFFF6AFF, 0x83FF846D,
    0xFFFF7F85, 0x705FF86, 0x3F3A1B18, 0x2D4E3B36,
    0x1211102E, 0x541D2117, 0x29FF3CFF, 0x706F2B2A,
    0xFFFFFFFF, 0xFFFFFFFF, 0xB40000, 0x0,
};

unsigned int data_800804E0[3] = {
    0x10000, 0x30002, 0x50004,
};

unsigned int data_800804EC[6] = {
    0xFFD8, 0xFFEC, 0x0, 0x14,
    0x28, 0x3C,
};

unsigned int data_80080504[8] = {
    0x0, 0x1F4, 0x3E8, 0x7D0,
    0x1388, 0x2710, 0x4E20, 0xC350,
};

unsigned int data_80080524[50] = {
    0x2010000, 0x2000200, 0x1000101, 0x4010000,
    0x2000102, 0x3020303, 0x2000203, 0x2000002,
    0x1000202, 0x2020202, 0x2020202, 0x2010002,
    0x1010100, 0x1000000, 0x20101, 0x2020000,
    0x201, 0x0, 0x100, 0x1010001,
    0x2030200, 0x2020000, 0x2000002, 0x0,
    0x0, 0x2010000, 0x102, 0x0,
    0x2020202, 0x2010002, 0x202, 0x2010100,
    0x201, 0x2020202, 0x0, 0x0,
    0x20101, 0x2020000, 0x100, 0x100,
    0x200, 0x3020002, 0x2020500, 0x1000200,
    0x2020301, 0x1020201, 0x2000100, 0x2020202,
    0x0, 0x0,
};

unsigned int data_800805EC[50] = {
    0x3020200, 0x5000401, 0x2010202, 0x3030303,
    0x3000402, 0x4020304, 0x4020304, 0x3060201,
    0x4020303, 0x4030303, 0x3020204, 0x5020002,
    0x3020200, 0x3060200, 0x50303, 0x5020000,
    0x303, 0x0, 0x300, 0x3020102,
    0x3030406, 0x2020600, 0x50002, 0x0,
    0x0, 0x3020000, 0x203, 0x6000000,
    0x2030403, 0x3020003, 0x2000303, 0x5030206,
    0x503, 0x4030203, 0x0, 0x0,
    0x20204, 0x4000000, 0x303, 0x1000502,
    0x307, 0x2030005, 0x3020301, 0x2020003,
    0x2050502, 0x2020304, 0x2000200, 0x3020302,
    0x0, 0x0,
};
