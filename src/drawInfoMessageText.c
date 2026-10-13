// FUNC 8004db4c 196 MAIN0
// MATCHING 8004db4c 196
extern int DAT_1f800398;
extern void drawMessageGlyph(int, int);

void drawInfoMessageText(int a, int idx)
{
    short *base = (short *)(DAT_1f800398 + *(short *)(DAT_1f800398 + 8));
    short *p;
    unsigned short v;

    p = (short *)((char *)base + *(short *)((char *)base + (idx << 1)));

    while (1) {
        v = *p;
        p++;
        if ((unsigned short)(v + 2) < 2) break;
        idx = (short)v;
        if (idx != -3 && idx != -7) {
            if (idx == -6) p += 2;
            else drawMessageGlyph(a, idx);
        }
    }
}
