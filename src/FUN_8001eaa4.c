// FUNC 8001eaa4 40 MAIN0
// MATCHING 8001eaa4 40
extern int queueSoundCommand(int a, int b);

void FUN_8001eaa4(int x)
{
    queueSoundCommand((x & 0xff) | 0x1000, x);
}
