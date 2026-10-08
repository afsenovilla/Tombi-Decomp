// FUNC 8006be6c 224 MAIN0
/* score 3: epilogue only (game: addiu sp in the jr delay slot with s0 saved; ours addiu sp; jr; nop). Body matches with CC1PSX 4.3 / ncheck.
   CC1PSX 4.4 and gcc 2.8.x fill the epilogue slot but also fill the bnez slot with the 0x49 load (game keeps lbu; nop; beqz), so
   neither reproduces it: library-range code (looks like an assembler that fills jr slots with the previous insn, cf. FUN_8006bac4). */
extern void (*DAT_800981b0)(unsigned char *);

void FUN_8006be6c(unsigned char *o)
{
    int s = o[0x46];
    unsigned int t;
    *(int *)(o + 0x4c) = *(int *)(o + 0x4c) + 1;
    if (s == 0)
        goto tail;
    if (s == 1) {
        t = o[0x4a];
        if (t > 10) {
            o[0x49] = 2;
            o[0x46] = 0xff;
            return;
        }
        o[0x4a] = t + 1;
        return;
    }
    t = o[0x4a];
    if (t <= 10) {
        o[0x4a] = t + 1;
        return;
    }
    if (o[0x49] != 0)
        DAT_800981b0(o);
tail:
    if (**(unsigned char **)(o + 0x3c) != 0xf3) {
        **(unsigned char **)(o + 0x30) = 0xff;
        (*(unsigned char **)(o + 0x30))[1] = 0;
        o[0xe8] = 0;
        o[0x35] = 0;
    }
}
