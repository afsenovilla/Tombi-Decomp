// DATA 8013b838 1856 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_8013B838[];
extern AnimWord anim_8013B840[];
extern AnimWord anim_8013B848[];
extern AnimWord anim_8013B850[];
extern AnimWord anim_8013B874[];
extern AnimWord anim_8013B87C[];
extern AnimWord anim_8013B88C[];
extern AnimWord anim_8013B8A0[];
extern AnimWord anim_8013B8C0[];
extern AnimWord anim_8013B8E8[];
extern AnimWord anim_8013B900[];
extern AnimWord anim_8013B910[];
extern AnimWord anim_8013B918[];
extern AnimWord anim_8013B920[];
extern AnimWord anim_8013B928[];
extern AnimWord anim_8013B94C[];
extern AnimWord anim_8013B954[];
extern AnimWord anim_8013B968[];
extern AnimWord anim_8013B97C[];
extern AnimWord anim_8013B984[];
extern AnimWord anim_8013B990[];
extern AnimWord anim_8013B998[];
extern AnimWord anim_8013B9A0[];
extern AnimWord anim_8013B9A8[];
extern AnimWord anim_8013B9B0[];
extern AnimWord anim_8013B9B8[];
extern AnimWord anim_8013B9C0[];
extern AnimWord anim_8013B9C8[];
extern AnimWord anim_8013B9D0[];
extern AnimWord anim_8013B9D8[];
extern AnimWord anim_8013B9E0[];
extern AnimWord anim_8013B9E8[];
extern AnimWord anim_8013B9F0[];
extern AnimWord anim_8013B9F8[];
extern AnimWord anim_8013BA00[];
extern AnimWord anim_8013BA08[];
extern AnimWord anim_8013BA10[];
extern AnimWord anim_8013BA18[];
extern AnimWord anim_8013BA20[];
extern AnimWord anim_8013BA28[];
extern AnimWord anim_8013BA30[];
extern AnimWord anim_8013BA38[];
extern AnimWord anim_8013BA40[];
extern AnimWord anim_8013BA48[];
extern AnimWord anim_8013BA80[];
extern AnimWord anim_8013BA88[];
extern AnimWord anim_8013BA90[];
extern AnimWord anim_8013BA98[];
extern AnimWord anim_8013BAA0[];
extern AnimWord anim_8013BAA8[];
extern AnimWord anim_8013BAB0[];
extern AnimWord anim_8013BAB8[];
extern AnimWord anim_8013BAC0[];
extern AnimWord anim_8013BAC8[];
extern AnimWord anim_8013BAF8[];
extern AnimWord anim_8013BB2C[];
extern AnimWord anim_8013BB34[];
extern AnimWord anim_8013BB4C[];
extern AnimWord anim_8013BB54[];
extern AnimWord anim_8013BB70[];
extern AnimWord anim_8013BB78[];
extern AnimWord anim_8013BBAC[];
extern AnimWord anim_8013BBDC[];
extern AnimWord anim_8013BBE4[];
extern AnimWord anim_8013BBEC[];
extern AnimWord anim_8013BBF4[];
extern AnimWord anim_8013BBFC[];
extern AnimWord anim_8013BC04[];
extern AnimWord anim_8013BC0C[];
extern AnimWord anim_8013BC14[];
extern AnimWord anim_8013BC1C[];
extern AnimWord anim_8013BC24[];
extern AnimWord anim_8013BC2C[];
extern AnimWord anim_8013BC34[];
extern AnimWord anim_8013BC3C[];
extern AnimWord anim_8013BC44[];
extern AnimWord anim_8013BC4C[];
extern AnimWord anim_8013BC70[];
extern AnimWord anim_8013BC78[];
extern AnimWord anim_8013BC9C[];
extern AnimWord anim_8013BCA4[];
extern AnimWord anim_8013BCB4[];
extern AnimWord anim_8013BCBC[];
extern AnimWord anim_8013BCD0[];
extern AnimWord anim_8013BCF8[];
extern AnimWord anim_8013BD38[];
extern AnimWord anim_8013BD40[];
extern AnimWord anim_8013BD78[];
extern AnimWord anim_8013BD80[];
extern AnimWord anim_8013BD88[];
extern AnimWord anim_8013BD90[];
extern AnimWord anim_8013BD98[];
extern AnimWord anim_8013BDA0[];
extern AnimWord anim_8013BDA8[];
extern AnimWord anim_8013BDB0[];
extern AnimWord anim_8013BDB8[];
extern AnimWord anim_8013BDC0[];
extern AnimWord anim_8013BE04[];
extern AnimWord anim_8013BE28[];
extern AnimWord anim_8013BE48[];
extern AnimWord anim_8013BE68[];
extern AnimWord anim_8013BE70[];
extern AnimWord anim_8013BE80[];
extern AnimWord anim_8013BEC0[];
extern AnimWord anim_8013BED8[];
extern AnimWord anim_8013BEEC[];
extern AnimWord anim_8013BF04[];
extern AnimWord anim_8013BF0C[];
extern AnimWord anim_8013BF14[];
extern AnimWord anim_8013BF1C[];
extern AnimWord anim_8013BF40[];
extern void *ptrs_8013BF48[];
extern void *ptrs_8013BF4C[];
extern void *ptrs_8013BF50[];
extern void *ptrs_8013BF54[];
extern AnimWord anim_8013BF58[];
extern AnimWord anim_8013BF60[];
extern AnimWord anim_8013BF68[];
extern AnimWord anim_8013BF70[];

