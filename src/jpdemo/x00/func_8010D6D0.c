// JPDEMO x00 8010D6D0 128
// Demo-only helper: on the first step, picks the area/subtype animation from D_8010F380.
extern unsigned short D_8009A810;
extern void **D_8010F380[];
extern void func_8002099C(unsigned char *);
extern void func_80020554(unsigned char *);
void func_8010D6D0(unsigned char *o)
{
    func_8002099C(o);
    if (o[5] == 0) {
        o[5] = o[5] + 1;
        *(short *)(o + 0xac) = 1;
        *(void **)(o + 0x24) = D_8010F380[D_8009A810 * 4 + (o[0xc] & 0x7f)][1];
        func_80020554(o);
    }
}
