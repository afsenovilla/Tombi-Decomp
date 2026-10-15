// DATA 80077d6c 420 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void dispatchAreaNpcHandler();
extern void updateL2Type02Object();
extern void updateL2Type30Object();
extern unsigned int data_80077D6C[];
extern unsigned int data_80077D9C[];
extern unsigned int data_80077F08[];
extern unsigned int data_80077F0C[];

unsigned int data_80077D6C[12] = {
    0x7000500, 0x700FB00, 0x0, 0x7000550,
    0x700FAB0, 0x0, 0x500000, 0xFFB00000,
    0x0, 0x1000000, 0xFF000000, 0x0,
};

unsigned int data_80077D9C[91] = {
    0x8012B2D4, 0x80126FEC, (unsigned int)updateL2Type02Object, 0x8012DEDC,
    0x8012E1C0, 0x8012E4C4, 0x8012E8C4, 0x80111AE0,
    0x80132174, 0x801328B4, 0x80123E24, 0x8012F448,
    0x8012F988, 0x801312F8, 0x80113484, 0x801315DC,
    0x80131904, 0x80131CA4, 0x80133474, 0x80134C00,
    0x80134404, 0x80134C20, 0x8013649C, 0x8013721C,
    0x8011468C, 0x800EDE4C, 0x801246D0, 0x80125248,
    0x801248D4, 0x80124FFC, 0x80126E54, 0x80114890,
    0x801251C4, 0x801283A4, 0x80138D4C, 0x80139914,
    0x8012552C, 0x800EE4C0, 0x800EE79C, 0x80125040,
    0x801281FC, 0x801274FC, 0x80116FA8, 0x8012D294,
    0x801288B8, 0x800EE748, 0x800EEAC8, 0x801161A8,
    (unsigned int)updateL2Type30Object, 0x80127EC4, (unsigned int)dispatchAreaNpcHandler, 0x8012B210,
    0x80134E5C, 0x8012B438, 0x8012BF58, 0x8012D280,
    0x8012F524, 0x8012F9A0, 0x8012AC48, 0x8011F4E0,
    0x8011F4E0, 0x8011F4E0, 0x8011F4E0, 0x8011F4E0,
    0x8011F4E0, 0x8011F4E0, 0x8011F4E0, 0x801206C0,
    0x800EC9D4, 0x800EDE78, 0x801215B0, 0x80122418,
    0x80122DE8, 0x80122FA0, 0x80118684, 0x8012415C,
    0x800EE140, 0x8012855C, 0x8012C8A0, 0x801244D0,
    0x801251B4, 0x8011E54C, 0x80119330, 0x8011ED24,
    0x8004AE7C, 0x80114A08, 0x80116FE0, 0x80136090,
    0x8012CE80, 0x8012955C, 0x801307B0,
};

unsigned int data_80077F08[1] = {
    0x7F007F,
};

unsigned int data_80077F0C[1] = {
    0x0,
};