AnimWord anim_8013B838[2] = {
    ANIM_STOP(135, 0, 0, 60),
};

AnimWord anim_8013B840[2] = {
    ANIM_STOP(136, 0, 0, 60),
};

AnimWord anim_8013B848[2] = {
    ANIM_STOP(137, 0, 0, 60),
};

AnimWord anim_8013B850[9] = {
    ANIM(138, 0, 0, 3),
    ANIM(139, 0, 0, 3),
    ANIM(140, 0, 0, 3),
    ANIM_JUMP(139, 0, 0, 3, anim_8013B850),
};

AnimWord anim_8013B874[2] = {
    ANIM_STOP(157, 0, 0, 60),
};

AnimWord anim_8013B87C[4] = {
    ANIM(157, 0, 0, 13),
    ANIM_STOP(158, 0, 0, 13),
};

AnimWord anim_8013B88C[5] = {
    ANIM(163, 0, 0, 8),
    ANIM_JUMP(163, 0, 1, 8, anim_8013B88C),
};

AnimWord anim_8013B8A0[8] = {
    ANIM(165, 0, 0, 6),
    ANIM(166, 0, 0, 6),
    ANIM(162, 0, 0, 6),
    ANIM_STOP(159, 0, 0, 6),
};

AnimWord anim_8013B8C0[10] = {
    ANIM(159, 0, 0, 6),
    ANIM(160, 0, 0, 6),
    ANIM(161, 0, 0, 6),
    ANIM(162, 0, 0, 6),
    ANIM_STOP(157, 0, 0, 6),
};

AnimWord anim_8013B8E8[6] = {
    ANIM(159, 0, 0, 6),
    ANIM(160, 0, 0, 6),
    ANIM_STOP(161, 0, 0, 60),
};

AnimWord anim_8013B900[4] = {
    ANIM(162, 0, 0, 6),
    ANIM_STOP(157, 0, 0, 6),
};

AnimWord anim_8013B910[2] = {
    ANIM_STOP(167, 0, 0, 60),
};

AnimWord anim_8013B918[2] = {
    ANIM_STOP(168, 0, 0, 60),
};

AnimWord anim_8013B920[2] = {
    ANIM_STOP(169, 0, 0, 60),
};

AnimWord anim_8013B928[9] = {
    ANIM(148, 0, 0, 9),
    ANIM(149, 0, 0, 9),
    ANIM(150, 0, 0, 9),
    ANIM_JUMP(151, 0, 0, 9, anim_8013B928),
};

AnimWord anim_8013B94C[2] = {
    ANIM_STOP(152, 0, 0, 9),
};

AnimWord anim_8013B954[5] = {
    ANIM(153, 0, 0, 10),
    ANIM_JUMP(154, 0, 0, 10, anim_8013B954),
};

AnimWord anim_8013B968[5] = {
    ANIM(153, 0, 0, 10),
    ANIM_JUMP(155, 0, 0, 10, anim_8013B968),
};

AnimWord anim_8013B97C[2] = {
    ANIM(155, 0, 0, 10),
};

AnimWord anim_8013B984[3] = {
    ANIM_JUMP(154, 0, 0, 10, anim_8013B97C),
};

AnimWord anim_8013B990[2] = {
    ANIM_STOP(156, 0, 0, 10),
};

AnimWord anim_8013B998[2] = {
    ANIM_STOP(141, 0, 0, 60),
};

AnimWord anim_8013B9A0[2] = {
    ANIM_STOP(142, 0, 0, 60),
};

AnimWord anim_8013B9A8[2] = {
    ANIM_STOP(143, 0, 0, 60),
};

AnimWord anim_8013B9B0[2] = {
    ANIM_STOP(144, 0, 0, 60),
};

