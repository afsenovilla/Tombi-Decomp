// JPDEMO slpm80165 8001B880 144
// Demo-only helper: saves the area/section/D_8009A832 triple, sets the section mode and starts loading section n.
extern unsigned short D_8009A810, D_8009A812, D_8009A832;
extern unsigned short D_8009B158, D_8009B15A, D_8009B15C;
extern unsigned char *D_8006A11C[];
extern short D_1F8001DE, D_1F8001DC;
extern void func_8001F888(int, int, short);
extern void func_80037394(void);
void func_8001B880(short n)
{
    unsigned short a = D_8009A810, s = D_8009A812;
    unsigned char m = D_8006A11C[a][s];
    D_1F8001DE = 0;
    D_8009B158 = a;
    D_8009B15A = s;
    D_1F8001DC = m;
    D_8009B15C = D_8009A832;
    func_8001F888(a, s, n);
    func_80037394();
}
