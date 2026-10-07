// FUNC 8001e228 96 MAIN0
// MATCHING 8001e228 96
extern unsigned char FLG[];
extern short TIM[];
extern short g(short a);

short FUN_8001e228(unsigned short a)
{
    if (FLG[a] != 0) {
        TIM[a] = 0xf;
        return g(a);
    }
    return -1;
}
