// DATA 80010fbc 4216 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80012298[];
extern char D_80012368[];
extern char D_80012530[];
extern char D_8001288C[];
extern unsigned int data_80010FBC[];
extern AnimWord anim_80011020[];
extern AnimWord anim_80011038[];
extern AnimWord anim_80011048[];
extern AnimWord anim_8001106C[];
extern AnimWord anim_80011084[];
extern AnimWord anim_80011098[];
extern AnimWord anim_800110E4[];
extern AnimWord anim_800110F0[];
extern AnimWord anim_80011130[];
extern AnimWord anim_80011148[];
extern AnimWord anim_8001117C[];
extern AnimWord anim_800111A0[];
extern AnimWord anim_800111C0[];
extern AnimWord anim_800111E0[];
extern AnimWord anim_80011224[];
extern AnimWord anim_80011258[];
extern AnimWord anim_8001129C[];
extern AnimWord anim_800112E0[];
extern AnimWord anim_80011304[];
extern AnimWord anim_800113DC[];
extern AnimWord anim_80011408[];
extern AnimWord anim_80011418[];
extern AnimWord anim_80011428[];
extern AnimWord anim_80011430[];
extern AnimWord anim_80011440[];
extern AnimWord anim_80011460[];
extern AnimWord anim_80011470[];
extern AnimWord anim_800114B8[];
extern AnimWord anim_80011518[];
extern AnimWord anim_8001157C[];
extern AnimWord anim_800115B0[];
extern AnimWord anim_80011600[];
extern AnimWord anim_80011634[];
extern AnimWord anim_80011660[];
extern AnimWord anim_80011668[];
extern AnimWord anim_8001167C[];
extern AnimWord anim_80011694[];
extern AnimWord anim_800116A4[];
extern AnimWord anim_800116B8[];
extern AnimWord anim_80011710[];
extern AnimWord anim_80011730[];
extern AnimWord anim_80011740[];
extern AnimWord anim_80011750[];
extern AnimWord anim_80011758[];
extern AnimWord anim_80011768[];
extern AnimWord anim_80011770[];
extern AnimWord anim_80011790[];
extern AnimWord anim_800117D4[];
extern AnimWord anim_80011818[];
extern AnimWord anim_8001185C[];
extern AnimWord anim_800118A0[];
extern AnimWord anim_800118E4[];
extern AnimWord anim_80011928[];
extern AnimWord anim_8001196C[];
extern AnimWord anim_800119B0[];
extern AnimWord anim_800119F4[];
extern AnimWord anim_80011A38[];
extern AnimWord anim_80011A58[];
extern AnimWord anim_80011A7C[];
extern AnimWord anim_80011A9C[];
extern AnimWord anim_80011AC0[];
extern AnimWord anim_80011AE0[];
extern AnimWord anim_80011B04[];
extern AnimWord anim_80011B24[];
extern AnimWord anim_80011B48[];
extern AnimWord anim_80011B68[];
extern AnimWord anim_80011B8C[];
extern AnimWord anim_80011B9C[];
extern AnimWord anim_80011BAC[];
extern AnimWord anim_80011BBC[];
extern AnimWord anim_80011BDC[];
extern AnimWord anim_80011BFC[];
extern AnimWord anim_80011C1C[];
extern AnimWord anim_80011C3C[];
extern AnimWord anim_80011C44[];
extern AnimWord anim_80011C4C[];
extern AnimWord anim_80011C54[];
extern AnimWord anim_80011C6C[];
extern AnimWord anim_80011C84[];
extern AnimWord anim_80011C9C[];
extern AnimWord anim_80011CBC[];
extern AnimWord anim_80011CDC[];
extern AnimWord anim_80011CFC[];
extern AnimWord anim_80011D0C[];
extern AnimWord anim_80011D1C[];
extern AnimWord anim_80011D2C[];
extern AnimWord anim_80011D54[];
extern AnimWord anim_80011D7C[];
extern unsigned int data_80011DA4[];
extern unsigned int data_80011EB4[];
extern void *ptrs_80012024[];
extern void *ptrs_80012028[];
extern void *ptrs_80012030[];

