// FUNC 8012f380 116 X000
// MATCHING 8012f380 116
extern void bitingPlantLungeHead(char *);
extern void bitingPlantLungeStemA(char *);
extern void bitingPlantLungeStemB(char *);

void bitingPlantLunge(char *o)
{
    char *p;
    if (o[3] == 0) {
        bitingPlantLungeHead(o);
        p = *(char **)(o + 0x94);
        bitingPlantLungeStemA(p);
        p[6] = o[6];
        p = *(char **)(p + 0x94);
        bitingPlantLungeStemB(p);
        p[6] = o[6];
    }
}
