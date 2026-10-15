// DATA 8007b958 9624 MAIN0
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

unsigned int data_8007B958[6] = {
    0x1000000, 0x1, 0x104, 0x204,
    0xD, 0xFF,
};

unsigned int data_8007B970[3] = {
    0x1000002, 0x1, 0xFF,
};

unsigned int data_8007B97C[4] = {
    0x1000002, 0x1, 0x4, 0xFF,
};

unsigned int data_8007B98C[5] = {
    0x1000000, 0x1, 0x3, 0x404,
    0xFF,
};

unsigned int data_8007B9A0[4] = {
    0x1000002, 0x1, 0x7, 0xFF,
};

unsigned int data_8007B9B0[5] = {
    0x1000002, 0x1, 0x304, 0x504,
    0xFF,
};

unsigned int data_8007B9C4[4] = {
    0x1000002, 0x1, 0x604, 0xFF,
};

unsigned int data_8007B9D4[4] = {
    0x1000002, 0x1, 0x2404, 0xFF,
};

unsigned int data_8007B9E4[3] = {
    0x1000002, 0x6, 0xFF,
};

unsigned int data_8007B9F0[4] = {
    0x1000002, 0x1, 0x2D04, 0xFF,
};

unsigned int data_8007BA00[4] = {
    0x1000002, 0x1, 0x2E04, 0xFF,
};

unsigned int data_8007BA10[4] = {
    0x1000002, 0x1, 0x2304, 0xFF,
};

unsigned int data_8007BA20[10] = {
    0x1000002, 0x1, 0x5, 0x1000008,
    0xA04, 0x2904, 0xA, 0x10A,
    0x20A, 0xFF,
};

unsigned int data_8007BA48[9] = {
    0x1000002, 0x1, 0x5, 0x1000008,
    0xA04, 0x2A04, 0x2004, 0x9,
    0xFF,
};

unsigned int data_8007BA6C[6] = {
    0x1000002, 0x1, 0x5, 0xA04,
    0x2C04, 0xFF,
};

unsigned int data_8007BA84[19] = {
    0x1000002, 0x1, 0x5, 0x2B04,
    0xFF, 0x1000002, 0x1, 0x1000008,
    0x2904, 0xFF, 0x1000002, 0x1,
    0x1000008, 0xA04, 0x2804, 0x2A04,
    0x2004, 0x9, 0xFF,
};

unsigned int data_8007BAD0[4] = {
    0x1000002, 0x1, 0x704, 0xFF,
};

unsigned int data_8007BAE0[7] = {
    0x1000002, 0x1, 0x904, 0xFF,
    0x1000000, 0x1, 0xFF,
};

unsigned int data_8007BAFC[4] = {
    0x1000002, 0x1, 0xC04, 0xFF,
};

unsigned int data_8007BB0C[9] = {
    0x1000002, 0x1, 0x804, 0xB04,
    0xFF, 0x1000002, 0x1, 0x1304,
    0xFF,
};

unsigned int data_8007BB30[7] = {
    0x1000002, 0x1, 0xB, 0xC,
    0x1D04, 0x2104, 0xFF,
};

unsigned int data_8007BB4C[4] = {
    0x1000002, 0x1, 0x2604, 0xFF,
};

unsigned int data_8007BB5C[4] = {
    0x1000002, 0x1, 0x3304, 0xFF,
};

unsigned int data_8007BB6C[4] = {
    0x1000002, 0x1, 0x1704, 0xFF,
};

unsigned int data_8007BB7C[4] = {
    0x1000002, 0x1, 0x1804, 0xFF,
};

unsigned int data_8007BB8C[4] = {
    0x1000002, 0x1, 0xD04, 0xFF,
};

unsigned int data_8007BB9C[4] = {
    0x1000002, 0x1, 0x1E04, 0xFF,
};

unsigned int data_8007BBAC[8] = {
    0x1000002, 0x1, 0xE04, 0xFF,
    0x1000002, 0x1, 0x1904, 0xFF,
};

unsigned int data_8007BBCC[4] = {
    0x1000002, 0x1, 0x2F04, 0xFF,
};

unsigned int data_8007BBDC[4] = {
    0x1000002, 0x1, 0x2204, 0xFF,
};

unsigned int data_8007BBEC[4] = {
    0x1000002, 0x1, 0x1C04, 0xFF,
};

unsigned int data_8007BBFC[4] = {
    0x1000002, 0x1, 0xF04, 0xFF,
};

unsigned int data_8007BC0C[4] = {
    0x1000002, 0x1, 0x2704, 0xFF,
};

unsigned int data_8007BC1C[4] = {
    0x1000002, 0x1, 0x1204, 0xFF,
};

unsigned int data_8007BC2C[7] = {
    0x1000002, 0x1, 0x3704, 0xFF,
    0x1000002, 0x1, 0xFF,
};

