// FUNC 80029f1c 280 MAIN0
// MATCHING 80029f1c 280
typedef struct S {
    char p0[0x20]; int vy; int vx; char p1[0x6e - 0x28];
    unsigned char b6e, b6f, b70, b71, b72, b73;
} S;
extern short VIEW_CAMERA_ROT_Y;
extern void cameraFollowTargetY(S *);
extern void cameraFollowTargetYB(S *);
void cameraModeHandler2(S *o)
{
    int f1, f2;
    short s;
    if (o->vx != 0) {
        if (o->vx > 0) o->vx -= 0x80;
        else o->vx += 0x80;
    } else {
        o->b71 = 0;
        o->b72 = 0;
        o->b73 = 0;
    }
    f1 = 0;
    if (o->vy != 0) {
        if (o->vy > 0) o->vy -= 0x100;
        else o->vy += 0x100;
        f1 = 1;
    }
    s = VIEW_CAMERA_ROT_Y;
    if (s != 0) {
        if (s > 0) {
            VIEW_CAMERA_ROT_Y = s -= 2;
            if (s < 0) VIEW_CAMERA_ROT_Y = 0;
        } else {
            VIEW_CAMERA_ROT_Y = s += 2;
            if (s > 0) VIEW_CAMERA_ROT_Y = 0;
        }
        f2 = 1;
    } else {
        f2 = 0;
    }
    if ((f1 | f2) == 0) {
        o->b6e = 0;
        o->b6f = 0;
    }
    cameraFollowTargetY(o);
    cameraFollowTargetYB(o);
}
