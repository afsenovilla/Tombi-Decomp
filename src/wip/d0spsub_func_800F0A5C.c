// FUNC 800f0a5c 944 D0SPSUB
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;
typedef struct { short x, y, z, pad; } SVec;
typedef struct { short x[4], y[4]; } Quad;
typedef struct { short type, a, b, c; } Ent;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
extern unsigned short D_8009D2C4[];
extern Ent *D_800FDA70[];
extern short D_800FD418[][4], D_800FD41A[][4], D_800FD41C[][4];
extern Quad D_800FE0B4[];
extern POLY_F4 *D_1F800164;
extern unsigned long *D_1F8001E0;
extern void func_800F0930(int pos, int pri);

void func_800F0A5C(void)
{
    SVec v;
    long sxy0, sxy1, otz;
    int i;
    Ent *e;
    int a, b, c;
    for (i = 0; i < 12; i++) {
        if (D_8009D2C4[i] == 0) continue;
        e = D_800FDA70[i];
        if (e->type == -1) continue;
        do {
            b = e->b;
            v.x = D_800FD418[b][0]; v.y = D_800FD41A[b][0]; v.z = D_800FD41C[b][0];
            gte_ldv0(&v);
            gte_rtps();
            gte_stsxy(&sxy0);
            switch (e->type) {
            case 0:
                a = e->a;
                v.x = D_800FD418[a][0]; v.y = D_800FD41A[a][0]; v.z = D_800FD41C[a][0];
                gte_ldv0(&v);
                gte_rtps();
                gte_stsxy(&sxy1);
                gte_stszotz(&otz);
                func_800F0930(sxy1, otz - 10);
                break;
            case 1: {
                register Quad *q asm("$4");
                POLY_F4 *p;
                unsigned long *ot;
                a = e->a;
                c = e->c;
                b = e->b;
                q = &D_800FE0B4[c];
                v.x = D_800FD418[a][0]; v.y = D_800FD41A[a][0]; v.z = D_800FD41C[a][0];
                gte_ldv0(&v);
                gte_rtps();
                gte_stsxy(&sxy1);
                gte_stszotz(&otz);
                v.x = D_800FD418[b][0]; v.y = D_800FD41A[b][0]; v.z = D_800FD41C[b][0];
                gte_ldv0(&v);
                gte_rtps();
                gte_stsxy(&sxy0);
                p = D_1F800164;
                p->len = 5;
                p->code = 0x28;
                p->r0 = 0xFF;
                p->g0 = 0xFF;
                p->b0 = 0xFF;
                p->x0 = q->x[0] + sxy1;
                p->y0 = q->y[0] + (sxy1 >> 16);
                p->x1 = q->x[1] + sxy0;
                p->y1 = q->y[1] + (sxy0 >> 16);
                p->x2 = q->x[2] + sxy1;
                p->y2 = q->y[2] + (sxy1 >> 16);
                p->x3 = q->x[3] + sxy0;
                p->y3 = q->y[3] + (sxy0 >> 16);
                ot = D_1F8001E0 + otz;
                addPrim(ot, p);
                D_1F800164++;
                break;
            }
            }
            e++;
        } while (e->type != -1);
    }
}
