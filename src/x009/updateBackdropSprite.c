// FUNC 801198dc 720 X009
// MATCHING 801198dc 720
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern char D_8012DBB4[];
extern char D_8012DC28[];
extern char D_8012DBA4[];
extern char D_8012DBAC[];
extern char D_8012DC9C[];
extern char D_8012DD10[];
extern char D_8012DDA4[];
static void *D_8012DAF4[7] = {
    D_8012DBB4, D_8012DC28, D_8012DBA4, D_8012DBAC,
    D_8012DC9C, D_8012DD10, D_8012DDA4,
};
extern unsigned short GetClut(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

void updateBackdropSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->w08 = GetClut(0x80, 0x1ff);
        if (o->subtype == 5) o->b0a = 0;
        else o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 3:
        case 4:
            o->b0d = 1;
            o->w1e = 6;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8012DAF4[o->b0c];
            break;
        case 5:
            o->b0d = 1;
            o->w1e = 4;
            o->b.p.whole = -0x64;
            o->b0f = 2;
            o->anim = D_8012DAF4[o->b0c];
            break;
        case 6:
            o->b0d = 1;
            o->w1e = 4;
            o->b.p.whole = 0xc8;
            if (((unsigned char*)&GCD.bd0ce)[0]) o->d30 = 0x98;
            else o->d30 = 0x4c;
            o->b0f = 2;
            o->anim = D_8012DAF4[o->b0c];
            break;
        case 7:
        case 8:
            o->b0d = 1;
            o->w1e = 6;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8012DAF4[o->b0c];
            break;
        case 9:
            o->b0d = 1;
            o->w1e = 0xe;
            o->b.p.whole = 0x64;
            *(signed char *)&o->b0f = -0x12;
            o->w08 = GetClut(0x80, 0x1fe);
            o->anim = D_8012DAF4[o->b0c];
            break;
        }
        o->d3c = SPAD.i2d4;
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->subtype == 5) {
            ObjCullRegister(o);
        } else {
            o->visible = 1;
            o->a.p.whole = o->d30 - ((unsigned short*)((char*)&SPAD.i174 + 2))[0];
            o->y.p.whole = o->d34 - ((unsigned short*)&SPAD.s186)[0];
            ObjListPush_1F800220(o);
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
