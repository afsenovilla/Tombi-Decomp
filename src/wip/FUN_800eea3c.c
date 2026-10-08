// FUNC 800eea3c 64 X000
// wip: solo falta intercambiar v0/v1 (game: lh v1; move v0,v1; tests sobre v1)
void FUN_800eea3c(char *o)
{
    char pad[4];
    int r = *(short *)(o + 0xb0);
    int s = r;
    if (s < 0) {
        r = r * 4 + 0x100;
    } else {
        r = r * 4;
        if (s <= 0) goto zero;
    }
    *(short *)(o + 0xb6) = r & 0xff;
    return;
zero:
    *(short *)(o + 0xb6) = 0;
}
