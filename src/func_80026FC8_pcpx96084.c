// VERSION PCPX_96084 tomba 80026FC8 136
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// The early form of stepScrollAndAngle (src/stepScrollAndAngle.c): steps the scroll word by 0x80 and the
// current object's counter at +0x46 (func_80026F40_pcpx96084, inlined) towards 0; 1 once both are 0.
typedef struct { char pad[0x46]; short n; } O80026FC8;
extern O80026FC8 *D_800DE544;

static __inline__ int stepScroll(char *o)
{
    int w = *(int *)(o + 0x20);
    if (w != 0) {
        if (w > 0) w -= 0x80;
        else w += 0x80;
        *(int *)(o + 0x20) = w;
        return 1;
    }
    return 0;
}

static __inline__ int stepCounter(void)
{
    O80026FC8 *p = D_800DE544;
    if (p->n != 0) {
        if (p->n > 0) p->n--;
        else p->n++;
        return 1;
    }
    return 0;
}

int func_80026FC8_pcpx96084(char *o)
{
    char pad[0];
    int a = stepScroll(o);
    int b = stepCounter();
    return (a | b) ^ 1;
}
#endif
