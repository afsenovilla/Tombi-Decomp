"""Rooms (sections) of every area on the disc, without importing numpy (the graphics editor lists them before the interpreter is loaded)."""

ROOM_COUNTS = [(0, 6), (1, 5), (2, 6), (3, 6), (4, 20), (5, 4), (6, 3), (8, 4), (9, 7), (10, 9), (11, 5), (13, 2), (14, 8), (16, 7), (17, 11), (18, 3), (19, 3)]
# empty load list: the room inherits the overlay / resources of the room before it and cannot be entered from the debug select
# (notes/formats.md "MIPS capture")
UNREACHABLE = {(11, 0), (11, 3), (11, 4), (16, 0), (16, 1), (16, 2), (17, 0), (17, 6), (17, 7), (17, 8), (17, 9), (17, 10), (18, 0)}
