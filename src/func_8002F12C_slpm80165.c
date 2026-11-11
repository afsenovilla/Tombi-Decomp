// VERSION SLPM_80165 slpm80165 8002F12C 176
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only layer 1 update: D_800A57BA is set in section 0 of areas 2, 5 and 8; except in area 6 the layer runs
// (with func_8002F1DC_slpm80165 first while D_800A57B8 is set).
extern unsigned short D_8009A810, D_8009A812;
extern unsigned char D_800A57B8[], D_800A57BA;
extern void func_8002F1DC_slpm80165(void);
extern void updateObjectsLayer1(void);

void func_8002F12C(void)
{
    register unsigned char *p asm("$4") = D_800A57B8;
    D_800A57BA = 0;
    switch (D_8009A810) {
    case 2:
    case 5:
    case 8:
        if (D_8009A812 == 0)
            D_800A57BA = 1;
        break;
    }
    if (D_8009A810 != 6) {
        if (*p)
            func_8002F1DC_slpm80165();
        updateObjectsLayer1();
    }
}
#endif
