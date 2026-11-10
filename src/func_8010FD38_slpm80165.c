// VERSION SLPM_80165 x00 8010FD38 416
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: the demo's play-time limit. Counts the minutes:seconds:frames clock D_800893D4/D5/D6 down (60
// frames a second) while D_8009A7EA is set and neither D_8009A7F2 nor D_8009A7EF; at 0:00, or when D_8009ACD3
// is 0xFF, it flags the end (D_800893D7, D_1F8001D3 during a movie), switches the object to substep 1 and calls
// func_8001EA1C(1). Unless D_1F8001CC is set the clock is printed into the text window D_8009A278.
extern unsigned char D_8009ACD3, D_8009A7F2, D_8009A7EF, D_8009A7EA;
extern unsigned char D_800893D4, D_800893D5, D_800893D6, D_800893D7;
extern unsigned char D_1F8001CC, D_1F8001D3;
extern char D_800A2868[];
extern char D_8010F564[];
extern char D_8009A278[];
extern void func_8001EA1C(int);
extern void func_80051E7C(char *, char *, int, int);
extern void func_80049420(char *, int, int, int);
extern void func_80049430(char *, char *);

void func_8010FD38(unsigned char *o)
{
    if (D_8009ACD3 == 0xff)
        goto expire;
    if (D_8009A7F2 == 0 && D_8009A7EF == 0 && D_8009A7EA != 0) {
        unsigned char *f = &D_800893D6;
        unsigned char k = *f;
        *f = k + 0xff;
        if (k == 0) {
            *f = 0x3b;
            k = D_800893D5;
            D_800893D5 = k + 0xff;
            if (k == 0) {
                if (D_800893D4 == 0 && D_800893D5 == 0xff) {
                expire:
                    D_800893D4 = 0;
                    D_800893D5 = 0;
                    D_800893D7 = 1;
                    if (D_1F8001CC)
                        D_1F8001D3 = 1;
                    o[6] = 0;
                    o[7] = 1;
                    func_8001EA1C(1);
                } else {
                    D_800893D5 = 0x3b;
                    D_800893D4--;
                }
            }
        }
    }
    if (D_1F8001CC == 0) {
        func_80051E7C(D_800A2868, D_8010F564, D_800893D4, D_800893D5);
        func_80049420(D_8009A278, 8, 1, 0);
        func_80049430(D_8009A278, D_800A2868);
    }
}
#endif
