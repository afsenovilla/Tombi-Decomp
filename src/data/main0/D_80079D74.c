// DATA 80079d74 104 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void dispatchAreaInitA();
extern void dispatchAreaInitB();
extern void func_80030404();
extern unsigned int data_80079D74[];

unsigned int data_80079D74[26] = {
    0x800ED024, 0x8011B8C0, 0x8011BF68, 0x800EDF94,
    (unsigned int)dispatchAreaInitA, 0x8011A1C0, 0x8011D010, (unsigned int)dispatchAreaInitB,
    (unsigned int)func_80030404, 0x8011E838, 0x8011EC68, 0x8011C158,
    0x8011C274, 0x8011F1D0, 0x8011C354, 0x8011C574,
    0x8011D4D0, 0x8011F288, 0x8011D73C, 0x800EE18C,
    0x8011DD94, 0x8011E0D4, 0x8011E40C, 0x8011F608,
    0x801202A4, 0x8011F1A8,
};
