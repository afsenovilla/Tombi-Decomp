/* --widescreen: scripted entrances of actors the game parks just past a 4:3 edge (docs/PORT.md "Widescreen",
 * "Parked actors").  On a PS1 such an actor is hidden only by standing a few columns outside the 320-column picture;
 * the wide frame showed it waiting there.  It is parked as far past the wide edge instead (port_ws_park_dx), and its
 * scripted walk-in covers the extra distance (port_ws_walkin_frames / port_ws_walkin_step) so it still arrives on the
 * same frame: the walk is spread over the retail number of frames, at most twice the retail speed (beyond that the
 * walk takes (extra - distance) / 2 frames longer, only at 21:9 / 32:9).  Identity without --widescreen: dx 0, the
 * retail frame count, one unit per frame.
 *
 * Users (port/fixes.txt): AREA02 s5 "Small Strange Room", Baron (SCRIPT_OBJECTS[1], spawned at x 340, z 0: 1 unit =
 * 1 column in that single-screen room) waits right of the picture until pigGateDiscoveryScene state 22/23 walks him
 * to Tomba. */
extern int port_ws_cull;

static int wi_dx, wi_total, wi_frames, wi_k;

/* the X offset to park an actor at world/screen x: past the right edge by pad, past the left one by -pad */
int port_ws_park_dx(int x)
{
    if (!port_ws_cull) return 0;
    if (x >= 0x140) return wi_dx = port_ws_cull;
    if (x < 0) return wi_dx = -port_ws_cull;
    return 0;
}

/* dist: the distance the script measured (it includes the parking offset); returns the walk's frame count */
int port_ws_walkin_frames(int dist)
{
    int d = wi_dx < 0 ? -wi_dx : wi_dx;
    wi_k = 0;
    wi_total = dist;
    if (!d || dist <= d) { wi_frames = 0; return dist; }           /* retail */
    {
        int orig = dist - d, half = (dist + 1) / 2;
        wi_frames = orig > half ? orig : half;
    }
    return wi_frames;
}

/* units to move this frame (1 when off); the sum over the walk is the measured distance */
int port_ws_walkin_step(void)
{
    int k = wi_k++;
    if (!wi_frames || k >= wi_frames) return 1;
    return (int)((long long)wi_total * (k + 1) / wi_frames - (long long)wi_total * k / wi_frames);
}
