/* "Quien" of each event in the second-screen panel: the character that gives it, named as the game's own text names it
   (its dialogue, the event title or the event description of the sub-screen); where that character has no name in any
   text, the thing the event is about as the text calls it (an item, a door, a sign).  Each name was checked against the
   line the handler opens (bank + line of showMessageBox* / WSS op 0x85 / the topdown dialogue tables) in the Spanish (S)
   and English (B) banks.  Not listed (the panel shows the kind): 24, 36, 79, 175, 176 (their characters have no name in
   any text and the descriptions name nothing), 77 (nothing in PAL starts it).  es / en. */
typedef struct { unsigned char ev; const char *es, *en; } PanelGiver;
static const PanelGiver panel_givers[] = {
    {0, "Cerdos Koma", "Koma Pigs"},   /* desc 0; MFNT001 #91 */
    {1, "Anciano sabio centenario", "100 Year Old Wise Man"},   /* 0000.WSS ev1; MFNT001 #85 'yo soy el sabio Anciano centenario' */
    {2, "La niebla", "The fog"},   /* updateA00L4Type0A; desc 2, MFNT000 #126 */
    {3, "Rana", "Frog"},   /* updateFrog; desc 3, item name */
    {4, "Cofre del tesoro milenario", "1,000 year old treasure chest"},   /* treasureChestGuardTalk 4/1 MFNT030 #232-233 */
    {5, "Yan", "Yan"},   /* a00DarkCloakedFigureScene 6/7 MFNT000 #156 */
    {7, "Yan", "Yan"},   /* a00DarkCloakedFigureScene 6/9; desc 7 */
    {8, "Yan", "Yan"},   /* a00L4Type18Active; bank 6 MFNT000 #159-160 (Yan's bank) */
    {9, "Anciano sabio centenario", "100 Year Old Wise Man"},   /* MFNT001 #86 (hut script) */
    {10, "Anciano sabio centenario", "100 Year Old Wise Man"},   /* MFNT001 #91 */
    {11, "Anciano milenario", "1,000 Year Old Man"},   /* AREA04:15 scene; MFNT040 #214; title 11 */
    {12, "Anciano sabio centenario", "100 Year Old Wise Man"},   /* MFNT001 #98 'Ve a preguntar al Enano Mayor' */
    {13, "Enanos", "Dwarves"},   /* SCR0100 obj0 MFNT010 #257; desc 13/17 */
    {14, "El padre del ni\xF1o", "The child's father"},   /* 0200 obj18 MFNT020 #229, #256 */
    {15, "Semillas de flores", "Flower Seeds"},   /* rewardFlowerSeeds; title 15 */
    {16, "Caja PA", "AP Box"},   /* updateA00ApBox50k; title 16 */
    {17, "Enanos", "Dwarves"},   /* 0210 obj0 MFNT020 #248; desc 17 */
    {19, "Bolas de cristal", "Crystal Balls"},   /* rewardCrystalBalls (item 0x8E); desc 19 */
    {20, "Enanos del bosque", "Dwarves from the forest"},   /* 0210 obj5 MFNT020 #269 */
    {21, "El ladr\xF3n", "The thief"},   /* SCR0100 MFNT010 #232; desc 21 */
    {22, "Telescopio", "Telescope"},   /* updateLookAndSeeViewpoint; MFNT020 #281 */
    {23, "Curandero Enano", "Dwarf healer"},   /* 0200 obj4 MFNT020 #287; desc 87 */
    {25, "El ladr\xF3n", "The thief"},   /* 0220 obj0 MFNT020 #280-281 -> ev25 start */
    {26, "Enano Mayor", "Dwarf Elder"},   /* 0210 obj1 MFNT020 #386 -> ev26 start; #254 */
    {27, "Fuente de caridad", "Charity Fountain"},   /* 0140; MFNT010 #260 */
    {28, "La Mansi\xF3n", "The Mansion"},   /* updateAreaScrollB; title 28 */
    {29, "Bolsa Cerdiablo", "Evil Pig Bag"},   /* phoenixSpiritHintTalkB MFNT030 #226; desc 29 */
    {30, "Cerdiablo", "Evil Pig"},   /* updateEvilPigBoss; desc 30 */
    {31, "Cerdo Koma", "Koma Pig"},   /* komaPigDropInIntro */
    {33, "El minero famoso", "The Famous Digger"},   /* tunnelDiggerTalk; desc 33 */
    {34, "Cerdiablo", "Evil Pig"},   /* updateEvilPigBoss; desc 34 */
    {35, "El F\xE9nix", "The Phoenix"},   /* soakedPhoenixGuideTalk MFNT030 #219-220; desc 35 */
    {37, "PROHIBIDO EL PASO", "NO TRESPASSING"},   /* sign subtype 0x11, MFNT010 #254 */
    {38, "Ciudad Baco", "Baccus Village"},   /* npcSet07SceneStep1 MFNT050 #223-226 */
    {39, "Semillas de flores", "Flower Seeds"},   /* desc 39 */
    {40, "L\xE1grimas de flor", "Flower Tears"},   /* flowerTearsSeekerTalk MFNT090 #219 */
    {41, "Puerta", "Door"},   /* desc 41 'Puerta cerrada con una gran mueca' */
    {42, "Puerta", "Door"},   /* desc 42 */
    {43, "Seta llorona", "Weeping Mushroom"},   /* topdownActorOpenDialogue case 16 MFNT050 #243-246 */
    {44, "Adivina Roja", "Red Fortune Teller"},   /* topdownActorTalkStep MFNT050 #249; sign #211; desc 44 */
    {46, "Tabl\xF3n de anuncios", "Bulletin board"},   /* sign 0x13 MFNT050 #212; #255 */
    {47, "Coleccionista", "Collector"},   /* npcSet07SceneStep2 MFNT050 #259; desc 47 */
    {49, "Tabl\xF3n de anuncios", "Bulletin board"},   /* sign 0x14 MFNT050 #214 */
    {50, "Ayuntamiento de Ciudad Baco", "Baccus Village, Town Hall"},   /* npcSet0CMessageScene MFNT080 #209; MFNT040 #212; MFNT050 #214 */
    {51, "Cerradura peque\xF1""a", "Small Key Hole"},   /* smallKeyHoleScene; title 51 */
    {53, "Bolsa Cerdiablo", "Evil Pig Bag"},   /* desc 53 */
    {54, "Cerdiablo", "Evil Pig"},   /* updateEvilPigBoss; desc 54 */
    {55, "Gran Cerradura", "Large Key Hole"},   /* largeKeyHoleScene; title 55 */
    {56, "Cuadro", "Painting"},   /* paintingSlotHintTalk MFNT040 #213; title 56 */
    {57, "Anciano milenario", "1,000 Year Old Man"},   /* AREA04:15 scene; desc 57 */
    {58, "Anciano milenario", "1,000 Year Old Man"},   /* AREA04:15 scene; MFNT040 #217 */
    {62, "\xC1rbol del Saber", "Tree of Knowledge"},   /* nobleTreeRestoredTalk MFNT100 #221; title 62 */
    {63, "\xC1rbol del Saber", "Tree of Knowledge"},   /* nobleTreeBiteTalk MFNT100 #217; desc 63 */
    {64, "Charles", "Charles"},   /* swimTeacherIntroTalk MFNT100 #230-231; desc 64 */
    {65, "Bonsugee amarillo", "Yellow Bonsugee"},   /* bonsugeeResearcherTalk MFNT110 #209; desc 65 */
    {73, "Torre reloj", "Clock tower"},   /* juiceSellerTalk MFNT160 #219; desc 73 */
    {78, "Minero", "Miner"},   /* lostHatManTalk MFNT100 #255-256; desc 78; MFNT160 #215-216 */
    {80, "Cerdiablo", "Evil Pig"},   /* updateEvilPigBoss; desc 80 */
    {81, "El jefe", "The Boss"},   /* lostTreasureBossTalk MFNT040 #247; title 81 */
    {82, "Taro", "Taro"},   /* topdownA19ActorOpenDialogue 3/0 -> wac 0x52; MFNT190 #408-409 */
    {85, "Cerdiablo", "Evil Pig"},   /* updateEvilPigBoss; desc 85 */
    {86, "Curandero Enano", "Dwarf healer"},   /* 0200 obj4 MFNT020 #387; desc 87 */
    {87, "Curandero Enano", "Dwarf healer"},   /* 0200 obj4 MFNT020 #389; desc 87 */
    {88, "Curandero Enano", "Dwarf healer"},   /* 0200 obj4 MFNT020 #393; desc 87 */
    {90, "Joya de agua", "Jewel of Water"},   /* rewardJewel (item 0x63); desc 90 */
    {102, "Puerta oxidada", "Rusted door"},   /* ironCastleMachineRoomTalk MFNT170 #252; desc 102 */
    {103, "La Bruja Mizuno", "Mizuno the Witch"},   /* juiceManJoinFightTalk MFNT180 #246; desc 103; #223 */
    {104, "Coche GoGo", "GoGoCar"},   /* goGoCarMechanicTalk MFNT110 #218; desc 104 */
    {105, "Bomba", "Bomb"},   /* factoryBossIronCastleDoorTalk MFNT110 #221-223; desc 105 */
    {111, "Cerdiablo", "Evil Pig"},   /* updateEvilPigBoss; desc 111 */
    {112, "La Sirena", "The Mermaid"},   /* mermaidDivingOfferTalk MFNT040 #235; desc 112 */
    {113, "Curandero Enano", "Dwarf healer"},   /* 0200 obj4 MFNT020 #391; desc 87 */
    {114, "La Bruja Mizuno", "Mizuno the Witch"},   /* witchMizunoLostCrystalBallsTalk MFNT180 #225; #223 */
    {115, "Seta normal", "Ordinary Mushroom"},   /* safeMushroomHintTalk MFNT050 #313; item name */
    {116, "Anciano de 10.000 a\xF1os", "10,000 Year Old Man"},   /* tenThousandYearElderTalk MFNT100 #263; #259 */
    {118, "Anciano de 10.000 a\xF1os", "10,000 Year Old Man"},   /* waterfallElderHintScene MFNT170 #243; title 118 */
    {119, "La Sirena", "The Mermaid"},   /* mermaidDivingLesson MFNT040 #243 */
    {120, "La Bruja Mizuno", "Mizuno the Witch"},   /* witchMizunoIntroTalk MFNT180 #223, #237 */
    {121, "La Sirena", "The Mermaid"},   /* updateMotocrossRaceStarter MFNT060 #208 */
    {123, "Bolsa Cerdiablo", "Evil Pig Bag"},   /* underwaterPigBagEventStart; desc 123 */
    {124, "Cerdiablo", "Evil Pig"},   /* updateEvilPigBoss; desc 124 */
    {125, "Puerta del ladr\xF3n", "Thief's Door"},   /* tunnelActorOpenDialogue 6/3 MFNT020 #426; title 125 */
    {126, "Anciano de 10.000 a\xF1os", "10,000 Year Old Man"},   /* tenThousandYearElderTalk MFNT100 #264; desc 126 */
    {127, "Pez potencia", "Psychic Fish"},   /* powerFishCollectorTalk MFNT100 #269, #271 */
    {128, "Cable irrompible", "Unbreakable Wire"},   /* tunnelActorOpenDialogue 6/10 MFNT020 #433; desc 128 */
    {129, "Joya de viento", "Jewel of Wind"},   /* rewardJewel (item 0x64); desc 129 */
    {132, "La Bruja Mizuno", "Mizuno the Witch"},   /* witchMizunoSneezeTalk MFNT180 #234; desc 132 */
    {133, "La Sirena", "The Mermaid"},   /* AREA06:2 scene; MFNT060 #208 */
    {134, "La Sirena", "The Mermaid"},   /* AREA06:2 scene; MFNT060 #208 */
    {135, "La Sirena", "The Mermaid"},   /* AREA06:2 scene; MFNT060 #208 */
    {144, "Anciano del mill\xF3n de a\xF1os", "Million Year Old Man"},   /* AREA02:4 scene; desc 144; MFNT020 #443 */
    {145, "Minero", "Digger"},   /* tunnelActorOpenDialogue 6/6 MFNT020 #429; desc 145 */
    {146, "El adivino azul", "The Blue Fortune Teller"},   /* tunnelActorOpenDialogue 6/12 MFNT020 #435; title 146 */
    {150, "Balsa", "Raft"},   /* waterfallElderHintScene MFNT170 #247; title 150 */
    {151, "Padre de Yan", "Yan's father"},   /* topdownA19 MFNT190 #411 'Yan... Es mi hijo'; desc 151; MFNT010 #343 */
    {153, "La Sirena", "The Mermaid"},   /* mermaidDivingOfferTalk MFNT040 #236 */
    {156, "Anciano del mill\xF3n de a\xF1os", "Million Year Old Man"},   /* tenThousandYearElderBeadsTalk MFNT100 #266; desc 156 */
    {157, "Anciano del mill\xF3n de a\xF1os", "Million Year Old Man"},   /* pigGateDiscoveryScene 7/4-6 MFNT020 #447-449 (speaker of #443) */
    {159, "Anciano del mill\xF3n de a\xF1os", "Million Year Old Man"},   /* pigGateDiscoveryScene 7/3 MFNT020 #446 */
    {160, "Anciano del mill\xF3n de a\xF1os", "Million Year Old Man"},   /* func_8011B47C 7/3 MFNT020 #446; sevenFriendsWaitTalk #454 */
    {161, "Tesoro subterr\xE1neo", "Underground Treasure"},   /* tunnelActorOpenDialogue 6/15 MFNT020 #438; title 161 */
    {164, "Torre de flores", "Flower Tower"},   /* updateFlowerTowerSprout; title 164 */
    {166, "Charles", "Charles"},   /* updateCharlesTalkScene 4/2 MFNT000 #131 'Me llamo Charles' */
    {167, "Cerdo Koma beb\xE9", "Baby Koma Pig"},   /* updateA00PinkPiglet; desc 167 */
    {168, "Enano Mayor", "Dwarf Elder"},   /* 0210 obj1 MFNT020 #258-259 */
    {169, "Flor carn\xEDvora", "Biting Plant Flower"},   /* rewardBitingPlantFlower; title 169 */
    {170, "Puerta", "Door"},   /* windDiesEventScene; desc 170 */
    {171, "El F\xE9nix", "The Phoenix"},   /* npcSet07SceneStep8 MFNT050 #234-242; desc 171 */
    {172, "Bolsa Cerdiablo", "Evil Pig Bag"},   /* flameBlockedNpcTalk MFNT030 #252; desc 172 */
    {173, "Charles", "Charles"},   /* windblownMonkeyAskTrousers MFNT030 #243; desc 173 */
    {174, "Bolsa Cerdiablo", "Evil Pig Bag"},   /* hauntedPigBagEventStart; desc 174 */
    {177, "Hojas quemadas", "Burning leaves"},   /* burningFireContact; desc 177 */
    {178, "Mariposas hoja", "Leaf Butterflies"},   /* leafButterflyEventStart; title 178 */
    {179, "El ladr\xF3n", "The thief"},   /* 0220 obj0 MFNT020 #228, #382 */
    {180, "Barril", "Barrel"},   /* updateRollingBarrel; desc 180 */
    {181, "El ladr\xF3n", "The thief"},   /* SCR0100 obj7 MFNT010 #237-239; desc 181 */
    {182, "La Bruja Mizuno", "Mizuno the Witch"},   /* witchMizunoMagicMirrorTalk MFNT180 #229; desc 114 */
    {183, "Bolsa Cerdiablo", "Evil Pig Bag"},   /* junglePigBagEventStart; desc 183 */
    {185, "Arc\xF3n del tesoro milenario", "1,000 year old treasure chest"},   /* treasureChestGuardTalk MFNT030 #232 */
    {187, "Seta misteriosa", "Mysterious Mushroom"},   /* 0140 obj14 MFNT010 #325; title 187 */
    {188, "Tobog\xE1n de hoja", "Leaf Slider"},   /* 0140 obj13 MFNT010 #327; title 188 */
    {189, "Flor roja", "Red flower"},   /* 0140 obj15 MFNT010 #328 */
    {190, "El ladr\xF3n", "The thief"},   /* lostBossItemNpcTalk MFNT030 #255; desc 190 */
    {191, "El ladr\xF3n", "The thief"},   /* lostBossItemNpcTalk MFNT040 #229; desc 191 */
};
