// VERSION BETA_980203 x00 80129024 400
#if defined(VERSION_BETA_980203)
// Beta version of src/komaPigContact.c (komaPigContact, PAL body): the object link field 0xe4 is at 0xd8.
#define DEBUG_INVINCIBLE D_1F8001A4
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern short objectBoxOverlapSideFacing(unsigned char *, unsigned char *);
extern void decreaseObjectTimer(unsigned char *, int);
extern unsigned char DEBUG_INVINCIBLE;

void komaPigContact(unsigned char *a, unsigned char *b)
{
    short r = objectBoxOverlapSideFacing(a, b);
    if (r == -1)
        return;
    switch (U8(a, 0xac)) {
    case 1:
        if (r < 3) {
            b[0] = 4;
            b[4] = 2;
            b[5] = 1;
            b[6] = 0;
            b[0x69] = 0;
            U16(b, 0x2e) = U16(a, 0x2e) & 1;
            PTR(a, 0xd8) = b;
            U8(a, 0xac) = 2;
            break;
        }
    case 0:
    case 3:
        if ((b[0] & 2) == 0) {
            b[0x69] = 8;
            if (DEBUG_INVINCIBLE == 0 && (a[0] & 2) == 0) {
                int ah, bh;
                a[0] = 2;
                bh = S16(PTR(b, 0x40), 2);
                ah = S16(PTR(a, 0x40), 2);
                a[4] = 2;
                a[5] = 0;
                a[6] = 0;
                U16(a, 0x2e) = ah < bh;
                decreaseObjectTimer(a, 1);
            }
        }
        break;
    case 2:
        if (b[0] != 3) {
            int ah, bh;
            ah = S16(PTR(a, 0x40), 2);
            bh = S16(PTR(b, 0x40), 2);
            b[0] = 3;
            b[4] = 2;
            b[5] = 0;
            b[6] = 0;
            b[0x69] = 0;
            U16(b, 0x7a) = bh < ah;
        }
        break;
    }
}
#endif
