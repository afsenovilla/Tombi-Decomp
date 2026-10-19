// FUNC 80116e54 340 X011
// MATCHING 80116e54 340
#include "TOBJ.H"

extern short D_8007A5F0[];
extern char D_80077CD0[];
extern char D_8011C46C[];
extern char D_8011C490[];
extern char D_8011C4B0[];
extern char D_8011C4D0[];
extern char D_8011C4F4[];
extern char D_8011C518[];
extern char D_8011C630[];
extern char D_8011C654[];
extern char D_8011C678[];
extern char D_8011C680[];
extern char D_8011C6A0[];
extern char D_8011C6B0[];
extern char D_8011C540[];
extern char D_8011C590[];
extern char D_8011C5C0[];
extern char D_8011C600[];
extern char D_8011C568[];
extern char D_8011C5A8[];
extern char D_8011C5E0[];
static void *D_8011C420[19] = {
    D_8011C46C, D_8011C490, D_8011C4B0, D_8011C4D0,
    D_8011C4F4, D_8011C518, D_8011C630, D_8011C654,
    D_8011C678, D_8011C680, D_8011C6A0, D_8011C6B0,
    D_8011C540, D_8011C590, D_8011C5C0, D_8011C600,
    D_8011C568, D_8011C5A8, D_8011C5E0,
};
extern int AnimAdvance(TObj *);
extern void applyAnimVelocityX(TObj *, int);
extern void FUN_8001fe6c(TObj *);

static inline void SetAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void flyingOctopusMove(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->movetab = D_80077CD0;
        o->d88 = 0;
        o->velV = 0x60;
        o->wac = 0;
        SetAnim(o, D_8011C420[0]);
        break;
    case 1:
        AnimAdvance(o);
        applyAnimVelocityX(o, o->animFrame);
        o->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
        o->d88 = (o->d88 + 1) & 0xff;
        if (o->d88 == 0)
            o->state++;
        break;
    case 2:
        AnimAdvance(o);
        o->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
        o->d88 = (o->d88 + 1) & 0xff;
        if (o->d88 == 0)
            o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->state = 1;
        o->animFrame = 1 - o->animFrame;
        break;
    }
}
