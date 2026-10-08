// FUNC 80119f90 1460 X003
// MATCHING 80119f90 1460
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;

typedef struct E {
    unsigned char active, visible, type, subtype;
    unsigned char b04;
    char p05[0xc - 5];
    unsigned char b0c;
    char p0d[0x10 - 0xd];
    int x, y, z;
    char p1c;
    unsigned char b1d;
    char p1e[0x2e - 0x1e];
    unsigned short animFrame;
    int d30, d34, d38;
    char p3c[0x68 - 0x3c];
    unsigned char b68, b69, b6a, b6b;
    short box0, box1, box2, box3;
    char p74[0x84 - 0x74];
    int d84, d88, d8c;
    int d90;
    struct E *next;
    char p98[0xa4 - 0x98];
    unsigned char ba4;
    char pa5[0xb4 - 0xa5];
    unsigned short wb4, wb6;
    short *p[7];
} E;

extern short D_801358B0[], D_801358D0[], D_80135978[], D_80135998[], D_801359CC[], D_801359EC[];
extern short D_80135A20[], D_80135A40[];
extern short D_80135944[], D_80135946[], D_80135948[];
extern void FUN_800201ac(E *, int);
extern MATRIX *RotMatrix(SVECTOR *, MATRIX *);
extern VECTOR *ApplyMatrixLV(MATRIX *, VECTOR *, VECTOR *);

void func_80119F90(E *o)
{
    E *e;
    int x, y, z;
    int flag;
    int b;
    unsigned int bt;
    short **pp;
    short *t0, *t1, *t2;
    unsigned short k;
    VECTOR v;
    VECTOR r;
    SVECTOR ang;
    MATRIX m;
    short *q;

    FUN_800201ac(o, 0x40);
    if (o->visible == 0) return;
    if (o->d90 != 0) return;
    e = o;
    while (1) {
        if (e->next == 0) {
            bt = e->b69; b = (unsigned char)bt;
            flag = 0;
            if (b == 1) goto hit;
            if (b == 0) goto miss;
            if (b >= 5) goto miss;
            if (b < 3) goto miss;
        hit:
            if (o->wb6 == 0) {
                flag = 1;
                o->wb4 = bt;
                o->wb6 = 4;
            }
            goto done;
        miss:
            if (o->wb6 != 0) {
                o->wb6--;
            }
        done:
            switch (e->b68) {
            case 1:
                o->wb4 = 2;
                flag = 1;
                break;
            case 2:
                if (o->wb6 == 0) {
                    flag = 1;
                    o->wb4 = 4;
                    o->wb6 = 4;
                }
                break;
            }
            e->b69 = 0;
            e->b68 = 0;
            o->animFrame = e->animFrame;
            break;
        }
        e = e->next;
    }
    e = o;
    x = o->x;
    y = o->y;
    z = o->z;
    t0 = o->p[3];
    k = 0x8000;
    t1 = o->p[4];
    t2 = o->p[5];
    while (1) {
        pp = e->p;
        if (flag) {
            e->wb4 = o->wb4;
            switch (o->wb4) {
            case 1:
                e->p[0] = D_80135978;
                e->p[6] = D_801358B0;
                break;
            case 2:
                e->p[1] = D_801359CC;
                e->p[2] = D_80135A20;
                e->p[6] = D_801358B0;
                break;
            case 3:
                e->p[0] = D_80135998;
                e->p[6] = D_801358D0;
                break;
            case 4:
                e->p[1] = D_801359EC;
                e->p[2] = D_80135A40;
                e->p[6] = D_801358D0;
                break;
            }
        }
        ang.vx = *pp[0] / *t0;
        if (o->animFrame == 0) {
            ang.vy = 0x1000 - *pp[1] / *t1;
            ang.vz = 0x1000 - *pp[2] / *t2;
        } else {
            ang.vy = *pp[1] / *t1;
            ang.vz = *pp[2] / *t2;
        }
        RotMatrix(&ang, &m);
        if (e->next == 0) {
            e->d84 = *(short *)((char *)D_80135944 + e->subtype * 6) + ang.vx;
            e->d88 = *(short *)((char *)D_80135946 + e->subtype * 6) + ang.vy;
            e->d8c = *(short *)((char *)D_80135948 + e->subtype * 6) + ang.vz + *pp[6];
        } else {
            e->d84 = ang.vx;
            e->d88 = ang.vy;
            e->d8c = ang.vz;
        }
        q = pp[0]; pp[0] = q + 1; if ((unsigned short)q[1] == k) pp[0] = q;
        q = pp[1]; pp[1] = q + 1; if ((unsigned short)q[1] == k) pp[1] = q;
        q = pp[2]; pp[2] = q + 1; if ((unsigned short)q[1] == k) pp[2] = q;
        q = pp[6]; pp[6] = q + 1; if ((unsigned short)q[1] == k) pp[6] = q;
        t0++;
        t1++;
        v.vx = e->d30;
        v.vy = e->d34;
        v.vz = e->d38;
        t2++;
        ApplyMatrixLV(&m, &v, &r);
        x += r.vx;
        y += r.vy;
        z += r.vz;
        if (e->next == 0) return;
        e = e->next;
        e->x = x;
        e->y = y;
        e->z = z;
    }
}