unsigned int data_80010FBC[25] = {
    0x20000, 0xB00AC, 0x20000, 0xB00AC,
    0x20000, 0x900AD, 0x20000, 0x900AD,
    0x20000, 0xA00AE, 0x20000, 0xA00AE,
    0x20000, 0xB00AF, 0x20000, 0xB00AF,
    0x20000, 0xB00B0, 0x20000, 0xB00B1,
    0x40000, 0xB00B2, 0x20000, 0xB00B2,
    0x80020000,
};

AnimWord anim_80011020[6] = {
    ANIM(179, 0, 0, 6),
    ANIM(180, 0, 0, 4),
    ANIM_STOP(181, 0, 0, 60),
};

AnimWord anim_80011038[4] = {
    ANIM(182, 0, 0, 10),
    ANIM_STOP(183, 0, 0, 10),
};

AnimWord anim_80011048[9] = {
    ANIM(184, 0, 0, 6),
    ANIM(185, 0, 0, 6),
    ANIM(184, 0, 0, 6),
    ANIM_JUMP(186, 0, 0, 6, anim_80011048),
};

AnimWord anim_8001106C[6] = {
    ANIM(187, 0, 0, 32),
    ANIM(188, 0, 0, 4),
    ANIM_STOP(189, 0, 0, 24),
};

AnimWord anim_80011084[5] = {
    ANIM(190, 0, 0, 4),
    ANIM_JUMP(191, 0, 0, 4, anim_80011084),
};

AnimWord anim_80011098[19] = {
    ANIM(192, 0, 0, 10),
    ANIM(193, 0, 0, 12),
    ANIM(194, 0, 0, 10),
    ANIM_JUMP(195, 0, 0, 12, anim_80011098),
    ANIM(196, 0, 0, 3),
    ANIM(197, 0, 0, 3),
    ANIM(198, 0, 0, 4),
    ANIM(199, 0, 0, 5),
    ANIM_STOP(200, 0, 0, 7),
};

AnimWord anim_800110E4[3] = {
    ANIM_JUMP(197, 8, 0, 4, anim_800110E4),
};

AnimWord anim_800110F0[16] = {
    ANIM(198, 0, 0, 4),
    ANIM(199, 0, 0, 8),
    ANIM_STOP(200, 0, 0, 4),
    ANIM(201, 0, 0, 4),
    ANIM(202, 0, 0, 4),
    ANIM(203, 0, 0, 4),
    ANIM(204, 0, 0, 4),
    ANIM_STOP(205, 0, 0, 4),
};

AnimWord anim_80011130[6] = {
    ANIM(206, 0, 0, 6),
    ANIM(207, 0, 1, 6),
    ANIM_STOP(208, 3, 3, 6),
};

AnimWord anim_80011148[13] = {
    ANIM(209, 0, 0, 5),
    ANIM(210, 0, 0, 5),
    ANIM(211, 0, 0, 5),
    ANIM(212, 0, 0, 5),
    ANIM(213, 0, 0, 5),
    ANIM_JUMP(214, 0, 0, 5, anim_80011148),
};

AnimWord anim_8001117C[9] = {
    ANIM(215, 0, 0, 6),
    ANIM(216, 0, 0, 6),
    ANIM(217, 0, 0, 6),
    ANIM_JUMP(218, 0, 0, 6, anim_8001117C),
};

AnimWord anim_800111A0[8] = {
    ANIM(209, 0, 0, 2),
    ANIM(215, 0, 0, 2),
    ANIM(216, 0, 0, 2),
    ANIM_STOP(217, 0, 0, 2),
};

AnimWord anim_800111C0[8] = {
    ANIM(219, 0, 0, 5),
    ANIM(220, 0, 0, 2),
    ANIM(221, 0, 0, 4),
    ANIM_STOP(222, 0, 0, 4),
};

