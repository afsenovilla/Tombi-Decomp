// FUNC 8004fb68 64 MAIN0
// MATCHING 8004fb68 64
extern int SOUND_SET_LOAD_LISTS[];
extern int LOAD_NEXT_ADDR;
extern int DAT_1f8002ac;
extern void FUN_8004f538(int);

void loadSoundSet(int i)
{
    FUN_8004f538(SOUND_SET_LOAD_LISTS[i]);
    DAT_1f8002ac = LOAD_NEXT_ADDR;
}
