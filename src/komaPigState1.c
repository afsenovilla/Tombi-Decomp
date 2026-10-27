// FUNC 80129bb0 1028 X000
// MATCHING 80129bb0 1028
#include "TOBJ.H"
typedef struct { char p0[0xa]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char MOVE_TABLE_2[];
extern unsigned char KOMA_PIG_ANIM_BOXES[];
extern unsigned short *D_8013A180[], *D_8013A18C[], *D_8013A178[];
extern void ObjSetFacingToPlayer(TObj *);
extern void applyFrameVelocityXY(TObj *);
extern int enemyProbeGround(TObj *);
extern int enemyCheckWallAhead(TObj *);
extern unsigned char enemyPickActionByPlayerDistance(TObj *);
extern int AnimAdvanceWithBox(TObj *);
extern void SfxPlay2(int, int);

static __inline__ short ahead(TObj *o)
{
    if (o->animFrame != 0) {
        if (o->h->p.whole >= *(short *)0x1F80016A - 0x20) return 0;
        return 1;
    }
    if (o->h->p.whole > *(short *)0x1F80016A + 0x20) return 1;
    return 0;
}
void komaPigState1(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    unsigned char *p;
    unsigned short *a;
    short s;
    unsigned char r;
    short f;
    int k;

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        x->w0e = 0;
        o->b69 = 0;
        o->substep++;
        ObjSetFacingToPlayer(o);
        o->movetab = MOVE_TABLE_2;
        o->wac = 8;
        {
        unsigned short *a;
        unsigned char *p;
        a = D_8013A180[0];
        o->anim = a;
        p = &KOMA_PIG_ANIM_BOXES[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        }
    case 1:
        AnimAdvanceWithBox(o);
        applyFrameVelocityXY(o);
        if (enemyProbeGround(o) == 0 && x->w0e++ > 3) {
            x->b0a = 1;
            x->b0b = 2;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (enemyCheckWallAhead(o) != 0) {
            o->substep = 5;
            break;
        }
        if (ahead(o)) o->substep = 2;
        break;
    case 2:
        o->timer = 0x5a;
        o->b9d = 0;
        o->wac = 0xb;
        o->substep++;
        a = D_8013A18C[0];
        o->anim = a;
        {
        unsigned char *p;
        p = &KOMA_PIG_ANIM_BOXES[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        }
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        o->y.p.whole += 2;
        enemyProbeGround(o);
        break;
    case 3:
        if (--o->timer == 0) o->substep++;
        AnimAdvanceWithBox(o);
        o->y.p.whole += 2;
        enemyProbeGround(o);
        break;
    case 4:
        o->state = 0;
        o->substep = 0;
        r = enemyPickActionByPlayerDistance(o);
        if (r != 0xff) o->state = r;
        break;
    case 5:
        o->wac = 6;
        o->substep++;
        a = D_8013A178[0];
        o->anim = a;
        {
        unsigned char *p;
        p = &KOMA_PIG_ANIM_BOXES[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        }
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 6:
        if (o->animTimer == 1) {
            k = AnimAdvanceWithBox(o);
            s = ((unsigned short *)o->anim)[2];
            if (s != 0) SfxPlay2((short)s >> 8, s & 0xff);
        } else {
            k = AnimAdvanceWithBox(o);
        }
        if (k != 0) {
            r = enemyPickActionByPlayerDistance(o);
            if (r != 0xff) {
                o->state = r;
                o->substep = 0;
            } else {
                o->state = 0;
                o->substep = 0;
                o->animFrame = 1 - o->animFrame;
            }
        }
        break;
    }
}
