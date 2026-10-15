// FUNC 8004eb08 2108 MAIN0
// HEADERS tomba24
// MATCHING 8004eb08 2108
// Portado de psx_tomba (main/system/cdfile.c, cdLoadTask); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

s32 fixedMulSin(s16 arg0, s16 arg1);
s32 fixedMulCos(s16 arg0, s16 arg1);
s32 fixedMulSin2(s16 arg0, s16 arg1);

/* PAL file table (MAIN0.EXE 0x8007DEF0, tools/cdfiles.py dump): CdlLOC + size of every file cdLoadTask reads,
   indexed by file id. The US source's load lists, sound set / movie / area tables are other data of MAIN0
   (src/data/main0); only this table belongs to cdLoadTask (FILE_LINK_SIZES below is its size column). */
fileLink FILE_LINKS[1119] = {
    { { itob(36), itob(56), itob(38), 0 }, 0x1E68 }, /*    0 SYS/LD0SYS.BIN */
    { { itob(36), itob(56), itob(12), 0 }, 0x890 }, /*    1 SYS/LD0AR00.BIN */
    { { itob(36), itob(56), itob(14), 0 }, 0x79C }, /*    2 SYS/LD0AR01.BIN */
    { { itob(36), itob(56), itob(15), 0 }, 0x87C }, /*    3 SYS/LD0AR02.BIN */
    { { itob(36), itob(56), itob(17), 0 }, 0x944 }, /*    4 SYS/LD0AR03.BIN */
    { { itob(36), itob(56), itob(19), 0 }, 0xE90 }, /*    5 SYS/LD0AR04.BIN */
    { { itob(36), itob(56), itob(21), 0 }, 0x680 }, /*    6 SYS/LD0AR05.BIN */
    { { itob(36), itob(56), itob(22), 0 }, 0x49C }, /*    7 SYS/LD0AR06.BIN */
    { { itob(36), itob(56), itob(23), 0 }, 0x724 }, /*    8 SYS/LD0AR07.BIN */
    { { itob(36), itob(56), itob(24), 0 }, 0x590 }, /*    9 SYS/LD0AR08.BIN */
    { { itob(36), itob(56), itob(25), 0 }, 0x7CC }, /*   10 SYS/LD0AR09.BIN */
    { { itob(36), itob(56), itob(26), 0 }, 0xB44 }, /*   11 SYS/LD0AR10.BIN */
    { { itob(36), itob(56), itob(28), 0 }, 0x418 }, /*   12 SYS/LD0AR11.BIN */
    { { itob(36), itob(56), itob(29), 0 }, 0xE40 }, /*   13 SYS/LD0AR12.BIN */
    { { itob(36), itob(56), itob(31), 0 }, 0x358 }, /*   14 SYS/LD0AR13.BIN */
    { { itob(36), itob(56), itob(32), 0 }, 0x834 }, /*   15 SYS/LD0AR14.BIN */
    { { itob(36), itob(56), itob(34), 0 }, 0x5C4 }, /*   16 SYS/LD0AR16.BIN */
    { { itob(36), itob(56), itob(35), 0 }, 0x69C }, /*   17 SYS/LD0AR17.BIN */
    { { itob(36), itob(56), itob(36), 0 }, 0x370 }, /*   18 SYS/LD0AR18.BIN */
    { { itob(36), itob(56), itob(37), 0 }, 0x564 }, /*   19 SYS/LD0AR19.BIN */
    { { itob(36), itob(59), itob(71), 0 }, 0x3436 }, /*   20 SYSTEM/A00000.GAM */
    { { itob(37), itob(0), itob(3), 0 }, 0x200 }, /*   21 SYSTEM/A00001.000 */
    { { itob(37), itob(0), itob(4), 0 }, 0x80 }, /*   22 SYSTEM/A00002.000 */
    { { itob(38), itob(31), itob(69), 0 }, 0x3CBE }, /*   23 SYSTEM/A10003.GAM */
    { { itob(38), itob(32), itob(2), 0 }, 0x200 }, /*   24 SYSTEM/A10004.000 */
    { { itob(37), itob(0), itob(5), 0 }, 0x120 }, /*   25 SYSTEM/A00005.000 */
    { { itob(37), itob(0), itob(6), 0 }, 0x6C84 }, /*   26 SYSTEM/A00006.GAM */
    { { itob(37), itob(0), itob(20), 0 }, 0x200 }, /*   27 SYSTEM/A00007.000 */
    { { itob(39), itob(12), itob(64), 0 }, 0x346C }, /*   28 SYSTEM/O0PTSUB0.BIN */
    { { itob(37), itob(0), itob(21), 0 }, 0x6C84 }, /*   29 SYSTEM/A00100.GAM */
    { { itob(37), itob(0), itob(35), 0 }, 0x200 }, /*   30 SYSTEM/A00101.000 */
    { { itob(37), itob(0), itob(36), 0 }, 0x10B9B }, /*   31 SYSTEM/A00200.GAM */
    { { itob(37), itob(0), itob(70), 0 }, 0x200 }, /*   32 SYSTEM/A00201.000 */
    { { itob(37), itob(0), itob(71), 0 }, 0x10000 }, /*   33 SYSTEM/A00202.000 */
    { { itob(37), itob(1), itob(28), 0 }, 0x200 }, /*   34 SYSTEM/A00203.000 */
    { { itob(37), itob(1), itob(29), 0 }, 0x8000 }, /*   35 SYSTEM/A00204.000 */
    { { itob(37), itob(1), itob(45), 0 }, 0x60 }, /*   36 SYSTEM/A00205.000 */
    { { itob(37), itob(1), itob(46), 0 }, 0x100 }, /*   37 SYSTEM/A00206.000 */
    { { itob(37), itob(1), itob(47), 0 }, 0x18000 }, /*   38 SYSTEM/A00300.000 */
    { { itob(38), itob(38), itob(57), 0 }, 0x2912C }, /*   39 SYSTEM/D0SPSUB.BIN */
    { { itob(38), itob(32), itob(3), 0 }, 0x8000 }, /*   40 SYSTEM/A10301.000 */
    { { itob(37), itob(2), itob(20), 0 }, 0x18000 }, /*   41 SYSTEM/A00400.000 */
    { { itob(37), itob(2), itob(68), 0 }, 0x8000 }, /*   42 SYSTEM/A00401.000 */
    { { itob(38), itob(58), itob(58), 0 }, 0xC00 }, /*   43 SYSTEM/DSPCLUT.TIM */
    { { itob(37), itob(3), itob(9), 0 }, 0x200 }, /*   44 SYSTEM/A00402.000 */
    { { itob(38), itob(39), itob(65), 0 }, 0xF6E8 }, /*   45 SYSTEM/D0SPSUB2.BIN */
    { { itob(37), itob(3), itob(10), 0 }, 0x18000 }, /*   46 SYSTEM/A00500.000 */
    { { itob(37), itob(3), itob(58), 0 }, 0x8000 }, /*   47 SYSTEM/A00501.000 */
    { { itob(37), itob(3), itob(74), 0 }, 0x200 }, /*   48 SYSTEM/A00502.000 */
    { { itob(37), itob(4), itob(0), 0 }, 0x10000 }, /*   49 SYSTEM/A00600.000 */
    { { itob(38), itob(40), itob(21), 0 }, 0x18B60 }, /*   50 SYSTEM/D0SPSUB3.BIN */
    { { itob(38), itob(32), itob(19), 0 }, 0x8000 }, /*   51 SYSTEM/A10601.000 */
    { { itob(37), itob(4), itob(32), 0 }, 0x8000 }, /*   52 SYSTEM/A00700.000 */
    { { itob(37), itob(4), itob(48), 0 }, 0x20000 }, /*   53 SYSTEM/A00701.000 */
    { { itob(37), itob(5), itob(37), 0 }, 0x200 }, /*   54 SYSTEM/A00702.000 */
    { { itob(39), itob(13), itob(52), 0 }, 0xA838 }, /*   55 SYSTEM/SUBMAP.WMD */
    { { itob(38), itob(32), itob(35), 0 }, 0x8000 }, /*   56 SYSTEM/A10703.000 */
    { { itob(37), itob(5), itob(38), 0 }, 0x2037 }, /*   57 SYSTEM/A00800.GAM */
    { { itob(37), itob(5), itob(43), 0 }, 0xBF8C }, /*   58 SYSTEM/A00801.GAM */
    { { itob(37), itob(5), itob(67), 0 }, 0x200 }, /*   59 SYSTEM/A00802.000 */
    { { itob(37), itob(5), itob(68), 0 }, 0x60 }, /*   60 SYSTEM/A00803.000 */
    { { itob(37), itob(5), itob(69), 0 }, 0x8000 }, /*   61 SYSTEM/A00804.000 */
    { { itob(37), itob(6), itob(10), 0 }, 0x200 }, /*   62 SYSTEM/A00805.000 */
    { { itob(37), itob(6), itob(11), 0 }, 0x2400 }, /*   63 SYSTEM/A00900.000 */
    { { itob(37), itob(6), itob(16), 0 }, 0x30000 }, /*   64 SYSTEM/A01000.000 */
    { { itob(39), itob(13), itob(74), 0 }, 0x53F50 }, /*   65 SYSTEM/X000.BIN */
    { { itob(37), itob(7), itob(37), 0 }, 0x40000 }, /*   66 SYSTEM/A01100.000 */
    { { itob(37), itob(9), itob(15), 0 }, 0x10000 }, /*   67 SYSTEM/A01200.000 */
    { { itob(37), itob(9), itob(47), 0 }, 0x10000 }, /*   68 SYSTEM/A01300.000 */
    { { itob(37), itob(10), itob(4), 0 }, 0x8000 }, /*   69 SYSTEM/A01301.000 */
    { { itob(37), itob(10), itob(20), 0 }, 0x8000 }, /*   70 SYSTEM/A01302.000 */
    { { itob(37), itob(10), itob(36), 0 }, 0x30000 }, /*   71 SYSTEM/A01400.000 */
    { { itob(39), itob(16), itob(17), 0 }, 0x57F7C }, /*   72 SYSTEM/X001.BIN */
    { { itob(37), itob(11), itob(57), 0 }, 0x30000 }, /*   73 SYSTEM/A01500.000 */
    { { itob(37), itob(13), itob(3), 0 }, 0x30000 }, /*   74 SYSTEM/A01600.000 */
    { { itob(37), itob(14), itob(24), 0 }, 0x30000 }, /*   75 SYSTEM/A01700.000 */
    { { itob(37), itob(15), itob(45), 0 }, 0x18000 }, /*   76 SYSTEM/A01800.000 */
    { { itob(37), itob(16), itob(18), 0 }, 0x20000 }, /*   77 SYSTEM/A01801.000 */
    { { itob(38), itob(59), itob(3), 0 }, 0xD4B4 }, /*   78 SYSTEM/I0NFO.BIN */
    { { itob(37), itob(17), itob(7), 0 }, 0x30000 }, /*   79 SYSTEM/A01900.000 */
    { { itob(39), itob(18), itob(43), 0 }, 0x37CE0 }, /*   80 SYSTEM/X002.BIN */
    { { itob(37), itob(18), itob(28), 0 }, 0x10000 }, /*   81 SYSTEM/A02000.000 */
    { { itob(37), itob(18), itob(60), 0 }, 0x10000 }, /*   82 SYSTEM/A02001.000 */
    { { itob(37), itob(19), itob(17), 0 }, 0x18000 }, /*   83 SYSTEM/A02100.000 */
    { { itob(39), itob(0), itob(23), 0 }, 0x9848 }, /*   84 SYSTEM/I0NFO3.BIN */
    { { itob(37), itob(19), itob(65), 0 }, 0x10000 }, /*   85 SYSTEM/A02200.000 */
    { { itob(37), itob(20), itob(22), 0 }, 0x10000 }, /*   86 SYSTEM/A02201.000 */
    { { itob(37), itob(20), itob(54), 0 }, 0x8000 }, /*   87 SYSTEM/A02202.000 */
    { { itob(37), itob(20), itob(70), 0 }, 0x10000 }, /*   88 SYSTEM/A02300.000 */
    { { itob(37), itob(21), itob(27), 0 }, 0x10000 }, /*   89 SYSTEM/A02301.000 */
    { { itob(37), itob(21), itob(59), 0 }, 0x8000 }, /*   90 SYSTEM/A02302.000 */
    { { itob(37), itob(22), itob(0), 0 }, 0x18000 }, /*   91 SYSTEM/A02400.000 */
    { { itob(37), itob(22), itob(48), 0 }, 0x18000 }, /*   92 SYSTEM/A02401.000 */
    { { itob(39), itob(20), itob(5), 0 }, 0x52A48 }, /*   93 SYSTEM/X003.BIN */
    { { itob(37), itob(23), itob(21), 0 }, 0x10000 }, /*   94 SYSTEM/A02500.000 */
    { { itob(37), itob(23), itob(53), 0 }, 0x10000 }, /*   95 SYSTEM/A02501.000 */
    { { itob(37), itob(24), itob(10), 0 }, 0x18000 }, /*   96 SYSTEM/A02600.000 */
    { { itob(37), itob(24), itob(58), 0 }, 0x18000 }, /*   97 SYSTEM/A02601.000 */
    { { itob(37), itob(25), itob(31), 0 }, 0x10000 }, /*   98 SYSTEM/A02700.000 */
    { { itob(37), itob(25), itob(63), 0 }, 0x8000 }, /*   99 SYSTEM/A02701.000 */
    { { itob(37), itob(26), itob(4), 0 }, 0x18000 }, /*  100 SYSTEM/A02800.000 */
    { { itob(37), itob(26), itob(52), 0 }, 0x18000 }, /*  101 SYSTEM/A02801.000 */
    { { itob(39), itob(22), itob(21), 0 }, 0x4DAE4 }, /*  102 SYSTEM/X004.BIN */
    { { itob(37), itob(27), itob(25), 0 }, 0x30000 }, /*  103 SYSTEM/A03000.000 */
    { { itob(37), itob(28), itob(46), 0 }, 0x30000 }, /*  104 SYSTEM/A03100.000 */
    { { itob(37), itob(29), itob(67), 0 }, 0x18000 }, /*  105 SYSTEM/A03200.000 */
    { { itob(37), itob(30), itob(40), 0 }, 0x8000 }, /*  106 SYSTEM/A03300.000 */
    { { itob(37), itob(30), itob(56), 0 }, 0x30000 }, /*  107 SYSTEM/A03400.000 */
    { { itob(37), itob(32), itob(2), 0 }, 0x30000 }, /*  108 SYSTEM/A03500.000 */
    { { itob(37), itob(33), itob(23), 0 }, 0x10000 }, /*  109 SYSTEM/A03600.000 */
    { { itob(37), itob(33), itob(55), 0 }, 0x30000 }, /*  110 SYSTEM/A03700.000 */
    { { itob(37), itob(35), itob(1), 0 }, 0x30000 }, /*  111 SYSTEM/A03800.000 */
    { { itob(37), itob(36), itob(22), 0 }, 0x38000 }, /*  112 SYSTEM/A03900.000 */
    { { itob(38), itob(59), itob(30), 0 }, 0x218DC }, /*  113 SYSTEM/I0NFO2.BIN */
    { { itob(37), itob(37), itob(59), 0 }, 0x30000 }, /*  114 SYSTEM/A04000.000 */
    { { itob(39), itob(24), itob(27), 0 }, 0x32718 }, /*  115 SYSTEM/X005.BIN */
    { { itob(37), itob(39), itob(5), 0 }, 0x18000 }, /*  116 SYSTEM/A04100.000 */
    { { itob(39), itob(25), itob(53), 0 }, 0x3B114 }, /*  117 SYSTEM/X006.BIN */
    { { itob(37), itob(39), itob(53), 0 }, 0x10000 }, /*  118 SYSTEM/A04200.000 */
    { { itob(37), itob(40), itob(10), 0 }, 0x10000 }, /*  119 SYSTEM/A04201.000 */
    { { itob(37), itob(40), itob(42), 0 }, 0x10000 }, /*  120 SYSTEM/A04202.000 */
    { { itob(37), itob(40), itob(74), 0 }, 0x18000 }, /*  121 SYSTEM/A04300.000 */
    { { itob(37), itob(41), itob(47), 0 }, 0x18000 }, /*  122 SYSTEM/A04301.000 */
    { { itob(37), itob(42), itob(20), 0 }, 0x18000 }, /*  123 SYSTEM/A04400.000 */
    { { itob(37), itob(42), itob(68), 0 }, 0x18000 }, /*  124 SYSTEM/A04401.000 */
    { { itob(37), itob(43), itob(41), 0 }, 0x20000 }, /*  125 SYSTEM/A04500.000 */
    { { itob(37), itob(44), itob(30), 0 }, 0x8000 }, /*  126 SYSTEM/A04501.000 */
    { { itob(37), itob(44), itob(46), 0 }, 0x30000 }, /*  127 SYSTEM/A04600.000 */
    { { itob(37), itob(45), itob(67), 0 }, 0x18000 }, /*  128 SYSTEM/A04700.000 */
    { { itob(37), itob(46), itob(40), 0 }, 0x10000 }, /*  129 SYSTEM/A04800.000 */
    { { itob(37), itob(46), itob(72), 0 }, 0x10000 }, /*  130 SYSTEM/A04801.000 */
    { { itob(37), itob(47), itob(29), 0 }, 0x8000 }, /*  131 SYSTEM/A04802.000 */
    { { itob(39), itob(27), itob(22), 0 }, 0x3058C }, /*  132 SYSTEM/X008.BIN */
    { { itob(37), itob(47), itob(45), 0 }, 0x30000 }, /*  133 SYSTEM/A04900.000 */
    { { itob(39), itob(28), itob(44), 0 }, 0x491EC }, /*  134 SYSTEM/X009.BIN */
    { { itob(37), itob(48), itob(66), 0 }, 0x38000 }, /*  135 SYSTEM/A05000.000 */
    { { itob(37), itob(50), itob(28), 0 }, 0x10000 }, /*  136 SYSTEM/A05100.000 */
    { { itob(37), itob(50), itob(60), 0 }, 0x10000 }, /*  137 SYSTEM/A05101.000 */
    { { itob(37), itob(51), itob(17), 0 }, 0x10000 }, /*  138 SYSTEM/A05102.000 */
    { { itob(37), itob(51), itob(49), 0 }, 0x8000 }, /*  139 SYSTEM/A05103.000 */
    { { itob(37), itob(51), itob(65), 0 }, 0x10000 }, /*  140 SYSTEM/A05200.000 */
    { { itob(37), itob(52), itob(22), 0 }, 0x10000 }, /*  141 SYSTEM/A05201.000 */
    { { itob(37), itob(52), itob(54), 0 }, 0x10000 }, /*  142 SYSTEM/A05300.000 */
    { { itob(37), itob(53), itob(11), 0 }, 0x10000 }, /*  143 SYSTEM/A05301.000 */
    { { itob(37), itob(53), itob(43), 0 }, 0x10000 }, /*  144 SYSTEM/A05400.000 */
    { { itob(37), itob(54), itob(0), 0 }, 0x10000 }, /*  145 SYSTEM/A05401.000 */
    { { itob(37), itob(54), itob(32), 0 }, 0x10000 }, /*  146 SYSTEM/A05500.000 */
    { { itob(37), itob(54), itob(64), 0 }, 0x8000 }, /*  147 SYSTEM/A05501.000 */
    { { itob(37), itob(55), itob(5), 0 }, 0x8000 }, /*  148 SYSTEM/A05502.000 */
    { { itob(39), itob(30), itob(41), 0 }, 0x4AE54 }, /*  149 SYSTEM/X010.BIN */
    { { itob(37), itob(55), itob(21), 0 }, 0x8000 }, /*  150 SYSTEM/A05600.000 */
    { { itob(37), itob(55), itob(37), 0 }, 0x20000 }, /*  151 SYSTEM/A05700.000 */
    { { itob(37), itob(56), itob(26), 0 }, 0x18000 }, /*  152 SYSTEM/A05800.000 */
    { { itob(37), itob(56), itob(74), 0 }, 0x18000 }, /*  153 SYSTEM/A05900.000 */
    { { itob(37), itob(57), itob(47), 0 }, 0x28000 }, /*  154 SYSTEM/A06000.000 */
    { { itob(37), itob(58), itob(52), 0 }, 0x38000 }, /*  155 SYSTEM/A06100.000 */
    { { itob(38), itob(0), itob(14), 0 }, 0x10000 }, /*  156 SYSTEM/A06200.000 */
    { { itob(38), itob(0), itob(46), 0 }, 0x10000 }, /*  157 SYSTEM/A06201.000 */
    { { itob(39), itob(32), itob(41), 0 }, 0x34704 }, /*  158 SYSTEM/X011.BIN */
    { { itob(38), itob(1), itob(3), 0 }, 0x10000 }, /*  159 SYSTEM/A06300.000 */
    { { itob(38), itob(1), itob(35), 0 }, 0x10000 }, /*  160 SYSTEM/A06301.000 */
    { { itob(38), itob(1), itob(67), 0 }, 0x20000 }, /*  161 SYSTEM/A06400.000 */
    { { itob(38), itob(2), itob(56), 0 }, 0x18000 }, /*  162 SYSTEM/A06500.000 */
    { { itob(38), itob(3), itob(29), 0 }, 0x10000 }, /*  163 SYSTEM/A06600.000 */
    { { itob(39), itob(33), itob(71), 0 }, 0x32D48 }, /*  164 SYSTEM/X013.BIN */
    { { itob(38), itob(3), itob(61), 0 }, 0x18000 }, /*  165 SYSTEM/A06800.000 */
    { { itob(38), itob(4), itob(34), 0 }, 0x18000 }, /*  166 SYSTEM/A06801.000 */
    { { itob(39), itob(35), itob(23), 0 }, 0x42780 }, /*  167 SYSTEM/X014.BIN */
    { { itob(38), itob(5), itob(7), 0 }, 0x18000 }, /*  168 SYSTEM/A06900.000 */
    { { itob(38), itob(5), itob(55), 0 }, 0x10000 }, /*  169 SYSTEM/A06901.000 */
    { { itob(38), itob(6), itob(12), 0 }, 0x18000 }, /*  170 SYSTEM/A07000.000 */
    { { itob(38), itob(6), itob(60), 0 }, 0x18000 }, /*  171 SYSTEM/A07001.000 */
    { { itob(38), itob(7), itob(33), 0 }, 0x18000 }, /*  172 SYSTEM/A07100.000 */
    { { itob(38), itob(8), itob(6), 0 }, 0x18000 }, /*  173 SYSTEM/A07101.000 */
    { { itob(38), itob(8), itob(54), 0 }, 0x18000 }, /*  174 SYSTEM/A07200.000 */
    { { itob(38), itob(9), itob(27), 0 }, 0x18000 }, /*  175 SYSTEM/A07201.000 */
    { { itob(38), itob(10), itob(0), 0 }, 0x18000 }, /*  176 SYSTEM/A07300.000 */
    { { itob(38), itob(10), itob(48), 0 }, 0x18000 }, /*  177 SYSTEM/A07301.000 */
    { { itob(38), itob(11), itob(21), 0 }, 0x18000 }, /*  178 SYSTEM/A07400.000 */
    { { itob(38), itob(11), itob(69), 0 }, 0x18000 }, /*  179 SYSTEM/A07401.000 */
    { { itob(38), itob(12), itob(42), 0 }, 0x18000 }, /*  180 SYSTEM/A07500.000 */
    { { itob(38), itob(13), itob(15), 0 }, 0x18000 }, /*  181 SYSTEM/A07501.000 */
    { { itob(38), itob(13), itob(63), 0 }, 0x28000 }, /*  182 SYSTEM/A07600.000 */
    { { itob(38), itob(14), itob(68), 0 }, 0x10000 }, /*  183 SYSTEM/A07601.000 */
    { { itob(38), itob(15), itob(25), 0 }, 0x10000 }, /*  184 SYSTEM/A07700.000 */
    { { itob(38), itob(15), itob(57), 0 }, 0x10000 }, /*  185 SYSTEM/A07701.000 */
    { { itob(38), itob(16), itob(14), 0 }, 0x10000 }, /*  186 SYSTEM/A07702.000 */
    { { itob(39), itob(37), itob(6), 0 }, 0x33704 }, /*  187 SYSTEM/X016.BIN */
    { { itob(38), itob(16), itob(46), 0 }, 0x10000 }, /*  188 SYSTEM/A07800.000 */
    { { itob(38), itob(17), itob(3), 0 }, 0x10000 }, /*  189 SYSTEM/A07801.000 */
    { { itob(38), itob(17), itob(35), 0 }, 0x10000 }, /*  190 SYSTEM/A07900.000 */
    { { itob(38), itob(17), itob(67), 0 }, 0x10000 }, /*  191 SYSTEM/A07901.000 */
    { { itob(38), itob(18), itob(24), 0 }, 0x38000 }, /*  192 SYSTEM/A08000.000 */
    { { itob(38), itob(19), itob(61), 0 }, 0x10000 }, /*  193 SYSTEM/A08100.000 */
    { { itob(38), itob(20), itob(18), 0 }, 0x10000 }, /*  194 SYSTEM/A08101.000 */
    { { itob(39), itob(38), itob(34), 0 }, 0x34678 }, /*  195 SYSTEM/X017.BIN */
    { { itob(38), itob(20), itob(50), 0 }, 0x10000 }, /*  196 SYSTEM/A08200.000 */
    { { itob(38), itob(21), itob(7), 0 }, 0x10000 }, /*  197 SYSTEM/A08201.000 */
    { { itob(38), itob(21), itob(39), 0 }, 0x10000 }, /*  198 SYSTEM/A08202.000 */
    { { itob(38), itob(21), itob(71), 0 }, 0x10000 }, /*  199 SYSTEM/A08300.000 */
    { { itob(38), itob(22), itob(28), 0 }, 0x10000 }, /*  200 SYSTEM/A08301.000 */
    { { itob(38), itob(22), itob(60), 0 }, 0x10000 }, /*  201 SYSTEM/A08400.000 */
    { { itob(38), itob(23), itob(17), 0 }, 0x10000 }, /*  202 SYSTEM/A08401.000 */
    { { itob(38), itob(23), itob(49), 0 }, 0x10000 }, /*  203 SYSTEM/A08500.000 */
    { { itob(38), itob(24), itob(6), 0 }, 0x10000 }, /*  204 SYSTEM/A08501.000 */
    { { itob(38), itob(24), itob(38), 0 }, 0x10000 }, /*  205 SYSTEM/A08502.000 */
    { { itob(38), itob(24), itob(70), 0 }, 0x28000 }, /*  206 SYSTEM/A08600.000 */
    { { itob(38), itob(26), itob(0), 0 }, 0x10000 }, /*  207 SYSTEM/A08601.000 */
    { { itob(39), itob(39), itob(64), 0 }, 0x34D2C }, /*  208 SYSTEM/X018.BIN */
    { { itob(38), itob(26), itob(32), 0 }, 0x10000 }, /*  209 SYSTEM/A08800.000 */
    { { itob(38), itob(26), itob(64), 0 }, 0x10000 }, /*  210 SYSTEM/A08801.000 */
    { { itob(38), itob(27), itob(21), 0 }, 0x8000 }, /*  211 SYSTEM/A08802.000 */
    { { itob(38), itob(27), itob(37), 0 }, 0x30000 }, /*  212 SYSTEM/A08900.000 */
    { { itob(38), itob(28), itob(58), 0 }, 0x20000 }, /*  213 SYSTEM/A09000.000 */
    { { itob(38), itob(29), itob(47), 0 }, 0x8000 }, /*  214 SYSTEM/A09001.000 */
    { { itob(38), itob(29), itob(63), 0 }, 0x8000 }, /*  215 SYSTEM/A09002.000 */
    { { itob(38), itob(30), itob(4), 0 }, 0x10000 }, /*  216 SYSTEM/A09100.000 */
    { { itob(38), itob(30), itob(36), 0 }, 0x10000 }, /*  217 SYSTEM/A09101.000 */
    { { itob(38), itob(30), itob(68), 0 }, 0x10000 }, /*  218 SYSTEM/A09102.000 */
    { { itob(38), itob(31), itob(25), 0 }, 0x8000 }, /*  219 SYSTEM/A09200.000 */
    { { itob(38), itob(58), itob(60), 0 }, 0xCAC }, /*  220 SYSTEM/G0OVER.BIN */
    { { itob(38), itob(31), itob(41), 0 }, 0xB19C }, /*  221 SYSTEM/A09300.000 */
    { { itob(38), itob(31), itob(64), 0 }, 0x1E7C }, /*  222 SYSTEM/A09400.GAM */
    { { itob(38), itob(31), itob(68), 0 }, 0x200 }, /*  223 SYSTEM/A09401.000 */
    { { itob(36), itob(50), itob(65), 0 }, 0x4A6CC }, /*  224 SOUND/SND_INI.WVD */
    { { itob(36), itob(52), itob(64), 0 }, 0x2582C }, /*  225 SOUND/SND_LOGO.WVD */
    { { itob(36), itob(54), itob(22), 0 }, 0xB7BC }, /*  226 SOUND/SND_TTL.WVD */
    { { itob(36), itob(56), itob(5), 0 }, 0x5A1 }, /*  227 SOUND/S_TITLE.SEQ */
    { { itob(36), itob(53), itob(65), 0 }, 0xFBCC }, /*  228 SOUND/SND_SYS.WVD */
    { { itob(36), itob(56), itob(3), 0 }, 0xE10 }, /*  229 SOUND/S_SYS00.SEQ */
    { { itob(36), itob(7), itob(8), 0 }, 0x2F564 }, /*  230 SOUND/SND0000.WVD */
    { { itob(36), itob(54), itob(45), 0 }, 0x2470 }, /*  231 SOUND/S_AR000.SEQ */
    { { itob(36), itob(8), itob(28), 0 }, 0x1E394 }, /*  232 SOUND/SND0003.WVD */
    { { itob(36), itob(54), itob(50), 0 }, 0xD19 }, /*  233 SOUND/S_AR003.SEQ */
    { { itob(36), itob(10), itob(42), 0 }, 0x1F77C }, /*  234 SOUND/SND001_.WVD */
    { { itob(36), itob(54), itob(52), 0 }, 0x1036 }, /*  235 SOUND/S_AR010.SEQ */
    { { itob(36), itob(11), itob(72), 0 }, 0x16994 }, /*  236 SOUND/SND0020.WVD */
    { { itob(36), itob(54), itob(59), 0 }, 0xF96 }, /*  237 SOUND/S_AR020.SEQ */
    { { itob(36), itob(12), itob(43), 0 }, 0x15E04 }, /*  238 SOUND/SND0023.WVD */
    { { itob(36), itob(54), itob(61), 0 }, 0xCC7 }, /*  239 SOUND/S_AR023.SEQ */
    { { itob(36), itob(13), itob(12), 0 }, 0x319A4 }, /*  240 SOUND/SND0030.WVD */
    { { itob(36), itob(54), itob(63), 0 }, 0x22D0 }, /*  241 SOUND/S_AR030.SEQ */
    { { itob(36), itob(14), itob(37), 0 }, 0x22CF4 }, /*  242 SOUND/SND0030X.WVD */
    { { itob(36), itob(54), itob(68), 0 }, 0x11CD }, /*  243 SOUND/S_AR030X.SEQ */
    { { itob(36), itob(15), itob(32), 0 }, 0x24A04 }, /*  244 SOUND/SND0032.WVD */
    { { itob(36), itob(54), itob(71), 0 }, 0x7FF }, /*  245 SOUND/S_AR032.SEQ */
    { { itob(36), itob(16), itob(31), 0 }, 0x1E6A4 }, /*  246 SOUND/SND0032X.WVD */
    { { itob(36), itob(54), itob(72), 0 }, 0xBA6 }, /*  247 SOUND/S_AR032X.SEQ */
    { { itob(36), itob(17), itob(17), 0 }, 0x2DEC4 }, /*  248 SOUND/SND0040.WVD */
    { { itob(36), itob(54), itob(74), 0 }, 0x1630 }, /*  249 SOUND/S_AR040.SEQ */
    { { itob(36), itob(19), itob(11), 0 }, 0x2F504 }, /*  250 SOUND/SND0044.WVD */
    { { itob(36), itob(55), itob(6), 0 }, 0xF04 }, /*  251 SOUND/S_AR044.SEQ */
    { { itob(36), itob(21), itob(65), 0 }, 0x1918C }, /*  252 SOUND/SND0050.WVD */
    { { itob(36), itob(55), itob(8), 0 }, 0xB03 }, /*  253 SOUND/S_AR050.SEQ */
    { { itob(36), itob(22), itob(41), 0 }, 0x2ABA4 }, /*  254 SOUND/SND0060.WVD */
    { { itob(36), itob(55), itob(10), 0 }, 0x2801 }, /*  255 SOUND/S_AR060.SEQ */
    { { itob(36), itob(23), itob(52), 0 }, 0x17A8C }, /*  256 SOUND/SND0062.WVD */
    { { itob(36), itob(55), itob(16), 0 }, 0xA94 }, /*  257 SOUND/S_AR062.SEQ */
    { { itob(36), itob(11), itob(30), 0 }, 0x14EDC }, /*  258 SOUND/SND001_X.WVD */
    { { itob(36), itob(54), itob(55), 0 }, 0x1945 }, /*  259 SOUND/S_AR010X.SEQ */
    { { itob(36), itob(24), itob(25), 0 }, 0x26064 }, /*  260 SOUND/SND0080.WVD */
    { { itob(36), itob(55), itob(18), 0 }, 0x72F }, /*  261 SOUND/S_AR080.SEQ */
    { { itob(36), itob(25), itob(27), 0 }, 0x2E8E4 }, /*  262 SOUND/SND0090.WVD */
    { { itob(36), itob(55), itob(19), 0 }, 0x19EB }, /*  263 SOUND/S_AR090.SEQ */
    { { itob(36), itob(26), itob(46), 0 }, 0x176BC }, /*  264 SOUND/SND0091.WVD */
    { { itob(36), itob(55), itob(23), 0 }, 0x125E }, /*  265 SOUND/S_AR091.SEQ */
    { { itob(36), itob(27), itob(18), 0 }, 0x14D0C }, /*  266 SOUND/SND0096.WVD */
    { { itob(36), itob(55), itob(26), 0 }, 0x84B }, /*  267 SOUND/S_AR096.SEQ */
    { { itob(36), itob(31), itob(64), 0 }, 0x1C25C }, /*  268 SOUND/SND010_.WVD */
    { { itob(36), itob(55), itob(28), 0 }, 0x1F61 }, /*  269 SOUND/S_AR100.SEQ */
    { { itob(36), itob(29), itob(17), 0 }, 0x2A9F4 }, /*  270 SOUND/SND0103.WVD */
    { { itob(36), itob(55), itob(37), 0 }, 0x89F }, /*  271 SOUND/S_AR103.SEQ */
    { { itob(36), itob(32), itob(46), 0 }, 0x1583C }, /*  272 SOUND/SND010_X.WVD */
    { { itob(36), itob(55), itob(32), 0 }, 0x264E }, /*  273 SOUND/S_AR100X.SEQ */
    { { itob(36), itob(30), itob(28), 0 }, 0x37304 }, /*  274 SOUND/SND0103X.WVD */
    { { itob(36), itob(55), itob(39), 0 }, 0xD95 }, /*  275 SOUND/S_AR103X.SEQ */
    { { itob(36), itob(33), itob(15), 0 }, 0x1174C }, /*  276 SOUND/SND0110.WVD */
    { { itob(36), itob(55), itob(41), 0 }, 0x2128 }, /*  277 SOUND/S_AR110.SEQ */
    { { itob(36), itob(21), itob(16), 0 }, 0x1805C }, /*  278 SOUND/SND004_X.WVD */
    { { itob(36), itob(55), itob(2), 0 }, 0x1AFB }, /*  279 SOUND/S_AR040X.SEQ */
    { { itob(36), itob(33), itob(50), 0 }, 0x1FF34 }, /*  280 SOUND/SND0130.WVD */
    { { itob(36), itob(55), itob(46), 0 }, 0x13A6 }, /*  281 SOUND/S_AR130.SEQ */
    { { itob(36), itob(50), itob(28), 0 }, 0x1242C }, /*  282 SOUND/SND_END.WVD */
    { { itob(36), itob(56), itob(0), 0 }, 0x17DB }, /*  283 SOUND/S_END.SEQ */
    { { itob(36), itob(34), itob(39), 0 }, 0x30B14 }, /*  284 SOUND/SND0140.WVD */
    { { itob(36), itob(55), itob(49), 0 }, 0x1BD7 }, /*  285 SOUND/S_AR140.SEQ */
    { { itob(36), itob(35), itob(62), 0 }, 0x2C9F4 }, /*  286 SOUND/SND0141.WVD */
    { { itob(36), itob(37), itob(2), 0 }, 0x30014 }, /*  287 SOUND/SND0142.WVD */
    { { itob(36), itob(38), itob(24), 0 }, 0x2BFD4 }, /*  288 SOUND/SND0143.WVD */
    { { itob(36), itob(39), itob(37), 0 }, 0x296D4 }, /*  289 SOUND/SND0144.WVD */
    { { itob(36), itob(40), itob(45), 0 }, 0x2A7D4 }, /*  290 SOUND/SND0145.WVD */
    { { itob(36), itob(41), itob(55), 0 }, 0x350E4 }, /*  291 SOUND/SND0146.WVD */
    { { itob(36), itob(43), itob(12), 0 }, 0x36344 }, /*  292 SOUND/SND0147.WVD */
    { { itob(36), itob(55), itob(53), 0 }, 0x349F }, /*  293 SOUND/S_AR147.SEQ */
    { { itob(36), itob(44), itob(46), 0 }, 0x2D8C4 }, /*  294 SOUND/SND0160.WVD */
    { { itob(36), itob(55), itob(60), 0 }, 0x107F }, /*  295 SOUND/S_AR160.SEQ */
    { { itob(36), itob(45), itob(63), 0 }, 0x34D34 }, /*  296 SOUND/SND0170.WVD */
    { { itob(36), itob(55), itob(63), 0 }, 0x193E }, /*  297 SOUND/S_AR170.SEQ */
    { { itob(36), itob(47), itob(19), 0 }, 0x32D84 }, /*  298 SOUND/SND0181.WVD */
    { { itob(36), itob(55), itob(69), 0 }, 0x22AF }, /*  299 SOUND/S_AR181.SEQ */
    { { itob(36), itob(48), itob(46), 0 }, 0x2F0C4 }, /*  300 SOUND/SND0182.WVD */
    { { itob(36), itob(55), itob(67), 0 }, 0xAE0 }, /*  301 SOUND/S_AR1802.SEQ */
    { { itob(36), itob(49), itob(66), 0 }, 0x12094 }, /*  302 SOUND/SND0192.WVD */
    { { itob(36), itob(55), itob(74), 0 }, 0x493 }, /*  303 SOUND/S_AR192.SEQ */
    { { itob(36), itob(9), itob(14), 0 }, 0xC77C }, /*  304 SOUND/SND0010.WVD */
    { { itob(36), itob(9), itob(39), 0 }, 0x14DCC }, /*  305 SOUND/SND0012.WVD */
    { { itob(36), itob(10), itob(6), 0 }, 0xE30C }, /*  306 SOUND/SND0013.WVD */
    { { itob(36), itob(10), itob(35), 0 }, 0x33EC }, /*  307 SOUND/SND0014.WVD */
    { { itob(36), itob(27), itob(60), 0 }, 0x1AF2C }, /*  308 SOUND/SND0100.WVD */
    { { itob(36), itob(28), itob(39), 0 }, 0x1A2DC }, /*  309 SOUND/SND0101.WVD */
    { { itob(36), itob(18), itob(34), 0 }, 0x19F5C }, /*  310 SOUND/SND0040X.WVD */
    { { itob(36), itob(20), itob(31), 0 }, 0x1DF0C }, /*  311 SOUND/SND0044X.WVD */
    { { itob(30), itob(41), itob(55), 0 }, 0x70EC00 }, /*  312 MOVIE/MIST.STR */
    { { itob(12), itob(13), itob(11), 0 }, 0xA6D100 }, /*  313 MOVIE/100_B.STR */
    { { itob(32), itob(9), itob(31), 0 }, 0x1F87100 }, /*  314 MOVIE/OP_INST.STR */
    { { itob(22), itob(36), itob(62), 0 }, 0x82E300 }, /*  315 MOVIE/BOY.STR */
    { { itob(29), itob(56), itob(54), 0 }, 0x275A00 }, /*  316 MOVIE/MCC.STR */
    { { itob(31), itob(23), itob(73), 0 }, 0x379B00 }, /*  317 MOVIE/NOZOITE1.STR */
    { { itob(35), itob(18), itob(8), 0 }, 0x41DF00 }, /*  318 MOVIE/SORA1.STR */
    { { itob(35), itob(42), itob(56), 0 }, 0x410400 }, /*  319 MOVIE/SORA2.STR */
    { { itob(31), itob(44), itob(58), 0 }, 0x41DF00 }, /*  320 MOVIE/NOZOITE2.STR */
    { { itob(30), itob(11), itob(33), 0 }, 0x50FC00 }, /*  321 MOVIE/MILLION.STR */
    { { itob(26), itob(55), itob(21), 0 }, 0x526900 }, /*  322 MOVIE/HONOO2.STR */
    { { itob(23), itob(25), itob(59), 0 }, 0x50FC00 }, /*  323 MOVIE/BUNMEI.STR */
    { { itob(23), itob(56), itob(6), 0 }, 0x39E300 }, /*  324 MOVIE/BUNMEI2.STR */
    { { itob(26), itob(24), itob(34), 0 }, 0x526900 }, /*  325 MOVIE/HANA.STR */
    { { itob(21), itob(34), itob(56), 0 }, 0x52FB00 }, /*  326 MOVIE/ARASHI.STR */
    { { itob(29), itob(25), itob(51), 0 }, 0x52FB00 }, /*  327 MOVIE/MANTION.STR */
    { { itob(22), itob(5), itob(59), 0 }, 0x52FB00 }, /*  328 MOVIE/BAKKASU.STR */
    { { itob(27), itob(26), itob(8), 0 }, 0x52FB00 }, /*  329 MOVIE/JUNGLE.STR */
    { { itob(27), itob(57), itob(11), 0 }, 0x52FB00 }, /*  330 MOVIE/KARAKURI.STR */
    { { itob(28), itob(43), itob(33), 0 }, 0x70EC00 }, /*  331 MOVIE/MABUTA.STR */
    { { itob(24), itob(17), itob(55), 0 }, 0x152C400 }, /*  332 MOVIE/END_EU.STR */
    { { itob(28), itob(28), itob(14), 0 }, 0x28C700 }, /*  333 MOVIE/LOGO.STR */
    { { itob(0), itob(6), itob(45), 0 }, 0xB2 }, /*  334 AREA00/A000.GAM */
    { { itob(0), itob(6), itob(46), 0 }, 0x367C }, /*  335 AREA00/A001.GAM */
    { { itob(0), itob(6), itob(53), 0 }, 0x22E7B }, /*  336 AREA00/A002.GAM */
    { { itob(0), itob(7), itob(48), 0 }, 0x12E1A }, /*  337 AREA00/A003.GAM */
    { { itob(0), itob(25), itob(20), 0 }, 0x53F50 }, /*  338 AREA00/X000.BIN */
    { { itob(0), itob(8), itob(11), 0 }, 0x42CE4 }, /*  339 AREA00/A004.000 */
    { { itob(0), itob(9), itob(70), 0 }, 0xF26B }, /*  340 AREA00/A005.GAM */
    { { itob(0), itob(10), itob(26), 0 }, 0x1DFB }, /*  341 AREA00/A106.GAM */
    { { itob(0), itob(15), itob(60), 0 }, 0x40EA }, /*  342 AREA00/CFNT999B.WFM */
    { { itob(0), itob(10), itob(62), 0 }, 0x24DF4 }, /*  343 AREA00/B000.GAM */
    { { itob(0), itob(16), itob(70), 0 }, 0x2993 }, /*  344 AREA00/CLUT01.GAM */
    { { itob(0), itob(17), itob(31), 0 }, 0x2D2BD }, /*  345 AREA00/D001.GAM */
    { { itob(0), itob(21), itob(72), 0 }, 0x8A16 }, /*  346 AREA00/MFNT000B.WFM */
    { { itob(0), itob(11), itob(61), 0 }, 0x24DF4 }, /*  347 AREA00/B100.GAM */
    { { itob(0), itob(17), itob(1), 0 }, 0x2993 }, /*  348 AREA00/CLUT02.GAM */
    { { itob(0), itob(18), itob(47), 0 }, 0x2D2BD }, /*  349 AREA00/D101.GAM */
    { { itob(0), itob(12), itob(60), 0 }, 0x24DF4 }, /*  350 AREA00/B200.GAM */
    { { itob(0), itob(17), itob(7), 0 }, 0x2993 }, /*  351 AREA00/CLUT03.GAM */
    { { itob(0), itob(19), itob(63), 0 }, 0x2D2BD }, /*  352 AREA00/D201.GAM */
    { { itob(0), itob(13), itob(59), 0 }, 0x63AE }, /*  353 AREA00/B300.GAM */
    { { itob(0), itob(13), itob(72), 0 }, 0x30000 }, /*  354 AREA00/B301.000 */
    { { itob(0), itob(17), itob(13), 0 }, 0x29E2 }, /*  355 AREA00/CLUT04.GAM */
    { { itob(0), itob(21), itob(4), 0 }, 0x1A9 }, /*  356 AREA00/D302.GAM */
    { { itob(0), itob(24), itob(10), 0 }, 0x4664 }, /*  357 AREA00/MFNT001B.WFM */
    { { itob(0), itob(15), itob(18), 0 }, 0x5DDC }, /*  358 AREA00/B400.GAM */
    { { itob(0), itob(15), itob(30), 0 }, 0x362 }, /*  359 AREA00/B401.GAM */
    { { itob(0), itob(17), itob(19), 0 }, 0x2949 }, /*  360 AREA00/CLUT05.GAM */
    { { itob(0), itob(21), itob(5), 0 }, 0x1400B }, /*  361 AREA00/D402.GAM */
    { { itob(0), itob(15), itob(31), 0 }, 0x6270 }, /*  362 AREA00/B500.GAM */
    { { itob(0), itob(15), itob(44), 0 }, 0xD24 }, /*  363 AREA00/B501.GAM */
    { { itob(0), itob(15), itob(46), 0 }, 0x390 }, /*  364 AREA00/B502.GAM */
    { { itob(0), itob(15), itob(47), 0 }, 0x671A }, /*  365 AREA00/B503.GAM */
    { { itob(0), itob(17), itob(25), 0 }, 0x2EA5 }, /*  366 AREA00/CLUT06.GAM */
    { { itob(0), itob(21), itob(46), 0 }, 0xC852 }, /*  367 AREA00/D504.GAM */
    { { itob(0), itob(45), itob(35), 0 }, 0xB2 }, /*  368 AREA01/A000.GAM */
    { { itob(0), itob(45), itob(36), 0 }, 0x367C }, /*  369 AREA01/A001.GAM */
    { { itob(0), itob(45), itob(43), 0 }, 0x14A51 }, /*  370 AREA01/A002.GAM */
    { { itob(0), itob(46), itob(10), 0 }, 0xEFBA }, /*  371 AREA01/A003.GAM */
    { { itob(0), itob(46), itob(40), 0 }, 0xB39A }, /*  372 AREA01/A004.GAM */
    { { itob(0), itob(46), itob(63), 0 }, 0x2E008 }, /*  373 AREA01/A005.000 */
    { { itob(0), itob(48), itob(6), 0 }, 0xB171 }, /*  374 AREA01/A006.GAM */
    { { itob(1), itob(1), itob(59), 0 }, 0x57F7C }, /*  375 AREA01/X001.BIN */
    { { itob(0), itob(48), itob(29), 0 }, 0x1DFB }, /*  376 AREA01/A107.GAM */
    { { itob(0), itob(59), itob(22), 0 }, 0x9F7A }, /*  377 AREA01/MFNT010B.WFM */
    { { itob(0), itob(53), itob(64), 0 }, 0x40EA }, /*  378 AREA01/CFNT999B.WFM */
    { { itob(0), itob(48), itob(65), 0 }, 0x17AB7 }, /*  379 AREA01/B000.GAM */
    { { itob(0), itob(49), itob(38), 0 }, 0x2C37 }, /*  380 AREA01/B001.GAM */
    { { itob(0), itob(49), itob(44), 0 }, 0xC1B8 }, /*  381 AREA01/B002.GAM */
    { { itob(0), itob(49), itob(69), 0 }, 0x2D50 }, /*  382 AREA01/B003.GAM */
    { { itob(0), itob(54), itob(74), 0 }, 0x2A0D }, /*  383 AREA01/CLUT01.GAM */
    { { itob(0), itob(55), itob(29), 0 }, 0x1DE75 }, /*  384 AREA01/D004.GAM */
    { { itob(0), itob(50), itob(0), 0 }, 0x17AB7 }, /*  385 AREA01/B100.GAM */
    { { itob(0), itob(50), itob(48), 0 }, 0x2C37 }, /*  386 AREA01/B101.GAM */
    { { itob(0), itob(50), itob(54), 0 }, 0xC1B8 }, /*  387 AREA01/B102.GAM */
    { { itob(0), itob(51), itob(4), 0 }, 0x2D50 }, /*  388 AREA01/B103.GAM */
    { { itob(0), itob(55), itob(5), 0 }, 0x2A0D }, /*  389 AREA01/CLUT02.GAM */
    { { itob(0), itob(56), itob(14), 0 }, 0x1DE75 }, /*  390 AREA01/D104.GAM */
    { { itob(0), itob(51), itob(10), 0 }, 0x166D4 }, /*  391 AREA01/B200.GAM */
    { { itob(0), itob(51), itob(55), 0 }, 0x1EDC }, /*  392 AREA01/B201.GAM */
    { { itob(0), itob(51), itob(59), 0 }, 0xAC3F }, /*  393 AREA01/B202.GAM */
    { { itob(0), itob(55), itob(11), 0 }, 0x2A82 }, /*  394 AREA01/CLUT03.GAM */
    { { itob(0), itob(56), itob(74), 0 }, 0x1AB74 }, /*  395 AREA01/D203.GAM */
    { { itob(0), itob(52), itob(6), 0 }, 0x116C3 }, /*  396 AREA01/B300.GAM */
    { { itob(0), itob(52), itob(41), 0 }, 0x2C37 }, /*  397 AREA01/B301.GAM */
    { { itob(0), itob(52), itob(47), 0 }, 0x34B2 }, /*  398 AREA01/B302.GAM */
    { { itob(0), itob(52), itob(54), 0 }, 0x4F83 }, /*  399 AREA01/B303.GAM */
    { { itob(0), itob(52), itob(64), 0 }, 0x2D50 }, /*  400 AREA01/B304.GAM */
    { { itob(0), itob(55), itob(17), 0 }, 0x296F }, /*  401 AREA01/CLUT04.GAM */
    { { itob(0), itob(57), itob(53), 0 }, 0x22FB2 }, /*  402 AREA01/D305.GAM */
    { { itob(0), itob(52), itob(70), 0 }, 0x16157 }, /*  403 AREA01/B400.GAM */
    { { itob(0), itob(53), itob(40), 0 }, 0xAC3F }, /*  404 AREA01/B401.GAM */
    { { itob(0), itob(53), itob(62), 0 }, 0x9F1 }, /*  405 AREA01/B402.GAM */
    { { itob(0), itob(55), itob(23), 0 }, 0x2A4E }, /*  406 AREA01/CLUT05.GAM */
    { { itob(0), itob(58), itob(48), 0 }, 0x1877D }, /*  407 AREA01/D403.GAM */
    { { itob(1), itob(22), itob(74), 0 }, 0xB2 }, /*  408 AREA02/A000.GAM */
    { { itob(1), itob(23), itob(0), 0 }, 0x5CF0 }, /*  409 AREA02/A001.GAM */
    { { itob(1), itob(23), itob(12), 0 }, 0x1DFB }, /*  410 AREA02/A102.GAM */
    { { itob(1), itob(49), itob(28), 0 }, 0x10206 }, /*  411 AREA02/MFNT020B.WFM */
    { { itob(1), itob(33), itob(34), 0 }, 0x40EA }, /*  412 AREA02/CFNT999B.WFM */
    { { itob(1), itob(23), itob(48), 0 }, 0x1252F }, /*  413 AREA02/B000.GAM */
    { { itob(1), itob(24), itob(10), 0 }, 0x30000 }, /*  414 AREA02/B001.000 */
    { { itob(1), itob(25), itob(31), 0 }, 0x20000 }, /*  415 AREA02/B002.000 */
    { { itob(1), itob(26), itob(20), 0 }, 0x8000 }, /*  416 AREA02/B003.000 */
    { { itob(1), itob(34), itob(44), 0 }, 0x156E }, /*  417 AREA02/CLUT01.GAM */
    { { itob(1), itob(34), itob(64), 0 }, 0x4FC64 }, /*  418 AREA02/D004.000 */
    { { itob(1), itob(36), itob(74), 0 }, 0x3291 }, /*  419 AREA02/D005.GAM */
    { { itob(1), itob(43), itob(54), 0 }, 0xD4B4 }, /*  420 AREA02/I0NFO.BIN */
    { { itob(1), itob(26), itob(36), 0 }, 0x113D3 }, /*  421 AREA02/B100.GAM */
    { { itob(1), itob(26), itob(71), 0 }, 0x1984D }, /*  422 AREA02/B101.GAM */
    { { itob(1), itob(27), itob(48), 0 }, 0x5073 }, /*  423 AREA02/B102.GAM */
    { { itob(1), itob(27), itob(59), 0 }, 0x8000 }, /*  424 AREA02/B103.000 */
    { { itob(1), itob(28), itob(0), 0 }, 0x6210 }, /*  425 AREA02/B104.GAM */
    { { itob(1), itob(28), itob(13), 0 }, 0x6F32 }, /*  426 AREA02/B105.GAM */
    { { itob(1), itob(34), itob(47), 0 }, 0x19BA }, /*  427 AREA02/CLUT02.GAM */
    { { itob(1), itob(37), itob(6), 0 }, 0x2E008 }, /*  428 AREA02/D106.000 */
    { { itob(1), itob(38), itob(24), 0 }, 0x2717 }, /*  429 AREA02/D107.GAM */
    { { itob(1), itob(38), itob(29), 0 }, 0x15DA }, /*  430 AREA02/D108.GAM */
    { { itob(1), itob(53), itob(23), 0 }, 0x37CE0 }, /*  431 AREA02/X002.BIN */
    { { itob(1), itob(28), itob(27), 0 }, 0x16765 }, /*  432 AREA02/B200.GAM */
    { { itob(1), itob(28), itob(72), 0 }, 0x6210 }, /*  433 AREA02/B201.GAM */
    { { itob(1), itob(29), itob(10), 0 }, 0xAAD6 }, /*  434 AREA02/B202.GAM */
    { { itob(1), itob(34), itob(51), 0 }, 0x15B1 }, /*  435 AREA02/CLUT03.GAM */
    { { itob(1), itob(38), itob(32), 0 }, 0x2E008 }, /*  436 AREA02/D203.000 */
    { { itob(1), itob(39), itob(50), 0 }, 0x2717 }, /*  437 AREA02/D204.GAM */
    { { itob(1), itob(39), itob(55), 0 }, 0x5EE }, /*  438 AREA02/D205.GAM */
    { { itob(1), itob(29), itob(32), 0 }, 0x9604 }, /*  439 AREA02/B300.GAM */
    { { itob(1), itob(29), itob(51), 0 }, 0x20000 }, /*  440 AREA02/B301.000 */
    { { itob(1), itob(30), itob(40), 0 }, 0x34B0 }, /*  441 AREA02/B302.GAM */
    { { itob(1), itob(34), itob(54), 0 }, 0x16F8 }, /*  442 AREA02/CLUT04.GAM */
    { { itob(1), itob(39), itob(56), 0 }, 0x9D84 }, /*  443 AREA02/D303.000 */
    { { itob(1), itob(40), itob(1), 0 }, 0xCA3C }, /*  444 AREA02/D304.000 */
    { { itob(1), itob(40), itob(27), 0 }, 0xF6F }, /*  445 AREA02/D305.GAM */
    { { itob(1), itob(40), itob(29), 0 }, 0x175DE }, /*  446 AREA02/D306.GAM */
    { { itob(1), itob(44), itob(6), 0 }, 0x9848 }, /*  447 AREA02/I0NFO3.BIN */
    { { itob(1), itob(30), itob(47), 0 }, 0x16724 }, /*  448 AREA02/B400.GAM */
    { { itob(1), itob(31), itob(17), 0 }, 0x4C9A }, /*  449 AREA02/B401.GAM */
    { { itob(1), itob(31), itob(27), 0 }, 0x8000 }, /*  450 AREA02/B402.000 */
    { { itob(1), itob(31), itob(43), 0 }, 0x831C }, /*  451 AREA02/B403.GAM */
    { { itob(1), itob(34), itob(57), 0 }, 0x166B }, /*  452 AREA02/CLUT05.GAM */
    { { itob(1), itob(41), itob(1), 0 }, 0x2E008 }, /*  453 AREA02/D404.000 */
    { { itob(1), itob(42), itob(19), 0 }, 0x1E06 }, /*  454 AREA02/D405.GAM */
    { { itob(1), itob(42), itob(23), 0 }, 0x11C6 }, /*  455 AREA02/D406.GAM */
    { { itob(1), itob(31), itob(60), 0 }, 0x1D04A }, /*  456 AREA02/B500.GAM */
    { { itob(1), itob(32), itob(44), 0 }, 0x9F41 }, /*  457 AREA02/B501.GAM */
    { { itob(1), itob(32), itob(64), 0 }, 0x221B }, /*  458 AREA02/B502.GAM */
    { { itob(1), itob(32), itob(69), 0 }, 0x8000 }, /*  459 AREA02/B503.000 */
    { { itob(1), itob(33), itob(10), 0 }, 0xBEE7 }, /*  460 AREA02/B504.GAM */
    { { itob(1), itob(34), itob(60), 0 }, 0x1DF5 }, /*  461 AREA02/CLUT06.GAM */
    { { itob(1), itob(42), itob(26), 0 }, 0x2E008 }, /*  462 AREA02/D505.000 */
    { { itob(1), itob(43), itob(44), 0 }, 0x4881 }, /*  463 AREA02/D506.GAM */
    { { itob(2), itob(6), itob(60), 0 }, 0xB2 }, /*  464 AREA03/A000.GAM */
    { { itob(2), itob(6), itob(61), 0 }, 0x367C }, /*  465 AREA03/A001.GAM */
    { { itob(2), itob(6), itob(68), 0 }, 0xC566 }, /*  466 AREA03/A002.GAM */
    { { itob(2), itob(7), itob(18), 0 }, 0x2193 }, /*  467 AREA03/A003.GAM */
    { { itob(2), itob(7), itob(23), 0 }, 0x2E008 }, /*  468 AREA03/A004.000 */
    { { itob(2), itob(8), itob(41), 0 }, 0xA1FB }, /*  469 AREA03/A005.GAM */
    { { itob(2), itob(26), itob(18), 0 }, 0x52A48 }, /*  470 AREA03/X003.BIN */
    { { itob(2), itob(8), itob(62), 0 }, 0x1DFB }, /*  471 AREA03/A106.GAM */
    { { itob(2), itob(24), itob(32), 0 }, 0x72E2 }, /*  472 AREA03/MFNT030B.WFM */
    { { itob(2), itob(16), itob(65), 0 }, 0x40EA }, /*  473 AREA03/CFNT999B.WFM */
    { { itob(2), itob(9), itob(23), 0 }, 0x1D8CD }, /*  474 AREA03/B000.GAM */
    { { itob(2), itob(10), itob(8), 0 }, 0xE11D }, /*  475 AREA03/B001.GAM */
    { { itob(2), itob(10), itob(37), 0 }, 0x8522 }, /*  476 AREA03/B002.GAM */
    { { itob(2), itob(10), itob(54), 0 }, 0x4411 }, /*  477 AREA03/B003.GAM */
    { { itob(2), itob(18), itob(0), 0 }, 0x1F25 }, /*  478 AREA03/CLUT01.GAM */
    { { itob(2), itob(18), itob(24), 0 }, 0x32B64 }, /*  479 AREA03/D004.GAM */
    { { itob(2), itob(10), itob(63), 0 }, 0x6DE2 }, /*  480 AREA03/B100.GAM */
    { { itob(2), itob(11), itob(2), 0 }, 0x53E0 }, /*  481 AREA03/B101.GAM */
    { { itob(2), itob(11), itob(13), 0 }, 0xB7D8 }, /*  482 AREA03/B102.GAM */
    { { itob(2), itob(11), itob(36), 0 }, 0x13016 }, /*  483 AREA03/B103.GAM */
    { { itob(2), itob(12), itob(0), 0 }, 0x4411 }, /*  484 AREA03/B104.GAM */
    { { itob(2), itob(18), itob(4), 0 }, 0x1E6F }, /*  485 AREA03/CLUT02.GAM */
    { { itob(2), itob(19), itob(51), 0 }, 0x26C7C }, /*  486 AREA03/D105.GAM */
    { { itob(2), itob(12), itob(9), 0 }, 0xFCC8 }, /*  487 AREA03/B200.GAM */
    { { itob(2), itob(12), itob(41), 0 }, 0x6925 }, /*  488 AREA03/B201.GAM */
    { { itob(2), itob(12), itob(55), 0 }, 0xBF57 }, /*  489 AREA03/B202.GAM */
    { { itob(2), itob(13), itob(4), 0 }, 0x9F1 }, /*  490 AREA03/B203.GAM */
    { { itob(2), itob(13), itob(6), 0 }, 0x9C8E }, /*  491 AREA03/B204.GAM */
    { { itob(2), itob(13), itob(26), 0 }, 0x4053 }, /*  492 AREA03/B205.GAM */
    { { itob(2), itob(18), itob(8), 0 }, 0x1E31 }, /*  493 AREA03/CLUT03.GAM */
    { { itob(2), itob(20), itob(54), 0 }, 0x219B5 }, /*  494 AREA03/D206.GAM */
    { { itob(2), itob(13), itob(35), 0 }, 0x6392 }, /*  495 AREA03/B300.GAM */
    { { itob(2), itob(13), itob(48), 0 }, 0x34B0 }, /*  496 AREA03/B301.GAM */
    { { itob(2), itob(13), itob(55), 0 }, 0xC602 }, /*  497 AREA03/B302.GAM */
    { { itob(2), itob(14), itob(5), 0 }, 0x4411 }, /*  498 AREA03/B303.GAM */
    { { itob(2), itob(18), itob(12), 0 }, 0x1DE7 }, /*  499 AREA03/CLUT04.GAM */
    { { itob(2), itob(21), itob(47), 0 }, 0xE9AF }, /*  500 AREA03/D304.GAM */
    { { itob(2), itob(14), itob(14), 0 }, 0x18E04 }, /*  501 AREA03/B400.GAM */
    { { itob(2), itob(14), itob(64), 0 }, 0xE11D }, /*  502 AREA03/B401.GAM */
    { { itob(2), itob(15), itob(18), 0 }, 0x8522 }, /*  503 AREA03/B402.GAM */
    { { itob(2), itob(15), itob(35), 0 }, 0x40E6 }, /*  504 AREA03/B403.GAM */
    { { itob(2), itob(18), itob(16), 0 }, 0x1DEC }, /*  505 AREA03/CLUT05.GAM */
    { { itob(2), itob(22), itob(2), 0 }, 0x329C0 }, /*  506 AREA03/D404.GAM */
    { { itob(2), itob(15), itob(44), 0 }, 0x6DE2 }, /*  507 AREA03/B500.GAM */
    { { itob(2), itob(15), itob(58), 0 }, 0x53E0 }, /*  508 AREA03/B501.GAM */
    { { itob(2), itob(15), itob(69), 0 }, 0xB7D8 }, /*  509 AREA03/B502.GAM */
    { { itob(2), itob(16), itob(17), 0 }, 0x13016 }, /*  510 AREA03/B503.GAM */
    { { itob(2), itob(16), itob(56), 0 }, 0x40E6 }, /*  511 AREA03/B504.GAM */
    { { itob(2), itob(18), itob(20), 0 }, 0x1E7A }, /*  512 AREA03/CLUT06.GAM */
    { { itob(2), itob(23), itob(29), 0 }, 0x26C49 }, /*  513 AREA03/D505.GAM */
    { { itob(2), itob(46), itob(16), 0 }, 0xB2 }, /*  514 AREA04/A000.GAM */
    { { itob(2), itob(46), itob(17), 0 }, 0x367C }, /*  515 AREA04/A001.GAM */
    { { itob(2), itob(46), itob(24), 0 }, 0x1112D }, /*  516 AREA04/A002.GAM */
    { { itob(2), itob(46), itob(59), 0 }, 0x1017D }, /*  517 AREA04/A003.GAM */
    { { itob(2), itob(47), itob(17), 0 }, 0x34B0 }, /*  518 AREA04/A004.GAM */
    { { itob(2), itob(47), itob(24), 0 }, 0x2E008 }, /*  519 AREA04/A005.000 */
    { { itob(2), itob(48), itob(42), 0 }, 0x9552 }, /*  520 AREA04/A006.GAM */
    { { itob(2), itob(48), itob(61), 0 }, 0x1DFB }, /*  521 AREA04/A107.GAM */
    { { itob(3), itob(6), itob(60), 0 }, 0x7564 }, /*  522 AREA04/MFNT040B.WFM */
    { { itob(2), itob(58), itob(56), 0 }, 0x40EA }, /*  523 AREA04/CFNT999B.WFM */
    { { itob(3), itob(8), itob(48), 0 }, 0x4DAE4 }, /*  524 AREA04/X004.BIN */
    { { itob(2), itob(49), itob(22), 0 }, 0x12DA4 }, /*  525 AREA04/B000.GAM */
    { { itob(2), itob(59), itob(66), 0 }, 0x1C1E }, /*  526 AREA04/CLUT01.GAM */
    { { itob(3), itob(0), itob(71), 0 }, 0x1E4F0 }, /*  527 AREA04/D001.GAM */
    { { itob(3), itob(1), itob(57), 0 }, 0x9D72 }, /*  528 AREA04/D002.GAM */
    { { itob(2), itob(49), itob(60), 0 }, 0x12DA4 }, /*  529 AREA04/B100.GAM */
    { { itob(2), itob(59), itob(70), 0 }, 0x1C1E }, /*  530 AREA04/CLUT02.GAM */
    { { itob(3), itob(2), itob(2), 0 }, 0x1E4F0 }, /*  531 AREA04/D101.GAM */
    { { itob(3), itob(2), itob(63), 0 }, 0xA7BD }, /*  532 AREA04/D102.GAM */
    { { itob(2), itob(50), itob(23), 0 }, 0x12DA4 }, /*  533 AREA04/B200.GAM */
    { { itob(2), itob(59), itob(74), 0 }, 0x1C1E }, /*  534 AREA04/CLUT03.GAM */
    { { itob(3), itob(3), itob(9), 0 }, 0x1CE70 }, /*  535 AREA04/D201.GAM */
    { { itob(3), itob(3), itob(67), 0 }, 0xAE87 }, /*  536 AREA04/D202.GAM */
    { { itob(2), itob(50), itob(61), 0 }, 0x12DA4 }, /*  537 AREA04/B300.GAM */
    { { itob(3), itob(0), itob(3), 0 }, 0x1C1E }, /*  538 AREA04/CLUT04.GAM */
    { { itob(3), itob(4), itob(14), 0 }, 0x1CE70 }, /*  539 AREA04/D301.GAM */
    { { itob(3), itob(4), itob(72), 0 }, 0xA43C }, /*  540 AREA04/D302.GAM */
    { { itob(2), itob(51), itob(24), 0 }, 0x1BD9C }, /*  541 AREA04/B400.GAM */
    { { itob(3), itob(0), itob(7), 0 }, 0x1C24 }, /*  542 AREA04/CLUT05.GAM */
    { { itob(3), itob(5), itob(18), 0 }, 0x3C7A }, /*  543 AREA04/D401.GAM */
    { { itob(2), itob(52), itob(5), 0 }, 0x1BD9C }, /*  544 AREA04/B500.GAM */
    { { itob(3), itob(0), itob(11), 0 }, 0x1D17 }, /*  545 AREA04/CLUT06.GAM */
    { { itob(3), itob(5), itob(26), 0 }, 0x2D6C }, /*  546 AREA04/D501.GAM */
    { { itob(2), itob(52), itob(61), 0 }, 0x1BD9C }, /*  547 AREA04/B600.GAM */
    { { itob(3), itob(0), itob(15), 0 }, 0x1C24 }, /*  548 AREA04/CLUT07.GAM */
    { { itob(3), itob(5), itob(32), 0 }, 0x5240 }, /*  549 AREA04/D601.GAM */
    { { itob(2), itob(53), itob(42), 0 }, 0x11C69 }, /*  550 AREA04/B700.GAM */
    { { itob(3), itob(0), itob(19), 0 }, 0x1C51 }, /*  551 AREA04/CLUT08.GAM */
    { { itob(3), itob(5), itob(43), 0 }, 0x5127 }, /*  552 AREA04/D701.GAM */
    { { itob(2), itob(54), itob(3), 0 }, 0x18000 }, /*  553 AREA04/B800.000 */
    { { itob(3), itob(0), itob(23), 0 }, 0x1BB4 }, /*  554 AREA04/CLUT09.GAM */
    { { itob(3), itob(5), itob(54), 0 }, 0x391B }, /*  555 AREA04/D801.GAM */
    { { itob(2), itob(54), itob(51), 0 }, 0x2349 }, /*  556 AREA04/B900.GAM */
    { { itob(3), itob(0), itob(27), 0 }, 0x1C78 }, /*  557 AREA04/CLUT10.GAM */
    { { itob(3), itob(5), itob(62), 0 }, 0x3DCA }, /*  558 AREA04/D901.GAM */
    { { itob(3), itob(0), itob(31), 0 }, 0x1B92 }, /*  559 AREA04/CLUT11.GAM */
    { { itob(3), itob(5), itob(70), 0 }, 0x2603 }, /*  560 AREA04/DA00.GAM */
    { { itob(2), itob(54), itob(56), 0 }, 0x1BD9C }, /*  561 AREA04/BB00.GAM */
    { { itob(3), itob(0), itob(35), 0 }, 0x1C24 }, /*  562 AREA04/CLUT12.GAM */
    { { itob(3), itob(6), itob(0), 0 }, 0x4A4D }, /*  563 AREA04/DB01.GAM */
    { { itob(2), itob(55), itob(37), 0 }, 0x1BD9C }, /*  564 AREA04/BC00.GAM */
    { { itob(2), itob(56), itob(18), 0 }, 0x22A6 }, /*  565 AREA04/BC01.GAM */
    { { itob(2), itob(56), itob(23), 0 }, 0x1F7B }, /*  566 AREA04/BC02.GAM */
    { { itob(3), itob(0), itob(39), 0 }, 0x1C3D }, /*  567 AREA04/CLUT13.GAM */
    { { itob(3), itob(6), itob(10), 0 }, 0x4CB7 }, /*  568 AREA04/DC03.GAM */
    { { itob(3), itob(0), itob(43), 0 }, 0x1B92 }, /*  569 AREA04/CLUT14.GAM */
    { { itob(3), itob(6), itob(20), 0 }, 0x3232 }, /*  570 AREA04/DD00.GAM */
    { { itob(3), itob(0), itob(47), 0 }, 0x1B92 }, /*  571 AREA04/CLUT15.GAM */
    { { itob(3), itob(6), itob(27), 0 }, 0x1DD0 }, /*  572 AREA04/DE00.GAM */
    { { itob(2), itob(56), itob(27), 0 }, 0x52A9 }, /*  573 AREA04/BF00.GAM */
    { { itob(3), itob(0), itob(51), 0 }, 0x1C13 }, /*  574 AREA04/CLUT16.GAM */
    { { itob(3), itob(6), itob(31), 0 }, 0x1949 }, /*  575 AREA04/DF01.GAM */
    { { itob(2), itob(56), itob(38), 0 }, 0x1BD9C }, /*  576 AREA04/BG00.GAM */
    { { itob(3), itob(0), itob(55), 0 }, 0x1C24 }, /*  577 AREA04/CLUT17.GAM */
    { { itob(3), itob(6), itob(35), 0 }, 0x3C13 }, /*  578 AREA04/DG01.GAM */
    { { itob(2), itob(57), itob(19), 0 }, 0x1BD9C }, /*  579 AREA04/BH00.GAM */
    { { itob(3), itob(0), itob(59), 0 }, 0x1C24 }, /*  580 AREA04/CLUT18.GAM */
    { { itob(3), itob(6), itob(43), 0 }, 0x2D29 }, /*  581 AREA04/DH01.GAM */
    { { itob(2), itob(58), itob(0), 0 }, 0x1BD9C }, /*  582 AREA04/BI00.GAM */
    { { itob(3), itob(0), itob(63), 0 }, 0x1C24 }, /*  583 AREA04/CLUT19.GAM */
    { { itob(3), itob(6), itob(49), 0 }, 0x2D3D }, /*  584 AREA04/DI01.GAM */
    { { itob(3), itob(0), itob(67), 0 }, 0x1B92 }, /*  585 AREA04/CLUT20.GAM */
    { { itob(3), itob(6), itob(55), 0 }, 0x21AC }, /*  586 AREA04/DJ00.GAM */
    { { itob(3), itob(27), itob(30), 0 }, 0xB2 }, /*  587 AREA05/A000.GAM */
    { { itob(3), itob(27), itob(31), 0 }, 0x367C }, /*  588 AREA05/A001.GAM */
    { { itob(3), itob(27), itob(38), 0 }, 0x5C18 }, /*  589 AREA05/A002.GAM */
    { { itob(3), itob(27), itob(50), 0 }, 0x1DFB }, /*  590 AREA05/A103.GAM */
    { { itob(3), itob(54), itob(18), 0 }, 0x95E2 }, /*  591 AREA05/MFNT050B.WFM */
    { { itob(3), itob(34), itob(9), 0 }, 0x40EA }, /*  592 AREA05/CFNT999B.WFM */
    { { itob(3), itob(28), itob(11), 0 }, 0x3611B }, /*  593 AREA05/B000.GAM */
    { { itob(3), itob(29), itob(45), 0 }, 0x4A44 }, /*  594 AREA05/B001.GAM */
    { { itob(3), itob(29), itob(55), 0 }, 0x8000 }, /*  595 AREA05/B002.000 */
    { { itob(3), itob(35), itob(19), 0 }, 0x18C0 }, /*  596 AREA05/CLUT01.GAM */
    { { itob(3), itob(35), itob(35), 0 }, 0x4008 }, /*  597 AREA05/D003.000 */
    { { itob(3), itob(35), itob(44), 0 }, 0x80620 }, /*  598 AREA05/D004.000 */
    { { itob(3), itob(39), itob(1), 0 }, 0xCA3C }, /*  599 AREA05/D005.000 */
    { { itob(3), itob(39), itob(27), 0 }, 0x1D12 }, /*  600 AREA05/D006.GAM */
    { { itob(3), itob(46), itob(6), 0 }, 0x218DC }, /*  601 AREA05/I0NFO2.BIN */
    { { itob(3), itob(29), itob(71), 0 }, 0x5FB2 }, /*  602 AREA05/B100.GAM */
    { { itob(3), itob(30), itob(8), 0 }, 0x62D9 }, /*  603 AREA05/B101.GAM */
    { { itob(3), itob(30), itob(21), 0 }, 0x1C294 }, /*  604 AREA05/B102.GAM */
    { { itob(3), itob(31), itob(3), 0 }, 0x33A8 }, /*  605 AREA05/B103.GAM */
    { { itob(3), itob(35), itob(23), 0 }, 0x1852 }, /*  606 AREA05/CLUT02.GAM */
    { { itob(3), itob(39), itob(31), 0 }, 0x2E008 }, /*  607 AREA05/D104.000 */
    { { itob(3), itob(40), itob(49), 0 }, 0x454C }, /*  608 AREA05/D105.GAM */
    { { itob(3), itob(56), itob(43), 0 }, 0x32718 }, /*  609 AREA05/X005.BIN */
    { { itob(3), itob(31), itob(10), 0 }, 0x3611B }, /*  610 AREA05/B200.GAM */
    { { itob(3), itob(32), itob(44), 0 }, 0x4A44 }, /*  611 AREA05/B201.GAM */
    { { itob(3), itob(32), itob(54), 0 }, 0x8000 }, /*  612 AREA05/B202.000 */
    { { itob(3), itob(35), itob(27), 0 }, 0x18C0 }, /*  613 AREA05/CLUT03.GAM */
    { { itob(3), itob(40), itob(58), 0 }, 0x4008 }, /*  614 AREA05/D203.000 */
    { { itob(3), itob(40), itob(67), 0 }, 0x80620 }, /*  615 AREA05/D204.000 */
    { { itob(3), itob(44), itob(24), 0 }, 0xCA3C }, /*  616 AREA05/D205.000 */
    { { itob(3), itob(44), itob(50), 0 }, 0x1D12 }, /*  617 AREA05/D206.GAM */
    { { itob(3), itob(32), itob(70), 0 }, 0x5FB2 }, /*  618 AREA05/B300.GAM */
    { { itob(3), itob(33), itob(7), 0 }, 0x62D9 }, /*  619 AREA05/B301.GAM */
    { { itob(3), itob(33), itob(20), 0 }, 0x1C294 }, /*  620 AREA05/B302.GAM */
    { { itob(3), itob(34), itob(2), 0 }, 0x33A8 }, /*  621 AREA05/B303.GAM */
    { { itob(3), itob(35), itob(31), 0 }, 0x1852 }, /*  622 AREA05/CLUT04.GAM */
    { { itob(3), itob(44), itob(54), 0 }, 0x2E008 }, /*  623 AREA05/D304.000 */
    { { itob(3), itob(45), itob(72), 0 }, 0x454C }, /*  624 AREA05/D305.GAM */
    { { itob(4), itob(8), itob(60), 0 }, 0xB2 }, /*  625 AREA06/A000.GAM */
    { { itob(4), itob(8), itob(61), 0 }, 0x5631 }, /*  626 AREA06/A001.GAM */
    { { itob(4), itob(8), itob(72), 0 }, 0x11373 }, /*  627 AREA06/A002.GAM */
    { { itob(4), itob(9), itob(32), 0 }, 0x1DFB }, /*  628 AREA06/A103.GAM */
    { { itob(4), itob(23), itob(40), 0 }, 0x446A }, /*  629 AREA06/MFNT060B.WFM */
    { { itob(4), itob(19), itob(63), 0 }, 0x40EA }, /*  630 AREA06/CFNT999B.WFM */
    { { itob(4), itob(24), itob(51), 0 }, 0x3B114 }, /*  631 AREA06/X006.BIN */
    { { itob(4), itob(9), itob(68), 0 }, 0xAF28 }, /*  632 AREA06/B000.GAM */
    { { itob(4), itob(10), itob(15), 0 }, 0x1957 }, /*  633 AREA06/B001.GAM */
    { { itob(4), itob(10), itob(19), 0 }, 0x9985 }, /*  634 AREA06/B002.GAM */
    { { itob(4), itob(10), itob(39), 0 }, 0x9395 }, /*  635 AREA06/B003.GAM */
    { { itob(4), itob(20), itob(73), 0 }, 0x196F }, /*  636 AREA06/CLUT01.GAM */
    { { itob(4), itob(21), itob(8), 0 }, 0x4FDDC }, /*  637 AREA06/D004.000 */
    { { itob(4), itob(23), itob(18), 0 }, 0xFCB }, /*  638 AREA06/D005.GAM */
    { { itob(4), itob(10), itob(58), 0 }, 0x38000 }, /*  639 AREA06/B100.000 */
    { { itob(4), itob(12), itob(20), 0 }, 0x30000 }, /*  640 AREA06/B101.000 */
    { { itob(4), itob(13), itob(41), 0 }, 0x34B0 }, /*  641 AREA06/B102.GAM */
    { { itob(4), itob(13), itob(48), 0 }, 0x10000 }, /*  642 AREA06/B103.000 */
    { { itob(4), itob(21), itob(2), 0 }, 0x1706 }, /*  643 AREA06/CLUT02.GAM */
    { { itob(4), itob(14), itob(5), 0 }, 0x2E008 }, /*  644 AREA06/B104.000 */
    { { itob(4), itob(23), itob(20), 0 }, 0x4954 }, /*  645 AREA06/D105.GAM */
    { { itob(4), itob(15), itob(23), 0 }, 0x38000 }, /*  646 AREA06/B200.000 */
    { { itob(4), itob(16), itob(60), 0 }, 0x30000 }, /*  647 AREA06/B201.000 */
    { { itob(4), itob(18), itob(6), 0 }, 0x34B0 }, /*  648 AREA06/B202.GAM */
    { { itob(4), itob(18), itob(13), 0 }, 0x10000 }, /*  649 AREA06/B203.000 */
    { { itob(4), itob(21), itob(5), 0 }, 0x1706 }, /*  650 AREA06/CLUT03.GAM */
    { { itob(4), itob(18), itob(45), 0 }, 0x2E008 }, /*  651 AREA06/B204.000 */
    { { itob(4), itob(23), itob(30), 0 }, 0x4934 }, /*  652 AREA06/D205.GAM */
    { { itob(4), itob(39), itob(0), 0 }, 0xB2 }, /*  653 AREA07/A000.GAM */
    { { itob(4), itob(39), itob(1), 0 }, 0x367C }, /*  654 AREA07/A001.GAM */
    { { itob(4), itob(39), itob(8), 0 }, 0x1512D }, /*  655 AREA07/A002.GAM */
    { { itob(4), itob(39), itob(51), 0 }, 0xEA73 }, /*  656 AREA07/A003.GAM */
    { { itob(4), itob(40), itob(6), 0 }, 0xB396 }, /*  657 AREA07/A004.GAM */
    { { itob(4), itob(40), itob(29), 0 }, 0x2E008 }, /*  658 AREA07/A005.000 */
    { { itob(4), itob(41), itob(47), 0 }, 0xB169 }, /*  659 AREA07/A006.GAM */
    { { itob(4), itob(41), itob(70), 0 }, 0x1DFB }, /*  660 AREA07/A107.GAM */
    { { itob(4), itob(52), itob(66), 0 }, 0x9F7A }, /*  661 AREA07/MFNT010B.WFM */
    { { itob(4), itob(47), itob(30), 0 }, 0x40EA }, /*  662 AREA07/CFNT999B.WFM */
    { { itob(4), itob(55), itob(28), 0 }, 0x57F7C }, /*  663 AREA07/X001.BIN */
    { { itob(4), itob(42), itob(31), 0 }, 0x17AB7 }, /*  664 AREA07/B000.GAM */
    { { itob(4), itob(43), itob(4), 0 }, 0x2C37 }, /*  665 AREA07/B001.GAM */
    { { itob(4), itob(43), itob(10), 0 }, 0xC18E }, /*  666 AREA07/B002.GAM */
    { { itob(4), itob(43), itob(35), 0 }, 0x2D50 }, /*  667 AREA07/B003.GAM */
    { { itob(4), itob(48), itob(40), 0 }, 0x2986 }, /*  668 AREA07/CLUT01.GAM */
    { { itob(4), itob(48), itob(70), 0 }, 0x1E196 }, /*  669 AREA07/D004.GAM */
    { { itob(4), itob(43), itob(41), 0 }, 0x17AB7 }, /*  670 AREA07/B100.GAM */
    { { itob(4), itob(44), itob(14), 0 }, 0x2C37 }, /*  671 AREA07/B101.GAM */
    { { itob(4), itob(44), itob(20), 0 }, 0xC18E }, /*  672 AREA07/B102.GAM */
    { { itob(4), itob(44), itob(45), 0 }, 0x2D50 }, /*  673 AREA07/B103.GAM */
    { { itob(4), itob(48), itob(46), 0 }, 0x2986 }, /*  674 AREA07/CLUT02.GAM */
    { { itob(4), itob(49), itob(56), 0 }, 0x1E196 }, /*  675 AREA07/D104.GAM */
    { { itob(4), itob(44), itob(51), 0 }, 0x166D4 }, /*  676 AREA07/B200.GAM */
    { { itob(4), itob(45), itob(21), 0 }, 0x1EDC }, /*  677 AREA07/B201.GAM */
    { { itob(4), itob(45), itob(25), 0 }, 0xACA6 }, /*  678 AREA07/B202.GAM */
    { { itob(4), itob(48), itob(52), 0 }, 0x29FF }, /*  679 AREA07/CLUT03.GAM */
    { { itob(4), itob(50), itob(42), 0 }, 0x1AB83 }, /*  680 AREA07/D203.GAM */
    { { itob(4), itob(45), itob(47), 0 }, 0x116C3 }, /*  681 AREA07/B300.GAM */
    { { itob(4), itob(46), itob(7), 0 }, 0x2C37 }, /*  682 AREA07/B301.GAM */
    { { itob(4), itob(46), itob(13), 0 }, 0x328F }, /*  683 AREA07/B302.GAM */
    { { itob(4), itob(46), itob(20), 0 }, 0x4F83 }, /*  684 AREA07/B303.GAM */
    { { itob(4), itob(46), itob(30), 0 }, 0x2D50 }, /*  685 AREA07/B304.GAM */
    { { itob(4), itob(48), itob(58), 0 }, 0x28E3 }, /*  686 AREA07/CLUT04.GAM */
    { { itob(4), itob(51), itob(21), 0 }, 0x23083 }, /*  687 AREA07/D305.GAM */
    { { itob(4), itob(46), itob(36), 0 }, 0x16157 }, /*  688 AREA07/B400.GAM */
    { { itob(4), itob(47), itob(6), 0 }, 0xACA6 }, /*  689 AREA07/B401.GAM */
    { { itob(4), itob(47), itob(28), 0 }, 0x9F1 }, /*  690 AREA07/B402.GAM */
    { { itob(4), itob(48), itob(64), 0 }, 0x29CA }, /*  691 AREA07/CLUT05.GAM */
    { { itob(4), itob(52), itob(17), 0 }, 0x18756 }, /*  692 AREA07/D403.GAM */
    { { itob(5), itob(16), itob(42), 0 }, 0xB2 }, /*  693 AREA08/A000.GAM */
    { { itob(5), itob(16), itob(43), 0 }, 0x367C }, /*  694 AREA08/A001.GAM */
    { { itob(5), itob(16), itob(50), 0 }, 0x5C18 }, /*  695 AREA08/A002.GAM */
    { { itob(5), itob(16), itob(62), 0 }, 0x1DFB }, /*  696 AREA08/A103.GAM */
    { { itob(5), itob(36), itob(9), 0 }, 0x3AAA }, /*  697 AREA08/MFNT080B.WFM */
    { { itob(5), itob(19), itob(55), 0 }, 0x40EA }, /*  698 AREA08/CFNT999B.WFM */
    { { itob(5), itob(17), itob(23), 0 }, 0x9A08 }, /*  699 AREA08/B000.GAM */
    { { itob(5), itob(17), itob(43), 0 }, 0x23C6 }, /*  700 AREA08/B001.GAM */
    { { itob(5), itob(20), itob(65), 0 }, 0x161B }, /*  701 AREA08/CLUT01.GAM */
    { { itob(5), itob(21), itob(2), 0 }, 0x4C464 }, /*  702 AREA08/D002.000 */
    { { itob(5), itob(23), itob(5), 0 }, 0x3EE4 }, /*  703 AREA08/D003.GAM */
    { { itob(5), itob(27), itob(72), 0 }, 0x218DC }, /*  704 AREA08/I0NFO2.BIN */
    { { itob(5), itob(17), itob(48), 0 }, 0x163D0 }, /*  705 AREA08/B100.GAM */
    { { itob(5), itob(18), itob(18), 0 }, 0x36EA }, /*  706 AREA08/B101.GAM */
    { { itob(5), itob(18), itob(25), 0 }, 0x6925 }, /*  707 AREA08/B102.GAM */
    { { itob(5), itob(20), itob(68), 0 }, 0x15ED }, /*  708 AREA08/CLUT02.GAM */
    { { itob(5), itob(23), itob(13), 0 }, 0x2E008 }, /*  709 AREA08/D103.000 */
    { { itob(5), itob(24), itob(31), 0 }, 0x2B9C }, /*  710 AREA08/D104.GAM */
    { { itob(5), itob(37), itob(6), 0 }, 0x3058C }, /*  711 AREA08/X008.BIN */
    { { itob(5), itob(18), itob(39), 0 }, 0x9A08 }, /*  712 AREA08/B200.GAM */
    { { itob(5), itob(18), itob(59), 0 }, 0x23C6 }, /*  713 AREA08/B201.GAM */
    { { itob(5), itob(20), itob(71), 0 }, 0x161B }, /*  714 AREA08/CLUT03.GAM */
    { { itob(5), itob(24), itob(37), 0 }, 0x4C464 }, /*  715 AREA08/D202.000 */
    { { itob(5), itob(26), itob(40), 0 }, 0x3EE4 }, /*  716 AREA08/D203.GAM */
    { { itob(5), itob(18), itob(64), 0 }, 0x163D0 }, /*  717 AREA08/B300.GAM */
    { { itob(5), itob(19), itob(34), 0 }, 0x36EA }, /*  718 AREA08/B301.GAM */
    { { itob(5), itob(19), itob(41), 0 }, 0x6925 }, /*  719 AREA08/B302.GAM */
    { { itob(5), itob(20), itob(74), 0 }, 0x15ED }, /*  720 AREA08/CLUT04.GAM */
    { { itob(5), itob(26), itob(48), 0 }, 0x2E008 }, /*  721 AREA08/D303.000 */
    { { itob(5), itob(27), itob(66), 0 }, 0x2B9C }, /*  722 AREA08/D304.GAM */
    { { itob(5), itob(48), itob(60), 0 }, 0xB2 }, /*  723 AREA09/A000.GAM */
    { { itob(5), itob(48), itob(61), 0 }, 0x367C }, /*  724 AREA09/A001.GAM */
    { { itob(5), itob(48), itob(68), 0 }, 0x2E008 }, /*  725 AREA09/A002.000 */
    { { itob(5), itob(50), itob(11), 0 }, 0x8327 }, /*  726 AREA09/A003.GAM */
    { { itob(5), itob(50), itob(28), 0 }, 0x1DFB }, /*  727 AREA09/A104.GAM */
    { { itob(6), itob(3), itob(7), 0 }, 0x4790 }, /*  728 AREA09/MFNT090B.WFM */
    { { itob(5), itob(59), itob(49), 0 }, 0x40EA }, /*  729 AREA09/CFNT999B.WFM */
    { { itob(6), itob(4), itob(19), 0 }, 0x491EC }, /*  730 AREA09/X009.BIN */
    { { itob(5), itob(50), itob(64), 0 }, 0x1A823 }, /*  731 AREA09/B000.GAM */
    { { itob(5), itob(51), itob(43), 0 }, 0x17096 }, /*  732 AREA09/B001.GAM */
    { { itob(5), itob(52), itob(15), 0 }, 0x1568B }, /*  733 AREA09/B002.GAM */
    { { itob(5), itob(52), itob(58), 0 }, 0x13D4B }, /*  734 AREA09/B003.GAM */
    { { itob(6), itob(0), itob(59), 0 }, 0x29E9 }, /*  735 AREA09/CLUT01.GAM */
    { { itob(6), itob(1), itob(9), 0 }, 0x346C0 }, /*  736 AREA09/D004.GAM */
    { { itob(5), itob(53), itob(23), 0 }, 0x2D6B0 }, /*  737 AREA09/B100.GAM */
    { { itob(5), itob(54), itob(39), 0 }, 0x12A1A }, /*  738 AREA09/B101.GAM */
    { { itob(5), itob(55), itob(2), 0 }, 0x14234 }, /*  739 AREA09/B102.GAM */
    { { itob(5), itob(55), itob(43), 0 }, 0x154C }, /*  740 AREA09/B103.GAM */
    { { itob(5), itob(55), itob(46), 0 }, 0xB314 }, /*  741 AREA09/B104.GAM */
    { { itob(5), itob(55), itob(69), 0 }, 0x34B0 }, /*  742 AREA09/B105.GAM */
    { { itob(6), itob(0), itob(65), 0 }, 0x1877 }, /*  743 AREA09/CLUT02.GAM */
    { { itob(6), itob(2), itob(39), 0 }, 0xEA7 }, /*  744 AREA09/D106.GAM */
    { { itob(5), itob(56), itob(1), 0 }, 0x25501 }, /*  745 AREA09/B200.GAM */
    { { itob(5), itob(57), itob(1), 0 }, 0x2290 }, /*  746 AREA09/B201.GAM */
    { { itob(5), itob(57), itob(6), 0 }, 0x42E6 }, /*  747 AREA09/B202.GAM */
    { { itob(6), itob(0), itob(69), 0 }, 0x1525 }, /*  748 AREA09/CLUT03.GAM */
    { { itob(6), itob(2), itob(41), 0 }, 0xF2C }, /*  749 AREA09/D203.GAM */
    { { itob(5), itob(57), itob(15), 0 }, 0x16386 }, /*  750 AREA09/B300.GAM */
    { { itob(5), itob(57), itob(60), 0 }, 0x5FB2 }, /*  751 AREA09/B301.GAM */
    { { itob(6), itob(0), itob(72), 0 }, 0x15B7 }, /*  752 AREA09/CLUT04.GAM */
    { { itob(6), itob(2), itob(43), 0 }, 0x19E4 }, /*  753 AREA09/D302.GAM */
    { { itob(5), itob(57), itob(72), 0 }, 0x1634D }, /*  754 AREA09/B400.GAM */
    { { itob(5), itob(58), itob(42), 0 }, 0x34B0 }, /*  755 AREA09/B401.GAM */
    { { itob(6), itob(1), itob(0), 0 }, 0x14C1 }, /*  756 AREA09/CLUT05.GAM */
    { { itob(6), itob(2), itob(47), 0 }, 0x995 }, /*  757 AREA09/D402.GAM */
    { { itob(5), itob(58), itob(49), 0 }, 0x164DA }, /*  758 AREA09/B500.GAM */
    { { itob(5), itob(59), itob(19), 0 }, 0x34B0 }, /*  759 AREA09/B501.GAM */
    { { itob(6), itob(1), itob(3), 0 }, 0x14C1 }, /*  760 AREA09/CLUT06.GAM */
    { { itob(6), itob(2), itob(49), 0 }, 0x1027 }, /*  761 AREA09/D502.GAM */
    { { itob(5), itob(59), itob(26), 0 }, 0x5DDC }, /*  762 AREA09/B600.GAM */
    { { itob(5), itob(59), itob(38), 0 }, 0x35B5 }, /*  763 AREA09/B601.GAM */
    { { itob(5), itob(59), itob(45), 0 }, 0x1BBF }, /*  764 AREA09/B602.GAM */
    { { itob(6), itob(1), itob(6), 0 }, 0x12CA }, /*  765 AREA09/CLUT07.GAM */
    { { itob(6), itob(2), itob(52), 0 }, 0xED41 }, /*  766 AREA09/D603.GAM */
    { { itob(6), itob(21), itob(70), 0 }, 0xB2 }, /*  767 AREA10/A000.GAM */
    { { itob(6), itob(21), itob(71), 0 }, 0x367C }, /*  768 AREA10/A001.GAM */
    { { itob(6), itob(22), itob(3), 0 }, 0x2E008 }, /*  769 AREA10/A002.000 */
    { { itob(6), itob(23), itob(21), 0 }, 0xA586 }, /*  770 AREA10/A003.GAM */
    { { itob(6), itob(23), itob(42), 0 }, 0x1DFB }, /*  771 AREA10/A104.GAM */
    { { itob(6), itob(42), itob(19), 0 }, 0x994C }, /*  772 AREA10/MFNT100B.WFM */
    { { itob(6), itob(36), itob(11), 0 }, 0x40EA }, /*  773 AREA10/CFNT999B.WFM */
    { { itob(6), itob(44), itob(47), 0 }, 0x4AE54 }, /*  774 AREA10/X010.BIN */
    { { itob(6), itob(24), itob(3), 0 }, 0x7E21 }, /*  775 AREA10/B000.GAM */
    { { itob(6), itob(24), itob(19), 0 }, 0xE13 }, /*  776 AREA10/B001.GAM */
    { { itob(6), itob(24), itob(21), 0 }, 0x12CC1 }, /*  777 AREA10/B002.GAM */
    { { itob(6), itob(24), itob(59), 0 }, 0x1632F }, /*  778 AREA10/B003.GAM */
    { { itob(6), itob(25), itob(29), 0 }, 0x10481 }, /*  779 AREA10/B004.GAM */
    { { itob(6), itob(37), itob(21), 0 }, 0x234E }, /*  780 AREA10/CLUT01.GAM */
    { { itob(6), itob(37), itob(62), 0 }, 0x1988B }, /*  781 AREA10/D005.GAM */
    { { itob(6), itob(25), itob(62), 0 }, 0x34B0 }, /*  782 AREA10/B100.GAM */
    { { itob(6), itob(25), itob(69), 0 }, 0xE13 }, /*  783 AREA10/B101.GAM */
    { { itob(6), itob(25), itob(71), 0 }, 0xD927 }, /*  784 AREA10/B102.GAM */
    { { itob(6), itob(26), itob(24), 0 }, 0x5957 }, /*  785 AREA10/B103.GAM */
    { { itob(6), itob(26), itob(36), 0 }, 0x10481 }, /*  786 AREA10/B104.GAM */
    { { itob(6), itob(37), itob(26), 0 }, 0x1D68 }, /*  787 AREA10/CLUT02.GAM */
    { { itob(6), itob(38), itob(39), 0 }, 0x6D7A }, /*  788 AREA10/D105.GAM */
    { { itob(6), itob(26), itob(69), 0 }, 0xD97F }, /*  789 AREA10/B200.GAM */
    { { itob(6), itob(27), itob(22), 0 }, 0x7B16 }, /*  790 AREA10/B201.GAM */
    { { itob(6), itob(27), itob(38), 0 }, 0x15601 }, /*  791 AREA10/B202.GAM */
    { { itob(6), itob(28), itob(6), 0 }, 0x10481 }, /*  792 AREA10/B203.GAM */
    { { itob(6), itob(37), itob(30), 0 }, 0x212C }, /*  793 AREA10/CLUT03.GAM */
    { { itob(6), itob(38), itob(53), 0 }, 0xFA06 }, /*  794 AREA10/D204.GAM */
    { { itob(6), itob(28), itob(39), 0 }, 0xF3EA }, /*  795 AREA10/B300.GAM */
    { { itob(6), itob(28), itob(70), 0 }, 0xD08C }, /*  796 AREA10/B301.GAM */
    { { itob(6), itob(29), itob(22), 0 }, 0x10481 }, /*  797 AREA10/B302.GAM */
    { { itob(6), itob(37), itob(35), 0 }, 0x2262 }, /*  798 AREA10/CLUT04.GAM */
    { { itob(6), itob(39), itob(10), 0 }, 0x234DF }, /*  799 AREA10/D303.GAM */
    { { itob(6), itob(29), itob(55), 0 }, 0x7E21 }, /*  800 AREA10/B400.GAM */
    { { itob(6), itob(29), itob(71), 0 }, 0xE13 }, /*  801 AREA10/B401.GAM */
    { { itob(6), itob(29), itob(73), 0 }, 0xAEBD }, /*  802 AREA10/B402.GAM */
    { { itob(6), itob(30), itob(20), 0 }, 0x1E07A }, /*  803 AREA10/B403.GAM */
    { { itob(6), itob(31), itob(6), 0 }, 0x10495 }, /*  804 AREA10/B404.GAM */
    { { itob(6), itob(37), itob(40), 0 }, 0x22AF }, /*  805 AREA10/CLUT05.GAM */
    { { itob(6), itob(40), itob(6), 0 }, 0x1900B }, /*  806 AREA10/D405.GAM */
    { { itob(6), itob(31), itob(39), 0 }, 0x34B0 }, /*  807 AREA10/B500.GAM */
    { { itob(6), itob(31), itob(46), 0 }, 0xE13 }, /*  808 AREA10/B501.GAM */
    { { itob(6), itob(31), itob(48), 0 }, 0xD929 }, /*  809 AREA10/B502.GAM */
    { { itob(6), itob(32), itob(1), 0 }, 0x5A0A }, /*  810 AREA10/B503.GAM */
    { { itob(6), itob(32), itob(13), 0 }, 0x10495 }, /*  811 AREA10/B504.GAM */
    { { itob(6), itob(37), itob(45), 0 }, 0x1D58 }, /*  812 AREA10/CLUT06.GAM */
    { { itob(6), itob(40), itob(57), 0 }, 0x6CB6 }, /*  813 AREA10/D505.GAM */
    { { itob(6), itob(32), itob(46), 0 }, 0xD97F }, /*  814 AREA10/B600.GAM */
    { { itob(6), itob(32), itob(74), 0 }, 0x7B16 }, /*  815 AREA10/B601.GAM */
    { { itob(6), itob(33), itob(15), 0 }, 0x1563F }, /*  816 AREA10/B602.GAM */
    { { itob(6), itob(33), itob(58), 0 }, 0x10495 }, /*  817 AREA10/B603.GAM */
    { { itob(6), itob(37), itob(49), 0 }, 0x2121 }, /*  818 AREA10/CLUT07.GAM */
    { { itob(6), itob(40), itob(71), 0 }, 0xFA06 }, /*  819 AREA10/D604.GAM */
    { { itob(6), itob(34), itob(16), 0 }, 0xF3EA }, /*  820 AREA10/B700.GAM */
    { { itob(6), itob(34), itob(47), 0 }, 0xD08C }, /*  821 AREA10/B701.GAM */
    { { itob(6), itob(34), itob(74), 0 }, 0x10495 }, /*  822 AREA10/B702.GAM */
    { { itob(6), itob(37), itob(54), 0 }, 0x226F }, /*  823 AREA10/CLUT08.GAM */
    { { itob(6), itob(41), itob(28), 0 }, 0x1FF3B }, /*  824 AREA10/D703.GAM */
    { { itob(6), itob(35), itob(32), 0 }, 0x3723 }, /*  825 AREA10/B800.GAM */
    { { itob(6), itob(35), itob(39), 0 }, 0x13B54 }, /*  826 AREA10/B801.GAM */
    { { itob(6), itob(36), itob(4), 0 }, 0x34B0 }, /*  827 AREA10/B802.GAM */
    { { itob(6), itob(37), itob(59), 0 }, 0x15E7 }, /*  828 AREA10/CLUT09.GAM */
    { { itob(6), itob(42), itob(17), 0 }, 0xC9B }, /*  829 AREA10/D803.GAM */
    { { itob(7), itob(2), itob(53), 0 }, 0xB2 }, /*  830 AREA11/A000.GAM */
    { { itob(7), itob(2), itob(54), 0 }, 0x367C }, /*  831 AREA11/A001.GAM */
    { { itob(7), itob(2), itob(61), 0 }, 0x652B }, /*  832 AREA11/A002.GAM */
    { { itob(7), itob(2), itob(74), 0 }, 0x1DFB }, /*  833 AREA11/A103.GAM */
    { { itob(7), itob(18), itob(3), 0 }, 0x636E }, /*  834 AREA11/MFNT110B.WFM */
    { { itob(7), itob(5), itob(46), 0 }, 0x40EA }, /*  835 AREA11/CFNT999B.WFM */
    { { itob(7), itob(3), itob(35), 0 }, 0xC019 }, /*  836 AREA11/B100.GAM */
    { { itob(7), itob(3), itob(60), 0 }, 0x9C62 }, /*  837 AREA11/B101.GAM */
    { { itob(7), itob(4), itob(5), 0 }, 0x20000 }, /*  838 AREA11/B102.000 */
    { { itob(7), itob(6), itob(59), 0 }, 0x1CD0 }, /*  839 AREA11/CLUT02.GAM */
    { { itob(7), itob(6), itob(72), 0 }, 0x2E008 }, /*  840 AREA11/D103.000 */
    { { itob(7), itob(8), itob(15), 0 }, 0xD4B6 }, /*  841 AREA11/D104.GAM */
    { { itob(7), itob(19), itob(47), 0 }, 0x34704 }, /*  842 AREA11/X011.BIN */
    { { itob(7), itob(4), itob(69), 0 }, 0x161FA }, /*  843 AREA11/B200.GAM */
    { { itob(7), itob(5), itob(39), 0 }, 0x34B0 }, /*  844 AREA11/B201.GAM */
    { { itob(7), itob(6), itob(63), 0 }, 0x158F }, /*  845 AREA11/CLUT03.GAM */
    { { itob(7), itob(8), itob(42), 0 }, 0x2E008 }, /*  846 AREA11/D202.000 */
    { { itob(7), itob(9), itob(60), 0 }, 0x1E06 }, /*  847 AREA11/D203.GAM */
    { { itob(7), itob(9), itob(64), 0 }, 0xD9D }, /*  848 AREA11/D204.GAM */
    { { itob(7), itob(32), itob(27), 0 }, 0xB2 }, /*  849 AREA12/A000.GAM */
    { { itob(7), itob(32), itob(28), 0 }, 0x367C }, /*  850 AREA12/A001.GAM */
    { { itob(7), itob(32), itob(35), 0 }, 0x10A88 }, /*  851 AREA12/A002.GAM */
    { { itob(7), itob(32), itob(69), 0 }, 0xFFE7 }, /*  852 AREA12/A003.GAM */
    { { itob(7), itob(33), itob(26), 0 }, 0x34B0 }, /*  853 AREA12/A004.GAM */
    { { itob(7), itob(33), itob(33), 0 }, 0x2E008 }, /*  854 AREA12/A005.000 */
    { { itob(7), itob(34), itob(51), 0 }, 0x9573 }, /*  855 AREA12/A006.GAM */
    { { itob(7), itob(34), itob(70), 0 }, 0x1DFB }, /*  856 AREA12/A107.GAM */
    { { itob(7), itob(49), itob(70), 0 }, 0x7564 }, /*  857 AREA12/MFNT040B.WFM */
    { { itob(7), itob(42), itob(11), 0 }, 0x40EA }, /*  858 AREA12/CFNT999B.WFM */
    { { itob(7), itob(51), itob(58), 0 }, 0x4DAE4 }, /*  859 AREA12/X004.BIN */
    { { itob(7), itob(35), itob(31), 0 }, 0x11C69 }, /*  860 AREA12/B000.GAM */
    { { itob(7), itob(43), itob(21), 0 }, 0x1C31 }, /*  861 AREA12/CLUT01.GAM */
    { { itob(7), itob(44), itob(26), 0 }, 0x1DF8B }, /*  862 AREA12/D001.GAM */
    { { itob(7), itob(45), itob(11), 0 }, 0x955A }, /*  863 AREA12/D002.GAM */
    { { itob(7), itob(35), itob(67), 0 }, 0x11C69 }, /*  864 AREA12/B100.GAM */
    { { itob(7), itob(43), itob(25), 0 }, 0x1C31 }, /*  865 AREA12/CLUT02.GAM */
    { { itob(7), itob(45), itob(30), 0 }, 0x1DF8B }, /*  866 AREA12/D101.GAM */
    { { itob(7), itob(46), itob(15), 0 }, 0x955A }, /*  867 AREA12/D102.GAM */
    { { itob(7), itob(36), itob(28), 0 }, 0x11C69 }, /*  868 AREA12/B200.GAM */
    { { itob(7), itob(43), itob(29), 0 }, 0x1C31 }, /*  869 AREA12/CLUT03.GAM */
    { { itob(7), itob(46), itob(34), 0 }, 0x1C8A8 }, /*  870 AREA12/D201.GAM */
    { { itob(7), itob(47), itob(17), 0 }, 0x9C04 }, /*  871 AREA12/D202.GAM */
    { { itob(7), itob(36), itob(64), 0 }, 0x11C69 }, /*  872 AREA12/B300.GAM */
    { { itob(7), itob(43), itob(33), 0 }, 0x1C31 }, /*  873 AREA12/CLUT04.GAM */
    { { itob(7), itob(47), itob(37), 0 }, 0x1C8A8 }, /*  874 AREA12/D301.GAM */
    { { itob(7), itob(48), itob(20), 0 }, 0x9C04 }, /*  875 AREA12/D302.GAM */
    { { itob(7), itob(37), itob(25), 0 }, 0x11C69 }, /*  876 AREA12/B400.GAM */
    { { itob(7), itob(43), itob(37), 0 }, 0x1C31 }, /*  877 AREA12/CLUT05.GAM */
    { { itob(7), itob(48), itob(40), 0 }, 0x315F }, /*  878 AREA12/D401.GAM */
    { { itob(7), itob(37), itob(61), 0 }, 0x11C69 }, /*  879 AREA12/B500.GAM */
    { { itob(7), itob(43), itob(41), 0 }, 0x1C31 }, /*  880 AREA12/CLUT06.GAM */
    { { itob(7), itob(48), itob(47), 0 }, 0x221C }, /*  881 AREA12/D501.GAM */
    { { itob(7), itob(38), itob(22), 0 }, 0x11C69 }, /*  882 AREA12/B600.GAM */
    { { itob(7), itob(43), itob(45), 0 }, 0x1C31 }, /*  883 AREA12/CLUT07.GAM */
    { { itob(7), itob(48), itob(52), 0 }, 0x471F }, /*  884 AREA12/D601.GAM */
    { { itob(7), itob(38), itob(58), 0 }, 0x11C69 }, /*  885 AREA12/B700.GAM */
    { { itob(7), itob(43), itob(49), 0 }, 0x1C31 }, /*  886 AREA12/CLUT08.GAM */
    { { itob(7), itob(48), itob(61), 0 }, 0x513F }, /*  887 AREA12/D701.GAM */
    { { itob(7), itob(39), itob(19), 0 }, 0x18000 }, /*  888 AREA12/B800.000 */
    { { itob(7), itob(43), itob(53), 0 }, 0x1B94 }, /*  889 AREA12/CLUT09.GAM */
    { { itob(7), itob(48), itob(72), 0 }, 0x391B }, /*  890 AREA12/D801.GAM */
    { { itob(7), itob(39), itob(67), 0 }, 0x2349 }, /*  891 AREA12/B900.GAM */
    { { itob(7), itob(43), itob(57), 0 }, 0x1C58 }, /*  892 AREA12/CLUT10.GAM */
    { { itob(7), itob(49), itob(5), 0 }, 0x3DCA }, /*  893 AREA12/D901.GAM */
    { { itob(7), itob(43), itob(61), 0 }, 0x1B72 }, /*  894 AREA12/CLUT11.GAM */
    { { itob(7), itob(49), itob(13), 0 }, 0x2603 }, /*  895 AREA12/DA00.GAM */
    { { itob(7), itob(39), itob(72), 0 }, 0x11C69 }, /*  896 AREA12/BB00.GAM */
    { { itob(7), itob(43), itob(65), 0 }, 0x1C31 }, /*  897 AREA12/CLUT12.GAM */
    { { itob(7), itob(49), itob(18), 0 }, 0x3F2C }, /*  898 AREA12/DB01.GAM */
    { { itob(7), itob(40), itob(33), 0 }, 0x11C69 }, /*  899 AREA12/BC00.GAM */
    { { itob(7), itob(40), itob(69), 0 }, 0x22A6 }, /*  900 AREA12/BC01.GAM */
    { { itob(7), itob(40), itob(74), 0 }, 0x1F7B }, /*  901 AREA12/BC02.GAM */
    { { itob(7), itob(43), itob(69), 0 }, 0x1C49 }, /*  902 AREA12/CLUT13.GAM */
    { { itob(7), itob(49), itob(26), 0 }, 0x419B }, /*  903 AREA12/DC03.GAM */
    { { itob(7), itob(43), itob(73), 0 }, 0x1B72 }, /*  904 AREA12/CLUT14.GAM */
    { { itob(7), itob(49), itob(35), 0 }, 0x3232 }, /*  905 AREA12/DD00.GAM */
    { { itob(7), itob(44), itob(2), 0 }, 0x1B72 }, /*  906 AREA12/CLUT15.GAM */
    { { itob(7), itob(49), itob(42), 0 }, 0x1DD0 }, /*  907 AREA12/DE00.GAM */
    { { itob(7), itob(41), itob(3), 0 }, 0x52A9 }, /*  908 AREA12/BF00.GAM */
    { { itob(7), itob(44), itob(6), 0 }, 0x1BF3 }, /*  909 AREA12/CLUT16.GAM */
    { { itob(7), itob(49), itob(46), 0 }, 0x1949 }, /*  910 AREA12/DF01.GAM */
    { { itob(7), itob(44), itob(10), 0 }, 0x1B72 }, /*  911 AREA12/CLUT17.GAM */
    { { itob(7), itob(49), itob(50), 0 }, 0x2777 }, /*  912 AREA12/DG00.GAM */
    { { itob(7), itob(41), itob(14), 0 }, 0x11C69 }, /*  913 AREA12/BH00.GAM */
    { { itob(7), itob(44), itob(14), 0 }, 0x1C31 }, /*  914 AREA12/CLUT18.GAM */
    { { itob(7), itob(49), itob(55), 0 }, 0x220A }, /*  915 AREA12/DH01.GAM */
    { { itob(7), itob(41), itob(50), 0 }, 0x11C69 }, /*  916 AREA12/BI00.GAM */
    { { itob(7), itob(44), itob(18), 0 }, 0x1C31 }, /*  917 AREA12/CLUT19.GAM */
    { { itob(7), itob(49), itob(60), 0 }, 0x222E }, /*  918 AREA12/DI01.GAM */
    { { itob(7), itob(44), itob(22), 0 }, 0x1B72 }, /*  919 AREA12/CLUT20.GAM */
    { { itob(7), itob(49), itob(65), 0 }, 0x21AC }, /*  920 AREA12/DJ01.GAM */
    { { itob(8), itob(10), itob(39), 0 }, 0xB2 }, /*  921 AREA13/A000.GAM */
    { { itob(8), itob(10), itob(40), 0 }, 0x34B0 }, /*  922 AREA13/A001.GAM */
    { { itob(8), itob(10), itob(47), 0 }, 0x367C }, /*  923 AREA13/A002.GAM */
    { { itob(8), itob(10), itob(54), 0 }, 0x2E008 }, /*  924 AREA13/A003.000 */
    { { itob(8), itob(11), itob(72), 0 }, 0xA470 }, /*  925 AREA13/A004.GAM */
    { { itob(8), itob(12), itob(18), 0 }, 0x1DFB }, /*  926 AREA13/A105.GAM */
    { { itob(8), itob(17), itob(70), 0 }, 0x2A7C }, /*  927 AREA13/MFNT130B.WFM */
    { { itob(8), itob(15), itob(27), 0 }, 0x40EA }, /*  928 AREA13/CFNT999B.WFM */
    { { itob(8), itob(18), itob(49), 0 }, 0x32D48 }, /*  929 AREA13/X013.BIN */
    { { itob(8), itob(12), itob(54), 0 }, 0x318C }, /*  930 AREA13/B100.GAM */
    { { itob(8), itob(12), itob(61), 0 }, 0x61A9 }, /*  931 AREA13/B101.GAM */
    { { itob(8), itob(16), itob(37), 0 }, 0x1578 }, /*  932 AREA13/CLUT01.GAM */
    { { itob(8), itob(16), itob(43), 0 }, 0x4731 }, /*  933 AREA13/D102.GAM */
    { { itob(8), itob(12), itob(74), 0 }, 0x63AE }, /*  934 AREA13/B300.GAM */
    { { itob(8), itob(13), itob(12), 0 }, 0x20000 }, /*  935 AREA13/B301.000 */
    { { itob(8), itob(14), itob(1), 0 }, 0x30000 }, /*  936 AREA13/B302.000 */
    { { itob(8), itob(15), itob(22), 0 }, 0x2037 }, /*  937 AREA13/B303.GAM */
    { { itob(8), itob(16), itob(40), 0 }, 0x17AE }, /*  938 AREA13/CLUT02.GAM */
    { { itob(8), itob(16), itob(52), 0 }, 0x3B17 }, /*  939 AREA13/D303.GAM */
    { { itob(8), itob(16), itob(60), 0 }, 0x4664 }, /*  940 AREA13/MFNT001B.WFM */
    { { itob(8), itob(30), itob(71), 0 }, 0xB2 }, /*  941 AREA14/A000.GAM */
    { { itob(8), itob(30), itob(72), 0 }, 0x367C }, /*  942 AREA14/A001.GAM */
    { { itob(8), itob(31), itob(4), 0 }, 0xC980 }, /*  943 AREA14/A002.GAM */
    { { itob(8), itob(31), itob(30), 0 }, 0x7D42 }, /*  944 AREA14/A003.GAM */
    { { itob(8), itob(31), itob(46), 0 }, 0x2E008 }, /*  945 AREA14/A004.000 */
    { { itob(8), itob(32), itob(64), 0 }, 0xA43E }, /*  946 AREA14/A005.GAM */
    { { itob(8), itob(33), itob(10), 0 }, 0x1DFB }, /*  947 AREA14/A106.GAM */
    { { itob(8), itob(42), itob(27), 0 }, 0x6306 }, /*  948 AREA14/MFNT140B.WFM */
    { { itob(8), itob(39), itob(5), 0 }, 0x40EA }, /*  949 AREA14/CFNT999B.WFM */
    { { itob(8), itob(43), itob(70), 0 }, 0x42780 }, /*  950 AREA14/X014.BIN */
    { { itob(8), itob(33), itob(46), 0 }, 0x92B2 }, /*  951 AREA14/B000.GAM */
    { { itob(8), itob(33), itob(65), 0 }, 0x3FB7 }, /*  952 AREA14/B001.GAM */
    { { itob(8), itob(33), itob(73), 0 }, 0x209B }, /*  953 AREA14/B002.GAM */
    { { itob(8), itob(40), itob(15), 0 }, 0x1943 }, /*  954 AREA14/CLUT01.GAM */
    { { itob(8), itob(40), itob(47), 0 }, 0xC305 }, /*  955 AREA14/D003.GAM */
    { { itob(8), itob(34), itob(3), 0 }, 0x7180 }, /*  956 AREA14/B100.GAM */
    { { itob(8), itob(34), itob(18), 0 }, 0xC7CC }, /*  957 AREA14/B101.GAM */
    { { itob(8), itob(34), itob(43), 0 }, 0x17B0 }, /*  958 AREA14/B102.GAM */
    { { itob(8), itob(40), itob(19), 0 }, 0x1999 }, /*  959 AREA14/CLUT02.GAM */
    { { itob(8), itob(40), itob(72), 0 }, 0x4F0E }, /*  960 AREA14/D103.GAM */
    { { itob(8), itob(34), itob(46), 0 }, 0xC743 }, /*  961 AREA14/B200.GAM */
    { { itob(8), itob(34), itob(71), 0 }, 0x6E3A }, /*  962 AREA14/B201.GAM */
    { { itob(8), itob(35), itob(10), 0 }, 0x1F4D }, /*  963 AREA14/B202.GAM */
    { { itob(8), itob(40), itob(23), 0 }, 0x19D6 }, /*  964 AREA14/CLUT03.GAM */
    { { itob(8), itob(41), itob(7), 0 }, 0x542E }, /*  965 AREA14/D203.GAM */
    { { itob(8), itob(35), itob(14), 0 }, 0x10163 }, /*  966 AREA14/B300.GAM */
    { { itob(8), itob(35), itob(47), 0 }, 0x148FA }, /*  967 AREA14/B301.GAM */
    { { itob(8), itob(36), itob(14), 0 }, 0x2D76 }, /*  968 AREA14/B302.GAM */
    { { itob(8), itob(40), itob(27), 0 }, 0x1ABA }, /*  969 AREA14/CLUT04.GAM */
    { { itob(8), itob(41), itob(18), 0 }, 0x66F7 }, /*  970 AREA14/D303.GAM */
    { { itob(8), itob(36), itob(20), 0 }, 0xB2DD }, /*  971 AREA14/B400.GAM */
    { { itob(8), itob(36), itob(43), 0 }, 0xC0F6 }, /*  972 AREA14/B401.GAM */
    { { itob(8), itob(40), itob(31), 0 }, 0x1BB8 }, /*  973 AREA14/CLUT05.GAM */
    { { itob(8), itob(41), itob(31), 0 }, 0x61F9 }, /*  974 AREA14/D402.GAM */
    { { itob(8), itob(36), itob(68), 0 }, 0xA217 }, /*  975 AREA14/B500.GAM */
    { { itob(8), itob(37), itob(14), 0 }, 0x111C1 }, /*  976 AREA14/B501.GAM */
    { { itob(8), itob(40), itob(35), 0 }, 0x1DC2 }, /*  977 AREA14/CLUT06.GAM */
    { { itob(8), itob(41), itob(44), 0 }, 0x6226 }, /*  978 AREA14/D502.GAM */
    { { itob(8), itob(37), itob(49), 0 }, 0xCEA2 }, /*  979 AREA14/B600.GAM */
    { { itob(8), itob(38), itob(0), 0 }, 0xABFC }, /*  980 AREA14/B601.GAM */
    { { itob(8), itob(40), itob(39), 0 }, 0x1981 }, /*  981 AREA14/CLUT07.GAM */
    { { itob(8), itob(41), itob(57), 0 }, 0xA829 }, /*  982 AREA14/D602.GAM */
    { { itob(8), itob(38), itob(22), 0 }, 0xCDFB }, /*  983 AREA14/B700.GAM */
    { { itob(8), itob(38), itob(48), 0 }, 0xA7D4 }, /*  984 AREA14/B701.GAM */
    { { itob(8), itob(38), itob(69), 0 }, 0x55EC }, /*  985 AREA14/B702.GAM */
    { { itob(8), itob(40), itob(43), 0 }, 0x1C8D }, /*  986 AREA14/CLUT08.GAM */
    { { itob(8), itob(42), itob(4), 0 }, 0xB56D }, /*  987 AREA14/D703.GAM */
    { { itob(9), itob(0), itob(37), 0 }, 0xF63 }, /*  988 AREA15/SCR0100.WSS */
    { { itob(9), itob(0), itob(7), 0 }, 0x4F35 }, /*  989 AREA15/0200.WSS */
    { { itob(9), itob(0), itob(22), 0 }, 0x1C50 }, /*  990 AREA15/0210.WSS */
    { { itob(9), itob(0), itob(3), 0 }, 0x1BE0 }, /*  991 AREA15/0140.WSS */
    { { itob(9), itob(0), itob(2), 0 }, 0x238 }, /*  992 AREA15/0130.WSS */
    { { itob(9), itob(0), itob(1), 0 }, 0x3AC }, /*  993 AREA15/0120.WSS */
    { { itob(8), itob(59), itob(74), 0 }, 0xA31 }, /*  994 AREA15/0000.WSS */
    { { itob(9), itob(0), itob(26), 0 }, 0xACF }, /*  995 AREA15/0220.WSS */
    { { itob(9), itob(0), itob(17), 0 }, 0x24CC }, /*  996 AREA15/0201.WSS */
    { { itob(9), itob(0), itob(42), 0 }, 0xB2 }, /*  997 AREA16/A000.GAM */
    { { itob(9), itob(0), itob(43), 0 }, 0x367C }, /*  998 AREA16/A001.GAM */
    { { itob(9), itob(0), itob(50), 0 }, 0x5C18 }, /*  999 AREA16/A002.GAM */
    { { itob(9), itob(0), itob(62), 0 }, 0x1DFB }, /* 1000 AREA16/A103.GAM */
    { { itob(9), itob(22), itob(56), 0 }, 0x57CA }, /* 1001 AREA16/MFNT160B.WFM */
    { { itob(9), itob(7), itob(65), 0 }, 0x40EA }, /* 1002 AREA16/CFNT999B.WFM */
    { { itob(9), itob(1), itob(23), 0 }, 0x20000 }, /* 1003 AREA16/B300.000 */
    { { itob(9), itob(2), itob(12), 0 }, 0x34B0 }, /* 1004 AREA16/B301.GAM */
    { { itob(9), itob(9), itob(9), 0 }, 0x138B }, /* 1005 AREA16/CLUT04.GAM */
    { { itob(9), itob(9), itob(21), 0 }, 0x2E008 }, /* 1006 AREA16/D302.000 */
    { { itob(9), itob(10), itob(39), 0 }, 0x1E06 }, /* 1007 AREA16/D303.GAM */
    { { itob(9), itob(10), itob(43), 0 }, 0x11E7 }, /* 1008 AREA16/D304.GAM */
    { { itob(9), itob(24), itob(10), 0 }, 0x33704 }, /* 1009 AREA16/X016.BIN */
    { { itob(9), itob(2), itob(19), 0 }, 0x20000 }, /* 1010 AREA16/B400.000 */
    { { itob(9), itob(3), itob(8), 0 }, 0x20000 }, /* 1011 AREA16/B401.000 */
    { { itob(9), itob(3), itob(72), 0 }, 0x20000 }, /* 1012 AREA16/B402.000 */
    { { itob(9), itob(4), itob(61), 0 }, 0x34B0 }, /* 1013 AREA16/B403.GAM */
    { { itob(9), itob(9), itob(12), 0 }, 0x158F }, /* 1014 AREA16/CLUT05.GAM */
    { { itob(9), itob(10), itob(46), 0 }, 0x2E008 }, /* 1015 AREA16/D404.000 */
    { { itob(9), itob(11), itob(64), 0 }, 0x283B }, /* 1016 AREA16/D405.GAM */
    { { itob(9), itob(4), itob(68), 0 }, 0x20000 }, /* 1017 AREA16/B500.000 */
    { { itob(9), itob(5), itob(57), 0 }, 0x18000 }, /* 1018 AREA16/B501.000 */
    { { itob(9), itob(6), itob(30), 0 }, 0x44B8 }, /* 1019 AREA16/B502.GAM */
    { { itob(9), itob(6), itob(39), 0 }, 0x9F1 }, /* 1020 AREA16/B503.GAM */
    { { itob(9), itob(9), itob(15), 0 }, 0x1539 }, /* 1021 AREA16/CLUT06.GAM */
    { { itob(9), itob(11), itob(70), 0 }, 0x2E008 }, /* 1022 AREA16/D504.000 */
    { { itob(9), itob(13), itob(13), 0 }, 0x3508 }, /* 1023 AREA16/D505.GAM */
    { { itob(9), itob(6), itob(41), 0 }, 0x20000 }, /* 1024 AREA16/B600.000 */
    { { itob(9), itob(7), itob(30), 0 }, 0xCCBF }, /* 1025 AREA16/B601.GAM */
    { { itob(9), itob(7), itob(56), 0 }, 0x44B8 }, /* 1026 AREA16/B602.GAM */
    { { itob(9), itob(9), itob(18), 0 }, 0x14F5 }, /* 1027 AREA16/CLUT07.GAM */
    { { itob(9), itob(13), itob(20), 0 }, 0x2E008 }, /* 1028 AREA16/D603.000 */
    { { itob(9), itob(14), itob(38), 0 }, 0x2FFC }, /* 1029 AREA16/D604.GAM */
    { { itob(9), itob(36), itob(46), 0 }, 0xB2 }, /* 1030 AREA17/A000.GAM */
    { { itob(9), itob(36), itob(47), 0 }, 0x34B0 }, /* 1031 AREA17/A001.GAM */
    { { itob(9), itob(36), itob(54), 0 }, 0x367C }, /* 1032 AREA17/A002.GAM */
    { { itob(9), itob(36), itob(61), 0 }, 0x5C18 }, /* 1033 AREA17/A003.GAM */
    { { itob(9), itob(36), itob(73), 0 }, 0x1DFB }, /* 1034 AREA17/A104.GAM */
    { { itob(9), itob(58), itob(61), 0 }, 0x5966 }, /* 1035 AREA17/MFNT170B.WFM */
    { { itob(9), itob(42), itob(32), 0 }, 0x40EA }, /* 1036 AREA17/CFNT999B.WFM */
    { { itob(9), itob(37), itob(34), 0 }, 0x1ADE2 }, /* 1037 AREA17/B100.GAM */
    { { itob(9), itob(43), itob(45), 0 }, 0x138B }, /* 1038 AREA17/CLUT02.GAM */
    { { itob(9), itob(44), itob(0), 0 }, 0x2E008 }, /* 1039 AREA17/D101.000 */
    { { itob(9), itob(45), itob(18), 0 }, 0x2717 }, /* 1040 AREA17/D102.GAM */
    { { itob(9), itob(45), itob(23), 0 }, 0x154 }, /* 1041 AREA17/D103.GAM */
    { { itob(10), itob(0), itob(18), 0 }, 0x34678 }, /* 1042 AREA17/X017.BIN */
    { { itob(9), itob(38), itob(13), 0 }, 0x2F03F }, /* 1043 AREA17/B200.GAM */
    { { itob(9), itob(39), itob(33), 0 }, 0xCC94 }, /* 1044 AREA17/B201.GAM */
    { { itob(9), itob(39), itob(59), 0 }, 0x44B8 }, /* 1045 AREA17/B202.GAM */
    { { itob(9), itob(43), itob(48), 0 }, 0x15D6 }, /* 1046 AREA17/CLUT03.GAM */
    { { itob(9), itob(45), itob(24), 0 }, 0x2E008 }, /* 1047 AREA17/D203.000 */
    { { itob(9), itob(46), itob(42), 0 }, 0x2717 }, /* 1048 AREA17/D204.GAM */
    { { itob(9), itob(46), itob(47), 0 }, 0xE70 }, /* 1049 AREA17/D205.GAM */
    { { itob(9), itob(39), itob(68), 0 }, 0x13E42 }, /* 1050 AREA17/B300.GAM */
    { { itob(9), itob(43), itob(51), 0 }, 0x138B }, /* 1051 AREA17/CLUT04.GAM */
    { { itob(9), itob(46), itob(49), 0 }, 0x2E008 }, /* 1052 AREA17/D301.000 */
    { { itob(9), itob(47), itob(67), 0 }, 0x2717 }, /* 1053 AREA17/D302.GAM */
    { { itob(9), itob(47), itob(72), 0 }, 0xDFC }, /* 1054 AREA17/D303.GAM */
    { { itob(9), itob(40), itob(33), 0 }, 0x13E42 }, /* 1055 AREA17/B400.GAM */
    { { itob(9), itob(43), itob(54), 0 }, 0x138B }, /* 1056 AREA17/CLUT05.GAM */
    { { itob(9), itob(47), itob(74), 0 }, 0x2E008 }, /* 1057 AREA17/D401.000 */
    { { itob(9), itob(49), itob(17), 0 }, 0x2717 }, /* 1058 AREA17/D402.GAM */
    { { itob(9), itob(49), itob(22), 0 }, 0xDFC }, /* 1059 AREA17/D403.GAM */
    { { itob(9), itob(40), itob(73), 0 }, 0x2491A }, /* 1060 AREA17/B500.GAM */
    { { itob(9), itob(41), itob(72), 0 }, 0xCC94 }, /* 1061 AREA17/B501.GAM */
    { { itob(9), itob(42), itob(23), 0 }, 0x44B8 }, /* 1062 AREA17/B502.GAM */
    { { itob(9), itob(43), itob(57), 0 }, 0x15D6 }, /* 1063 AREA17/CLUT06.GAM */
    { { itob(9), itob(49), itob(24), 0 }, 0x2E008 }, /* 1064 AREA17/D503.000 */
    { { itob(9), itob(50), itob(42), 0 }, 0x2717 }, /* 1065 AREA17/D504.GAM */
    { { itob(9), itob(50), itob(47), 0 }, 0xE40 }, /* 1066 AREA17/D505.GAM */
    { { itob(10), itob(12), itob(69), 0 }, 0xB2 }, /* 1067 AREA18/A000.GAM */
    { { itob(10), itob(12), itob(70), 0 }, 0x367C }, /* 1068 AREA18/A001.GAM */
    { { itob(10), itob(13), itob(2), 0 }, 0x5C18 }, /* 1069 AREA18/A002.GAM */
    { { itob(10), itob(13), itob(14), 0 }, 0x1DFB }, /* 1070 AREA18/A103.GAM */
    { { itob(10), itob(27), itob(40), 0 }, 0x54E4 }, /* 1071 AREA18/MFNT180B.WFM */
    { { itob(10), itob(15), itob(29), 0 }, 0x40EA }, /* 1072 AREA18/CFNT999B.WFM */
    { { itob(10), itob(13), itob(50), 0 }, 0x7304 }, /* 1073 AREA18/B100.GAM */
    { { itob(10), itob(13), itob(65), 0 }, 0x1150D }, /* 1074 AREA18/B101.GAM */
    { { itob(10), itob(14), itob(25), 0 }, 0x44B8 }, /* 1075 AREA18/B102.GAM */
    { { itob(10), itob(16), itob(42), 0 }, 0x15A1 }, /* 1076 AREA18/CLUT02.GAM */
    { { itob(10), itob(16), itob(48), 0 }, 0x2E008 }, /* 1077 AREA18/D103.000 */
    { { itob(10), itob(17), itob(66), 0 }, 0x6289 }, /* 1078 AREA18/D104.GAM */
    { { itob(10), itob(28), itob(67), 0 }, 0x34D2C }, /* 1079 AREA18/X018.BIN */
    { { itob(10), itob(14), itob(34), 0 }, 0x1C136 }, /* 1080 AREA18/B200.GAM */
    { { itob(10), itob(15), itob(16), 0 }, 0x60CB }, /* 1081 AREA18/B201.GAM */
    { { itob(10), itob(16), itob(45), 0 }, 0x1692 }, /* 1082 AREA18/CLUT03.GAM */
    { { itob(10), itob(18), itob(4), 0 }, 0x2E008 }, /* 1083 AREA18/D202.000 */
    { { itob(10), itob(19), itob(22), 0 }, 0x2DA3 }, /* 1084 AREA18/D203.GAM */
    { { itob(10), itob(41), itob(50), 0 }, 0xB2 }, /* 1085 AREA19/A000.GAM */
    { { itob(10), itob(41), itob(51), 0 }, 0x5483 }, /* 1086 AREA19/A001.GAM */
    { { itob(10), itob(41), itob(62), 0 }, 0x877 }, /* 1087 AREA19/A002.GAM */
    { { itob(10), itob(41), itob(64), 0 }, 0x1DFB }, /* 1088 AREA19/A103.GAM */
    { { itob(10), itob(42), itob(25), 0 }, 0x1252F }, /* 1089 AREA19/B000.GAM */
    { { itob(10), itob(42), itob(62), 0 }, 0x30000 }, /* 1090 AREA19/B001.000 */
    { { itob(10), itob(44), itob(8), 0 }, 0x20000 }, /* 1091 AREA19/B002.000 */
    { { itob(10), itob(44), itob(72), 0 }, 0x6210 }, /* 1092 AREA19/B003.GAM */
    { { itob(10), itob(51), itob(20), 0 }, 0x156E }, /* 1093 AREA19/CLUT01.GAM */
    { { itob(10), itob(51), itob(30), 0 }, 0x4FC64 }, /* 1094 AREA19/D004.000 */
    { { itob(10), itob(53), itob(40), 0 }, 0x3291 }, /* 1095 AREA19/D005.GAM */
    { { itob(11), itob(8), itob(10), 0 }, 0x10206 }, /* 1096 AREA19/MFNT020B.WFM */
    { { itob(10), itob(50), itob(10), 0 }, 0x40EA }, /* 1097 AREA19/CFNT999B.WFM */
    { { itob(10), itob(56), itob(54), 0 }, 0xD4B4 }, /* 1098 AREA19/I0NFO.BIN */
    { { itob(10), itob(45), itob(10), 0 }, 0x113D3 }, /* 1099 AREA19/B100.GAM */
    { { itob(10), itob(45), itob(45), 0 }, 0x20000 }, /* 1100 AREA19/B101.000 */
    { { itob(10), itob(46), itob(34), 0 }, 0x45B8 }, /* 1101 AREA19/B102.GAM */
    { { itob(10), itob(46), itob(43), 0 }, 0x8000 }, /* 1102 AREA19/B103.000 */
    { { itob(10), itob(46), itob(59), 0 }, 0x6210 }, /* 1103 AREA19/B104.GAM */
    { { itob(10), itob(46), itob(72), 0 }, 0x6F32 }, /* 1104 AREA19/B105.GAM */
    { { itob(10), itob(51), itob(23), 0 }, 0x19BA }, /* 1105 AREA19/CLUT02.GAM */
    { { itob(10), itob(53), itob(47), 0 }, 0x2E008 }, /* 1106 AREA19/D106.000 */
    { { itob(10), itob(54), itob(65), 0 }, 0x2717 }, /* 1107 AREA19/D107.GAM */
    { { itob(10), itob(54), itob(70), 0 }, 0x15DC }, /* 1108 AREA19/D108.GAM */
    { { itob(11), itob(15), itob(38), 0 }, 0x37CE0 }, /* 1109 AREA19/X002.BIN */
    { { itob(10), itob(47), itob(11), 0 }, 0x30000 }, /* 1110 AREA19/B200.000 */
    { { itob(10), itob(48), itob(32), 0 }, 0x20000 }, /* 1111 AREA19/B201.000 */
    { { itob(10), itob(49), itob(21), 0 }, 0x18000 }, /* 1112 AREA19/B202.000 */
    { { itob(10), itob(49), itob(69), 0 }, 0x8000 }, /* 1113 AREA19/B203.000 */
    { { itob(10), itob(51), itob(27), 0 }, 0x16C6 }, /* 1114 AREA19/CLUT03.GAM */
    { { itob(10), itob(54), itob(73), 0 }, 0x3F464 }, /* 1115 AREA19/D204.000 */
    { { itob(10), itob(56), itob(50), 0 }, 0x1F82 }, /* 1116 AREA19/D205.GAM */
    { { itob(11), itob(12), itob(5), 0 }, 0xDF9A }, /* 1117 AREA19/MFNT190B.WFM */
    { { itob(10), itob(57), itob(6), 0 }, 0x218DC }, /* 1118 AREA19/I0NFO2.BIN */
};
asm(".globl FILE_LINK_SIZES
FILE_LINK_SIZES = FILE_LINKS + 4");

#define CD_REQ   (*(u8**)0x1F800288)
#define CD_HDR   (*(s16**)0x1F80028C)
#define CD_DST   (*(u8**)0x1F800290)
#define CD_NSEC  (*(s32*)0x1F800294)
#define CD_HEAD  (*(s32*)0x1F80029C)
#define CD_TAIL  (*(s32*)0x1F8002A0)
#define CD_FLAGS (*(s32*)((u8*)CD_HDR + 0x10))

void cdLoadTask(void)
{
    RECT    rect;
    u_char  param[40];
    s32     i;
    s32     r;
    s32     base;
    s32     nhead;
    s32     nbody;
    s32     tail;
    s32     head;
    s32     id;
    s32     flags;
    u32     spuAddr;
    u32     len;
    u8*     headTop;
    u8*     bodyTop;
    u8*     headCur;
    u8*     bodyCur;
    u8*     data;
    u8*     entry;
    s16*    hdr;
    s16     vabId;

    LOAD_COMPLETE = 0;
    D_8009C8B0 = 0;
    param[0] = CdlModeSpeed;
    CURRENT_TASK->state0 = 0;
    CURRENT_TASK->state1 = 0;
    while (CdControl(CdlSetmode, param, 0) == 0) {
    }

    for (;;) {
        switch ((u16)CURRENT_TASK->state0) {
        case 0:
            if (CD_HEAD == CD_TAIL) {
                break;
            }
            CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
            break;

        case 1:
            entry = (u8*)(CD_QUEUE + CD_TAIL * 2);
            hdr = *(s16**)entry;
            id = ((LoadRecord*)hdr)->fileId;
            CD_REQ = entry;
            CD_HDR = hdr;
            CdControlF(CdlSetloc, (u8*)&FILE_LINKS + (id * 8));
            CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
            break;

        case 2:
            r = CdSync(1, 0);
            if (r == 2) {
                CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
            } else if (r == 5) {
                CURRENT_TASK->state0 = 1;
                D_8009C8B0++;
            }
            break;

        case 3:
            flags = CD_FLAGS;
            CD_NSEC = (u32)(FILE_LINK_SIZES[*CD_HDR * 2] + 0x7FF) >> 11;
            switch (flags & 0xF) {
            case LOAD_TYPE_STAGING:
            case LOAD_TYPE_LZ_TO_RAM:
                if (CD_FLAGS & LOAD_TYPE_ALT_BUFFER) {
                    CD_DST = (u8*)&LOAD_BUFFER_ALT;
                } else {
                    CD_DST = LOAD_BUFFER;
                }
                break;
            case LOAD_TYPE_LZ_TO_VRAM:
            case LOAD_TYPE_RAW:
            case LOAD_TYPE_RAW_4:
                CD_DST = *(u8**)(CD_REQ + 4);
                break;
            }
            if (CdRead(CD_NSEC, (u_long*)CD_DST, CdlModeSpeed) == 0) {
                D_8009C8B0++;
            } else {
                CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
            }
            break;

        case 4:
            r = CdReadSync(1, 0);
            if (r == 0) {
                CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
            } else if (r == -1) {
                CURRENT_TASK->state0 = 1;
                D_8009C8B0++;
            }
            break;

        case 5:
            switch ((u16)CURRENT_TASK->state1) {
            case 0:
                switch (CD_FLAGS & 0xF) {
                case LOAD_TYPE_STAGING:
                    switch (*((u8*)CD_HDR + 3) & 0xF0) {
                    case LOAD_KIND_GRPX:
                        CURRENT_TASK->state1 = 2;
                        break;
                    case LOAD_KIND_WVD:
                        CURRENT_TASK->state1 = 5;
                        break;
                    }
                    break;
                case LOAD_TYPE_LZ_TO_VRAM:
                    CURRENT_TASK->state1 = (u16)CURRENT_TASK->state1 + 1;
                    break;
                case LOAD_TYPE_RAW:
                case LOAD_TYPE_RAW_4:
                    if ((*((u8*)CD_HDR + 3) & 0xF0) == LOAD_KIND_WVD) {
                        CURRENT_TASK->state1 = 5;
                        break;
                    }
                    CURRENT_TASK->state1 = 0;
                    CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
                    break;
                case LOAD_TYPE_LZ_TO_RAM:
                    CURRENT_TASK->state1 = 4;
                    break;
                }
                break;

            case 1:
                if (CD_FLAGS & LOAD_TYPE_ALT_BUFFER) {
                    lzDecompress(*(u8**)(CD_REQ + 4), (u8*)&LOAD_BUFFER_ALT);
                } else {
                    lzDecompress(*(u8**)(CD_REQ + 4), LOAD_BUFFER);
                }
                CURRENT_TASK->state1 = (u16)CURRENT_TASK->state1 + 1;

            case 2:
                rect.x = CD_HDR[4];
                rect.y = CD_HDR[5];
                rect.w = CD_HDR[6];
                rect.h = CD_HDR[7];
                if (CD_FLAGS & LOAD_TYPE_ALT_BUFFER) {
                    LoadImage(&rect, (u_long*)&LOAD_BUFFER_ALT);
                } else {
                    LoadImage(&rect, (u_long*)LOAD_BUFFER);
                }
                CURRENT_TASK->state1 = (u16)CURRENT_TASK->state1 + 1;
                break;

            case 3:
                DrawSync(0);
                CURRENT_TASK->state1 = 0;
                CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
                break;

            case 4:
                if (CD_FLAGS & LOAD_TYPE_ALT_BUFFER) {
                    lzDecompress((u8*)&LOAD_BUFFER_ALT, *(u8**)(CD_REQ + 4));
                } else {
                    lzDecompress(LOAD_BUFFER, *(u8**)(CD_REQ + 4));
                }
                CURRENT_TASK->state1 = 0;
                CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
                break;

            case 5:
                base = *((u8*)CD_HDR + 3) & 0xF;
                if (base == 0xF) {
                    base = 0;
                }
                data = *(u8**)(CD_REQ + 4);
                headTop = data + *(s32*)(data + 4);
                i = 0;
                headCur = headTop;
                bodyTop = data + *(s32*)data;
                bodyCur = bodyTop;
                head = *(s32*)headCur;
                tail = *(s32*)bodyTop;
                head = (u32)head >> 2;
                nhead = head - 1;
                nbody = ((u32)tail >> 2) - 1;

                if (nhead > 0) {
                    do {
                        vabId = VAB_IDS[base + i];
                        if (vabId != -1) {
                            SsVabClose(vabId);
                        }
                        i++;
                    } while (i < nhead);
                }

                i = 0;
                if (nhead > 0) {
                    do {
                        WVD_HEADERS[base + i] = (s32)(headTop + *(s32*)headCur);
                        headCur += 4;
                        i++;
                    } while (i < nhead);
                }

                i = 0;
                if (nbody > 0) {
                    do {
                        WVD_BODIES[base + i] = (s32)(bodyTop + *(s32*)bodyCur);
                        bodyCur += 4;
                        i++;
                    } while (i < nbody);
                }

                spuAddr = SPU_BANK_ADDRS[CD_HDR[7] * 2];
                i = 0;
                if (nhead > 0) {
                    do {
                        if (i == (nhead - 1)) {
                            len = (s32)(bodyTop + *(s32*)bodyCur) - WVD_BODIES[base + i];
                        } else {
                            len = WVD_BODIES[base + i + 1] - WVD_BODIES[base + i];
                        }
                        SpuSetTransferStartAddr(spuAddr);
                        SpuRead((u_char*)WVD_BODIES[base + i], len);
                        SpuIsTransferCompleted(1);
                        VAB_IDS[base + i] = SsVabFakeHead((u_char*)WVD_HEADERS[base + i], -1, spuAddr);
                        spuAddr += len;
                        SsVabFakeBody(VAB_IDS[base + i]);
                        i++;
                    } while (i < nhead);
                }
                CURRENT_TASK->state1 = 0;
                CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
                break;
            }
            break;

        case 6:
            tail = CD_TAIL;
            head = CD_HEAD;
            base = (s32)&CD_TAIL;
            tail = (tail + 1) & 0x7F;
            *(s32*)base = tail;
            if (head == tail) {
                LOAD_COMPLETE = 1;
                exitTask();
            } else {
                CURRENT_TASK->state0 = 1;
                CURRENT_TASK->state1 = 0;
            }
            break;
        }
        sleepTask(1);
    }
}

int stubCdFunction(void);

void enqueueCdRead(u8* dst, s16* hdr);

void loadWssFile(int arg1, int arg2);

void startSoundTask(void);

void loadAreaListFile(s32 area);

void queueAreaSectionLists(s32 area, s32 section, s32 reloadArea);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/system/cdfile", queueLoadList);

void queueSystemLoadList(s32 id, s32 start);

void loadSoundSet(s32 arg0);

void func_800223E0(void);

void updateObjectSideFlag(u8* self);

s32 isObjectPastReferenceX(u8* self);

void playObjectSfx(u8* self, s32 arg1);

void func_800224FC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

u16 nextRandom(void);

void applyAnimVelocityXY(u8* self, u16 arg1);

void applyAnimVelocityY(u8* self, u16 arg1);

void applyAnimVelocityX(u8* self, u16 arg1);

void applyFrameVelocityXY(u8* self);

void applyFrameVelocityX(u8* self);

void applyFrameVelocityY(u8* self);

void applyTableVelocityXY(u8* self, s16* row);

void applyTableVelocityX(u8* self, s16* tab);

void setObjectSpeedPolar(u8* self, s16 arg1, s16 arg2);

void setObjectSpeedPolar2(u8* self, s16 arg1, s16 arg2);

void setObjectAltSpeedPolar(u8* self, s16 arg1, s16 arg2);

void applyObjectSpeedXY(u8* self);

void applyObjectSpeedX(u8* self);

void applyObjectSpeedVertical(u_short* id);

void applyObjectAltSpeedXY(u8* self);

void applyObjectAltSpeedX(u8* self);

void applyObjectAltSpeedVertical(short* id);

s32 fixedMulSin(s16 arg0, s16 arg1);

s32 fixedMulCos(s16 arg0, s16 arg1);

s32 fixedMulSin2(s16 arg0, s16 arg1);

s32 fixedMulCos2(s16 arg0, s16 arg1);

void readAnimFrameCount(u8* self);

void resetAnimTimer(u_short* id);

void advanceAnimFrame(u8* self, s16 arg1);

s32 tickAnimation(u8* self);

s32 tickAnimationFixedRate(u8* self);