AnimWord anim_800111E0[17] = {
    ANIM(223, 0, 0, 3),
    ANIM(224, 0, 0, 2),
    ANIM(225, 0, 0, 2),
    ANIM(226, 0, 0, 2),
    ANIM(227, 0, 0, 1),
    ANIM(228, 0, 0, 3),
    ANIM(229, 0, 0, 2),
    ANIM_JUMP(230, 0, 0, 1, anim_800111E0),
};

AnimWord anim_80011224[13] = {
    ANIM(231, 0, 0, 24),
    ANIM(232, 0, 0, 4),
    ANIM(233, 0, 0, 30),
    ANIM(232, 0, 0, 12),
    ANIM(233, 0, 0, 30),
    ANIM_JUMP(232, 0, 0, 5, anim_80011224),
};

AnimWord anim_80011258[17] = {
    ANIM(234, 0, 0, 5),
    ANIM(235, 0, 0, 5),
    ANIM(236, 0, 0, 5),
    ANIM(237, 0, 0, 5),
    ANIM(238, 0, 0, 5),
    ANIM(239, 0, 0, 5),
    ANIM(240, 0, 0, 5),
    ANIM_JUMP(241, 0, 0, 5, anim_80011258),
};

AnimWord anim_8001129C[17] = {
    ANIM(242, 0, 0, 5),
    ANIM(243, 0, 0, 5),
    ANIM(244, 0, 0, 5),
    ANIM(245, 0, 0, 5),
    ANIM(246, 0, 0, 5),
    ANIM(247, 0, 0, 5),
    ANIM(248, 0, 0, 5),
    ANIM_JUMP(249, 0, 0, 5, anim_8001129C),
};

AnimWord anim_800112E0[9] = {
    ANIM(250, 6, 0, 6),
    ANIM(251, 6, 0, 4),
    ANIM(252, 6, 0, 6),
    ANIM_JUMP(251, 6, 0, 4, anim_800112E0),
};

AnimWord anim_80011304[54] = {
    ANIM(253, 0, 0, 2),
    ANIM(254, 0, 0, 2),
    ANIM(255, 0, 0, 2),
    ANIM(256, 0, 0, 2),
    ANIM(257, 0, 0, 2),
    ANIM(258, 0, 0, 2),
    ANIM(259, 0, 0, 2),
    ANIM(260, 0, 0, 2),
    ANIM(261, 0, 0, 2),
    ANIM(262, 0, 0, 2),
    ANIM(263, 0, 0, 2),
    ANIM(264, 0, 0, 2),
    ANIM(265, 0, 0, 2),
    ANIM(266, 0, 0, 2),
    ANIM(267, 0, 0, 2),
    ANIM_STOP(268, 0, 0, 2),
    ANIM(269, 0, 0, 6),
    ANIM(270, 0, 0, 5),
    ANIM_STOP(271, 0, 0, 24),
    ANIM(272, 0, 0, 48),
    ANIM(273, 0, 0, 6),
    ANIM(274, 0, 0, 6),
    ANIM(275, 0, 0, 64),
    ANIM(274, 0, 0, 6),
    ANIM_STOP(273, 0, 0, 6),
    ANIM(276, 0, 0, 3),
    ANIM_STOP(277, 0, 0, 5),
};

AnimWord anim_800113DC[11] = {
    ANIM(278, 0, 0, 12),
    ANIM(279, 0, 0, 3),
    ANIM(280, 0, 0, 6),
    ANIM(281, 0, 0, 8),
    ANIM_JUMP(282, 0, 0, 6, anim_800113DC),
};

AnimWord anim_80011408[4] = {
    ANIM(283, 0, 0, 5),
    ANIM_STOP(284, 0, 0, 8),
};

AnimWord anim_80011418[4] = {
    ANIM(285, 0, 0, 8),
    ANIM_STOP(286, 0, 0, 60),
};

AnimWord anim_80011428[2] = {
    ANIM_STOP(287, 0, 0, 60),
};

