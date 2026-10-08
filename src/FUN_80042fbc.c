// FUNC 80042fbc 176 MAIN0
// MATCHING 80042fbc 176
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))

static __inline__ int hit(char *a, char *b)
{
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)((U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2)) + (U16(a, 0x6c) + U16(b, 0x6c))) > S16(a, 0x6e) + S16(b, 0x6e))
        return 0;
    return (unsigned short)((U16(a, 0x16) - U16(b, 0x16)) + (U16(a, 0x70) + U16(b, 0x70))) <= S16(a, 0x72) + S16(b, 0x72);
}

int FUN_80042fbc(char *a, char *b)
{
    return hit(a, b);
}
