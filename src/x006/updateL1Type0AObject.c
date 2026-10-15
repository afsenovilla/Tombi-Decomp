// FUNC 8011191c 308 X006
// MATCHING 8011191c 308
extern unsigned char PLAYER;
extern void l1Type0AInit(void *);
extern void FUN_800202b4(void *);
extern void l1Type0AStepSub0(void *);
extern void l1Type0AStepSub1(void *);
extern void freeObjectLayer1(void *);

void updateL1Type0AObject(unsigned char *o)
{
    switch (o[4]) {
    case 0:
        l1Type0AInit(o);
        break;
    case 1:
        if ((unsigned)(PLAYER - 4) >= 2) {
            FUN_800202b4(o);
            switch (o[3]) {
            case 0:
                l1Type0AStepSub0(o);
                break;
            case 1:
                l1Type0AStepSub1(o);
                break;
            }
            if (*(short *)(o + 0x22) != 0) {
                *(short *)(o + 0x22) = *(short *)(o + 0x22) - 1;
                if (*(short *)(o + 0x22) < 1)
                    o[0] = 1;
            }
            if (o[0x6a] != 0) {
                o[0x6a] = 0;
                *(short *)(o + 0x22) = 3;
            }
        }
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        freeObjectLayer1(o);
        break;
    }
}
