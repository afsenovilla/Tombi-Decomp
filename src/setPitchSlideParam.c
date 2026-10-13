// FUNC 8001e368 200 MAIN0
// MATCHING 8001e368 200
extern short PITCH_SLIDE_STEP;
extern short PITCH_SLIDE_INTERVAL;
extern short PITCH_SLIDE_TIMER;
extern short PITCH_SLIDE_DURATION;
extern short PITCH_SLIDE_ACTIVE;
extern short PITCH_SLIDE_VOICE;
extern int LAST_KEYON_VOICE;

int setPitchSlideParam(unsigned short a)
{
    switch (a & 0xf00) {
    case 0:
        PITCH_SLIDE_INTERVAL = a & 0xff;
        PITCH_SLIDE_TIMER = 1;
        break;
    case 0x100:
        PITCH_SLIDE_STEP = a & 0xff;
        if (PITCH_SLIDE_STEP >= 0x80)
            PITCH_SLIDE_STEP = PITCH_SLIDE_STEP | 0xff00;
        break;
    case 0x200:
        PITCH_SLIDE_DURATION = a & 0xff;
        break;
    case 0x300:
        PITCH_SLIDE_ACTIVE = 1;
        PITCH_SLIDE_VOICE = LAST_KEYON_VOICE;
        break;
    }
    return 0;
}
