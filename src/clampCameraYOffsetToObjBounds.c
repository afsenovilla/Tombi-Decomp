// FUNC 80028b84 92 MAIN0
// MATCHING 80028b84 92
// VERSION SLPS_01144 slps01144 80029994 92
// probably: clamps the camera offset VIEW_CAMERA_Y[-6] so VIEW_CAMERA_Y[0]+VIEW_CAMERA_Y[-6] stays inside o->h30..h32 (object scroll bounds); unreferenced in MAIN0/X000
#if defined(VERSION_SLPS_01144)
typedef struct O { char p0[0x30]; short h30; short h32; } O;
extern short VIEW_CAMERA_Y[];

void clampCameraYOffsetToObjBounds(O *o)
{
    if (VIEW_CAMERA_Y[0] + VIEW_CAMERA_Y[-6] < o->h30)
        VIEW_CAMERA_Y[-6] = o->h30 - VIEW_CAMERA_Y[0];
    else if (VIEW_CAMERA_Y[0] + VIEW_CAMERA_Y[-6] > o->h32)
        VIEW_CAMERA_Y[-6] = o->h32 - VIEW_CAMERA_Y[0];
}
#else
typedef struct O { char p0[0x30]; short h30; short h32; } O;
extern short VIEW_CAMERA_Y[];

void clampCameraYOffsetToObjBounds(O *o)
{
    if (VIEW_CAMERA_Y[0] + VIEW_CAMERA_Y[-6] < o->h30)
        VIEW_CAMERA_Y[-6] = o->h30 - VIEW_CAMERA_Y[0];
    else if (VIEW_CAMERA_Y[0] + VIEW_CAMERA_Y[-6] > o->h32)
        VIEW_CAMERA_Y[-6] = o->h32 - VIEW_CAMERA_Y[0];
}
#endif