unsigned int data_8007BC48[4] = {
    0x1000002, 0x1, 0x1004, 0xFF,
};

unsigned int data_8007BC58[4] = {
    0x1000002, 0x1, 0x3904, 0xFF,
};

unsigned int data_8007BC68[4] = {
    0x1000002, 0x1, 0x3804, 0xFF,
};

unsigned int data_8007BC78[4] = {
    0x1000002, 0x1, 0x1104, 0xFF,
};

unsigned int data_8007BC88[4] = {
    0x1000002, 0x1, 0x1504, 0xFF,
};

unsigned int data_8007BC98[4] = {
    0x1000002, 0x1, 0x2504, 0xFF,
};

unsigned int data_8007BCA8[4] = {
    0x1000002, 0x1, 0x1A04, 0xFF,
};

unsigned int data_8007BCB8[4] = {
    0x1000002, 0x1, 0x1B04, 0xFF,
};

unsigned int data_8007BCC8[4] = {
    0x1000002, 0x1, 0x1F04, 0xFF,
};

unsigned int data_8007BCD8[4] = {
    0x1000002, 0x1, 0x1604, 0xFF,
};

unsigned int data_8007BCE8[4] = {
    0x1000002, 0x1, 0x1404, 0xFF,
};

void *ptrs_8007BCF8[6] = {
    data_8007B958,
    data_8007B958,
    data_8007B958,
    data_8007B970,
    data_8007B970,
    data_8007B97C,
};

void *ptrs_8007BD10[5] = {
    data_8007B98C,
    data_8007B98C,
    data_8007B9A0,
    data_8007B9B0,
    data_8007B9C4,
};

void *ptrs_8007BD24[6] = {
    data_8007B970,
    data_8007B9D4,
    data_8007B9E4,
    data_8007B9F0,
    data_8007BA00,
    data_8007BA10,
};

void *ptrs_8007BD3C[7] = {
    data_8007BA20,
    data_8007BA48,
    data_8007BA6C,
    data_8007BA84,
    data_8007BA20,
    data_8007BA48,
    data_8007BA84,
};

void *ptrs_8007BD58[20] = {
    data_8007B970,
    data_8007B970,
    data_8007BC88,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007BC98,
    data_8007BCA8,
    data_8007BCB8,
    data_8007B970,
    data_8007B970,
    data_8007BCC8,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007BCD8,
    data_8007BCE8,
    data_8007B970,
};

void *ptrs_8007BDA8[4] = {
    data_8007BAD0,
    data_8007BAE0,
    data_8007BAD0,
    data_8007BAE0,
};

void *ptrs_8007BDB8[3] = {
    data_8007B970,
    data_8007B970,
    data_8007B970,
};

void *ptrs_8007BDC4[4] = {
    data_8007BAFC,
    data_8007B970,
    data_8007BAFC,
    data_8007B970,
};

void *ptrs_8007BDD4[7] = {
    data_8007BB0C,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
};

void *ptrs_8007BDF0[9] = {
    data_8007BB30,
    data_8007BB4C,
    data_8007BB5C,
    data_8007BB6C,
    data_8007BB30,
    data_8007BB4C,
    data_8007BB5C,
    data_8007BB6C,
    data_8007BB7C,
};

void *ptrs_8007BE14[13] = {
    data_8007BB8C,
    data_8007BB9C,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
};

void *ptrs_8007BE48[5] = {
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
};

void *ptrs_8007BE5C[2] = {
    data_8007B970,
    data_8007B970,
};

void *ptrs_8007BE64[8] = {
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007B970,
};

void *ptrs_8007BE84[7] = {
    data_8007BBAC,
    data_8007B970,
    data_8007B970,
    data_8007B970,
    data_8007BBCC,
    data_8007BBDC,
    data_8007BBEC,
};

void *ptrs_8007BEA0[11] = {
    data_8007BBFC,
    data_8007BC0C,
    data_8007BC1C,
    data_8007B970,
    data_8007B970,
    data_8007BC2C,
    data_8007BBFC,
    data_8007BC1C,
    data_8007B970,
    data_8007B970,
    data_8007BC2C,
};

void *ptrs_8007BECC[3] = {
    data_8007BC48,
    data_8007BC58,
    data_8007BC68,
};

void *ptrs_8007BED8[3] = {
    data_8007B970,
    data_8007B970,
    data_8007BC78,
};

void *ptrs_8007BEE4[20] = {
    ptrs_8007BCF8,
    ptrs_8007BD10,
    ptrs_8007BD24,
    ptrs_8007BD3C,
    ptrs_8007BD58,
    ptrs_8007BDA8,
    ptrs_8007BDB8,
    ptrs_8007BD10,
    ptrs_8007BDC4,
    ptrs_8007BDD4,
    ptrs_8007BDF0,
    ptrs_8007BE14,
    ptrs_8007BE48,
    ptrs_8007BE5C,
    ptrs_8007BE64,
    ptrs_8007BE84,
    ptrs_8007BE84,
    ptrs_8007BEA0,
    ptrs_8007BECC,
    ptrs_8007BED8,
};

