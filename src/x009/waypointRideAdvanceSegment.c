// FUNC 80119d2c 288 X009
// MATCHING 80119d2c 288
typedef struct { short x, y, z, pad; } P8;
typedef struct { short i, b6, b8, ba; int bc, c0; } X;
extern P8 WAYPOINT_RIDE_PATH[];
extern int SquareRoot0(int);
extern short ratan2(int, int);

void waypointRideAdvanceSegment(char *o)
{
    X *x = (X *)(o + 0xb4);
    int dx, dz, dy, d;

    for (;;) {
        dx = WAYPOINT_RIDE_PATH[x->i + 1].x - WAYPOINT_RIDE_PATH[x->i].x;
        dz = WAYPOINT_RIDE_PATH[x->i + 1].z - WAYPOINT_RIDE_PATH[x->i].z;
        dy = WAYPOINT_RIDE_PATH[x->i + 1].y - WAYPOINT_RIDE_PATH[x->i].y;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d != 0) {
            x->c0 = d << 8;
            if (x->bc < (d << 8)) break;
            x->bc -= d << 8;
        }
        x->i++;
    }
    x->ba = ratan2(dy, d);
    x->b8 = ratan2(dz, dx);
}
