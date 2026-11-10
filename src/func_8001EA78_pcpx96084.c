// VERSION PCPX_96084 tomba 8001EA78 112
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Draws a list of text lines with func_8001E2D4 (debug text) until it returns <= 0: x, y, colour, string.
typedef struct {
    unsigned char x, y, c, pad;
    char *s;
} Line8001EA78;
extern int func_8001E2D4(void *, char *);

void func_8001EA78_pcpx96084(char *o, Line8001EA78 *p)
{
    do {
        short x, y, c;
        x = p->x; y = p->y; c = p->c;
        *(short *)(o + 0x24) = x;
        *(short *)(o + 0x26) = y;
        *(short *)(o + 0x28) = c;
    } while (func_8001E2D4(o, (p++)->s) > 0);
}
#endif
