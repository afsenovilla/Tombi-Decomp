// FUNC 8002b0bc 780 MAIN0
// MATCHING 8002b0bc 780
#include "gcd.h"
#include "gvars.h"
#include "player.h"
typedef struct S {
    unsigned char b00, b01; char p0[2]; unsigned char b04; char p1[5]; unsigned char b0a; char p2[0x6c - 0xb];
    unsigned short w6c, w6e, w70, w72; char p3[0xa4 - 0x74]; unsigned char ba4; char p4[0xbc - 0xa5];
    unsigned short wbc, wbe, wc0, wc2; char p5[0xce - 0xc4]; unsigned short wce, wd0, wd2;
} S;
extern char SPRITE_SLOTS[];
extern char D_800B0BBA[];
extern unsigned char D_800B0BBC[], D_800B0BBD[], D_800B0BBE[], D_800B0BBF[];
extern signed char GAME_PLAYER_MODE;
extern unsigned char D_8009C933;
extern unsigned short acquireSpriteSlot(int);
extern void l8Type01MoveStep(S *);
extern void ObjListPush_1F800230(S *);
extern void setEventStarted(int, int, int);
extern void freeObjectLayer8(S *);
void updateL8Type01Object(S *o)
{
    int x, y, x2, y2, c0, c1, c2, c3;
    unsigned short m;
    switch (o->b04) {
    case 0:
        o->b0a = 0x14;
        o->ba4 = 1;
        o->wc2 = acquireSpriteSlot(o->wc0);
        if (o->b00 == 1) {
            short sl = acquireSpriteSlot(o->wbc);
            int off;
            o->wbe = sl;
            off = sl * 10;
            c1 = D_800B0BBD[off];
            c0 = D_800B0BBC[off];
            c2 = D_800B0BBE[off];
            c3 = D_800B0BBF[off];
            x = c0 << 2;
            y = c1 << 8;
            o->w6e = x | y;
            x2 = x + c2;
            y2 = y + (c3 << 8);
            o->w72 = y | x2;
            o->w6c = x | y2;
            o->w70 = x2 | y2;
        }
        o->b04++;
        break;
    case 1:
        if (GCD.bcda2 != 1)
            l8Type01MoveStep(o);
        if (o->b00 & 1) {
            o->b01 = 1;
            ObjListPush_1F800230(o);
        }
        break;
    case 2:
        if (o->b00 == 1) {
            int off2 = o->wbe * 10;
            if (--*(short *)(D_800B0BBA + off2) == 0)
                *(short *)(SPRITE_SLOTS + o->wbe * 10) = -1;
        }
        if (GAME_PLAYER_MODE != 3) {
            if (o->wce & 0x7fff) {
                if (o->wd2)
                    setEventStarted(o->wd2, 0, 3);
                m = o->wce & 0x7fff;
                if (m == 2)
                    break;
                if (o->wd0 == 0 || m == 3) {
                    ((unsigned char*)&GVARS.b3f)[0] = 0;
                    ((unsigned char*)&GVARS.b42)[0] = 0;
                    if ((o->wce & 0x7fff) != 4 && (PLAYER.obj.b9e == 0 || (*(unsigned char*)&PLAYER.obj.wac) < 2)) {
                        PLAYER.obj.b04 = 1;
                        PLAYER.obj.step = 0;
                        PLAYER.obj.state = 0;
                        PLAYER.obj.substep = 0;
                    } else {
                        GVARS.b3e = 0;
                    }
                }
            }
            if ((o->wce & 0x8000) && (o->wce & 0x7fff) != 2)
                D_8009C933 = 0;
        }
        o->b04++;
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
