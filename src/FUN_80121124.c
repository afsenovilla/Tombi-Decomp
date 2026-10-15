// FUNC 80121124 344 X000
// MATCHING 80121124 344
extern int spawnPoolType44Object(int, int, int, int);
extern void a00L4Type26ReleaseParts(void *);

void FUN_80121124(unsigned char *o)
{
    switch (o[6]) {
    case 0:
        if (o[0x69] == 1) {
            *(int *)(o + 0xb4) = spawnPoolType44Object(5, 0xa78, -0x4d8, 0x172);
            *(int *)(o + 0xb8) = spawnPoolType44Object(0, 0xa88, -0x4d8, 0x172);
            *(int *)(o + 0xbc) = spawnPoolType44Object(0, 0xa98, -0x4d8, 0x172);
            *(int *)(o + 0xc0) = spawnPoolType44Object(0, 0xaa8, -0x4d8, 0x172);
            *(int *)(o + 0xc8) = *(int *)(o + 0xc4) = spawnPoolType44Object(0, 0xab8, -0x4d8, 0x172);
            *(short *)(o + 0x22) = 0x78;
            o[6] = o[6] + 1;
        }
        break;
    case 1:
        *(short *)(o + 0x22) = *(short *)(o + 0x22) - 1;
        if (*(short *)(o + 0x22) == -1) {
            a00L4Type26ReleaseParts(o);
            *(short *)(o + 0x22) = 0x1e;
            o[6] = o[6] + 1;
        }
        break;
    case 2:
        *(short *)(o + 0x22) = *(short *)(o + 0x22) - 1;
        if (*(short *)(o + 0x22) == -1)
            o[6] = 0;
        break;
    }
}
