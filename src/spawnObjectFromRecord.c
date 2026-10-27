// FUNC 8004cb00 1240 MAIN0
// MATCHING 8004cb00 1240
#include "TOBJ.H"
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5;
    short s6, s8, sa;
    unsigned char bc, bd, be, bf;
} Ent;
extern TObj *SPAWN_RECORD_OBJECT;
extern TObj *SPAWN_RECORD_PREV_OBJECT;
extern int DAT_1f800334;
extern int DAT_1f800338;
extern TObj *allocObjectLayer2(void);
extern TObj *ObjAlloc(void);
extern TObj *allocObjectLayer4(void);
extern TObj *allocObjectLayer5(void);
extern TObj *allocObjectLayer1(void);
extern TObj *allocObjectLayer7(void);


void spawnObjectFromRecord(Ent *e, int z)
{
    unsigned char kind;
    TObj *p;
    TObj *q2;
    unsigned char ty;
    volatile int *q;
    int idx;
    int tt;
    short s;

    kind = e->b1 & 0x7f;
    switch (kind) {
    case 2:
        p = allocObjectLayer2();
        SPAWN_RECORD_OBJECT = p;
        if (p == 0)
            return;
        if (e->bc & 0x10) {
            q = &DAT_1f800334;
            goto link;
        }
        break;
    case 3:
        if ((SPAWN_RECORD_OBJECT = ObjAlloc()) == 0)
            return;
        break;
    case 4:
        p = allocObjectLayer4();
        SPAWN_RECORD_OBJECT = p;
        if (p == 0)
            return;
        idx = e->be;
        if (idx == 0xff) {
            p->da0 = 0;
            break;
        }
        if (e->bc & 0x10) {
            q = &DAT_1f800334;
            s = idx << 2;
            tt = *q;
            q = (volatile int *)*q;
            tt = *(int *)(tt + s + 4);
            q = (volatile int *)((int)q + tt);
            p->ba4 = 1;
            p->da0 = (int)q;
        }
        break;
    case 5:
        if ((SPAWN_RECORD_OBJECT = allocObjectLayer5()) == 0)
            return;
        break;
    case 8:
        p = allocObjectLayer1();
        SPAWN_RECORD_OBJECT = p;
        if (p == 0)
            return;
        q = &DAT_1f800338;
    link:
        s = *(volatile unsigned char *)&e->be << 2; /* volatile: keeps the lbu before the *q loads */
        tt = *q;
        q = (volatile int *)*q;
        tt = *(int *)(tt + s + 4);
        q = (volatile int *)((int)q + tt);
        p->ba4 = 1;
        p->da0 = (int)q;
        break;
    case 7:
        if ((SPAWN_RECORD_OBJECT = allocObjectLayer7()) == 0)
            return;
        break;
    }
    if (z >= 0)
        SPAWN_RECORD_OBJECT->b6b = z;
    else
        SPAWN_RECORD_OBJECT->b6b = (e->bf >> 2) - 1;
    SPAWN_RECORD_OBJECT->category |= e->b1 & 0x80;
    SPAWN_RECORD_OBJECT->active = 1;
    SPAWN_RECORD_OBJECT->b1d = e->b0;
    SPAWN_RECORD_OBJECT->type = e->b2;
    SPAWN_RECORD_OBJECT->subtype = e->b4;
    SPAWN_RECORD_OBJECT->b0c = e->b3;
    SPAWN_RECORD_OBJECT->b0a = e->bc;
    SPAWN_RECORD_OBJECT->animFrame = e->b5;
    SPAWN_RECORD_OBJECT->b0f = e->bd;
    SPAWN_RECORD_OBJECT->a.raw = e->s6 << 16;
    SPAWN_RECORD_OBJECT->y.raw = e->s8 << 16;
    SPAWN_RECORD_OBJECT->b.raw = e->sa << 16;
    if (kind == 3) {
        ty = e->b2;
        if (ty == 0x18 || ty == 0x19 || ty == 0x24 || ty == 0x33)
            SPAWN_RECORD_OBJECT->box0 = e->be;
    }
    if (kind == 6 || kind == 7)
        return;
    switch (e->bf & 3) {
    case 0:
        SPAWN_RECORD_OBJECT->a.raw = e->s6 << 16;
        SPAWN_RECORD_OBJECT->y.raw = e->s8 << 16;
        SPAWN_RECORD_OBJECT->b.raw = e->sa << 16;
        SPAWN_RECORD_OBJECT->d90 = 0;
        SPAWN_RECORD_OBJECT->d94 = 0;
        break;
    case 1:
        SPAWN_RECORD_OBJECT->a.raw = e->s6 << 16;
        SPAWN_RECORD_OBJECT->y.raw = e->s8 << 16;
        SPAWN_RECORD_OBJECT->b.raw = e->sa << 16;
        SPAWN_RECORD_PREV_OBJECT = SPAWN_RECORD_OBJECT;
        SPAWN_RECORD_OBJECT->d90 = 0;
        break;
    case 2:
        SPAWN_RECORD_OBJECT->a.raw = SPAWN_RECORD_PREV_OBJECT->a.raw + (e->s6 << 16);
        SPAWN_RECORD_OBJECT->y.raw = SPAWN_RECORD_PREV_OBJECT->y.raw + (e->s8 << 16);
        SPAWN_RECORD_OBJECT->b.raw = SPAWN_RECORD_PREV_OBJECT->b.raw + (e->sa << 16);
        SPAWN_RECORD_OBJECT->d30 = e->s6 << 16;
        SPAWN_RECORD_OBJECT->d34 = e->s8 << 16;
        SPAWN_RECORD_OBJECT->d38 = e->sa << 16;
        q2 = SPAWN_RECORD_PREV_OBJECT;
        SPAWN_RECORD_PREV_OBJECT = SPAWN_RECORD_OBJECT;
        SPAWN_RECORD_OBJECT->d90 = (int)q2;
        q2->d94 = (int)SPAWN_RECORD_OBJECT;
        break;
    case 3:
        SPAWN_RECORD_OBJECT->a.raw = SPAWN_RECORD_PREV_OBJECT->a.raw + (e->s6 << 16);
        SPAWN_RECORD_OBJECT->y.raw = SPAWN_RECORD_PREV_OBJECT->y.raw + (e->s8 << 16);
        SPAWN_RECORD_OBJECT->b.raw = SPAWN_RECORD_PREV_OBJECT->b.raw + (e->sa << 16);
        SPAWN_RECORD_OBJECT->d30 = e->s6 << 16;
        SPAWN_RECORD_OBJECT->d34 = e->s8 << 16;
        SPAWN_RECORD_OBJECT->d38 = e->sa << 16;
        SPAWN_RECORD_OBJECT->d90 = (int)SPAWN_RECORD_PREV_OBJECT;
        SPAWN_RECORD_PREV_OBJECT->d94 = (int)SPAWN_RECORD_OBJECT;
        SPAWN_RECORD_OBJECT->d94 = 0;
        break;
    }
}
