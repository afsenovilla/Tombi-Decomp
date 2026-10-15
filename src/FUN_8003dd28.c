// FUNC 8003dd28 1496 MAIN0
// MATCHING 8003dd28 1496
typedef struct { unsigned short frac; short whole; } FixParts;
typedef union { int raw; FixParts p; } Fix16;
typedef struct O {
    unsigned char active;
    unsigned char visible;
    unsigned char type;
    unsigned char subtype;
    unsigned char b04;
    char p05[0x0c - 0x05];
    unsigned char b0c;
    char p0d[0x14 - 0x0d];
    Fix16 y;
    char p18[0x20 - 0x18];
    short timer;
    short w22;
    char p24[0x2e - 0x24];
    unsigned short animFrame;
    int d30;
    Fix16 d34;
    int d38;
    char p3c[0x40 - 0x3c];
    Fix16 *h;
    Fix16 *d;
    char p48[0x68 - 0x48];
    unsigned char b68;
    unsigned char b69;
    char p6a[0x7e - 0x6a];
    short velY;
    short velH;
    short velV;
    char p84[0xb4 - 0x84];
    unsigned short wb4;
} O;
typedef struct V { unsigned short h, v; } V;

extern V D_8007B35C[];
extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(O *);
extern unsigned char D_8009CDAC;
extern unsigned char D_8009CDCA;
extern unsigned char D_8009CDFB;
extern unsigned char D_8009CE0D;
extern unsigned char D_8009D009;
extern unsigned char D_8009D087;
extern void initItemObject(O *);
extern void freeObjectLayer5(O *);
extern void ObjApplyVelocity(O *);
extern int ObjCullRegister(O *);
extern void AnimAdvance(O *);
extern short TileCollideAt(O *, short, short);
extern short probeWallAt(O *, short, short, int);

void FUN_8003dd28(O *o)
{
    int k;

    switch (o->b04) {
    case 0:
        o->w22 = 0;
        if (o->subtype == 0x3f && D_8009CDCA != 0xff) {
            o->b04 = 3;
            break;
        }
        if (o->subtype == 0x9e && D_8009CDFB != 0xff) {
            o->b04 = 3;
            break;
        }
        if (o->subtype == 0x90 && D_8009D087 < 4) break;
        if (o->subtype == 0x3c && (D_8009CE0D == 0 || D_8009CE0D == 0xff)) {
            o->b04 = 3;
            break;
        }
        if (o->subtype == 3 && o->b0c == 1 && (D_8009CDAC != 0xff || D_8009D009 != 0)) {
            o->b04 = 3;
            break;
        }
        initItemObject(o);
        switch (o->type) {
        case 1:
            o->timer = 0x3c;
            break;
        case 2:
            o->wb4 = 1;
            break;
        case 3:
            o->active = 4;
            o->wb4 = 1;
            o->timer = 0x1e;
            break;
        case 4:
            o->wb4 = 0;
            break;
        case 5:
        case 8:
            o->velY = -0x500;
            o->timer = 0x3c;
            o->d34.raw = o->y.p.whole << 16;
            break;
        case 10:
            if (o->velV != 0) o->wb4 = 1;
            else o->wb4 = 0;
            o->timer = 0x1e;
            break;
        }
        o->b04++;
        break;
    case 1:
        k = 8;
        if (o->subtype == 2 && o->b0c >= 4) k = 0x1c;
        switch (o->type) {
        case 1:
        case 3:
        case 10:
            if (o->timer != 0 && --o->timer == 0) o->active = 1;
            break;
        }
        switch (o->type) {
        case 2:
        case 3:
        case 4:
        case 10:
            if (o->b68 & 1) {
                o->b68 = 0;
                o->wb4 = 1;
                o->velH = D_8007B35C[o->animFrame].h;
                o->velV = D_8007B35C[o->animFrame].v;
            } else if (o->b69 & 1) {
                o->b69 = 0;
                if (o->velV <= 0x100) goto land;
                o->velV = -o->velV / 4;
            } else if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + k)) {
                if (o->velV <= 0x100) {
                land:
                    if (o->type == 4) {
                        o->velH = 0;
                        o->velV = 0;
                    }
                    o->wb4 = 0;
                    o->active = 2;
                } else {
                    o->velV = -o->velV / 4;
                }
            } else if (probeWallAt(o, o->h->p.whole + 8, o->y.p.whole, 0) ||
                       probeWallAt(o, o->h->p.whole - 8, o->y.p.whole, 1)) {
                o->velH = -o->velH;
            }
            if (o->wb4 != 0) {
                if (o->type == 10) {
                    ObjApplyVelocity(o);
                    if (o->velV < 0x800) o->velV += 0x20;
                    if (o->y.p.whole - o->d34.p.whole >= -7) {
                        o->wb4 = 0;
                        o->velV = 0;
                    }
                } else {
                    ObjApplyVelocity(o);
                    o->velV += 0x20;
                }
            }
            break;
        case 5:
        case 8:
            o->d30 += o->velH << 8;
            o->d34.raw += o->velV << 8;
            o->d34.raw += o->velY << 8;
            o->h->raw = o->d30;
            o->y.raw = o->d34.raw;
            o->d->raw = o->d38;
            if (o->velY < 0x800) o->velY += 0x28;
            if (o->velY > 0) o->active = 1;
            if (--o->timer == 0) o->b04++;
            break;
        }
        if (o->w22 != 0) o->w22--;
        if (ObjCullRegister(o)) AnimAdvance(o);
        break;
    case 2:
        D_8007A890[D_8007A7F0[o->subtype]](o);
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
