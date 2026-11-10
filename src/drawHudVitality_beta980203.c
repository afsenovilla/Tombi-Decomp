// VERSION BETA_980203 beta980203 8004D64C 500
#if defined(VERSION_BETA_980203)
// Beta version: the vitality count is read from the sprite object (+0x52), not passed
#include "anim.h"
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    char pad[0x18];
    unsigned char *base;
    short **tbl;
} SPR;
extern unsigned char GAME_PLAYER_MAX_HEALTH;
extern SPRT *NEXT_PRIM;
extern char *CURRENT_OT;
extern unsigned short *PTR_DAT_800120a8[];
extern void SetSprt(SPRT *p);
extern void SetSemiTrans(SPRT *p, int abe);
extern unsigned short GetClut(int x, int y);
extern void AddPrim(void *ot, void *p);
extern void drawSpriteFrameParts(SPR *s, int x, int y, int v);
extern void AddDrawMode(int a, int b);

#define N (*(short *)((char *)s + 0x52))
void drawHudVitality(SPR *s, short x, short y)
{
    int i = 0;
    SPRT *p;
    SprFrag *e;
    int cy;

    for (; i < GAME_PLAYER_MAX_HEALTH; i++) {
        {
            p = NEXT_PRIM;
            e = (SprFrag *)(s->base + *(short *)(*(unsigned char *volatile *)&s->base + (*s->tbl[i] << 2) + 2));
            SetSprt(p);
            p->code |= 1;
            SetSemiTrans(p, 0);
            p->x0 = x + e->dx;
            p->y0 = y + e->dy;
            p->u0 = e->u;
            p->v0 = e->v;
            p->w = e->w;
            p->h = e->h;
            if (i < N) {
                if (N < 3)
                    p->clut = GetClut(0x170, 0x1f7);
                else if (N < 5)
                    p->clut = GetClut(0x170, 0x1f6);
                else if (N < 7)
                    p->clut = GetClut(0x170, 0x1f5);
                else
                    p->clut = GetClut(0x170, 0x1f4);
            } else {
                p->clut = GetClut(0x170, 0x1f8);
            }
            AddPrim(CURRENT_OT + 4, p);
            NEXT_PRIM = NEXT_PRIM + 1;
        }
    }
    drawSpriteFrameParts(s, (short)x, (short)y, *PTR_DAT_800120a8[N]);
    AddDrawMode(0x15, 1);
}
#endif
