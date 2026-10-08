// FUNC 800352f4 308 MAIN0
#include "TOBJ.H"
extern char *DAT_8009c338;
extern char DAT_800a6100;
extern int DAT_800a6068, DAT_800a606c, DAT_8009c934;
extern Fix16 *DAT_800a6078;
extern short DAT_800a604e, DAT_800a60ec, DAT_800a60b2;
extern unsigned char DAT_800a60d4;
extern void SfxPlay(int);
extern int csqrt(int);
extern short FUN_800335d4(TObj *);
void FUN_800352f4(TObj *o)
{
    int iVar1, iVar2;
    DAT_8009c338[0xd] = 0;
    SfxPlay(0x26);
    DAT_800a6100 = 1;
    DAT_800a6068 = o->h->p.whole;
    DAT_800a606c = o->y.p.whole + 4;
    iVar1 = o->h->p.whole - DAT_800a6078->p.whole;
    if (iVar1 < 0) iVar1 = -iVar1;
    iVar2 = o->y.p.whole - DAT_800a604e;
    if (iVar2 < 0) iVar2 = -iVar2;
    iVar1 = csqrt((iVar1 * iVar1 + iVar2 * iVar2) * 0x1000);
    DAT_800a60ec = iVar1 >> 12;
    if (DAT_800a60d4 == 0) DAT_800a60ec = DAT_800a60ec - 0x10;
    if (DAT_800a60ec < 0x28) DAT_800a60ec = 0x28;
    DAT_800a60b2 = FUN_800335d4(o);
    iVar1 = DAT_8009c934;
    o->step = 3;
    o->state = 0;
    o->d90 = iVar1;
}
