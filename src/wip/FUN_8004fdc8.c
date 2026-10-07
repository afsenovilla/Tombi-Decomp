// FUNC 8004fdc8 84 MAIN0
extern int DAT_1f8001e0;
int FUN_8004fdc8(unsigned *a, char *b, int c, short d, unsigned e){ char *p; unsigned v; register int i = d << 2; if (i < 0) i = 0; p = i + b; if ((unsigned)(p - DAT_1f8001e0) < 0xca0) { v = *(unsigned*)p; *(unsigned*)p = (unsigned)a; *a = v | e; return 0; } return 1; }
