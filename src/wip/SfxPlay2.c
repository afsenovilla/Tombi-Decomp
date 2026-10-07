// FUNC 8001e560 148 MAIN0
extern int SoundFindFreeVoice(void);
extern int g(int a, int b);
extern unsigned short SFX[];
int f(int a, int b){ int v, r; v = SoundFindFreeVoice(); if (v != -1) { r = g((b & 0xff) | 0x8000, v); if (r != -1) { r = g((a & 0x3ff) | 0x400, v); if (r != -1) SFX[v] = a; return v; } } return -1; }
