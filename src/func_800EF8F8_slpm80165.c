// VERSION SLPM_80165 x00 800EF8F8 580
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerEventWalk (src/x010/playerEventWalk.c): playerSetAngleFromSlope inline, the move by angle
// and speed takes them as arguments, the drift is added here, and the player is pushed down 3 or 5 pixels a frame
// (by the slope) unless +0xAD is set.
typedef struct { char p0[5]; unsigned char step, state; char p1[0x24 - 7]; int anim; char p2[4]; unsigned int fl; } H;
typedef struct { char p0[0x2e]; unsigned short af; } P;
typedef struct { char p0[8]; unsigned char b8; } Q;
typedef struct {
    char p0[5]; unsigned char step, state; char p1[0x24 - 7]; int anim; char p2[4]; union { unsigned int fl; struct { unsigned short t, af; } s; } u;
    char p3[0x8c - 0x30]; int d8c; char p4[0xa0 - 0x90]; unsigned char da0, pa1, da2, da3; char p5[0xac - 0xa4]; unsigned char wac;
} O;
extern P *PLAYER_EXT;
extern Q *DAT_8009c330q;
extern unsigned short TICK_COUNT;
extern char A0de0[], A0e2c[];
extern void FUN_8001fec0(O *), playerSelectGroundAnim(O *), playerStickToGround(O *), FUN_8001e4f0(int);
extern void playerMoveAt(O *, int, int) asm("func_800202FC");
extern short D_8009A7F4, D_8009A7F6;
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define U8(o, k) (*(unsigned char *)((char *)(o) + (k)))

static __inline__ void demoSetAngleFromSlope(void *o)
{
    short v = *(short *)((char *)o + 0xb0);
    if (v < 0) *(short *)((char *)o + 0xb6) = (short)((v << 2) + 0x100) & 0xff;
    else if (v > 0) *(short *)((char *)o + 0xb6) = (short)(v << 2) & 0xff;
    else *(short *)((char *)o + 0xb6) = 0;
}

void func_800EF8F8(O *o)
{
    o->wac = 0;
    if (o->u.s.af < 2) {
        switch (o->state) {
        case 0:
            o->d8c = 0;
            ((Q *)PLAYER_EXT)->b8 = 0;
            o->state = o->state + 1;
        case 1:
            break;
        default:
            return;
        }
        FUN_8001fec0(o);
        demoSetAngleFromSlope(o);
        playerSelectGroundAnim(o);
        playerMoveAt(o, S16(o, 0xb6), S16(o, 0xb2));
        *(int *)(*(char **)((char *)o + 0x40)) += D_8009A7F4 << 8;
        *(int *)((char *)o + 0x14) += D_8009A7F6 << 8;
        playerStickToGround(o);
        if (U8(o, 0xad) == 0) {
            if ((unsigned short)(S16(o, 0xb0) + 5) >= 11)
                *(int *)((char *)o + 0x14) += 0x50000;
            else
                *(int *)((char *)o + 0x14) += 0x30000;
        }
        if ((TICK_COUNT & 7) == 0) {
            if ((TICK_COUNT & 0xf) == 0)
                FUN_8001e4f0(0);
            else
                FUN_8001e4f0(1);
        }
    } else {
        if ((o->u.fl & 0xa0000) == 0xa0000 && (o->da0 & 2)) {
            PLAYER_EXT->af = 0xffff;
            o->anim = (int)A0de0;
            o->da2 = 1;
        } else if ((o->u.fl & 0xc0000) == 0xc0000 && (o->da0 & 1)) {
            PLAYER_EXT->af = 0xffff;
            o->anim = (int)A0e2c;
            o->da3 = 1;
        } else {
            playerSelectGroundAnim(o);
        }
        o->step = 0;
        o->state = 0;
    }
}
#endif
