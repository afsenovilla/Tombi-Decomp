# Scratchpad variables (0x1F800000, 1 KB fast RAM) used by MAIN0
The game keeps its hottest globals here (Ghidra calls them `DAT_1f8001xx`).

| Address | Use (to be confirmed) |
|---|---|
| 1F800164 | pointer to the next free primitive |
| 1F8001C8 | bit 0 = horizontal axis orientation (swaps the objects' X/Z); see object_layout.md |
| 1F8001D4 | pointer to the current entry of the thread table |
| 1F8001E0 | ordering table (OT) of the current buffer |
| 1F8001E4 | OT of the previous buffer |
| 1F8001F4 | double-buffer index (0/1) |
| 1F800208 | pointer to the free object list (pool) |
| 1F800238 | number of free objects |

Other structures (main RAM): drawing double buffer at 8009D6A8 (stride 0xD10),
thread table at 801FD800 (0x38 bytes per entry, 6 entries), 24 sound voices.
