// FUNC 80118028 644 X002
// MATCHING 80118028 644
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct {
    TObj t;
    char pad[0x12];
    unsigned short wd2;
} TO2;
extern AT D_8011C778[];
extern short D_1F8001C6;
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int Rand(void);
extern int showMessageBoxDirect(int, int, V6 *);

void sevenFriendsGoodLuckTalk(TO2 *o)
{
    V6 v;
    TObj *p;

    switch (o->t.state) {
    case 0:
        if (o->t.b68 == 0) break;
        v = *(V6 *)&o->t.a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        GV2.bb0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        if (o->wd2 != 0) {
            o->t.anim = D_8011C778[o->t.subtype].anims[0];
            AnimJump(&o->t, 0);
            o->wd2 = 0;
        }
        o->t.wbc = o->t.animFrame;
        o->t.animFrame = o->t.b68 & 1;
        switch (Rand() & 3) {
        case 0:
        case 1:
            o->t.d90 = showMessageBoxDirect(3, 0, &v);
            break;
        case 2:
            o->t.d90 = showMessageBoxDirect(3, 1, &v);
            break;
        case 3:
            o->t.d90 = showMessageBoxDirect(3, 2, &v);
            break;
        }
        o->t.state++;
        break;
    case 1:
        AnimAdvance(&o->t);
        p = (TObj *)o->t.d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        if (o->wd2 != 0) {
            o->t.anim = D_8011C778[o->t.subtype].anims[0];
            AnimJump(&o->t, 0);
            o->wd2 = 0;
        }
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->t.timer = 4;
        o->t.state = 9;
        break;
    case 9:
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->t.animFrame = o->t.wbc;
        D_1F8001C6 = 0;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->t.b68 = 0;
        o->t.step = 0;
        o->t.state = 0;
        break;
    }
}
