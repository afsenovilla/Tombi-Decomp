// FUNC 80039e54 848 MAIN0
// MATCHING 80039e54 848
#include "TOBJ.H"
typedef struct {
    char pad0[0x8a];
    unsigned short w8a;
    char pad1[0x1190 - 0x8c];
    int idx;
    unsigned int cmd;
    int a[6];
} S;
typedef struct { short v[6]; } V6;
extern S *D_8009F0F0;
extern TObj *D_8009F2D8[];
extern unsigned char D_8009D2B0, D_8009C93F, D_800A603C, D_800A603D, D_800A603E, D_800A60E4, D_8009C942;
extern unsigned short D_800A6066;
extern unsigned char D_800A60E4x[], D_8009D2B0x[], D_8009C93Fx[];
extern int FUN_8002dcc8(int, int, V6 *);
extern void func_80113754(TObj *);
extern void func_800ECBBC(TObj *);

void func_80039E54(void)
{
    S *s = D_8009F0F0;
    TObj *o = D_8009F2D8[s->idx];
    V6 tmp;
    unsigned int cmd = s->cmd;

    if (o != 0) {
        switch (cmd) {
        case 1:
            o->b6a = 1;
            o->timer = s->a[0];
        case 0:
        case 9:
            o->b6a = 0;
            o->step = cmd;
            o->state = 0;
            break;
        case 2:
            o->timer = s->a[0];
            o->velX = s->a[1];
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        case 3:
            o->timer = s->a[0];
            o->d30 = s->a[1];
            o->d34 = s->a[2];
            o->w98 = s->a[3];
            o->w9a = s->a[4];
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        case 8:
            tmp.v[1] = s->a[3];
            tmp.v[3] = s->a[4];
            tmp.v[5] = s->a[5];
        case 4:
            if (cmd == 4) tmp = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(s->a[0], s->a[1], &tmp);
            switch (o->type & 0x7f) {
            case 0x18:
                o->w74 = s->a[2];
                o->w76 = 0;
                func_80113754(o);
                D_8009D2B0 = 2;
                D_8009C93F = 1;
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
                break;
            case 0x19:
                o->b68 = 2;
                D_800A60E4x[0] = 3;
                D_8009D2B0x[0] = 2;
                D_8009C93Fx[0] = 1;
                if (s->a[2] & 0x80) {
                    o->w74 = o->animFrame + (s->a[2] & 0x7f);
                } else {
                    o->animFrame = o->b69 - 1;
                    o->w74 = o->animFrame + s->a[2];
                }
                o->w76 = 0;
                func_800ECBBC(o);
                {
                    unsigned short *k = &D_800A6066;
                    D_800A603C = 1;
                    D_800A603D = 2;
                    D_800A603E = 0;
                    *k |= 8;
                }
                break;
            }
            D_8009C942 = 1;
            o->b6a = 1;
            o->step = 4;
            o->state = 0;
            break;
        case 5:
            {
            int ty = o->type & 0x7f;
            if (ty == 0x18) goto L1;
            if (ty == 0x19) {
            L1:
                o->velX = s->a[0];
                o->velY = s->a[1];
            }
            }
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        case 6:
            o->b6a = 0;
            o->step = cmd;
            o->state = 0;
            break;
        case 7:
            {
            int ty = o->type & 0x7f;
            if (ty == 0x18) goto L2;
            if (ty == 0x19) {
            L2:
                o->velX = s->a[0];
                o->velY = s->a[1];
                o->w78 = s->a[2];
            }
            }
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        }
    }
    s->w8a++;
}