AnimWord anim_80011430[4] = {
    ANIM(288, 0, 0, 8),
    ANIM_STOP(289, 0, 0, 255),
};

AnimWord anim_80011440[8] = {
    ANIM(290, 0, 0, 8),
    ANIM(291, 0, 0, 3),
    ANIM(292, 0, 0, 3),
    ANIM_STOP(293, 0, 0, 4),
};

AnimWord anim_80011460[4] = {
    ANIM(294, 0, 0, 60),
    ANIM_STOP(0, 0, 0, 8),
};

AnimWord anim_80011470[18] = {
    ANIM(295, 0, 0, 30),
    ANIM(296, 0, 0, 5),
    ANIM(297, 0, 0, 30),
    ANIM(296, 0, 0, 5),
    ANIM(295, 0, 0, 30),
    ANIM(296, 0, 0, 5),
    ANIM(297, 0, 0, 30),
    ANIM(296, 0, 0, 5),
    ANIM_STOP(0, 0, 0, 8),
};

AnimWord anim_800114B8[24] = {
    ANIM(298, 12, 0, 1),
    ANIM(299, 12, 0, 1),
    ANIM(300, 12, 0, 1),
    ANIM(301, 12, 0, 1),
    ANIM(302, 12, 0, 1),
    ANIM(303, 12, 0, 1),
    ANIM(304, 12, 0, 1),
    ANIM(305, 12, 0, 1),
    ANIM(306, 12, 0, 1),
    ANIM(307, 12, 0, 1),
    ANIM(308, 12, 0, 1),
    ANIM_STOP(309, 12, 0, 1),
};

AnimWord anim_80011518[25] = {
    ANIM(298, 12, 0, 3),
    ANIM(299, 12, 0, 3),
    ANIM(300, 12, 0, 3),
    ANIM(301, 12, 0, 3),
    ANIM(302, 12, 0, 3),
    ANIM(303, 12, 0, 3),
    ANIM(304, 12, 0, 3),
    ANIM(305, 12, 0, 3),
    ANIM(306, 12, 0, 3),
    ANIM(307, 12, 0, 3),
    ANIM(308, 12, 0, 3),
    ANIM_JUMP(309, 12, 0, 3, anim_80011518),
};

AnimWord anim_8001157C[13] = {
    ANIM(310, 12, 0, 6),
    ANIM(311, 12, 0, 6),
    ANIM(312, 12, 0, 6),
    ANIM(313, 12, 0, 6),
    ANIM(312, 12, 0, 6),
    ANIM_JUMP(311, 12, 0, 6, anim_8001157C),
};

AnimWord anim_800115B0[20] = {
    ANIM(314, 12, 0, 2),
    ANIM(315, 12, 0, 2),
    ANIM(316, 12, 0, 2),
    ANIM(317, 12, 0, 2),
    ANIM(318, 12, 0, 2),
    ANIM_STOP(319, 12, 0, 2),
    ANIM(320, 0, 0, 6),
    ANIM(321, 0, 0, 6),
    ANIM(322, 0, 0, 6),
    ANIM_STOP(323, 0, 0, 6),
};

AnimWord anim_80011600[13] = {
    ANIM(324, 0, 0, 24),
    ANIM(325, 0, 0, 4),
    ANIM(326, 0, 0, 30),
    ANIM(325, 0, 0, 12),
    ANIM(326, 0, 0, 30),
    ANIM_JUMP(325, 0, 0, 5, anim_80011600),
};

AnimWord anim_80011634[11] = {
    ANIM(327, 0, 0, 4),
    ANIM_JUMP(328, 0, 0, 4, anim_80011634),
    ANIM(329, 0, 0, 6),
    ANIM(330, 0, 0, 12),
    ANIM_STOP(331, 0, 0, 12),
};

AnimWord anim_80011660[2] = {
    ANIM(332, 0, 0, 6),
};

