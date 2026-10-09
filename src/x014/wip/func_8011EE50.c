// FUNC 8011ee50 968 X014
/* score 106: logic complete. Main diff: the game reads the 4 box bytes as lbu (p); addiu p,1 ... lbu 1(p) with p = d90 + 0x3c kept in a register (no offset folding); every C form tried (*p++, p++ statements, inline helper, loop, int pointer) folds to lbu 0x3c..0x3f(base). The do/while(0) macro form (debt) improved the rest of the schedule. Also case-0 store order around D_800A4553/D_1F800190. */
#include "TOBJ.H"
extern int D_1F80018C, D_1F800190;
extern unsigned char D_8009C93F[], D_8009C93E[], D_8009C942[];
extern unsigned char D_800A4553;
extern int D_800A4568;
extern char D_80077D6C[], D_80077D0C[];
extern void AnimLoadDuration(TObj *);
extern void FUN_8001faf4(TObj *);
extern int func_8011D7B0(TObj *);
extern int func_8011D6BC(TObj *);
extern void func_8011D604(TObj *);
extern void FUN_8001f96c(int, short, short, short);
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void func_80116C10(void);

#define setbox(o, q) do { unsigned char *p_ = (q); (o)->box0 = *p_++; (o)->box1 = *p_++; (o)->box2 = *p_++; (o)->box3 = *p_++; } while (0)
static __inline__ void setbox_unused(TObj *o, unsigned char *p)
{
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p++;
}

void func_8011EE50(TObj *o)
{
    unsigned char *p;
    int r;

    switch (o->state) {
    case 0:
        D_8009C93F[0] = 1;
        D_8009C93E[0] = 1;
        D_8009C942[0] = 1;
        D_800A4553 = 3;
        D_800A4568 = 0;
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        p = (unsigned char *)o->d90 + 0x3c;
        o->b9c = 0;
        o->b6a = 1;
        o->state++;
        setbox(o, p);
        o->wac = 0xf;
        o->anim = ((void **)*(int *)&o->wa8)[15];
        AnimLoadDuration(o);
        o->movetab = D_80077D6C;
        break;
    case 1:
        if (o->animFrame & 2) {
            o->y.raw += 0x48000;
            r = func_8011D7B0(o);
        } else {
            FUN_8001faf4(o);
            r = func_8011D6BC(o);
        }
        if (r) {
            o->b6a = 0;
            o->state++;
            FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            playSFX(6);
        }
        if (o->animFrame & 1) o->d8c = (unsigned char)(o->d8c + 0x14);
        else o->d8c = (unsigned char)(o->d8c - 0x14);
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        break;
    case 2:
        if (o->animFrame & 2) o->w7a = o->animFrame & 1;
        else o->w7a = ~o->animFrame & 1;
        playSFX(7);
        o->velV = -0x400;
        o->b9c = 1;
        o->b6a = 0;
        o->movetab = D_80077D0C;
        o->state++;
    case 3:
        if (o->w7a) o->d8c = (unsigned char)(o->d8c + 0x14);
        else o->d8c = (unsigned char)(o->d8c - 0x14);
        FUN_8001fa88(o, o->w7a);
        func_8011D604(o);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (func_8011D7B0(o)) {
                o->b9c = 0;
                o->d8c = 0;
                o->state++;
                o->animFrame &= 1;
            }
        }
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        break;
    case 4:
        p = (unsigned char *)o->d90 + 0x34;
        setbox(o, p);
        o->wac = 0xd;
        o->anim = ((void **)*(int *)&o->wa8)[13];
        AnimLoadDuration(o);
        o->timer = 0x3c;
        o->state++;
        D_1F800190 = o->y.raw;
        D_800A4553 = 6;
        D_1F80018C = o->h->raw;
        func_80116C10();
        break;
    case 5:
        if (--o->timer == -1) {
            o->d8c = 0;
            o->step = 2;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
