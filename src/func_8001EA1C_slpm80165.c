// VERSION SLPM_80165 slpm80165 8001EA1C 84
#if defined(VERSION_SLPM_80165)
// Demo-only helper: queues the sound commands for switching an effect on (3 setup commands, 0x3100) or off (0x3000).
extern void queueSoundCommand(int, int);
void func_8001EA1C_slpm80165(int on)
{
    int c;
    if (on == 0) {
        c = 0x3000;
    } else {
        queueSoundCommand(0x9002, 0);
        queueSoundCommand(0x91ff, 0);
        queueSoundCommand(0x927f, 0);
        c = 0x3100;
    }
    queueSoundCommand(c, 0);
}
#endif
