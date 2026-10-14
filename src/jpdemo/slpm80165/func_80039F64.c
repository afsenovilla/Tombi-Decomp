// JPDEMO slpm80165 80039F64 484
// Demo version of PAL main0 8003C338 (FUN_8003c338.c): one animation (no area split), ground probe at y (PAL y+20).
extern void applyFrameVelocityX(unsigned char *), FUN_8001fe6c(unsigned char *);
extern short probeCollisionAtDepthB(unsigned char *, int, int);
extern unsigned short DAT_8009c960;
extern int DAT_8013b104, DAT_80134d0c;
extern char DAT_80077d54[];

void FUN_8003c338(unsigned char *o)
{
    unsigned x;
    short v, w;
    switch (o[5]) {
    case 0:
        if ((*(unsigned short *)(o + 0x2e) & 2) == 0)
            *(short *)(o + 0x7e) = -0x200;
        else
            *(short *)(o + 0x7e) = 0x200;
        *(char **)(o + 0x28) = DAT_80077d54;
        o[5] = o[5] + 1;
        *(int *)(o + 0x24) = DAT_8013b104;
        FUN_8001fe6c(o);
    case 1:
        if (*(unsigned short *)(o + 0x2e) & 1)
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) - 0x14) & 0xff;
        else
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) + 0x14) & 0xff;
        if ((*(unsigned short *)(o + 0x2e) & 2) == 0)
            applyFrameVelocityX(o);
        *(int *)(o + 0x14) = *(int *)(o + 0x14) + (*(short *)(o + 0x7e) << 8);
        *(short *)(o + 0x7e) += (*(unsigned short *)(o + 0x2e) & 2) ? 0x50 : 0x20;
        if (*(short *)(o + 0x7e) > 0)
            o[5] = o[5] + 1;
        break;
    case 2:
        if (*(unsigned short *)(o + 0x2e) & 1)
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) - 0x14) & 0xff;
        else
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) + 0x14) & 0xff;
        if ((*(unsigned short *)(o + 0x2e) & 2) == 0)
            applyFrameVelocityX(o);
        *(int *)(o + 0x14) = *(int *)(o + 0x14) + (*(short *)(o + 0x7e) << 8);
        *(short *)(o + 0x7e) += 0x20;
        if (probeCollisionAtDepthB(o, (*(short **)(o + 0x40))[1], (short)*(short *)(o + 0x16)) != 0) {
            o[4] = 2;
            o[5] = 0;
        }
        break;
    }
}
