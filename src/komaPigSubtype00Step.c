// FUNC 8012b5d0 1220 X000
// MATCHING 8012b5d0 1220
// VERSION SLPM_80165 x00 801214B4 1116
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 8012B5D0 (komaPigSubtype00Step.c): State 6 asks FUN_80127720 instead of the inline range test.
#include "spad.h"
#include "TOBJ.H"
extern char MOVE_TABLE_1[], MOVE_TABLE_0[];
extern unsigned char KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[], KOMA_PIG_WAIT_ANIM_BY_SUBTYPE[];
extern unsigned short *KOMA_PIG_ANIMS[];
extern unsigned short *D_8013A21C, *D_8013A220, *D_8013A268;
extern unsigned char KOMA_PIG_ANIM_BOXES[];
extern int AnimAdvanceWithBox(TObj *);
extern void applyFrameVelocityY(TObj *);
extern void applyFrameVelocityXY(TObj *);
extern int FUN_801274cc(TObj *);
extern int FUN_801275e4(TObj *);
extern int FUN_80127720(TObj *);
extern void ObjSetFacingToPlayer(TObj *);

static __inline__ void SetBox(TObj *o)
{
    unsigned char *p;
    p = &KOMA_PIG_ANIM_BOXES[((unsigned short *)o->anim)[1] * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p;
    o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
}

static __inline__ short InRange(TObj *o)
{
    if ((unsigned short)(o->d->p.whole - (*(unsigned short*)((char*)&SPAD.i170 + 2)) + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0x46) >= 0x6f) return 0;
    return (unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x80) < 0x101;
}

void komaPigSubtype00Step(TObj *o)
{
    short *q = &o->wb4;
    int r;
    unsigned short dx;
    unsigned short dy;
    switch (o->state) {
    case 0:
        o->movetab = MOVE_TABLE_1;
        o->b69 = 0;
        o->state++;
        o->wac = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
        o->anim = KOMA_PIG_ANIMS[o->wac];
        SetBox(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        applyFrameVelocityY(o);
        if (FUN_801274cc(o)) {
            int t = (unsigned short)q[1];
            o->timer = 0;
            o->d8c = t;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        if (o->b0c == 1)
            break;
        dx = o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2));
        dx += 0xa0;
        if ((unsigned short)(o->d->p.whole - (*(unsigned short*)((char*)&SPAD.i170 + 2)) + 0x2d) >= 0x5b
            || dx >= 0x141) {
            o->timer = 0;
        } else if (o->timer++ > 0x3c) {
            o->state++;
        }
        break;
    case 3:
        o->timer = 0x78;
        o->state++;
        o->wac = KOMA_PIG_WAIT_ANIM_BY_SUBTYPE[o->subtype];
        o->anim = KOMA_PIG_ANIMS[o->wac];
        SetBox(o);
        break;
    case 4:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (o->subtype) {
                r = FUN_80127720(o);
                if ((unsigned char)r == 0xff) {
                    o->state = 2;
                    o->wac = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
                    o->anim = KOMA_PIG_ANIMS[o->wac];
                    SetBox(o);
                } else {
                    o->state = r;
                    o->substep = 0;
                    o->step++;
                }
            } else {
                o->timer = 0x78;
                o->state++;
                ObjSetFacingToPlayer(o);
                *(char **)((char *)o + 0x28) = MOVE_TABLE_0;
                *(short *)((char *)o + 0xac) = 0x2f;
                o->anim = D_8013A21C;
                SetBox(o);
            }
        }
        break;
    case 5:
        AnimAdvanceWithBox(o);
        applyFrameVelocityXY(o);
        if (FUN_801275e4(o))
            o->timer = 0;
        FUN_801274cc(o);
        if (--o->timer == -1) {
            *(short *)((char *)o + 0x20) = 0x3c;
            *(short *)((char *)o + 0xac) = 0x30;
            o->state++;
            o->anim = D_8013A220;
            SetBox(o);
        }
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if ((unsigned char)FUN_80127720(o) == 0xff) {
                o->state = 2;
                o->wac = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
                o->anim = KOMA_PIG_ANIMS[o->wac];
                SetBox(o);
            } else {
                *(short *)((char *)o + 0xac) = 0x42;
                o->state++;
                o->anim = D_8013A268;
                SetBox(o);
            }
        }
        break;
    case 7:
        if (AnimAdvanceWithBox(o)) {
            o->step++;
            ObjSetFacingToPlayer(o);
            o->state = 0;
            o->substep = 0;
            r = FUN_80127720(o);
            if ((unsigned char)r != 0xff)
                o->state = r;
        }
        break;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"
extern char MOVE_TABLE_1[], MOVE_TABLE_0[];
extern unsigned char KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[], KOMA_PIG_WAIT_ANIM_BY_SUBTYPE[];
extern unsigned short *KOMA_PIG_ANIMS[];
extern unsigned short *D_8013A21C, *D_8013A220, *D_8013A268;
extern unsigned char KOMA_PIG_ANIM_BOXES[];
extern int AnimAdvanceWithBox(TObj *);
extern void applyFrameVelocityY(TObj *);
extern void applyFrameVelocityXY(TObj *);
extern int enemyProbeGround(TObj *);
extern int enemyCheckWallAhead(TObj *);
extern int enemyPickActionByPlayerDistance(TObj *);
extern void ObjSetFacingToPlayer(TObj *);

static __inline__ void SetBox(TObj *o)
{
    unsigned char *p;
    p = &KOMA_PIG_ANIM_BOXES[((unsigned short *)o->anim)[1] * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p;
    o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
}

static __inline__ short InRange(TObj *o)
{
    if ((unsigned short)(o->d->p.whole - (*(unsigned short*)((char*)&SPAD.i170 + 2)) + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0x46) >= 0x6f) return 0;
    return (unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x80) < 0x101;
}

void komaPigSubtype00Step(TObj *o)
{
    short *q = &o->wb4;
    int r;
    unsigned short dx;
    unsigned short dy;
    switch (o->state) {
    case 0:
        o->movetab = MOVE_TABLE_1;
        o->b69 = 0;
        o->state++;
        o->wac = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
        o->anim = KOMA_PIG_ANIMS[o->wac];
        SetBox(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        applyFrameVelocityY(o);
        if (enemyProbeGround(o)) {
            int t = (unsigned short)q[1];
            o->timer = 0;
            o->d8c = t;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        if (o->b0c == 1)
            break;
        dx = o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2));
        dx += 0xa0;
        if ((unsigned short)(o->d->p.whole - (*(unsigned short*)((char*)&SPAD.i170 + 2)) + 0x2d) >= 0x5b
            || dx >= 0x141) {
            o->timer = 0;
        } else if (o->timer++ > 0x3c) {
            o->state++;
        }
        break;
    case 3:
        o->timer = 0x78;
        o->state++;
        o->wac = KOMA_PIG_WAIT_ANIM_BY_SUBTYPE[o->subtype];
        o->anim = KOMA_PIG_ANIMS[o->wac];
        SetBox(o);
        break;
    case 4:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (o->subtype) {
                r = enemyPickActionByPlayerDistance(o);
                if ((unsigned char)r == 0xff) {
                    o->state = 2;
                    o->wac = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
                    o->anim = KOMA_PIG_ANIMS[o->wac];
                    SetBox(o);
                } else {
                    o->state = r;
                    o->substep = 0;
                    o->step++;
                }
            } else {
                o->timer = 0x78;
                o->state++;
                ObjSetFacingToPlayer(o);
                *(char **)((char *)o + 0x28) = MOVE_TABLE_0;
                *(short *)((char *)o + 0xac) = 0x2f;
                o->anim = D_8013A21C;
                SetBox(o);
            }
        }
        break;
    case 5:
        AnimAdvanceWithBox(o);
        applyFrameVelocityXY(o);
        if (enemyCheckWallAhead(o))
            o->timer = 0;
        enemyProbeGround(o);
        if (--o->timer == -1) {
            *(short *)((char *)o + 0x20) = 0x3c;
            *(short *)((char *)o + 0xac) = 0x30;
            o->state++;
            o->anim = D_8013A220;
            SetBox(o);
        }
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (InRange(o)) {
                o->state = 2;
                o->wac = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
                o->anim = KOMA_PIG_ANIMS[o->wac];
                SetBox(o);
            } else {
                *(short *)((char *)o + 0xac) = 0x42;
                o->state++;
                o->anim = D_8013A268;
                SetBox(o);
            }
        }
        break;
    case 7:
        if (AnimAdvanceWithBox(o)) {
            o->step++;
            ObjSetFacingToPlayer(o);
            o->state = 0;
            o->substep = 0;
            r = enemyPickActionByPlayerDistance(o);
            if ((unsigned char)r != 0xff)
                o->state = r;
        }
        break;
    }
}
#endif
