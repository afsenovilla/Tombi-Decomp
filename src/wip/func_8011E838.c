/* wip: score 272 (ncheck). Funcion completa = 688 + 384 B (splat corta en 8011EAE8, que es fragmento de esta).
   Falla: frame 0x88 vs 0x80 (8 B de locales de mas), el switch del juego hace andi 0xff sobre b04 copiado,
   y la asignacion de registros del bucle de case 0 (juego: cur en s0, prev en a1, tab en a3). Bucles con goto ya ok. */
// FUNC 8011e838 1072 X000
typedef struct { unsigned short frac; short whole; } FP;
typedef union { int raw; FP p; } FX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct E {
    char p00[4];
    unsigned char b04;
    char p05[7];
    unsigned char b0c;
    char p0d[3];
    FX a, y, b;
    char p1c[0x68 - 0x1c];
    unsigned char b68, b69;
    char p6a[2];
    short box0, box1, box2, box3;
    char p74[0x84 - 0x74];
    int d84, d88, d8c;
    struct E *parent;
    struct E *next;
    char p98[0xb4 - 0x98];
    short wb4, wb6, wb8, wba, wbc, wbe;
    short c0, c2;
    int c4, c8, cc;
} E;
extern short D_80138BAC[];
extern short D_80138BC8[];
extern MATRIX *RotMatrix(SVECTOR *, MATRIX *);
extern VECTOR *ApplyMatrixLV(MATRIX *, VECTOR *, VECTOR *);
extern int ObjCullRegister(E *);
extern void ObjFreeDup(E *);

void func_8011E838(E *o)
{
    E *s;
    E *n;
    short *tab;
    int x, y, z;
    short i;
    int d, k, dy;
    MATRIX *mp;
    VECTOR in;
    VECTOR out;
    SVECTOR v;
    MATRIX m;

    switch (o->b04) {
    case 0:
        if (o->b0c == 0) {
            s = o->parent;
            tab = D_80138BAC;
            z = s->b.raw;
            s->next = 0;
            x = s->a.raw - 0x170000;
            y = s->y.raw + 0x100000;
            s = o;
        loop0:
                s->wb4 = 0;
                s->wb6 = 0;
                s->wbc = 0x1000;
                s->wbe = 0x1000;
                s->wb8 = *tab++;
                s->c0 = s->a.p.whole;
                s->c2 = s->y.p.whole;
                n = s->next;
                if (n == 0) goto done0;
                s->c4 = n->a.raw - x;
                s->c8 = n->y.raw - y;
                s->cc = n->b.raw - z;
                s->box0 = n->a.p.whole;
                s->box2 = n->y.p.whole;
                x = n->a.raw;
                y = n->y.raw;
                z = n->b.raw;
                s = n;
                goto loop0;
        done0:
            s->a.p.whole -= 0x1e;
            s->c8 = s->y.raw - y;
            s->cc = s->b.raw - z;
            s->box2 = s->y.p.whole;
            s->c4 = s->a.raw - x;
            s->box0 = s->a.p.whole;
        }
        o->b04++;
        break;
    case 1:
        if (o->b0c == 0) {
            s = o;
            o->wb4 = 0;
            i = 0;
            do {
                if (s->b69 != 0) {
                    o->wb4 = i;
                    s->b69 = 0;
                    break;
                }
                if (s->b68 != 0) {
                    o->wb4 = i;
                    s->b68 = 0;
                    break;
                }
                s = s->next;
                i++;
            } while (s != 0);
            dy = o->y.p.whole - (unsigned short)o->c2;
            if (o->wb4 != o->wb6) {
                o->wb6 = o->wb4;
                o->wbe = D_80138BC8[(unsigned short)o->wb4];
            }
            if ((unsigned short)o->wbc != (unsigned short)o->wbe) {
                d = (unsigned short)o->wbe - (unsigned short)o->wbc;
                if ((unsigned)(d + 1) < 3)
                    o->wbc = o->wbe;
                else
                    o->wbc = o->wbc + d / 4;
            }
            s = o;
            mp = &m;
            k = 0;
            n = o->parent;
            x = n->a.raw - 0x170000;
            z = n->b.raw;
            y = n->y.raw + 0x100000;
            o->a.raw = x;
            o->b.raw = z;
            o->y.raw = y;
        loop1:
                v.vx = 0;
                v.vy = 0;
                v.vz = k / 4 + (o->wbc - 0x1000) / s->wb8;
                RotMatrix(&v, mp);
                s->d84 = v.vx;
                s->d88 = v.vy;
                s->d8c = v.vz;
                in.vx = s->c4;
                in.vy = s->c8;
                in.vz = s->cc;
                ApplyMatrixLV(mp, &in, &out);
                x += out.vx;
                y += out.vy;
                n = s->next;
                z += out.vz;
                if (n == 0) goto done1;
                n->a.raw = x;
                n->y.raw = y;
                n->b.raw = z;
                s->box0 = n->a.p.whole;
                k += dy;
                s->box2 = n->y.p.whole;
                s = n;
                goto loop1;
        done1:
            s->box0 = x >> 16;
            s->box2 = y >> 16;
        }
        if (o->b0c < 9) ObjCullRegister(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
