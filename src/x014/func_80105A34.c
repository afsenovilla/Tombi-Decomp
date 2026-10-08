// FUNC 80105a34 176 X014
// MATCHING 80105a34 176
typedef struct {
    char p0[5];
    unsigned char step;   /* 05 */
    unsigned char state;  /* 06 */
    char p7[0x7e - 7];
    short w7e;            /* 7e */
    char p80[4];
    int d84;              /* 84 */
    char p88[4];
    int d8c;              /* 8c */
    char p90[0x9c - 0x90];
    unsigned char b9c;    /* 9c */
    char p9d[0xa7 - 0x9d];
    unsigned char ba7;    /* a7 */
    char pa8[0xab - 0xa8];
    unsigned char bab;    /* ab */
    unsigned char bac;    /* ac */
    char pad[0xb0 - 0xad];
    short wb0;            /* b0 */
    short wb2;            /* b2 */
} S;
typedef struct { char p[2]; unsigned char type; } T;
typedef struct { char p[8]; unsigned char b8; } U;
extern unsigned char D_801152E8[];
extern U *D_8009C330;
extern T *D_8009D2E8;
extern unsigned char D_8009D2B1;

void func_80105A34(S *o)
{
    o->d8c = D_801152E8[o->wb0];
    D_8009C330->b8 = 0;
    o->b9c = 0;
    o->ba7 = 0;
    o->bac = 0;
    o->d84 = 0;
    o->wb2 = 0;
    o->w7e = 0;
    if (D_8009D2E8->type == 0x1c) {
        switch (D_8009D2B1) {
        case 1:
            o->bac = 0;
            o->step = 0x2a;
            o->state = 0;
            o->bab |= 0x80;
            return;
        case 2:
            o->bac = 0;
            o->step = 0x2b;
            o->state = 0;
            o->bab |= 0x80;
            return;
        }
        o->bac = 0;
    }
    o->step = 0;
    o->state = 0;
}
