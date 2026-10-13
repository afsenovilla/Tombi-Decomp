// FUNC 8001e288 68 MAIN0
// MATCHING 8001e288 68
extern unsigned short SFX_NOTE_OVERRIDE;
extern unsigned short SFX_VOLUME_OVERRIDE;
int setSfxOverride(unsigned short a)
{
    switch (a & 0xf00) {
    case 0:
        SFX_NOTE_OVERRIDE = a & 0xff;
        break;
    case 0x100:
        SFX_VOLUME_OVERRIDE = a & 0xff;
        break;
    }
    return 0;
}
