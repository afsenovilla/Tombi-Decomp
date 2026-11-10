// VERSION SLPM_80165 x00 800FD168 596
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStep48Area10 (src/x010/playerStep48Area10.c): the landing calls func_800FD048 (the PAL
// bite-entry copy) instead of setting anim 0xD, no floor test; in step 2 the bitten object follows the player and
// the fixed button 0x40 lets go. D-pad bits 0x8000 / 0x2000.
typedef struct { short s0, s2; } H;
typedef struct O {
    char p0[2]; unsigned char b2, b3; char p0b; unsigned char b5, b6; char p1[0x12 - 7]; short x12; char p2[2]; short y16; char p3[2]; short z1a;
    char p4a[4]; short s20; char p4b[0x2e - 0x22]; unsigned short w2e; char p4[0x40 - 0x30]; H *h;
    char p5[0x69 - 0x44]; unsigned char b69; char p5b[0x70 - 0x6a]; short s70, s72; char p6[0x7c - 0x74]; short vx, vy, s80, s82;
    char p7[0x8c - 0x84]; int a8c; char p7b[0x9c - 0x90]; unsigned char b9c; char p8[0xac - 0x9d]; unsigned char bac;
    char p9[0xb2 - 0xad]; unsigned short wb2;
} O;
extern O *BITTEN_OBJECT;
extern unsigned short PAD_PRESSED;
extern unsigned short BUTTON_MASK_B, JOYPAD_STATE;
extern void playerAirAccelerateX(O *);
extern void FUN_8001fd94(O *);
extern void playerBiteEntryCopyPos(O *);
extern void PlayerSetAnimIfChanged(O *, int);
extern short FUN_8003fd78(O *, int, int);

#define PADCHK() { volatile unsigned short *pad = &PAD_PRESSED; if (*pad & 0x8000) o->w2e = 1; if (*pad & 0x2000) o->w2e = 0; }
extern unsigned short D_1F8001FC;

void func_800FD168(O *o)
{
    short flag;
    O *p;

    switch (o->b6) {
    case 1:
        o->s20++;
        playerAirAccelerateX(o);
        *(int *)o->h += o->vx << 8;
        o->vy += 8;
        if (o->vy > 0x680) o->vy = 0x680;
        if (o->vy < -0x680) o->vy = -0x680;
        FUN_8001fd94(o);
        flag = 0;
        o->y16 += 8;
        if (o->y16 + BITTEN_OBJECT->s70 >= BITTEN_OBJECT->y16) {
            o->y16 = BITTEN_OBJECT->y16 - BITTEN_OBJECT->s70;
            flag = 1;
        }
        if (o->vx < 0) {
            o->h->s2 -= 8;
            if (o->h->s2 <= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) playerBiteEntryCopyPos(o);
            }
        } else {
            o->h->s2 += 8;
            if (o->h->s2 >= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) playerBiteEntryCopyPos(o);
            }
        }
        if (o->s20 > 10)
            playerBiteEntryCopyPos(o);
        return;
    case 2:
        PADCHK();
        p = BITTEN_OBJECT;
        p->h->s2 = o->h->s2;
        p->y16 = o->y16 + p->s70;
        if (D_1F8001FC & 0x40) {
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            BITTEN_OBJECT->b6 = 4;
            o->b5 = 0xf;
            o->b6 = 0;
        }
        break;
    }
}
#endif
