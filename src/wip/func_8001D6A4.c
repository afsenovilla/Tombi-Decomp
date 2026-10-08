// FUNC 8001d6a4 820 MAIN0
// wip score 24 (was 83; short cnt): prologue s-reg save order, 0x7fffff as lui/addiu, addu order in DecDCTin. Old note: falta 0x7fffff como lui 0x80/addiu -1, orden de s-regs en el prologo y addu en DecDCTin
typedef struct { unsigned short w; unsigned short h; int pad; } Ent;
typedef struct {
    int *in[2];
    int idx;
    int *out[2];
    int oidx;
    Ent ent[2];
    int f28;
    unsigned short w2c;
    unsigned short w2e;
    short w30;
    short w32;
    int f34;
} M;
typedef struct { char p[0x48]; unsigned short w48; char p2[0x1e]; unsigned char b68; } T;
typedef struct { char d[8]; } E8;
extern M D_8009BCA8;
extern volatile int D_8009BCDC;
extern unsigned char D_1F8001CC, D_1F8001CD, D_1F8001D3;
extern T *D_1F8001D4;
extern short D_1F8001F4, D_1F8001E8;
extern char D_80077F0C[];
extern unsigned char D_80077F3C[];
extern short D_8007D284[];
extern E8 D_8007DEF0[];
extern void CdMix(void *);
extern void FUN_8001dad4(M *, int, int, int, int);
extern void FUN_8001db38(E8 *);
extern int MovieDecodeFrame(M *);
extern void DecDCTin(int *, int);
extern void DecDCTout(int *, int);
extern void SetDispMask(int);
extern void DecDCTinSync(int);
extern void DecDCToutCallback(void *);
extern void StUnSetRing(void);
extern void StClearRing(void);
extern int CdControlB(int, int, int);
extern void FUN_8001726c(void);
extern void ThreadWaitFrames(int);

void func_8001D6A4(void)
{
    M *m = &D_8009BCA8;
    volatile int *d = &m->f34;
    int k;
    short cnt;
    D_1F8001CC = 1;
    D_1F8001D4->w48 = 0;
    D_1F8001D4->b68 = 0;
    for (;;) {
        if (D_1F8001D3 == 1) {
            D_1F8001D4->w48 = 3;
            CdMix(D_80077F0C);
        }
        switch (D_1F8001D4->w48) {
        case 0:
            FUN_8001dad4(&D_8009BCA8, 0x180, 0x110, 0x2c0, 0x110);
            FUN_8001db38(&D_8007DEF0[D_8007D284[D_80077F3C[D_1F8001CD]]]);
            D_1F8001D4->w48++;
            while (MovieDecodeFrame(&D_8009BCA8) == -1)
                ;
        case 1:
            D_1F8001CC = 2;
            D_1F8001D4->w48 = 2;
        case 2:
            DecDCTin(m->in[D_8009BCA8.idx], 2);
            k = D_1F8001F4;
            m->f28 = k;
            m->w2c = D_8009BCA8.ent[k].w;
            m->w2e = D_8009BCA8.ent[k].h;
            DecDCTout(m->out[D_8009BCA8.oidx], m->w30 * m->w32 / 2);
            while (MovieDecodeFrame(&D_8009BCA8) == -1)
                ;
            if (*d == 0) {
                cnt = 0x7fffff;
                do {
                    if (cnt == 0) {
                        d[0] = 1;
                        d[-3] = D_1F8001F4;
                        ((unsigned short *)d)[-4] = D_8009BCA8.ent[D_1F8001F4].w;
                        ((unsigned short *)d)[-3] = D_8009BCA8.ent[D_1F8001F4].h;
                    }
                    cnt--;
                } while (D_8009BCDC == 0);
            }
            SetDispMask(1);
            D_8009BCDC = 0;
            D_1F8001CC = 3;
            D_1F8001E8 = 0;
            break;
        case 3:
            DecDCTinSync(0);
            DecDCToutCallback(0);
            StUnSetRing();
            StClearRing();
            while (CdControlB(9, 0, 0) == 0)
                ;
            D_1F8001CC = 0;
            D_1F8001D3 = 0;
            FUN_8001726c();
            break;
        }
        ThreadWaitFrames(1);
    }
}
