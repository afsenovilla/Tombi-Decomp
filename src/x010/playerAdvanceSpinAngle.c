// FUNC 800ee5c8 184 X010
// MATCHING 800ee5c8 184
typedef struct { char pad[8]; unsigned char b8; char pad2[5]; unsigned short we; } P;
extern P *PLAYER_EXT;
typedef struct { char pad[0xb6]; unsigned short wb6; } O;

void playerAdvanceSpinAngle(O *o)
{
    if ((unsigned short)(o->wb6 += PLAYER_EXT->we) < 0x800) {
        PLAYER_EXT->b8 = 1;
    }
    if ((unsigned)(o->wb6 - 0x800) < 0x800) {
        PLAYER_EXT->b8 = 0;
    }
    if ((unsigned short)(o->wb6 + 0x7ff) < 0x800) {
        PLAYER_EXT->b8 = 0;
    }
    if ((unsigned short)(o->wb6 + 0xfff) < 0x800) {
        PLAYER_EXT->b8 = 1;
    }
}