AnimWord anim_80011668[5] = {
    ANIM(333, 0, 0, 8),
    ANIM_JUMP(334, 0, 0, 8, anim_80011668),
};

AnimWord anim_8001167C[6] = {
    ANIM(335, 0, 0, 8),
    ANIM(336, 0, 0, 30),
    ANIM_STOP(0, 0, 0, 8),
};

AnimWord anim_80011694[4] = {
    ANIM(337, 0, 0, 8),
    ANIM_STOP(0, 0, 0, 8),
};

AnimWord anim_800116A4[5] = {
    ANIM(338, 0, 0, 4),
    ANIM_JUMP(339, 0, 0, 4, anim_800116A4),
};

AnimWord anim_800116B8[22] = {
    ANIM(340, 0, 0, 6),
    ANIM(341, 0, 0, 6),
    ANIM(342, 0, 0, 6),
    ANIM(343, 0, 0, 6),
    ANIM(344, 0, 0, 4),
    ANIM(345, 0, 0, 6),
    ANIM(344, 0, 0, 4),
    ANIM(343, 0, 0, 6),
    ANIM(344, 0, 0, 4),
    ANIM(345, 0, 0, 6),
    ANIM_STOP(344, 0, 0, 4),
};

AnimWord anim_80011710[8] = {
    ANIM(343, 0, 0, 6),
    ANIM(344, 0, 0, 4),
    ANIM(345, 0, 0, 6),
    ANIM_STOP(344, 0, 0, 4),
};

AnimWord anim_80011730[4] = {
    ANIM(346, 0, 0, 5),
    ANIM_STOP(347, 0, 0, 60),
};

AnimWord anim_80011740[4] = {
    ANIM(348, 0, 0, 8),
    ANIM_STOP(349, 0, 0, 60),
};

AnimWord anim_80011750[2] = {
    ANIM_STOP(350, 0, 0, 60),
};

AnimWord anim_80011758[4] = {
    ANIM(351, 0, 0, 8),
    ANIM_STOP(352, 0, 0, 30),
};

AnimWord anim_80011768[2] = {
    ANIM_STOP(353, 0, 0, 60),
};

AnimWord anim_80011770[8] = {
    ANIM(354, 0, 0, 6),
    ANIM(355, 0, 0, 5),
    ANIM(356, 0, 0, 4),
    ANIM_STOP(357, 0, 0, 3),
};

AnimWord anim_80011790[17] = {
    ANIM(0, 0, 0, 6),
    ANIM(1, 0, 0, 6),
    ANIM(2, 0, 0, 6),
    ANIM(3, 0, 0, 6),
    ANIM(4, 0, 0, 6),
    ANIM(3, 0, 0, 6),
    ANIM(2, 0, 0, 6),
    ANIM_JUMP(1, 0, 0, 6, anim_80011790),
};

AnimWord anim_800117D4[17] = {
    ANIM(5, 0, 0, 6),
    ANIM(6, 0, 0, 6),
    ANIM(7, 0, 0, 6),
    ANIM(8, 0, 0, 6),
    ANIM(9, 0, 0, 6),
    ANIM(8, 0, 0, 6),
    ANIM(7, 0, 0, 6),
    ANIM_JUMP(6, 0, 0, 6, anim_800117D4),
};

AnimWord anim_80011818[17] = {
    ANIM(10, 0, 0, 6),
    ANIM(11, 0, 0, 6),
    ANIM(12, 0, 0, 6),
    ANIM(13, 0, 0, 6),
    ANIM(14, 0, 0, 6),
    ANIM(13, 0, 0, 6),
    ANIM(12, 0, 0, 6),
    ANIM_JUMP(11, 0, 0, 6, anim_80011818),
};

AnimWord anim_8001185C[17] = {
    ANIM(15, 0, 0, 6),
    ANIM(16, 0, 0, 6),
    ANIM(17, 0, 0, 6),
    ANIM(18, 0, 0, 6),
    ANIM(19, 0, 0, 6),
    ANIM(18, 0, 0, 6),
    ANIM(17, 0, 0, 6),
    ANIM_JUMP(16, 0, 0, 6, anim_8001185C),
};