AnimWord anim_8013B9B8[2] = {
    ANIM_STOP(145, 0, 0, 60),
};

AnimWord anim_8013B9C0[2] = {
    ANIM_STOP(146, 0, 0, 60),
};

AnimWord anim_8013B9C8[2] = {
    ANIM_STOP(147, 0, 0, 60),
};

AnimWord anim_8013B9D0[2] = {
    ANIM_STOP(171, 0, 0, 60),
};

AnimWord anim_8013B9D8[2] = {
    ANIM_STOP(172, 0, 0, 60),
};

AnimWord anim_8013B9E0[2] = {
    ANIM_STOP(173, 0, 0, 60),
};

AnimWord anim_8013B9E8[2] = {
    ANIM_STOP(174, 0, 0, 60),
};

AnimWord anim_8013B9F0[2] = {
    ANIM_STOP(175, 0, 0, 60),
};

AnimWord anim_8013B9F8[2] = {
    ANIM_STOP(176, 0, 0, 60),
};

AnimWord anim_8013BA00[2] = {
    ANIM_STOP(177, 0, 0, 60),
};

AnimWord anim_8013BA08[2] = {
    ANIM_STOP(178, 0, 0, 60),
};

AnimWord anim_8013BA10[2] = {
    ANIM_STOP(179, 0, 0, 60),
};

AnimWord anim_8013BA18[2] = {
    ANIM_STOP(180, 0, 0, 60),
};

AnimWord anim_8013BA20[2] = {
    ANIM_STOP(181, 0, 0, 60),
};

AnimWord anim_8013BA28[2] = {
    ANIM_STOP(182, 0, 0, 60),
};

AnimWord anim_8013BA30[2] = {
    ANIM_STOP(183, 0, 0, 60),
};

AnimWord anim_8013BA38[2] = {
    ANIM_STOP(184, 0, 0, 60),
};

AnimWord anim_8013BA40[2] = {
    ANIM_STOP(170, 0, 0, 60),
};

AnimWord anim_8013BA48[14] = {
    ANIM(185, 0, 0, 6),
    ANIM(186, 0, 0, 6),
    ANIM(187, 0, 0, 6),
    ANIM(188, 0, 0, 6),
    ANIM(189, 0, 0, 6),
    ANIM(187, 0, 0, 6),
    ANIM_STOP(186, 0, 0, 6),
};

AnimWord anim_8013BA80[2] = {
    ANIM_STOP(190, 0, 0, 60),
};

AnimWord anim_8013BA88[2] = {
    ANIM_STOP(191, 0, 0, 60),
};

AnimWord anim_8013BA90[2] = {
    ANIM_STOP(192, 0, 0, 60),
};

AnimWord anim_8013BA98[2] = {
    ANIM_STOP(193, 0, 0, 60),
};

AnimWord anim_8013BAA0[2] = {
    ANIM_STOP(194, 0, 0, 60),
};

AnimWord anim_8013BAA8[2] = {
    ANIM_STOP(195, 0, 0, 60),
};

AnimWord anim_8013BAB0[2] = {
    ANIM_STOP(196, 0, 0, 60),
};

AnimWord anim_8013BAB8[2] = {
    ANIM_STOP(198, 0, 0, 60),
};

AnimWord anim_8013BAC0[2] = {
    ANIM_STOP(197, 0, 0, 60),
};

AnimWord anim_8013BAC8[12] = {
    ANIM(199, 0, 0, 3),
    ANIM(200, 0, 0, 3),
    ANIM(201, 0, 0, 3),
    ANIM(202, 0, 0, 3),
    ANIM(201, 0, 0, 3),
    ANIM_STOP(200, 0, 0, 3),
};

AnimWord anim_8013BAF8[13] = {
    ANIM(199, 0, 0, 3),
    ANIM(200, 0, 0, 3),
    ANIM(201, 0, 0, 3),
    ANIM(202, 0, 0, 3),
    ANIM(201, 0, 0, 3),
    ANIM_JUMP(200, 0, 0, 3, anim_8013BAF8),
};

AnimWord anim_8013BB2C[2] = {
    ANIM_STOP(205, 0, 0, 60),
};

AnimWord anim_8013BB34[6] = {
    ANIM(205, 0, 0, 8),
    ANIM(206, 0, 0, 8),
    ANIM_STOP(207, 0, 0, 60),
};

AnimWord anim_8013BB4C[2] = {
    ANIM_STOP(208, 0, 0, 60),
};

AnimWord anim_8013BB54[7] = {
    ANIM(209, 0, 0, 8),
    ANIM(210, 0, 0, 8),
    ANIM_JUMP(211, 0, 0, 8, anim_8013BB54),
};

