// VERSION BETA_980203 beta980203 800311E4 972
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8002F994_slps01144.c (func_8002F994_slps01144) (0.967 masked), edited (tools/jp_autofix.py edits) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#include "spad.h"
#include "TOBJ.H"
extern void *D_80011F00;
extern char D_80010618[], D_80010624[], D_80010634[], D_80010644[], D_80010658[], D_80010664[], D_80010670[];
extern char SPRINTF_BUFFER_MSG[];
extern unsigned short D_8009F180[];
extern volatile unsigned short D_8009F200[];
extern unsigned char D_8009E4E6, D_8009E4E7;
extern volatile unsigned char D_800A609B;
extern void AnimLoadDuration(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);
extern short probeCollisionAtDepthC(TObj *, short, short);
extern int sprintf(char *, char *, ...);
extern void drawDebugText(int, int, int, char *);

void func_800311E4(TObj *o)
{
    short r;
    int st;

    int c;
    switch (c = o->b04) {
    case 0:
        o->b04++;
        o->active = 2;
        o->type = 21;
        *(signed char *)&o->b0f = -18;
        o->w1e = 20;
        o->animFrame = 0;
        o->b0a = 0;
        o->b0b = 0;
        o->b0d = 1;
        o->w08 = ((o->b0c + 508) << 6) | 0x17;
        {
            int t = SPAD.i2d8;
            void *an = D_80011F00;
            o->anim = an;
            o->d3c = t;
        }
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        asm("" : "=r"(c) : "0"(c));
        if (D_8009F180[0] & 0x8000)
            o->step = c - o->step;
        st = o->step;
        if (st != 0 && st == c) {
        if (SPAD.s1fc & 0x8000)
            o->animFrame = st - o->animFrame;
        if (D_8009F200[0] & 0x20)
            o->h->p.whole++;
        else if (D_8009F200[0] & 0x80)
            o->h->p.whole--;
        if (D_8009F200[0] & 0x10)
            o->y.p.whole--;
        else if (D_8009F200[0] & 0x40)
            o->y.p.whole++;
        if (D_8009F200[0] & 0x2000) {
            r = probeCollisionAtDepthC(o, o->h->p.whole, o->y.p.whole);
            if (r != 0) {
                sprintf(SPRINTF_BUFFER_MSG, D_80010618, r);
                drawDebugText(8, 128, 0, SPRINTF_BUFFER_MSG);
                sprintf(SPRINTF_BUFFER_MSG, D_80010624, o->h->p.whole);
                drawDebugText(8, 136, 0, SPRINTF_BUFFER_MSG);
                sprintf(SPRINTF_BUFFER_MSG, D_80010634, o->y.p.whole);
                drawDebugText(8, 144, 0, SPRINTF_BUFFER_MSG);
            } else {
                drawDebugText(8, 136, 0, D_80010644);
            }
        }
        if (D_8009F200[0] & 0x4000) {
            sprintf(SPRINTF_BUFFER_MSG, D_80010658, o->h->p.whole);
            drawDebugText(8, 160, 0, SPRINTF_BUFFER_MSG);
            sprintf(SPRINTF_BUFFER_MSG, D_80010664, o->y.p.whole);
            drawDebugText(8, 168, 0, SPRINTF_BUFFER_MSG);
            sprintf(SPRINTF_BUFFER_MSG, D_80010670, o->animFrame);
            drawDebugText(8, 176, 0, SPRINTF_BUFFER_MSG);
        }
        D_8009E4E7 = 1;
        D_8009E4E6 = 1;
        D_800A609B = 3;
        asm volatile("");
        { Fix16 *hp = o->h; int y = o->y.raw; int hv = hp->raw; SPAD.i190 = y; SPAD.i18c = hv; }
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
