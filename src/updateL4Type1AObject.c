// FUNC 80030510 1516 MAIN0
// MATCHING 80030510 1516
typedef struct M { char p0[4]; unsigned char b04; unsigned char b05; } M;
typedef struct O {
    unsigned char active;
    unsigned char visible;
    unsigned char type;
    unsigned char subtype;
    unsigned char b04;
    unsigned char step;
    char p06[0x08 - 0x06];
    short w08;
    char p0a[0x0d - 0x0a];
    unsigned char b0d;
    unsigned char b0e;
    char p0f[0x1e - 0x0f];
    short w1e;
    char p20[0x24 - 0x20];
    void *anim;
    char p28[0x2e - 0x28];
    unsigned short animFrame;
    char p30[0x3c - 0x30];
    int d3c;
    char p40[0x69 - 0x40];
    unsigned char b69;
    char p6a[0x6c - 0x6a];
    short box0;
    short box1;
    short box2;
    short box3;
    char p74[0x84 - 0x74];
    int d84;
    int d88;
    int d8c;
    M *msg;
    char p94[0xa0 - 0x94];
    int da0;
    char pa4[0xb4 - 0xa4];
    short wb4, wb6, wb8, pba;
    short wbc, wbe, wc0, pc2;
    short wc4, wc6, wc8, pca;
    short wcc, wce, wd0;
} O;
typedef struct T { unsigned short text; unsigned short opt; char pad[4]; } T;

extern T D_80079EBC[];
extern unsigned short D_80079E8C[];
extern unsigned short D_80079E98[];
extern unsigned short D_80079EA4[];
extern unsigned short D_80079EB0[];
extern unsigned char D_8009C930;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned short D_8009C960[];
extern int D_8009C984;
extern unsigned char D_8009CDAC;
extern unsigned char D_8009CDD2;
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CDD6;
extern unsigned char D_8009CEED;
extern short D_8009F3DC;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern short D_800A604A[];
extern unsigned char D_800A60F8;
extern short D_1F8001C6;
extern int D_1F8002D4;
extern void *D_80138EE8;
extern void *D_8013E738;
extern short GetClut(int, int);
extern void ObjCullRegister(O *);
extern void ObjFreeDup(O *);
extern void setEventStarted(int, int, int);
extern M *showMessageBox(short, short, int, int);

void updateL4Type1AObject(O *o)
{
    unsigned short text;
    unsigned short opt;
    M *m;
    unsigned short af;
    int v;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0x13:
            o->da0 = 0;
            break;
        case 0x12:
            o->b04 = 3;
            return;
        case 0x22:
            if (D_8009CDAC != 0 || D_800A604A[0] < 0x280) {
                o->b04 = 3;
                return;
            }
            break;
        case 0x63:
            o->wb4 = -0x10;
            o->wb6 = -0x18;
            o->wb8 = 0;
            o->wbc = 0x18;
            o->wbe = -0x18;
            o->wc0 = 0;
            o->wc4 = -0x10;
            o->wc6 = 0x18;
            o->wc8 = 0;
            o->wcc = 0x18;
            o->wce = 0x18;
            o->wd0 = 0;
            if (D_8009C960[0] == 1 || D_8009C960[0] == 7) {
                o->b0d = 1;
                o->w08 = GetClut(0xd0, 0x1e5);
                o->w1e = 7;
                o->anim = D_8013E738;
            } else {
                o->b0d = 0;
                o->w1e = 8;
                o->anim = D_80138EE8;
            }
            o->d3c = D_1F8002D4;
            o->active = 2;
            o->b0e = 0;
            break;
        }
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 0x28;
        o->box3 = 0x28;
        o->b04++;
        o->b69 = 0;
        o->step = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        af = o->animFrame;
        if (af == 1) {
            o->d88 = -0x400;
        } else if (af == 2) {
            v = 0x800;
            goto set;
        } else if (af == 3) {
            v = 0x400;
        set:
            o->d88 = v;
            o->active = 2;
        }
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (o->b69 == 1) {
                if (D_800A60F8 & 1) {
                    switch (o->subtype) {
                    case 0x13:
                        if (D_8009CEED == 2) o->subtype = 0x14;
                        break;
                    case 0x14:
                        if (D_8009CDD6 == 0xff) o->subtype = 0x23;
                        break;
                    }
                    text = D_80079EBC[o->subtype].text;
                    switch (o->subtype) {
                    case 2:
                        opt = D_80079E8C[D_8009F3DC];
                        break;
                    case 3:
                        opt = D_80079E98[D_8009F3DC];
                        break;
                    case 4:
                        opt = D_80079EA4[D_8009F3DC];
                        break;
                    case 6:
                        opt = D_80079EB0[D_8009F3DC];
                        break;
                    default:
                        opt = D_80079EBC[o->subtype].opt;
                        break;
                    }
                    o->msg = showMessageBox(text, opt, 0xa0, 0x8c);
                    o->step++;
                }
                o->b69 = 0;
            }
            break;
        case 1:
            m = o->msg;
            if (m->b04 >= 2) {
                m->b04++;
                D_8009C942 = 0;
                D_8009C93F = 0;
                D_800A603C = 1;
                D_800A603D = 0;
                D_800A603E = 0;
                D_1F8001C6 = 0;
                D_800A60F8 = 0;
                o->step = 0;
                if (o->subtype == 0x11) setEventStarted(0x25, 0, 0);
                if (o->subtype == 0x13 && D_8009CDD2 == 0) {
                    setEventStarted(0x2e, 0, 0);
                    D_8009CEED = 1;
                }
                if (o->subtype == 0x14 && D_8009CDD5 == 0) setEventStarted(0x31, 0, 0);
            } else if (o->subtype == 1 && (D_8009C984 & 1) && D_8009C930 == o->subtype) {
                o->step++;
            }
            break;
        case 2:
            m = o->msg;
            if (*(unsigned short *)&m->b04 == 0x101) {
                m->b05 = 0;
                o->step++;
            }
            break;
        case 3:
            m = o->msg;
            if (m->b04 >= 2) {
                m->b04++;
                D_800A60F8 = 0;
                o->step++;
            }
            break;
        case 4:
            if (D_8009C930 >= 2) o->step = 0;
            break;
        }
        if (o->subtype == 0x22 && D_800A604A[0] < 0x3e8) D_800A604A[0] = 0x3e8;
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
