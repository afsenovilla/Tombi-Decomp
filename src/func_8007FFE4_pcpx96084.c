// VERSION PCPX_96084 tomba 8007FFE4 116
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL bitingPlantSway (src/bitingPlantSway.c): flag at +0xC, six chained parts.
typedef struct O { char p0[6]; unsigned char s; char p1[5]; unsigned char t; char p2[0x94 - 13]; struct O *next; } O;
extern void func_80080058_pcpx96084(O *);
extern void func_80080108(O *);
void func_8007FFE4_pcpx96084(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        func_80080058_pcpx96084(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 6; i++) { func_80080108(p); p->s = o->s; p = p->next; }
    }
}
#endif
