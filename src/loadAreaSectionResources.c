// FUNC 8001d1bc 432 MAIN0
// MATCHING 8001d1bc 432
extern unsigned short GAME[];
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned short AREA_VARIANT;
extern unsigned char *PTR_DAT_80077c4c[];
extern unsigned char DAT_8009d2c3;
extern unsigned char LOADED_SOUND_SET;
extern char DAT_80077c9c[];
extern char DAT_80077ca4[];
static char DAT_80077cb0[44] = {
    0x32, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x20, 0, 0, 3, -0x20, -1, 0, 3,
};
extern void loadSoundSet(int);
extern void SoundStopAll(void);
extern void keyOffActiveSfxVoices(void);

int loadAreaSectionResources(int force)
{
    int idx;
    unsigned char b;
    int ret;
    unsigned char *t;
    unsigned short *g;

    g = GAME;
    idx = g[0] + AREA_VARIANT;
    t = PTR_DAT_80077c4c[idx];
    b = t[GAME_SELECTED_SECTION];
    ret = -1;
    if (b == 9 && (DAT_8009d2c3 & 2))
        b = 10;
    if (b == 11 && (DAT_8009d2c3 & 4))
        b = 12;
    if (LOADED_SOUND_SET != b || force) {
        loadSoundSet(b);
        SoundStopAll();
        ret = b;
        LOADED_SOUND_SET = b;
    }
    if (idx == 1 || idx == 7) {
        keyOffActiveSfxVoices();
        loadSoundSet(DAT_80077c9c[g[1]]);
    } else if (idx == 10 && g[1] != 3 && g[1] != 7) {
        keyOffActiveSfxVoices();
        loadSoundSet(DAT_80077ca4[g[1]]);
    } else if (idx == 12) {
        keyOffActiveSfxVoices();
        loadSoundSet(DAT_80077cb0[g[1]]);
    }
    return ret;
}