AnimWord anim_800118A0[17] = {
    ANIM(20, 0, 0, 6),
    ANIM(21, 0, 0, 6),
    ANIM(22, 0, 0, 6),
    ANIM(23, 0, 0, 6),
    ANIM(24, 0, 0, 6),
    ANIM(23, 0, 0, 6),
    ANIM(22, 0, 0, 6),
    ANIM_JUMP(21, 0, 0, 6, anim_800118A0),
};

AnimWord anim_800118E4[17] = {
    ANIM(25, 0, 0, 4),
    ANIM(26, 0, 0, 4),
    ANIM(27, 0, 0, 4),
    ANIM(28, 0, 0, 4),
    ANIM(29, 0, 0, 4),
    ANIM(30, 0, 0, 4),
    ANIM(31, 0, 0, 4),
    ANIM_JUMP(32, 0, 0, 4, anim_800118E4),
};

AnimWord anim_80011928[17] = {
    ANIM(33, 0, 0, 4),
    ANIM(34, 0, 0, 4),
    ANIM(35, 0, 0, 4),
    ANIM(36, 0, 0, 4),
    ANIM(37, 0, 0, 4),
    ANIM(38, 0, 0, 4),
    ANIM(39, 0, 0, 4),
    ANIM_JUMP(40, 0, 0, 4, anim_80011928),
};

AnimWord anim_8001196C[17] = {
    ANIM(41, 0, 0, 4),
    ANIM(42, 0, 0, 4),
    ANIM(43, 0, 0, 4),
    ANIM(44, 0, 0, 4),
    ANIM(45, 0, 0, 4),
    ANIM(46, 0, 0, 4),
    ANIM(47, 0, 0, 4),
    ANIM_JUMP(48, 0, 0, 4, anim_8001196C),
};

AnimWord anim_800119B0[17] = {
    ANIM(49, 0, 0, 4),
    ANIM(50, 0, 0, 4),
    ANIM(51, 0, 0, 4),
    ANIM(52, 0, 0, 4),
    ANIM(53, 0, 0, 4),
    ANIM(54, 0, 0, 4),
    ANIM(55, 0, 0, 4),
    ANIM_JUMP(56, 0, 0, 4, anim_800119B0),
};

AnimWord anim_800119F4[17] = {
    ANIM(57, 0, 0, 4),
    ANIM(58, 0, 0, 4),
    ANIM(59, 0, 0, 4),
    ANIM(60, 0, 0, 4),
    ANIM(61, 0, 0, 4),
    ANIM(62, 0, 0, 4),
    ANIM(63, 0, 0, 4),
    ANIM_JUMP(64, 0, 0, 4, anim_800119F4),
};

AnimWord anim_80011A38[8] = {
    ANIM(65, 0, 0, 360),
    ANIM(87, 0, 0, 6),
    ANIM(88, 0, 0, 5),
    ANIM(89, 0, 0, 24),
};

AnimWord anim_80011A58[9] = {
    ANIM(96, 0, 0, 48),
    ANIM(97, 0, 0, 6),
    ANIM(98, 0, 0, 64),
    ANIM_JUMP(97, 0, 0, 6, anim_80011A58),
};

AnimWord anim_80011A7C[8] = {
    ANIM(66, 0, 0, 360),
    ANIM(90, 0, 0, 6),
    ANIM(91, 0, 0, 5),
    ANIM(92, 0, 0, 24),
};

AnimWord anim_80011A9C[9] = {
    ANIM(99, 0, 0, 48),
    ANIM(100, 0, 0, 6),
    ANIM(101, 0, 0, 64),
    ANIM_JUMP(100, 0, 0, 6, anim_80011A9C),
};

AnimWord anim_80011AC0[8] = {
    ANIM(67, 0, 0, 360),
    ANIM(90, 0, 0, 6),
    ANIM(91, 0, 0, 5),
    ANIM(92, 0, 0, 24),
};

