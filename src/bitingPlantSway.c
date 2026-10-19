// FUNC 8012f1b4 116 X000
// MATCHING 8012f1b4 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void bitingPlantSwayStep(O *);
extern void bitingPlantCopyParentAngle(O *);
void bitingPlantSway(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        bitingPlantSwayStep(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { bitingPlantCopyParentAngle(p); p->s = o->s; p = p->next; }
    }
}
