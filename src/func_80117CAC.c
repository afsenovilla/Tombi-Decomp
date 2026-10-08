// FUNC 80117cac 916 X000
// MATCHING 80117cac 916
// Whole function: splat splits it into func_80117CAC (16), func_80117CBC (728) and func_80117F98 (168).
typedef struct {
    unsigned char b00[3];
    unsigned char subtype;
    unsigned char b04;
    unsigned char b05[0x1b];
    short timer;
    unsigned char b22[0x6e];
    short *d90;
    unsigned char b94[0x32];
    unsigned short wc6;
} O;
typedef struct {
    short type;
    short w02, w04, w06, w08, w0a, w0c, w0e, w10, w12;
} E;
extern E D_800A3FE0[][7];
extern short D_801384B6[][9];
extern short *D_80138630[];
extern int ObjCullRegister(O *);
extern void ObjFree(O *);

void func_80117CAC(O *o)
{
    int i;
    E *e;
    short *s;

    switch (o->b04) {
    case 0:
        if (--o->timer == 0)
            o->b04++;
        break;
    case 1:
        if (ObjCullRegister(o) == 0)
            o->b04++;
        for (i = 0; i < 6; i++) {
            e = &D_800A3FE0[o->wc6][i];
            switch (e->type) {
            case 2:
            case 3:
                if (e->w0c <= 0)
                    e->w0e = 0;
                if (e->w10 <= 0)
                    e->w12 = 0;
            case 1:
            case 4:
            case 5:
                e->w06 -= e->w0c;
                e->w0a -= e->w10;
                e->w0c += e->w0e;
                e->w10 += e->w12;
                if (--e->w02 <= 0) {
                    s = D_801384B6[e->type];
                    e->w02 = *s++;
                    e->w0c = *s++;
                    e->w0e = *s++;
                    e->w10 = *s;
                    e->w12 = s[1];
                    e->type++;
                }
                break;
            case 0:
            case 6:
                break;
            }
        }
        e = &D_800A3FE0[o->wc6][6];
        switch (e->type) {
        case 1:
            if (--e->w02 <= 0) {
                o->d90 = D_80138630[o->subtype];
                e->w02 = 0x12;
                e->type++;
            }
            break;
        case 2:
            s = o->d90;
            e->w04 += s[0];
            e->w06 += s[1];
            e->w08 -= s[0];
            e->w0a -= s[1];
            o->d90 = s + 2;
            if (--e->w02 <= 0) {
                e->w02 = 0x1e;
                e->type++;
            }
            break;
        case 3:
            if (--e->w02 <= 0)
                o->b04 = 2;
            break;
        case 0:
            break;
        }
        break;
    case 2:
        D_800A3FE0[o->wc6][0].type = -1;
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
