"""Rooms (sections) of every area on the disc, without importing numpy (the graphics editor lists them before the interpreter is loaded)."""

ROOM_COUNTS = [(0, 6), (1, 5), (2, 6), (3, 6), (4, 20), (5, 4), (6, 3), (8, 4), (9, 7), (10, 9), (11, 5), (13, 2), (14, 8), (16, 7), (17, 11), (18, 3), (19, 3)]
# empty load list: the room inherits the overlay / resources of the room before it and cannot be entered from the debug select
# (notes/formats.md "MIPS capture")
UNREACHABLE = {(11, 0), (11, 3), (11, 4), (16, 0), (16, 1), (16, 2), (17, 0), (17, 6), (17, 7), (17, 8), (17, 9), (17, 10), (18, 0)}


# Mapping between the model files (load lists of SYS/LD0nn.BIN, tools/area000.load_lists: {'ARnn': [area list, section 0, section 1, ...]}) and the
# debug select ids of the capture (area nn, section k): the list of section k is list k + 1 (list 0 = the area-wide resources); verified against
# ROOM_COUNTS (every area has sections + 1 lists).  The WMD model of a room is the AREAnn/D*.GAM (kind 0x50) record of its list.
def list_to_room(ld, idx):
    """('AR04', 3) -> (4, 2) (area, section), or None for the area-wide list 0."""
    return (int(ld[2:]), idx - 1) if idx >= 1 else None


def room_to_list(area, section):
    """(4, 2) -> ('AR04', 3)."""
    return "AR%02d" % area, section + 1