unsigned int data_8007BF34[14] = {
    (unsigned int)updateSectionSpawnManager, (unsigned int)updateSectionFadeController, 0x8004D20C, 0x8013BE48,
    (unsigned int)dispatchUnlayeredByVariant, 0x80130990, 0x801194AC, 0x8013C028,
    0x80130E3C, 0x8013123C, 0x801315B0, 0x8012D1BC,
    0x8012D200, 0x80138218,
};

unsigned int data_8007BF6C[6] = {
    0xFF9760, 0xFF9760, 0xFF9760, 0x0,
    0xFF9760, 0xFF9760,
};

unsigned int data_8007BF84[5] = {
    0xFF9760, 0xFF9760, 0xFF9760, 0xFF9760,
    0xFF9760,
};

unsigned int data_8007BF98[4] = {
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_8007BFA8[6] = {
    0x404040, 0x404040, 0x0, 0x404040,
    0xFF9760, 0xFF9760,
};

unsigned int data_8007BFC0[20] = {
    0x703840, 0x703840, 0x703840, 0x703840,
    0xFFFFFF, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_8007C010[4] = {
    0x0, 0x606060, 0x0, 0x606060,
};

unsigned int data_8007C020[3] = {
    0xFF9760, 0xFF9760, 0xFF9760,
};

unsigned int data_8007C02C[6] = {
    0xFF9760, 0xFF9760, 0xFF9760, 0xFF9760,
    0xFF9760, 0xFF9760,
};

unsigned int data_8007C044[4] = {
    0x0, 0x606060, 0x0, 0x606060,
};

unsigned int data_8007C054[7] = {
    0xFF9760, 0xF0C860, 0x0, 0x0,
    0x0, 0x0, 0xFF9760,
};

unsigned int data_8007C070[8] = {
    0x0, 0x0, 0x0, 0x0,
    0xFF9760, 0xFF9760, 0xFF9760, 0xFF9760,
};

unsigned int data_8007C090[5] = {
    0x0, 0x0, 0x0, 0x0,
    0x0,
};

unsigned int data_8007C0A4[20] = {
    0xFF9068, 0xFF9068, 0xFF9068, 0xFF9068,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_8007C0F4[2] = {
    0xFF9870, 0x0,
};

unsigned int data_8007C0FC[8] = {
    0x0, 0x0, 0x0, 0x0,
    0x2880FF, 0x0, 0x0, 0x4070FF,
};

unsigned int data_8007C11C[7] = {
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C138[11] = {
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C164[2] = {
    0x0, 0xFF9870,
};

unsigned int data_8007C16C[3] = {
    0x0, 0x0, 0x0,
};

void *ptrs_8007C178[12] = {
    data_8007BF6C,
    data_8007BF84,
    data_8007BF98,
    data_8007BFA8,
    data_8007BFC0,
    data_8007C010,
    data_8007C020,
    data_8007C02C,
    data_8007C044,
    data_8007C054,
    data_8007C070,
    data_8007C090,
};

void *ptrs_8007C1A8[8] = {
    data_8007C0A4,
    data_8007C0F4,
    data_8007C0FC,
    data_8007C11C,
    data_8007C11C,
    data_8007C138,
    data_8007C164,
    data_8007C16C,
};

unsigned int data_8007C1C8[58] = {
    0x80137F54, 0x80138008, 0x801380B8, 0x8013B984,
    0x8013BAB0, 0x8013BC24, 0x8013BDA0, 0x800FA5C8,
    0x80129B70, 0x8011762C, 0x801332CC, 0x8012A70C,
    0x800FAFAC, 0x800FB32C, 0x800FC050, 0x800FDB34,
    0x800FDE6C, 0x800FEB28, 0x80118B8C, 0x8012E6D0,
    0x8012F35C, 0x8012F69C, 0x8012EA30, 0x8012ED44,
    0x8012EEE0, 0x801179A4, 0x8012E828, 0x8012DC5C,
    0x80117408, 0x8012E494, 0x80119BA8, 0x8012DF14,
    0x80133668, 0x8012D5D8, 0x80117190, 0x8011C628,
    0x80119914, 0x8012D9C8, 0x8012D858, 0x80118880,
    0x80133A2C, 0x8013433C, 0x80134630, 0x80134080,
    0x80133DF4, 0x800EEC18, 0x8011C6D0, 0x80116EFC,
    0x80137F54, 0x80137F54, 0x80137F54, 0x8012E728,
    0x80137F54, 0x801199E8, 0x80137F54, 0x8011974C,
    0x8011765C, 0x801173EC,
};

unsigned int data_8007C2B0[10] = {
    0xF8FF0000, 0x0, 0x1E68, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C2D8[10] = {
    0xF8FF0001, 0x0, 0x890, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C300[10] = {
    0xF8FF0002, 0x0, 0x79C, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C328[10] = {
    0xF8FF0003, 0x0, 0x87C, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C350[10] = {
    0xF8FF0004, 0x0, 0x944, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C378[10] = {
    0xF8FF0005, 0x0, 0xE90, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C3A0[10] = {
    0xF8FF0006, 0x0, 0x680, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C3C8[10] = {
    0xF8FF0007, 0x0, 0x49C, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C3F0[10] = {
    0xF8FF0008, 0x0, 0x724, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C418[10] = {
    0xF8FF0009, 0x0, 0x590, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C440[10] = {
    0xF8FF000A, 0x0, 0x7CC, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C468[10] = {
    0xF8FF000B, 0x0, 0xB44, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C490[10] = {
    0xF8FF000C, 0x0, 0x418, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C4B8[10] = {
    0xF8FF000D, 0x0, 0xE40, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C4E0[10] = {
    0xF8FF000E, 0x0, 0x358, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C508[10] = {
    0xF8FF000F, 0x0, 0x834, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C530[10] = {
    0xF8FF000F, 0x0, 0x834, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C558[10] = {
    0xF8FF0010, 0x0, 0x5C4, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C580[10] = {
    0xF8FF0011, 0x0, 0x69C, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C5A8[10] = {
    0xF8FF0012, 0x0, 0x370, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C5D0[10] = {
    0xF8FF0013, 0x0, 0x564, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

void *ptrs_8007C5F8[1] = {
    data_8007C2B0,
};

void *ptrs_8007C5FC[20] = {
    data_8007C2D8,
    data_8007C300,
    data_8007C328,
    data_8007C350,
    data_8007C378,
    data_8007C3A0,
    data_8007C3C8,
    data_8007C3F0,
    data_8007C418,
    data_8007C440,
    data_8007C468,
    data_8007C490,
    data_8007C4B8,
    data_8007C4E0,
    data_8007C508,
    data_8007C530,
    data_8007C558,
    data_8007C580,
    data_8007C5A8,
    data_8007C5D0,
};

unsigned int data_8007C64C[10] = {
    0x9FFF00E0, 0x801B0900, 0x4A6CC, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C674[10] = {
    0x93FF00E1, 0x801D1A00, 0x2582C, 0x90000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007C69C[15] = {
    0x93FF00E2, 0x801EBA00, 0xB7BC, 0xB0000,
    0x2, 0xB0FF00E3, 0x0, 0x5A1,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C6D8[15] = {
    0x93FF00E4, 0x801E6E00, 0xFBCC, 0x90000,
    0x2, 0xB0FF00E5, 0x0, 0xE10,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C714[15] = {
    0x93FF00E6, 0x801C5D00, 0x2F564, 0x10000,
    0x2, 0xB0FF00E7, 0x0, 0x2470,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C750[15] = {
    0x93FF00E8, 0x801D8600, 0x1E394, 0x20000,
    0x2, 0xB0FF00E9, 0x0, 0xD19,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C78C[15] = {
    0x93FF00EA, 0x801D6B00, 0x1F77C, 0x30000,
    0x2, 0xB0FF00EB, 0x0, 0x1036,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C7C8[15] = {
    0x93FF00EC, 0x801E0000, 0x16994, 0x80000,
    0x2, 0xB0FF00ED, 0x0, 0xF96,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C804[15] = {
    0x93FF00EE, 0x801E0C00, 0x15E04, 0xA0000,
    0x2, 0xB0FF00EF, 0x0, 0xCC7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C840[15] = {
    0x93FF00F0, 0x801C3800, 0x319A4, 0xC0000,
    0x2, 0xB0FF00F1, 0x0, 0x22D0,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C87C[15] = {
    0x93FF00F2, 0x801D3500, 0x22CF4, 0x3E0002,
    0x2, 0xB0FF00F3, 0x0, 0x11CD,
    0x2, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C8B8[15] = {
    0x93FF00F4, 0x801D2800, 0x24A04, 0xD0003,
    0x2, 0xB0FF00F5, 0x0, 0x7FF,
    0x3, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C8F4[15] = {
    0x93FF00F6, 0x801D8300, 0x1E6A4, 0x3F0004,
    0x2, 0xB0FF00F7, 0x0, 0xBA6,
    0x4, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C930[15] = {
    0x93FF00F8, 0x801C8300, 0x2DEC4, 0x100000,
    0x2, 0xB0FF00F9, 0x0, 0x1630,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C96C[15] = {
    0x93FF00FA, 0x801C7500, 0x2F504, 0x110000,
    0x2, 0xB0FF00FB, 0x0, 0xF04,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C9A8[15] = {
    0x93FF00FC, 0x801DD800, 0x1918C, 0x400000,
    0x2, 0xB0FF00FD, 0x0, 0xB03,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007C9E4[15] = {
    0x93FF00FE, 0x801C9E00, 0x2ABA4, 0x180000,
    0x2, 0xB0FF00FF, 0x0, 0x2801,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CA20[15] = {
    0x93FF0100, 0x801DEF00, 0x17A8C, 0x190000,
    0x2, 0xB0FF0101, 0x0, 0xA94,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CA5C[15] = {
    0x93FF0102, 0x801E0B00, 0x14EDC, 0x30000,
    0x2, 0xB0FF0103, 0x0, 0x1945,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CA98[15] = {
    0x93FF0104, 0x801D1200, 0x26064, 0x200000,
    0x2, 0xB0FF0105, 0x0, 0x72F,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CAD4[15] = {
    0x93FF0106, 0x801C7100, 0x2E8E4, 0x240000,
    0x2, 0xB0FF0107, 0x0, 0x19EB,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CB10[15] = {
    0x93FF0108, 0x801DEB00, 0x176BC, 0x250000,
    0x2, 0xB0FF0109, 0x0, 0x125E,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CB4C[15] = {
    0x93FF010A, 0x801E1D00, 0x14D0C, 0x270000,
    0x2, 0xB0FF010B, 0x0, 0x84B,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CB88[15] = {
    0x93FF010C, 0x801D9800, 0x1C25C, 0x280000,
    0x2, 0xB0FF010D, 0x0, 0x1F61,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CBC4[15] = {
    0x93FF010E, 0x801CC000, 0x2A9F4, 0x2A0000,
    0x2, 0xB0FF010F, 0x0, 0x89F,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CC00[15] = {
    0x93FF0110, 0x801DFA00, 0x1583C, 0x280000,
    0x2, 0xB0FF0111, 0x0, 0x264E,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CC3C[15] = {
    0x93FF0112, 0x801BF000, 0x37304, 0x2B0000,
    0x2, 0xB0FF0113, 0x0, 0xD95,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CC78[15] = {
    0x93FF0114, 0x801E3B00, 0x1174C, 0x2C0000,
    0x2, 0xB0FF0115, 0x0, 0x2128,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CCB4[15] = {
    0x93FF0116, 0x801DDA00, 0x1805C, 0x120000,
    0x2, 0xB0FF0117, 0x0, 0x1AFB,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CCF0[15] = {
    0x93FF0118, 0x801D6300, 0x1FF34, 0x390000,
    0x2, 0xB0FF0119, 0x0, 0x13A6,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CD2C[15] = {
    0x93FF011A, 0x801E3E00, 0x1242C, 0xF0000,
    0x2, 0xB0FF011B, 0x0, 0x17DB,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CD68[15] = {
    0x93FF011C, 0x801C4F00, 0x30B14, 0x300000,
    0x2, 0xB0FF011D, 0x0, 0x1BD7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CDA4[15] = {
    0x93FF011E, 0x801C9000, 0x2C9F4, 0x310000,
    0x2, 0xB0FF011D, 0x0, 0x1BD7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CDE0[15] = {
    0x93FF011F, 0x801C5A00, 0x30014, 0x320000,
    0x2, 0xB0FF011D, 0x0, 0x1BD7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CE1C[15] = {
    0x93FF0120, 0x801C9A00, 0x2BFD4, 0x330000,
    0x2, 0xB0FF011D, 0x0, 0x1BD7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CE58[15] = {
    0x93FF0121, 0x801CC300, 0x296D4, 0x340000,
    0x2, 0xB0FF011D, 0x0, 0x1BD7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CE94[15] = {
    0x93FF0122, 0x801CB200, 0x2A7D4, 0x350000,
    0x2, 0xB0FF011D, 0x0, 0x1BD7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CED0[15] = {
    0x93FF0123, 0x801C0900, 0x350E4, 0x360000,
    0x2, 0xB0FF011D, 0x0, 0x1BD7,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CF0C[15] = {
    0x93FF0124, 0x801BDF00, 0x36344, 0x370000,
    0x2, 0xB0FF0125, 0x0, 0x349F,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CF48[15] = {
    0x93FF0126, 0x801C8900, 0x2D8C4, 0x460000,
    0x2, 0xB0FF0127, 0x0, 0x107F,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CF84[15] = {
    0x93FF0128, 0x801C0D00, 0x34D34, 0x480000,
    0x2, 0xB0FF0129, 0x0, 0x193E,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CFC0[15] = {
    0x93FF012A, 0x801C2400, 0x32D84, 0x490000,
    0x2, 0xB0FF012B, 0x0, 0x22AF,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007CFFC[15] = {
    0x93FF012C, 0x801C7900, 0x2F0C4, 0x4A0000,
    0x2, 0xB0FF012D, 0x0, 0xAE0,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007D038[15] = {
    0x93FF012E, 0x801E5100, 0x12094, 0x3B0000,
    0x2, 0xB0FF012F, 0x0, 0x493,
    0x0, 0x2, 0xFFFF, 0x0,
    0x0, 0x0, 0x0,
};

unsigned int data_8007D074[10] = {
    0x94FF0130, 0x801E7700, 0xC77C, 0x40000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D09C[10] = {
    0x94FF0131, 0x801DEF00, 0x14DCC, 0x50000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D0C4[10] = {
    0x94FF0132, 0x801E5700, 0xE30C, 0x60000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D0EC[10] = {
    0x94FF0133, 0x801F0700, 0x33EC, 0x70000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D114[10] = {
    0x94FF0134, 0x801D8A00, 0x1AF2C, 0x1C0000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D13C[10] = {
    0x94FF0135, 0x801D9200, 0x1A2DC, 0x1C0000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D164[10] = {
    0x94FF0136, 0x801D9A00, 0x19F5C, 0x130000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D18C[10] = {
    0x94FF0137, 0x801D5A00, 0x1DF0C, 0x130000,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

void *ptrs_8007D1B4[52] = {
    data_8007C64C,
    data_8007C674,
    data_8007C69C,
    data_8007C6D8,
    data_8007C714,
    data_8007C750,
    data_8007C78C,
    data_8007C7C8,
    data_8007C804,
    data_8007C840,
    data_8007C87C,
    data_8007C8B8,
    data_8007C8F4,
    data_8007C930,
    data_8007C96C,
    data_8007C9A8,
    data_8007C9E4,
    data_8007CA20,
    data_8007CA5C,
    data_8007CA98,
    data_8007CAD4,
    data_8007CB10,
    data_8007CB4C,
    data_8007CB88,
    data_8007CBC4,
    data_8007CC00,
    data_8007CC3C,
    data_8007CC78,
    data_8007CCB4,
    data_8007CCF0,
    data_8007CD2C,
    data_8007CD68,
    data_8007CDA4,
    data_8007CDE0,
    data_8007CE1C,
    data_8007CE58,
    data_8007CE94,
    data_8007CED0,
    data_8007CF0C,
    data_8007CF48,
    data_8007CF84,
    data_8007CFC0,
    data_8007CFFC,
    data_8007D038,
    data_8007D074,
    data_8007D09C,
    data_8007D0C4,
    data_8007D0EC,
    data_8007D114,
    data_8007D13C,
    data_8007D164,
    data_8007D18C,
};

unsigned int data_8007D284[11] = {
    0x1390138, 0x13B013A, 0x13D013C, 0x13F013E,
    0x1410140, 0x1430142, 0x1450144, 0x1470146,
    0x1490148, 0x14B014A, 0x14D014C,
};

unsigned int data_8007D2B0[5] = {
    0xFFFF, 0x0, 0x0, 0x0,
    0x0,
};

unsigned int data_8007D2C4[10] = {
    0xC08303DC, 0x0, 0xF63, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D2EC[10] = {
    0xC1FF03DD, 0x801EF800, 0x4F35, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D314[10] = {
    0xC08303DE, 0x0, 0x1C50, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D33C[10] = {
    0xC08303DF, 0x0, 0x1BE0, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D364[10] = {
    0xC08303E0, 0x0, 0x238, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D38C[10] = {
    0xC08303E1, 0x0, 0x3AC, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D3B4[10] = {
    0xC08303E2, 0x0, 0xA31, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D3DC[10] = {
    0xC08303E3, 0x0, 0xACF, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007D404[10] = {
    0xC1FF03E4, 0x801F2000, 0x24CC, 0x0,
    0x2, 0xFFFF, 0x0, 0x0,
    0x0, 0x0,
};

void *ptrs_8007D42C[1] = {
    data_8007D2B0,
};

void *ptrs_8007D430[9] = {
    data_8007D2C4,
    data_8007D2EC,
    data_8007D314,
    data_8007D33C,
    data_8007D364,
    data_8007D38C,
    data_8007D3B4,
    data_8007D3DC,
    data_8007D404,
};

unsigned int data_8007D454[84] = {
    0x200028, 0x380050, 0x880078, 0x680080,
    0x800098, 0x480030, 0x600040, 0x480028,
    0x280070, 0x600078, 0x700068, 0x700078,
    0x180068, 0x480058, 0x680058, 0x780048,
    0x380060, 0x400058, 0x800090, 0x980098,
    0x880090, 0x880088, 0xA00098, 0x880088,
    0x880088, 0x400088, 0x380060, 0x300048,
    0x500060, 0x680050, 0x200020, 0x600050,
    0x200020, 0x580058, 0x580058, 0x580058,
    0x580058, 0x580058, 0x280060, 0x280050,
    0x280028, 0x280028, 0x280028, 0x600028,
    0x500060, 0x200030, 0xA80020, 0x580060,
    0x300060, 0x700068, 0x580068, 0x400068,
    0x780050, 0xE00088, 0x580060, 0x500048,
    0x580058, 0x500060, 0x580090, 0x680040,
    0x580090, 0x480050, 0x500098, 0x580080,
    0x800090, 0x280098, 0x200058, 0x580078,
    0x500060, 0x6000A8, 0x700068, 0x980098,
    0x5800A8, 0x200088, 0x600088, 0xA00058,
    0x6800A8, 0x780078, 0x780068, 0x580038,
    0x0, 0x0, 0x700000, 0xB0,
};

unsigned int data_8007D5A4[30] = {
    0xB00070, 0xB800B8, 0xB000C0, 0xA000A0,
    0xB800C0, 0xB800B8, 0xC800A8, 0xA000C8,
    0xA800A0, 0xB80090, 0xC80070, 0xD800D0,
    0xA000A0, 0xA000A0, 0xA000A0, 0xD000A8,
    0xD800C8, 0x7000E8, 0x880060, 0xB800B0,
    0xF800A0, 0xF00100, 0xE000E8, 0xA000A0,
    0xA000A0, 0x8800A8, 0xA00090, 0x980098,
    0xA00090, 0xA000A0,
};

unsigned int data_8007D61C[40] = {
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

unsigned int data_8007D6BC[40] = {
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

unsigned int data_8007D75C[40] = {
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

unsigned int data_8007D7FC[40] = {
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

unsigned int data_8007D89C[40] = {
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

unsigned int data_8007D93C[8] = {
    0xA8004400, 0xA800A800, 0xA800A800, 0xA800A800,
    0xA800A800, 0xA800A800, 0xA800A800, 0xA800A800,
};

unsigned int data_8007D95C[3] = {
    0xA900FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D968[3] = {
    0xAA00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D974[3] = {
    0xAB00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D980[3] = {
    0xAC00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D98C[3] = {
    0xAD00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D998[3] = {
    0xAE00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D9A4[3] = {
    0xAF00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D9B0[3] = {
    0xB000FFFF, 0xFFFFB000, 0xFFFF,
};

unsigned int data_8007D9BC[3] = {
    0xB100FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D9C8[3] = {
    0xB200FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D9D4[3] = {
    0xB300FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007D9E0[5] = {
    0xB4004500, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xB400FFFF,
};

unsigned int data_8007D9F4[3] = {
    0xB500FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA00[3] = {
    0xB600FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA0C[3] = {
    0xB700FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA18[3] = {
    0xFFFF4600, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA24[3] = {
    0xB900FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA30[3] = {
    0xBA00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA3C[3] = {
    0xBB00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA48[8] = {
    0x88003300, 0x88008800, 0x88008800, 0x88008800,
    0x88008800, 0x88008800, 0x88008800, 0x88008800,
};

unsigned int data_8007DA68[10] = {
    0xBD004800, 0xBD00BD00, 0xBD00BD00, 0xBD00BD00,
    0xBD00BD00, 0xBD00BD00, 0xBD00BD00, 0xBD00BD00,
    0xBD00BD00, 0xBD00,
};

unsigned int data_8007DA90[3] = {
    0xBE00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DA9C[3] = {
    0xBF00FFFF, 0xFFFFFFFF, 0xFFFF,
};

unsigned int data_8007DAA8[10] = {
    0xC9004B00, 0xC700C800, 0xC700C800, 0xC900FFFF,
    0xCC00C700, 0xC700C800, 0xC700C700, 0xC700C700,
    0xC700C700, 0xC800C700,
};

unsigned int data_8007DAD0[10] = {
    0xCA004C00, 0xC800C900, 0xC800C900, 0xCA00C800,
    0xCD00C800, 0xC800C900, 0xC800C800, 0xC800C800,
    0xC800C800, 0xC900C800,
};

unsigned int data_8007DAF8[10] = {
    0xCB004D00, 0xC900CA00, 0xC900CA00, 0xCB00C900,
    0xCE00C900, 0xC900CA00, 0xC900C900, 0xC900C900,
    0xC900C900, 0xCA00C900,
};

unsigned int data_8007DB20[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC900,
};

unsigned int data_8007DB34[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xCA00,
};

unsigned int data_8007DB48[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xCB00,
};

unsigned int data_8007DB5C[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xCD00,
};

unsigned int data_8007DB70[4] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xBE00,
};

unsigned int data_8007DB80[6] = {
    0xFFFFFFFF, 0xCD00FFFF, 0xFFFFCE00, 0xFFFFFFFF,
    0xFFFFFFFF, 0xCE00,
};

unsigned int data_8007DB98[2] = {
    0xFFFFFFFF, 0xBF00,
};

unsigned int data_8007DBA0[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xBF00FFFF,
};

unsigned int data_8007DBB4[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC000FFFF,
};

unsigned int data_8007DBC8[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC100FFFF,
};

unsigned int data_8007DBDC[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC200FFFF,
};

unsigned int data_8007DBF0[5] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC300FFFF,
};

unsigned int data_8007DC04[6] = {
    0xFFFF4900, 0xFFFFFFFF, 0xFFFFBF00, 0xFFFFFFFF,
    0xFFFFFFFF, 0xBF00,
};

unsigned int data_8007DC1C[10] = {
    0xFFFFFFFF, 0xFFFFC100, 0xC000C100, 0xFFFFFFFF,
    0xFFFFFFFF, 0xC000FFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC000FFFF, 0xC000,
};

unsigned int data_8007DC44[10] = {
    0xFFFFFFFF, 0xFFFFC200, 0xC100C200, 0xFFFFFFFF,
    0xFFFFFFFF, 0xC100FFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC100FFFF, 0xC100,
};

unsigned int data_8007DC6C[10] = {
    0xFFFFFFFF, 0xFFFFC300, 0xC200C300, 0xFFFFFFFF,
    0xFFFFFFFF, 0xC200FFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC200FFFF, 0xC200,
};

unsigned int data_8007DC94[10] = {
    0xFFFFFFFF, 0xFFFFC400, 0xC300C400, 0xFFFFFFFF,
    0xFFFFFFFF, 0xC300FFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC300FFFF, 0xC300,
};

unsigned int data_8007DCBC[10] = {
    0xFFFFFFFF, 0xFFFFC500, 0xC400C500, 0xFFFFFFFF,
    0xFFFFFFFF, 0xC400FFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC400FFFF, 0xC400,
};

unsigned int data_8007DCE4[10] = {
    0xFFFFFFFF, 0xFFFFC600, 0xC500C600, 0xFFFFFFFF,
    0xFFFFFFFF, 0xC500FFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xC500FFFF, 0xC500,
};

unsigned int data_8007DD0C[10] = {
    0xD0005300, 0xCE00CF00, 0xCE00CF00, 0xD000CE00,
    0xD300CF00, 0xCE00CF00, 0xCE00CE00, 0xCE00CE00,
    0xCE00CE00, 0xCE00,
};

void *ptrs_8007DD34[60] = {
    data_8007D93C,
    data_8007D95C,
    data_8007D968,
    data_8007D974,
    data_8007D980,
    data_8007D98C,
    data_8007D998,
    data_8007D9A4,
    data_8007D9B0,
    data_8007D9BC,
    data_8007D9C8,
    data_8007D9D4,
    data_8007D9E0,
    data_8007D9F4,
    data_8007DA00,
    data_8007DA0C,
    data_8007DA18,
    data_8007DA24,
    data_8007DA30,
    data_8007DA3C,
    data_8007DA48,
    data_8007DA68,
    data_8007DA90,
    data_8007DA9C,
    data_8007D93C,
    data_8007D93C,
    data_8007D93C,
    data_8007D93C,
    data_8007D93C,
    data_8007D93C,
    data_8007DAA8,
    data_8007DAD0,
    data_8007DAF8,
    data_8007DB20,
    data_8007DB34,
    data_8007DB48,
    data_8007DB5C,
    data_8007DB70,
    data_8007DB80,
    data_8007DB98,
    data_8007DAA8,
    data_8007DBA0,
    data_8007DBB4,
    data_8007DBC8,
    data_8007DBDC,
    data_8007DBF0,
    data_8007D93C,
    data_8007D93C,
    data_8007D93C,
    data_8007D93C,
    data_8007DC04,
    data_8007DC1C,
    data_8007DC44,
    data_8007DC6C,
    data_8007DC94,
    data_8007DCBC,
    data_8007DCE4,
    data_8007D93C,
    data_8007D93C,
    data_8007D93C,
};

unsigned int data_8007DE24[5] = {
    0x0, 0x0, 0x0, 0x0,
    0x0,
};

unsigned int data_8007DE38[20] = {
    0x8013A048, 0x8013D8A8, (unsigned int)data_8007D61C, 0x801385E8,
    0x801339A4, (unsigned int)data_8007D6BC, 0x801228BC, 0x80118C68,
    (unsigned int)data_8007D75C, 0x8012D9E4, 0x80131A7C, 0x8011C20C,
    0x8013A048, 0x8011A7F0, 0x80128C74, 0x8013A048,
    0x8011B354, 0x8011C08C, (unsigned int)data_8007D7FC, (unsigned int)data_8007D89C,
};

unsigned int data_8007DE88[4] = {
    0x1615140A, 0x1C1B1817, 0x2221201D, 0xFF,
};

unsigned int data_8007DE98[2] = {
    0xAC00E4, 0x74,
};

unsigned int data_8007DEA0[20] = {
    0x800994B8, 0x800993C8, 0x800994A4, 0x8009956C,
    0x80099A80, 0x800992B0, 0x800990D0, 0x80099350,
    0x800991C0, 0x800993F0, 0x80099760, 0x80099044,
    0x80099A30, 0x80098F90, 0x80099454, (unsigned int)ptrs_8007D42C,
    0x800991E8, 0x800992B0, 0x80098FA4, 0x80099198,
};
