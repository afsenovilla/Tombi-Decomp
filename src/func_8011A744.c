// FUNC 8011a744 748 X000
// MATCHING 8011a744 748
#include "TOBJ.H"
extern void *D_8013B244[];
extern int Rand(void);
extern int D_1F8002D4[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);
void func_8011A744(TObj *o)
{
    short f;
    switch (o->b04) {
    case 0:
        if (o->timer == 0) {
        o->b04++;
        o->w1e = 9;
        o->b0a = 3;
        o->d64 = 0xac8;
        o->b0d = 0;
        *(signed char *)&o->b0f = -10;
        o->anim = D_8013B244[o->b0c];
        o->d3c = D_1F8002D4[0];
        o->animFrame = Rand() & 1;
        switch (o->subtype) {
        case 0:
            o->d8c = 0x40;
            o->d84 = -2;
            o->velH = 0x100;
            o->velX = -8;
            o->velV = 0x100;
            break;
        case 1:
            o->d8c = -0x40;
            o->d84 = 2;
            o->velH = -0x100;
            o->velX = 8;
            o->velV = 0xc0;
            break;
        case 2:
            o->d8c = -0x60;
            o->d84 = 2;
            o->velH = 0x100;
            o->velX = -8;
            o->velV = 0x80;
            break;
        case 3:
            o->d8c = 0x60;
            o->d84 = -2;
            o->velH = -0x100;
            o->velX = 8;
            o->velV = 0x80;
            break;
        }
        AnimLoadDuration(o);
        } else {
            o->timer--;
        }
        break;
    case 1:
        if (ObjCullRegister(o) == 0) {
            o->b04 = 3;
            break;
        }
        o->y.raw += o->velV << 8;
        o->h->raw += o->velH << 8;
        o->velH += o->velX;
        {
            int vx = o->velX;
            if (vx < 0 && o->velH < -0xff)
                o->velX = -vx;
            else {
                int w = o->velX;
                if (w > 0 && o->velH > 0xff)
                    o->velX = -w;
            }
        }
        o->d8c += o->d84;
        switch (o->subtype) {
        case 0 ... 1:
            f = (unsigned)(o->d8c + 0x3f) < 0x7f;
            goto tail;
        case 2 ... 3:
            f = (unsigned)(o->d8c + 0x5f) < 0xbf;
        tail:
            if (!f)
                o->d84 = -o->d84;
            break;
        }
        if (++o->timer >= 0x78)
            o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
