// FUNC 80031664 348 MAIN0
#include "TOBJ.H"
extern short *DAT_8009c338;
extern short DAT_8009d600, DAT_8009d602, DAT_8009d604;
extern Fix16 *DAT_800a6078;
extern Fix16 *DAT_800a607c;
extern unsigned short DAT_800a604e;
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
        DAT_8009d600 = 0xf;
        DAT_8009d602 = 8;
        DAT_8009d604 = 0x5a;
        o->b0f = 0xf7;
        o->b0a = 0;
        o->h->raw = *(short *)((char *)DAT_800a6078 + 2);
        o->y.raw = DAT_800a604e - 0x10;
        o->d->raw = *(short *)((char *)DAT_800a607c + 2);
        o->velH = 0;
        o->velV = 0;
        o->animFrame = 0;
        o->visible = 1;
        o->state = 0;
    }
}