AnimWord anim_80011AE0[9] = {
    ANIM(99, 0, 0, 48),
    ANIM(100, 0, 0, 6),
    ANIM(101, 0, 0, 64),
    ANIM_JUMP(100, 0, 0, 6, anim_80011AE0),
};

AnimWord anim_80011B04[8] = {
    ANIM(68, 0, 0, 360),
    ANIM(90, 0, 0, 6),
    ANIM(91, 0, 0, 5),
    ANIM(92, 0, 0, 24),
};

AnimWord anim_80011B24[9] = {
    ANIM(99, 0, 0, 48),
    ANIM(100, 0, 0, 6),
    ANIM(101, 0, 0, 64),
    ANIM_JUMP(100, 0, 0, 6, anim_80011B24),
};

AnimWord anim_80011B48[8] = {
    ANIM(69, 0, 0, 360),
    ANIM(93, 0, 0, 6),
    ANIM(94, 0, 0, 5),
    ANIM(95, 0, 0, 24),
};

AnimWord anim_80011B68[9] = {
    ANIM(102, 0, 0, 48),
    ANIM(103, 0, 0, 6),
    ANIM(104, 0, 0, 64),
    ANIM_JUMP(103, 0, 0, 6, anim_80011B68),
};

AnimWord anim_80011B8C[4] = {
    ANIM(126, 0, 0, 10),
    ANIM_STOP(65, 0, 0, 60),
};

AnimWord anim_80011B9C[4] = {
    ANIM(70, 0, 0, 10),
    ANIM_STOP(67, 0, 0, 60),
};

AnimWord anim_80011BAC[4] = {
    ANIM(71, 0, 0, 10),
    ANIM_STOP(69, 0, 0, 60),
};

AnimWord anim_80011BBC[8] = {
    ANIM(72, 0, 0, 16),
    ANIM(73, 0, 0, 9),
    ANIM(74, 0, 0, 10),
    ANIM_STOP(73, 0, 0, 4),
};

AnimWord anim_80011BDC[8] = {
    ANIM(76, 0, 0, 5),
    ANIM(75, 0, 0, 30),
    ANIM(76, 0, 0, 5),
    ANIM_STOP(77, 0, 0, 30),
};

AnimWord anim_80011BFC[8] = {
    ANIM(79, 0, 0, 5),
    ANIM(78, 0, 0, 30),
    ANIM(79, 0, 0, 5),
    ANIM_STOP(80, 0, 0, 30),
};

AnimWord anim_80011C1C[8] = {
    ANIM(82, 0, 0, 5),
    ANIM(81, 0, 0, 30),
    ANIM(82, 0, 0, 5),
    ANIM_STOP(83, 0, 0, 30),
};

AnimWord anim_80011C3C[2] = {
    ANIM_STOP(84, 0, 0, 60),
};

AnimWord anim_80011C44[2] = {
    ANIM_STOP(85, 0, 0, 60),
};

AnimWord anim_80011C4C[2] = {
    ANIM_STOP(86, 0, 0, 60),
};

AnimWord anim_80011C54[6] = {
    ANIM(87, 0, 0, 6),
    ANIM(88, 0, 0, 5),
    ANIM_STOP(89, 0, 0, 24),
};

AnimWord anim_80011C6C[6] = {
    ANIM(90, 0, 0, 6),
    ANIM(91, 0, 0, 5),
    ANIM_STOP(92, 0, 0, 24),
};

AnimWord anim_80011C84[6] = {
    ANIM(93, 0, 0, 6),
    ANIM(94, 0, 0, 5),
    ANIM_STOP(95, 0, 0, 24),
};

AnimWord anim_80011C9C[8] = {
    ANIM(96, 0, 0, 48),
    ANIM(97, 0, 0, 6),
    ANIM(98, 0, 0, 64),
    ANIM_STOP(97, 0, 0, 6),
};

