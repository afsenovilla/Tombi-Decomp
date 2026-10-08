// FUNC 800f0b5c 204 X006
// MATCHING 800f0b5c 204
typedef struct { short x; short y; } Cam;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern unsigned char DAT_8009cdb5;
extern unsigned char DAT_8009d0ce;
extern Cam *DAT_800a6078;

short FUN_800f0b5c(void)
{
    short r = 0;
    switch (DAT_8009c960) {
    case 1:
        if (DAT_8009c962 != 1)
            r = 1;
        else if (DAT_8009cdb5)
            r = 1;
        break;
    case 9:
        if (DAT_8009c962 != 1)
            r = 1;
        else if (DAT_8009d0ce == 0) {
            if (DAT_800a6078->y >= 0x73)
                r = 1;
        } else if (DAT_800a6078->y < 0x72)
            r = 1;
        break;
    default:
        r++;
        break;
    }
    return r;
}
