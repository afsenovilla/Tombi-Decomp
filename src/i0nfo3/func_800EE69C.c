// FUNC 800ee69c 560 I0NFO3
// MATCHING 800ee69c 560
#include "TOBJ.H"
#include "raw7.h"
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_800A6038;
extern unsigned char D_800A6039;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern unsigned short D_800A604E;
extern TObj D_800A6038_o;
#define X6078 D_800A6038_o.h->p.whole
extern short D_800A60B4;
extern short D_800A60B6;
extern unsigned char D_800A60E4;
extern short D_800A6126;
extern short D_800A612A;

static __inline__ int stepX(void)
{
    int r = 0;
    if (D_800A60B4 != 0) {
        X6078--;
        if (X6078 < D_800A6126) {
            X6078 = D_800A6126;
            r = 1;
        }
    } else {
        X6078++;
        if (D_800A6126 < X6078) {
            X6078 = D_800A6126;
            r = 1;
        }
    }
    return r;
}

void func_800EE69C(TObj *o)
{
    short r;
    switch (o->state) {
    case 0:
        D_800A603D = 2;
        D_800A6038 = 4;
        D_800A603E = 0;
        D_800A6039 = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        r = stepX();
        if (D_800A60B6 != 0) {
            D_800A604E--;
            if ((short)D_800A604E < D_800A612A) {
                D_800A604E = D_800A612A;
                if (r) {
                    o->state = 9;
                }
            }
        } else {
            D_800A604E++;
            if ((short)D_800A604E > D_800A612A) {
                D_800A604E = D_800A612A;
                if (r) {
                    o->state = 9;
                }
            }
        }
        break;
    case 9:
        D_8009C942 = 0;
        D_8009C93F = 0;
        D_800A6038 = 1;
        D_800A60B4 = 0;
        D_800A60B6 = 0;
        D_800A6126 = 0;
        D_800A612A = 0;
        D_800A6039 = 1;
        D_800A60E4 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
