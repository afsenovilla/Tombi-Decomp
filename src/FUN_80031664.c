// FUNC 80031664 348 MAIN0
// MATCHING 80031664 348
#include "TOBJ.H"
extern short *DAT_8009c338;
extern short DAT_8009d600[], DAT_8009d602[], DAT_8009d604[];
extern Fix16 *DAT_800a6078;
extern Fix16 *DAT_800a607c;
extern unsigned short DAT_800a604e[];
extern int GetGraphType(void);
extern unsigned short GetClut(int, int);

void FUN_80031664(TObj *o)
{
    short *p;
    int v;
    DAT_8009c338 = &o->wac;
    if (o->state == 0) {
        if (GetGraphType() != 1) GetGraphType();
        p = DAT_8009c338;
        o->wb0 = 0;
        if (GetGraphType() == 1 || GetGraphType() == 2) v = 0x80; else v = 0x20;
        p[4] = v;
        DAT_8009c338[3] = GetClut(0x80, 0x1ef);
        DAT_8009c338[5] = GetClut(0x80, 0x1f0);
        DAT_8009d600[0] = 0xf;
        DAT_8009d602[0] = 8;
        DAT_8009d604[0] = 0x5a;
        *(signed char *)&o->b0f = -9;
        o->b0a = 0;
        o->h->p.whole = DAT_800a6078->p.whole;
        o->y.p.whole = DAT_800a604e[0] - 0x10;
        o->d->p.whole = DAT_800a607c->p.whole;
        o->velH = 0;
        o->velV = 0;
        o->animFrame = 0;
        o->visible = 1;
        o->state = 0;
    }
}
