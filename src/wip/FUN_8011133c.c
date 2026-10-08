// FUNC 8011133c 128 X000
/* score 38 (was 42): inline with short s param passed from an int local gives both li 0x200. Left: game keeps o in a1 (move a1,a0), s in a0 and a copy of o in a2 for the sh $zero; nested inline / extra pointer param copies did not reproduce it. */
extern unsigned short DAT_8009d610, DAT_8009d670;
extern unsigned char DAT_8009d618;

static __inline__ void inl(short s, char *o)
{
    volatile unsigned short *k;
    if (DAT_8009d610 == 7)
        s = 0x200 >> (3 - DAT_8009d618);
    k = &DAT_8009d670;
    if (*k & 0x80)
        *(short *)(o + 0x7c) = -s;
    else if (*k & 0x20)
        *(short *)(o + 0x7c) = s;
    else
        *(short *)(o + 0x7c) = 0;
}
void FUN_8011133c(char *o) { int s = 0x200; inl(s, o); }
