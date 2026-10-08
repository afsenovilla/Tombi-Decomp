// FUNC 8010d048 568 X008
// MATCHING 8010d048 568
typedef struct {
    char p0[0x20]; short s20; char p1[0x76 - 0x22]; unsigned short w76; char p2[0xb6 - 0x78]; unsigned short wb6;
} O;
extern unsigned short DAT_8009d670;

void FUN_8010d048(O *o)
{
    char pad;
    volatile unsigned short *p = &DAT_8009d670;
    if (!(*p & 0xf0)) {
        if (o->s20 != 0) {
            if (--o->s20 <= 0) {
                if ((unsigned short)(o->wb6 - 0x40) < 0x80) o->w76 = 4;
                else o->w76 = 0;
                o->w76 |= 8;
            }
        }
        return;
    }
    o->s20 = 10;
    if (*p & 0x20) {
        if (*p & 0x10) {
            if ((unsigned short)(o->wb6 - 0x90) <= 0x20) o->wb6 = 0x20;
            o->w76 = 1;
            return;
        }
        if (*p & 0x40) {
            if ((unsigned short)(o->wb6 - 0x50) <= 0x20) o->wb6 = 0xe0;
            o->w76 = 7;
            return;
        }
        if ((short)o->wb6 == 0x80) o->wb6 = 0;
        o->w76 = 0;
        return;
    }
    if (*p & 0x80) {
        if (*p & 0x10) {
            if ((unsigned short)(o->wb6 - 0xd0) <= 0x20) o->wb6 = 0x60;
            o->w76 = 3;
            return;
        }
        if (*p & 0x40) {
            if ((unsigned short)(o->wb6 - 0x10) <= 0x20) o->wb6 = 0xa0;
            o->w76 = 5;
            return;
        }
        if ((short)o->wb6 == 0) o->wb6 = 0x80;
        o->w76 = 4;
        return;
    }
    if (*p & 0x10) {
        if ((unsigned short)(o->wb6 - 0xb0) <= 0x20) o->wb6 = 0x40;
        o->w76 = 2;
        return;
    }
    if (*p & 0x40) {
        if ((unsigned short)(o->wb6 - 0x30) <= 0x20) o->wb6 = 0xc0;
        o->w76 = 6;
    }
}
