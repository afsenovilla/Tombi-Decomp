// FUNC 80120164 320 X000
// MATCHING 80120164 320
extern void a00L4Type18Idle(unsigned char *), a00L4Type18Active(unsigned char *), FUN_8001fe6c(unsigned char *), FUN_8001fec0(unsigned char *);
extern short FUN_8001fdac(int, int);
extern unsigned char *PTR_8013b14c;

void a00L4Type18Step(unsigned char *o)
{
    unsigned v;
    switch (o[5]) {
    case 0:
        a00L4Type18Idle(o);
        break;
    case 1:
        a00L4Type18Active(o);
        break;
    case 2:
        switch (o[6]) {
        case 0:
            o[0x69] = 0;
            *(unsigned char **)(o + 0x24) = PTR_8013b14c;
            *(int *)(o + 0x88) = 0;
            *(unsigned short *)(o + 0x7e) = *(unsigned short *)(o + 0x16);
            FUN_8001fe6c(o);
            o[6] = o[6] + 1;
        case 1:
            (*(unsigned short **)(o + 0x40))[1] -= 1;
            v = *(int *)(o + 0x88) + 1;
            *(int *)(o + 0x88) = v;
            *(unsigned short *)(o + 0x16) = *(unsigned short *)(o + 0x7c) + FUN_8001fdac(v & 0xff, 4);
            FUN_8001fec0(o);
            if ((*(short **)(o + 0x40))[1] < 0x128) {
                (*(short **)(o + 0x40))[1] = 0x128;
                o[5] = 0;
                o[6] = 0;
            }
        }
        break;
    }
}
