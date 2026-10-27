// FUNC 801037d8 1252 X000
// MATCHING 801037d8 1252
// VERSION SLPS_01144 x00 801032E8 1252
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 801037D8 (FUN_801037d8.c): 128 -> 32768, 32 -> 8192.
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
extern void FUN_8010eaf8(O *);
extern void FUN_8001fd94(O *);
extern void FUN_80103628(O *);
extern void playerSetSlopeTilt(O *, int);
extern short FUN_8003fd78(O *, int, int);

#define PADCHK() { volatile unsigned short *pad = &PAD_PRESSED; if (*pad & 0x8000) o->w2e = 1; if (*pad & 0x2000) o->w2e = 0; }

void FUN_801037d8(O *o)
{
    short flag;
    O *p;

    o->vy += 8;
    if (o->vy > 0x680) o->vy = 0x680;
    if (o->vy < -0x680) o->vy = -0x680;
    switch (o->b6) {
    case 1:
        o->s20++;
        FUN_8001fd94(o);
        FUN_8010eaf8(o);
        *(int *)o->h += o->vx << 8;
        flag = 0;
        o->y16 += 8;
        if (o->y16 + BITTEN_OBJECT->s70 > BITTEN_OBJECT->y16) {
            o->y16 = BITTEN_OBJECT->y16 - BITTEN_OBJECT->s70;
            flag = 1;
        }
        if (o->vx < 0) {
            o->h->s2 -= 8;
            if (o->h->s2 <= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) FUN_80103628(o);
            }
        } else {
            o->h->s2 += 8;
            if (o->h->s2 >= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) FUN_80103628(o);
            }
        }
        if (o->b69 != 0 || FUN_8003fd78(o, (short)(BITTEN_OBJECT->y16 - o->y16 - o->s70 + (BITTEN_OBJECT->s72 - BITTEN_OBJECT->s70)), 1) || o->s20 > 10) {
            FUN_80103628(o);
        }
        return;
    case 2:
        switch (BITTEN_OBJECT->b2) {
        case 0x38:
            PADCHK();
            {
                O *q = BITTEN_OBJECT;
                o->h->s2 = q->h->s2;
                o->y16 = q->y16 - q->s70;
            }
            break;
        case 10:
            o->h->s2 = BITTEN_OBJECT->h->s2;
            o->y16 = BITTEN_OBJECT->y16 - BITTEN_OBJECT->s70;
            o->b9c = 0;
            PADCHK();
            break;
        default:
            PADCHK();
            p = BITTEN_OBJECT;
            p->w2e = o->w2e & 1;
            p->h->s2 = o->h->s2;
            p->y16 = o->y16 + p->s70;
            FUN_8001fd94(o);
            if (o->b69 != 0 || FUN_8003fd78(o, (short)(BITTEN_OBJECT->y16 - o->y16 - o->s70 + (BITTEN_OBJECT->s72 - BITTEN_OBJECT->s70)), 1)) {
                o->b69 = 0;
                o->bac = 3;
                o->b9c = 0;
                o->vx = 0;
                o->vy = 0;
                o->s80 = 0;
                o->s82 = 0;
                o->wb2 = 0;
                playerSetSlopeTilt(o, 0);
                BITTEN_OBJECT->a8c = ((unsigned char *)o)[0x8c];
            }
            break;
        }
        if (JOYPAD_STATE & BUTTON_MASK_B) {
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            o->vx = 0;
            o->vy = 0;
            switch (BITTEN_OBJECT->b2) {
            case 0: case 0x13: case 0x1f: case 0x28:
            case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: case 0x40: case 0x41: case 0x42:
                BITTEN_OBJECT->b6 = 7;
                break;
            case 10:
                BITTEN_OBJECT->b6 = BITTEN_OBJECT->b6 + 1;
                break;
            case 0x38:
                if (BITTEN_OBJECT->b3 != 0) {
                    BITTEN_OBJECT->b6 = 6;
                    break;
                }
            default:
                BITTEN_OBJECT->b6 = 4;
                break;
            }
            o->b5 = 0xf;
            o->b6 = 0;
        }
        break;
    }
}
#else
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
extern void playerBiteHoldAttach(O *);
extern void playerSetSlopeTilt(O *, int);
extern short FUN_8003fd78(O *, int, int);

