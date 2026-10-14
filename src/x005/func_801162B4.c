// FUNC 801162b4 988 X005
// MATCHING 801162b4 988
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { unsigned char c[12]; } B12;
typedef struct { void **p; int x, y; } AT;
extern int showMessageBoxDirect(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void setEventComplete(int, int);
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CDD2;
extern unsigned char D_8009CF00;
extern unsigned char D_8009CF04;
extern unsigned char D_8009CEED;
extern short D_1F8001C6;
extern AT D_801176CC[];

void func_801162B4(TObj *o)
{
    B12 v;
    int t;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(B12 *)&o->a;
        if (D_8009CDD5 == 2) {
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                o->d90 = showMessageBoxDirect(2, 0x32, &v);
                o->state = 5;
            }
        } else if (D_8009CDD5 == 1) {
            o->d90 = showMessageBoxDirect(2, 0x29, &v);
            o->state++;
        } else {
            switch (D_8009CDD2) {
            case 0:
            case 1:
                o->d90 = showMessageBoxDirect(2, 0x27, &v);
                o->state++;
                break;
            case 2:
                o->d90 = showMessageBoxDirect(2, 0x28, &v);
                o->state = 2;
                break;
            default:
                o->d90 = showMessageBoxDirect(2, 0x29, &v);
                o->state++;
                break;
            }
        }
        ((unsigned char*)&GVARS.b42)[0] = 1;
        GV2.bb0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        break;
    case 1:
        if (((unsigned char *)o->d90)[4] == 2) {
            ((unsigned char *)o->d90)[4] = 3;
            D_1F8001C6 = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((short*)&PLAYER.obj.wb2)[0] = 0;
            if (((unsigned char*)&GVARS.b3f)[0] == 0) {
                ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
                ((unsigned char*)&PLAYER.obj.step)[0] = 0;
                ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            }
            t = (unsigned short)o->wbc;
            o->b68 = 0;
            goto reset;
        }
        break;
    case 2:
        if (((unsigned char *)o->d90)[4] == 2) {
            ((unsigned char *)o->d90)[4] = 3;
            o->animFrame = 1;
            o->active = 4;
            o->anim = D_801176CC[o->subtype].p[1];
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += -0x20000;
        if (o->visible == 0) o->state++;
        break;
    case 4:
        setEventComplete(0x2e, 0);
        D_1F8001C6 = 0;
        D_8009CEED = 2;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        if (((unsigned char*)&GVARS.b3f)[0] == 0) {
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        }
        t = (unsigned short)o->wbc;
        o->b68 = 0;
        goto reset;
    case 5:
        if (((unsigned char *)o->d90)[4] == 2) {
            ((unsigned char *)o->d90)[4] = 3;
            o->anim = D_801176CC[o->subtype].p[0];
            AnimLoadDuration(o);
            setEventComplete(0x31, 0);
            o->state++;
        }
        break;
    case 6:
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        t = (unsigned short)o->wbc;
        D_1F8001C6 = 0;
        o->b68 = 0;
    reset:
        o->step = 0;
        o->state = 0;
        o->animFrame = t;
        break;
    }
}
