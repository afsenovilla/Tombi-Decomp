/* the side-view room exits (runtime/ws_exits.c): inclusive H / Y / D ranges of the tests before each
   tryStartItemInteraction(o, 1|2, N) of the area code, from tools/colview_exits.json; cond = other conditions too;
   ws0 / ws1 = how many pads h0 / h1 move by with --widescreen */
typedef struct { unsigned char area, exit, cond; unsigned int secs; short h0, h1, y0, y1, d0, d1; signed char ws0, ws1; } Exit;
extern const Exit port_exits[];
extern const int port_exit_count;