#define PADCHK() { volatile unsigned short *pad = &PAD_PRESSED; if (*pad & 0x80) o->w2e = 1; if (*pad & 0x20) o->w2e = 0; }

void playerBiteHoldGeneric(O *o)
{
    short flag;
    O *p;

    o->vy += 8;
    if (o->vy > 0x680) o->vy = 0x680;
    if (o->vy < -0x680) o->vy = -0x680;
    switch (o->b6) {
    case 1:
        o->s20++;
        FUN_8001fd94(o);
        playerAirAccelerateX(o);
        *(int *)o->h += o->vx << 8;
        flag = 0;
        o->y16 += 8;
        if (o->y16 + BITTEN_OBJECT->s70 > BITTEN_OBJECT->y16) {
            o->y16 = BITTEN_OBJECT->y16 - BITTEN_OBJECT->s70;
            flag = 1;
        }
        if (o->vx < 0) {
            o->h->s2 -= 8;
            if (o->h->s2 <= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) playerBiteHoldAttach(o);
            }
        } else {
            o->h->s2 += 8;
            if (o->h->s2 >= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) playerBiteHoldAttach(o);
            }
        }
        if (o->b69 != 0 || FUN_8003fd78(o, (short)(BITTEN_OBJECT->y16 - o->y16 - o->s70 + (BITTEN_OBJECT->s72 - BITTEN_OBJECT->s70)), 1) || o->s20 > 10) {
            playerBiteHoldAttach(o);
        }
        return;
    case 2:
        switch (BITTEN_OBJECT->b2) {
        case 0x38:
            PADCHK();
            {
                O *q = BITTEN_OBJECT;
                o->h->s2 = q->h->s2;
                o->y16 = q->y16 - q->s70;
            }
            break;
        case 10:
            o->h->s2 = BITTEN_OBJECT->h->s2;
            o->y16 = BITTEN_OBJECT->y16 - BITTEN_OBJECT->s70;
            o->b9c = 0;
            PADCHK();
            break;
        default:
            PADCHK();
            p = BITTEN_OBJECT;
            p->w2e = o->w2e & 1;
            p->h->s2 = o->h->s2;
            p->y16 = o->y16 + p->s70;
            FUN_8001fd94(o);
            if (o->b69 != 0 || FUN_8003fd78(o, (short)(BITTEN_OBJECT->y16 - o->y16 - o->s70 + (BITTEN_OBJECT->s72 - BITTEN_OBJECT->s70)), 1)) {
                o->b69 = 0;
                o->bac = 3;
                o->b9c = 0;
                o->vx = 0;
                o->vy = 0;
                o->s80 = 0;
                o->s82 = 0;
                o->wb2 = 0;
                playerSetSlopeTilt(o, 0);
                BITTEN_OBJECT->a8c = ((unsigned char *)o)[0x8c];
            }
            break;
        }
        if (JOYPAD_STATE & BUTTON_MASK_B) {
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            o->vx = 0;
            o->vy = 0;
            switch (BITTEN_OBJECT->b2) {
            case 0: case 0x13: case 0x1f: case 0x28:
            case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: case 0x40: case 0x41: case 0x42:
                BITTEN_OBJECT->b6 = 7;
                break;
            case 10:
                BITTEN_OBJECT->b6 = BITTEN_OBJECT->b6 + 1;
                break;
            case 0x38:
                if (BITTEN_OBJECT->b3 != 0) {
                    BITTEN_OBJECT->b6 = 6;
                    break;
                }
            default:
                BITTEN_OBJECT->b6 = 4;
                break;
            }
            o->b5 = 0xf;
            o->b6 = 0;
        }
        break;
    }
}
#endif
