// FUNC 80121180 212 X009
// MATCHING 80121180 212
#include "TOBJ.H"
typedef struct {
    char p00[0x40]; Fix16 *h;
    char p44[0xac - 0x44]; unsigned char bac;
    char pad[0xb0 - 0xad]; short wb0;
    char pb2[0xbe - 0xb2]; unsigned char bbe;
    char pbf[0xe4 - 0xbf]; TObj *de4;
} PL;
extern short D_1F80019E;
extern short func_8004306C(PL *, TObj *);

void func_80121180(PL *p, TObj *o)
{
    short r = func_8004306C(p, o);
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
    D_1F80019E = 0;
}
