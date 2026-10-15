// FUNC 80035428 580 MAIN0
// MATCHING 80035428 580
#include "TOBJ.H"
extern short probeCollisionAtDepthB(TObj *, short, short);
extern void spawnWeaponHitDebris(short, short, short, unsigned char);
extern void spawnImpactEffect(int, short, short, short);
extern void FUN_8001e4f0(int);
extern short probeFloorPlaneAt(void *, short, short);
extern void weapon8to9AttachAnchor(TObj *);
extern unsigned char DAT_800a60d6;
extern unsigned short DAT_1f800282;
extern unsigned short DAT_1f800282b;
extern TObj *DAT_8009c330;
extern char PLAYER[];
extern short *DAT_800a6078;
extern unsigned short DAT_800a604e;
extern unsigned short DAT_800a60a8;

int weapon8to9HitWall(TObj *o)
{
    short n;
    int k;

    if (o->active == 1 && probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole)) {
        o->active = 2;
        o->ba5 = 0;
        o->b69 = 0;
        if (DAT_800a60d6 == 0) {
            if (!(DAT_1f800282 & 0x800)) {
                if (DAT_1f800282 & 2) o->b69 = 1;
            }
        }
        k = (DAT_1f800282b >> 5) & 0xf;
        if (!(DAT_1f800282b & 0x3000)) {
            switch (k) {
            case 1:
            case 2:
            case 3:
                spawnWeaponHitDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, o->animFrame);
                break;
            }
        }
        spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e4f0(5);
        o->wa8 = 0x4ff;
    }
    if (o->b69 != 0 && DAT_800a60d6 != 4 && DAT_800a60d6 != 7) {
    n = 0;
    switch ((signed char)DAT_8009c330->substep) {
    case 0:
    case 2:
        if (probeFloorPlaneAt(PLAYER, DAT_800a6078[1] + 0x10, DAT_800a604e + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    case 1:
    case 3:
        if (probeFloorPlaneAt(PLAYER, DAT_800a6078[1] - 0x10, DAT_800a604e + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    }
    if (n == 0) {
        weapon8to9AttachAnchor(o);
        return 1;
    }
    return 0;
    }
    o->b69 = 0;
    return 0;
}
