// FUNC 8012cae4 532 X000
// MATCHING 8012cae4 532
#include "TOBJ.H"
extern char MOVE_TABLE_1[];
extern unsigned char KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[];
extern unsigned short *KOMA_PIG_ANIMS[];
extern unsigned char KOMA_PIG_ANIM_BOXES[];
extern unsigned short PLAYER_H_WHOLE, PLAYER_D_WHOLE;
extern void FUN_80127214(TObj *);
extern void applyFrameVelocityY(TObj *);
extern int enemyProbeGround(TObj *);

void komaPigSubtype06Step(TObj *o)
{
    unsigned char *p;
    unsigned short k;
    int dx;
    switch (o->state) {
    case 0:
        o->movetab = MOVE_TABLE_1;
        o->state++;
        o->b69 = 0;
        k = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
        o->wac = k;
        o->anim = KOMA_PIG_ANIMS[k];
        p = &KOMA_PIG_ANIM_BOXES[((unsigned short *)o->anim)[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        FUN_80127214(o);
        applyFrameVelocityY(o);
        if (enemyProbeGround(o) != 0) {
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        FUN_80127214(o);
        dx = (unsigned short)o->h->p.whole - PLAYER_H_WHOLE + 0x60;
        if ((unsigned short)(o->d->p.whole - PLAYER_D_WHOLE + 0x2d) >= 0x5b || (unsigned short)dx >= 0xc1) {
            o->timer = 0;
        } else if (o->timer++ >= 0x46) {
            o->timer = 10;
            o->state++;
        }
        break;
    case 3:
        FUN_80127214(o);
        if (--o->timer == -1) {
            o->step = 1;
            o->state = 1;
            o->substep = 2;
            o->b69 = 0;
            o->b68 = 0;
        }
        break;
    }
}