AnimWord anim_80011CBC[8] = {
    ANIM(99, 0, 0, 48),
    ANIM(100, 0, 0, 6),
    ANIM(101, 0, 0, 64),
    ANIM_STOP(100, 0, 0, 6),
};

AnimWord anim_80011CDC[8] = {
    ANIM(102, 0, 0, 48),
    ANIM(103, 0, 0, 6),
    ANIM(104, 0, 0, 64),
    ANIM_STOP(103, 0, 0, 6),
};

AnimWord anim_80011CFC[4] = {
    ANIM(105, 0, 0, 3),
    ANIM_STOP(106, 0, 0, 5),
};

AnimWord anim_80011D0C[4] = {
    ANIM(107, 0, 0, 3),
    ANIM_STOP(108, 0, 0, 5),
};

AnimWord anim_80011D1C[4] = {
    ANIM(109, 0, 0, 3),
    ANIM_STOP(110, 0, 0, 5),
};

AnimWord anim_80011D2C[10] = {
    ANIM(111, 0, 0, 12),
    ANIM(112, 0, 0, 3),
    ANIM(113, 0, 0, 6),
    ANIM(114, 0, 0, 8),
    ANIM_STOP(115, 0, 0, 6),
};

AnimWord anim_80011D54[10] = {
    ANIM(116, 0, 0, 12),
    ANIM(117, 0, 0, 3),
    ANIM(118, 0, 0, 6),
    ANIM(119, 0, 0, 8),
    ANIM_STOP(120, 0, 0, 6),
};

AnimWord anim_80011D7C[10] = {
    ANIM(121, 0, 0, 12),
    ANIM(122, 0, 0, 3),
    ANIM(123, 0, 0, 6),
    ANIM(124, 0, 0, 8),
    ANIM_STOP(125, 0, 0, 6),
};

unsigned int data_80011DA4[68] = {
    0x0, 0x80000000, 0xA700105, 0xA7F0106,
    0x240FE07, 0x107, 0xC400100, 0x147F0105,
    0xFE300100, 0x8300100, 0x18400100, 0x140FF00,
    0x1000FF00, 0xC600101, 0xE600100, 0x240FE02,
    0x100, 0xE400102, 0xC600101, 0xFE300102,
    0x8300102, 0xE400102, 0x140FF00, 0x1000FF00,
    0xC700103, 0xCE80106, 0x240FE04, 0x105,
    0xA400100, 0xC7F0103, 0xFE300100, 0x8300100,
    0xA400100, 0x140FF00, 0x1000FF00, 0x10700100,
    0x10700100, 0x240FE00, 0x100, 0x12400100,
    0x107F0100, 0xFE300100, 0x8300100, 0x12400100,
    0x140FF00, 0x1000FF00, 0x10700100, 0x87F0100,
    0x240FE00, 0x100, 0x12400100, 0x107F0100,
    0xFE300100, 0x8300100, 0x12400100, 0x140FF00,
    0x1000FF00, 0x10700100, 0x87F0100, 0x240FE00,
    0x100, 0x12400100, 0x107F0100, 0xFE300100,
    0x8300100, 0x12400100, 0x140FF00, 0x1000FF00,
};

unsigned int data_80011EB4[92] = {
    0x201, 0x0, 0x1, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x6060700, 0x6060606, 0x0,
    0x0, 0x4000000, 0x4040404, 0x4040404,
    0x4000004, 0x5080004, 0x5050505, 0x50505,
    0x4, 0x3020200, 0x5000203, 0x4,
    0x0, 0x2020002, 0x2020202, 0x2020202,
    0x3030202, 0x30303, 0x1000000, 0x1010101,
    0x1010101, 0x0, 0x1, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x30A09, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3030300,
    0x3030303, 0x303, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x4040404, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
};

void *ptrs_80012024[1] = {
    D_80012530,
};

void *ptrs_80012028[2] = {
    D_80012368,
    D_8001288C,
};

void *ptrs_80012030[1] = {
    D_80012298,
};
