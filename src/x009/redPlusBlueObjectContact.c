// FUNC 80121180 212 X009
// MATCHING 80121180 212
// VERSION SLPS_01144 x09 801203EC 212
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x009 80121180 (x009/func_80121180.c): 228 -> 212.
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    char p00[0x40]; Fix16 *h;
    char p44[0xac - 0x44]; unsigned char bac;
    char pad[0xb0 - 0xad]; short wb0;
    char pb2[0xbe - 0xb2]; unsigned char bbe;
    char pbf[0xd4 - 0xbf]; TObj *de4;
} PL;
extern short playerPushOutOfObjectBox(PL *, TObj *);

void func_80121180(PL *p, TObj *o)
{
    short r = playerPushOutOfObjectBox(p, o);
    if (r == 0)
        return;
    if (p->bac == 1 && r == 1) {
        if (o->active == 5) {
            if (p->h->p.whole > o->h->p.whole) {
                p->bbe = 8;
                p->wb0 = 1;
            } else {
                p->bbe = 9;
                p->wb0 = -1;
            }
        } else {
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            o->b69 = 0;
            p->de4 = o;
            p->bac = 2;
        }
    }
    SPAD.s19e = 0;
}
#else
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    char p00[0x40]; Fix16 *h;
    char p44[0xac - 0x44]; unsigned char bac;
    char pad[0xb0 - 0xad]; short wb0;
    char pb2[0xbe - 0xb2]; unsigned char bbe;
    char pbf[0xe4 - 0xbf]; TObj *de4;
} PL;
extern short playerPushOutOfObjectBox(PL *, TObj *);

void redPlusBlueObjectContact(PL *p, TObj *o)
{
    short r = playerPushOutOfObjectBox(p, o);
    if (r == 0)
        return;
    if (p->bac == 1 && r == 1) {
        if (o->active == 5) {
            if (p->h->p.whole > o->h->p.whole) {
                p->bbe = 8;
                p->wb0 = 1;
            } else {
                p->bbe = 9;
                p->wb0 = -1;
            }
        } else {
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            o->b69 = 0;
            p->de4 = o;
            p->bac = 2;
        }
    }
    SPAD.s19e = 0;
}
#endif
