// FUNC 800ff074 116 X018
// MATCHING 800ff074 116
extern unsigned char D_8009CFFB;
extern unsigned char D_8009CDAC;
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern unsigned char D_8009C990;

int func_800FF074(void)
{
    short t = D_8009CFFB != 0;
    short r;
    r = t;
    if (D_8009CDAC == 0xff) r = t + 1;
    if (D_8009C960 == 0 && D_8009C962 < 2) r = 0;
    if (D_8009C990 == 1) r = 0;
    return r;
}
