// VERSION PCPX_96084 tomba 80026F40 76
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct { char pad[0x46]; short n; } O26F40;
extern O26F40 *D_800DE544;

int func_80026F40_pcpx96084(void)
{
    char pad[0];
    O26F40 *p = D_800DE544;
    if (p->n != 0) {
        if (p->n > 0) p->n--;
        else p->n++;
        return 1;
    }
    return 0;
}
#endif