AnimWord anim_8013BB70[2] = {
    ANIM_STOP(212, 0, 0, 60),
};

AnimWord anim_8013BB78[13] = {
    ANIM(213, 0, 0, 4),
    ANIM(214, 0, 0, 4),
    ANIM(215, 0, 0, 4),
    ANIM(213, 0, 0, 4),
    ANIM(214, 0, 0, 4),
    ANIM_JUMP(216, 0, 0, 4, anim_8013BB78),
};

AnimWord anim_8013BBAC[12] = {
    ANIM(217, 0, 0, 4),
    ANIM(218, 0, 0, 4),
    ANIM(219, 0, 0, 4),
    ANIM(220, 0, 0, 4),
    ANIM_STOP(221, 0, 0, 4),
    ANIM_STOP(222, 0, 0, 60),
};

AnimWord anim_8013BBDC[2] = {
    ANIM_STOP(227, 0, 0, 60),
};

AnimWord anim_8013BBE4[2] = {
    ANIM_STOP(228, 0, 0, 60),
};

AnimWord anim_8013BBEC[2] = {
    ANIM_STOP(229, 0, 0, 60),
};

AnimWord anim_8013BBF4[2] = {
    ANIM_STOP(230, 0, 0, 60),
};

AnimWord anim_8013BBFC[2] = {
    ANIM_STOP(231, 0, 0, 60),
};

AnimWord anim_8013BC04[2] = {
    ANIM_STOP(237, 0, 0, 60),
};

AnimWord anim_8013BC0C[2] = {
    ANIM_STOP(238, 0, 0, 60),
};

AnimWord anim_8013BC14[2] = {
    ANIM_STOP(239, 0, 0, 60),
};

AnimWord anim_8013BC1C[2] = {
    ANIM_STOP(240, 0, 0, 60),
};

AnimWord anim_8013BC24[2] = {
    ANIM_STOP(241, 0, 0, 60),
};

AnimWord anim_8013BC2C[2] = {
    ANIM_STOP(242, 0, 0, 60),
};

AnimWord anim_8013BC34[2] = {
    ANIM_STOP(243, 0, 0, 60),
};

AnimWord anim_8013BC3C[2] = {
    ANIM_STOP(244, 0, 0, 60),
};

AnimWord anim_8013BC44[2] = {
    ANIM_STOP(222, 0, 0, 60),
};

AnimWord anim_8013BC4C[9] = {
    ANIM(223, 0, 0, 10),
    ANIM(224, 0, 0, 10),
    ANIM(225, 0, 0, 10),
    ANIM_JUMP(226, 0, 0, 10, anim_8013BC4C),
};

AnimWord anim_8013BC70[2] = {
    ANIM_STOP(232, 0, 0, 60),
};

AnimWord anim_8013BC78[9] = {
    ANIM(233, 0, 0, 10),
    ANIM(234, 0, 0, 10),
    ANIM(235, 0, 0, 10),
    ANIM_JUMP(236, 0, 0, 10, anim_8013BC78),
};

