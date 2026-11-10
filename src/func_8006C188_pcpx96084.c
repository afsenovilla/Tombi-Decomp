// VERSION PCPX_96084 tomba 8006C188 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// On a hit of func_8006BA14 against the three-part object b (subtype 0: b -> +0x94 -> +0x94, subtype 1: +0x90 and
// +0x94), flags all three parts (+0x68 = result | 2), spawns the hit effect and, for type 0, plays sound 7.
#include "TOBJ.H"
extern short func_8006BA14(TObj *, TObj *);
extern void func_80017C14_pcpx96084(int, int, int, int);
extern void func_80022F5C_pcpx96084(int, int);

void func_8006C188_pcpx96084(TObj *a, TObj *b)
{
    TObj *p, *q;
    short r = func_8006BA14(a, b);
    if (r < 0) return;
    switch (b->subtype) {
    case 0:
        p = (TObj *)b->d94;
        q = (TObj *)p->d94;
        break;
    case 1:
        p = (TObj *)b->d90;
        q = (TObj *)b->d94;
        break;
    }
    b->b68 = r | 2;
    p->b68 = r | 2;
    q->b68 = r | 2;
    func_80017C14_pcpx96084(1, *(short *)((char *)a + 0x12), *(short *)((char *)a + 0x16), *(short *)((char *)a + 0x1a));
    if (a->type == 0) func_80022F5C_pcpx96084(0, 7);
}
#endif
