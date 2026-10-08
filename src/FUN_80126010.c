// FUNC 80126010 232 X000
// MATCHING 80126010 232
extern short FUN_80043464(void);

void FUN_80126010(unsigned char *o, unsigned char *s)
{
    short r;
    r = FUN_80043464();
    if (r != -1) {
        if (r == 3 && o[0xac] == 1) {
            s[0] = 2;
            s[4] = 2;
            s[5] = 0;
            s[6] = 0;
            s[0x69] = 0;
            *(unsigned char **)(o + 0xe4) = s;
            o[0xac] = 2;
        } else if ((s[0] & 2) == 0 && (o[0] & 2) == 0 && o[0x9c] == 0 && r < 2) {
            o[0] = 2;
            o[4] = 1;
            *(short *)(o + 0x2e) = r;
            o[5] = 0x35;
            o[6] = 0;
        }
    }
}