AnimWord anim_8013BC9C[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8013BCA4[4] = {
    ANIM(1, 0, 0, 4),
    ANIM_STOP(0, 0, 0, 4),
};

AnimWord anim_8013BCB4[2] = {
    ANIM(1, 0, 0, 12),
};

AnimWord anim_8013BCBC[5] = {
    ANIM(2, 0, 0, 12),
    ANIM_JUMP(3, 0, 0, 12, anim_8013BCBC),
};

AnimWord anim_8013BCD0[10] = {
    ANIM(4, 0, 0, 5),
    ANIM(5, 0, 0, 3),
    ANIM(6, 0, 0, 8),
    ANIM(7, 0, 0, 4),
    ANIM_STOP(0, 0, 0, 5),
};

AnimWord anim_8013BCF8[16] = {
    ANIM(8, 0, 0, 5),
    ANIM(9, 0, 0, 7),
    ANIM(10, 0, 0, 5),
    ANIM(0, 0, 0, 5),
    ANIM(8, 0, 0, 5),
    ANIM(9, 0, 0, 7),
    ANIM(10, 0, 0, 5),
    ANIM_STOP(0, 0, 0, 5),
};

AnimWord anim_8013BD38[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8013BD40[14] = {
    ANIM(12, 0, 0, 5),
    ANIM(13, 0, 0, 6),
    ANIM(14, 0, 0, 5),
    ANIM(15, 0, 0, 3),
    ANIM(16, 0, 0, 3),
    ANIM(17, 0, 0, 4),
    ANIM_STOP(0, 0, 0, 3),
};

AnimWord anim_8013BD78[2] = {
    ANIM_STOP(245, 0, 0, 60),
};

AnimWord anim_8013BD80[2] = {
    ANIM_STOP(246, 0, 0, 60),
};

AnimWord anim_8013BD88[2] = {
    ANIM_STOP(247, 0, 0, 60),
};

AnimWord anim_8013BD90[2] = {
    ANIM_STOP(248, 0, 0, 60),
};

AnimWord anim_8013BD98[2] = {
    ANIM_STOP(249, 0, 0, 60),
};

AnimWord anim_8013BDA0[2] = {
    ANIM_STOP(250, 0, 0, 60),
};

AnimWord anim_8013BDA8[2] = {
    ANIM_STOP(251, 0, 0, 60),
};

AnimWord anim_8013BDB0[2] = {
    ANIM_STOP(252, 0, 0, 60),
};

AnimWord anim_8013BDB8[2] = {
    ANIM_STOP(253, 0, 0, 60),
};

AnimWord anim_8013BDC0[17] = {
    ANIM(254, 0, 0, 8),
    ANIM(255, 0, 0, 8),
    ANIM(256, 0, 0, 8),
    ANIM(257, 0, 0, 8),
    ANIM(258, 0, 0, 8),
    ANIM(257, 0, 0, 8),
    ANIM(256, 0, 0, 8),
    ANIM_JUMP(255, 0, 0, 8, anim_8013BDC0),
};

AnimWord anim_8013BE04[9] = {
    ANIM(259, 0, 0, 6),
    ANIM(260, 0, 0, 7),
    ANIM(259, 0, 0, 6),
    ANIM_JUMP(260, 0, 0, 10, anim_8013BE04),
};

AnimWord anim_8013BE28[8] = {
    ANIM(261, 0, 0, 5),
    ANIM(262, 0, 0, 5),
    ANIM(263, 0, 0, 12),
    ANIM(264, 0, 0, 8),
};

AnimWord anim_8013BE48[8] = {
    ANIM(265, 0, 0, 16),
    ANIM(266, 0, 0, 8),
    ANIM(267, 0, 0, 7),
    ANIM_STOP(268, 0, 0, 6),
};

AnimWord anim_8013BE68[2] = {
    ANIM_STOP(269, 0, 0, 10),
};

AnimWord anim_8013BE70[4] = {
    ANIM(270, 0, 0, 8),
    ANIM_STOP(253, 0, 0, 60),
};

AnimWord anim_8013BE80[16] = {
    ANIM(271, 0, 0, 8),
    ANIM(272, 0, 0, 8),
    ANIM(273, 0, 0, 8),
    ANIM(274, 0, 0, 8),
    ANIM(275, 0, 0, 8),
    ANIM(274, 0, 0, 8),
    ANIM(273, 0, 0, 8),
    ANIM_STOP(272, 0, 0, 8),
};

AnimWord anim_8013BEC0[6] = {
    ANIM(276, 0, 0, 8),
    ANIM(277, 0, 0, 1),
    ANIM_STOP(278, 0, 0, 16),
};

AnimWord anim_8013BED8[5] = {
    ANIM(273, 0, 0, 16),
    ANIM_JUMP(279, 0, 0, 48, anim_8013BED8),
};

AnimWord anim_8013BEEC[6] = {
    ANIM(280, 0, 0, 8),
    ANIM(281, 0, 0, 7),
    ANIM_STOP(282, 0, 0, 6),
};

AnimWord anim_8013BF04[2] = {
    ANIM_STOP(283, 0, 0, 60),
};

AnimWord anim_8013BF0C[2] = {
    ANIM_STOP(284, 0, 0, 60),
};

AnimWord anim_8013BF14[2] = {
    ANIM_STOP(285, 0, 0, 60),
};

AnimWord anim_8013BF1C[9] = {
    ANIM(286, 0, 0, 9),
    ANIM(287, 0, 0, 9),
    ANIM(286, 0, 0, 9),
    ANIM_JUMP(288, 0, 0, 9, anim_8013BF1C),
};

AnimWord anim_8013BF40[2] = {
    ANIM_STOP(289, 0, 0, 60),
};

void *ptrs_8013BF48[1] = {
    anim_8013BF58,
};

void *ptrs_8013BF4C[1] = {
    anim_8013BF60,
};

void *ptrs_8013BF50[1] = {
    anim_8013BF68,
};

void *ptrs_8013BF54[1] = {
    anim_8013BF70,
};

AnimWord anim_8013BF58[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8013BF60[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8013BF68[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8013BF70[2] = {
    ANIM_STOP(1, 0, 0, 60),
};
