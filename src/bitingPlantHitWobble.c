// FUNC 8012fb3c 116 X000
// MATCHING 8012fb3c 116
typedef struct O {
    char pad0[3];
    unsigned char sub;
    char pad1[2];
    unsigned char state;
    char pad2[0x94 - 7];
    struct O *next;
} O;
extern void bitingPlantHitWobbleStep();
extern void bitingPlantHitCopyParent();

void bitingPlantHitWobble(O *o)
{
    O *p;
    int i;
    if (o->sub == 0) {
        i = 1;
        bitingPlantHitWobbleStep();
        p = o->next;
        do {
            bitingPlantHitCopyParent(p);
            i++;
            p->state = o->state;
            p = p->next;
        } while (i < 3);
    }
}
