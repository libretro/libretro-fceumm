#ifndef _FCEU_INES_CORRECT_H
#define _FCEU_INES_CORRECT_H

	/* ROM images that have the battery-backed bit set in the header that really
	don't have battery-backed RAM is not that big of a problem, so I'll
	treat this differently by only listing games that should have battery-backed RAM.
	*/
	{ 0xb17574f3,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* AD&D Heroes of the Lance */
	{ 0x5de61639,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* AD&D Hillsfar */
	{ 0x25952141,    4, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* AD&D Pool of Radiance */
	{ 0xb5ff71ab,   19, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Battle Fleet */
	{ 0x1335cb05,    4, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Crystalis */
	{ 0x2545214c,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* DW */
	{ 0x3b3f88f0,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* DW */
	{ 0x8c5a784e,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* DW 2 */
	{ 0x506e259d,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* DW 4*/
	{ 0xa86a5318,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dw 3 */
	{ 0x0c1792da,   19, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Famista '90 */
	{ 0x45f03d2e,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Faria */
	{ 0xb8b88130,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Final Fantasy (FFE Hack) */
	{ 0xcebd2a31,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Final Fantasy */
	{ 0xc9556b36,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Final Fantasy 1+2 */
	{ 0xd29db3c7,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Final Fantasy 2 */
	{ 0x57e220d0,    4, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Final Fantasy 3 */
	{ 0x466efdc2,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Final Fantasy J */
	{ 0x47c2020b,   19, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Hydlide 3*/
	{ 0xbc11e61a,   19, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kaijuu Monogatari */
	{ 0xde9c9c64,   80, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kyonshiizu 2 */
	{ 0xeaf7ed72,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Legend of Zelda */
	{ 0x3fe272fb,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Legend of Zelda */
	{ 0xace56f39,   19, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mindseeker */
	{ 0x0e1683c5,   80, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mirai Shinwa Jarvas */
	{ 0xe1383deb,   26, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mouryou Senki Madara */
	{ 0x889129cb,    4, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Startropics */
	{ 0xd054ffb0,    4, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Startropics 2*/
	{ 0xba322865,    1, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Zelda 2 */

	{ 0xaf5d7aa2,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Clu Clu Land (W) [o3].nes */
	{ 0xcfb224e6,  222, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ninja (J) (PRG0) [p1][!].nes */
	{ 0x82f204ae,  163, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Liang Shan Ying Xiong (NJ023) (Ch) [!].nes */
	{ 0x684afccd,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Space Hunter (Japan).nes */
	{ 0xad9c63e2,   70, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Space Shadow (Japan).nes */
	{ 0xe1526228,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ki no Bouken - The Quest of Ki (Japan).nes */
	{ 0xaf5d7aa2,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Clu Clu Land (W) [o3].nes */
	{ 0xcfb224e6,  222, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ninja (J) (PRG0) [p1][!].nes */
	{ 0xfcdaca80,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Elevator Action (Japan).nes */
	{ 0xc05a365b,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Chou Fuyuu Yousai Exed Exes (Japan).nes */
	{ 0xc4c3949a,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mario Bros. (World).nes */
	{ 0x32fa246f,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tag Team Pro-Wrestling (Japan).nes */
	{ 0x43d30c2f,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ms. Pac-Man (USA) (Tengen) (Unl).nes */
	{ 0xb3c30bea,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Xevious (Japan) (En).nes */
	{ 0xe492d45a,    0, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Zippy Race (Japan).nes */
	{ 0x24598791,    0, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Duck Hunt (World).nes */
	{ 0x49aeb3a6,    0, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Excitebike (Japan, USA).nes */
	{ 0xb8535ca3,    0, DEFAULT,    MI_V, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mahjong (Japan).nes */
	{ 0x330de468,    0, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Obake no Q Tarou - Wanwan Panic (Japan).nes */
	{ 0xe28f2596,    0, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pac-Land (J) [b2].nes */
	{ 0x5112dc21,    0, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Wild Gunman (World) (Rev 1).nes */
	{ 0xd8ee7669,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Adventures of Rad Gravity, The (USA).nes */
	{ 0x5b837e8d,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Alien Syndrome (Japan).nes */
	{ 0x37ba3261,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Back to the Future Part II & III (USA).nes */
	{ 0x5b6ca654,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Barbie (USA).nes */
	{ 0x61a852ea,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Battle Stadium - Senbatsu Pro Yakyuu (Japan).nes */
	{ 0xf6fa4453,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bigfoot (U) [b4].nes */
	{ 0x391aa1b8,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bloody Warriors - Shan-Go no Gyakushuu (Japan).nes */
	{ 0xa5e8d2cd,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* BreakThru (USA).nes */
	{ 0x3f56a392,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Captain ED (Japan).nes */
	{ 0x078ced30,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Choujin - Ultra Baseball (Japan).nes */
	{ 0xfe364be5,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Deep Dungeon IV - Kuro no Youjutsushi (Japan).nes */
	{ 0x57c12280,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Demon Sword (U) [b1].nes */
	{ 0xd09b74dc,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Great Tank (Japan).nes */
	{ 0xe8baa782,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Gunhed - Aratanaru Tatakai (Japan).nes */
	{ 0x970bd9c2,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Hanjuku Eiyuu (J) [b1].nes */
	{ 0xcd7a2fd7,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Hanjuku Hero (Japan).nes */
	{ 0x63469396,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Hokuto no Ken 4 - Shichisei Haken Den - Hokuto Shinken no Kanata e (Japan).nes */
	{ 0xe94d5181,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Future Wars - Mirai Senshi Lios (Japan).nes */
	{ 0x7156cb4d,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Muppet Adventure - Chaos at the Carnival (USA).nes */
	{ 0x70f67ab7,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Musashi no Bouken (Japan).nes */
	{ 0x958e4bae,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Orb-3D (USA).nes */
	{ 0x291bcd7d,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pachio-kun 2 (Japan).nes */
	{ 0xa9a4ea4c,    1, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Satomi Hakkenden (J) [b2].nes */
	{ 0xcc3544b0,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Triathron, The (Japan).nes */
	{ 0x52ab2d17,    1, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Toukyou Pachi-Slot Adventure (Japan).nes */
	{ 0x934db14a,    1, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* All-Pro Basketball (USA).nes */
	{ 0xf74dfc91,    1, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Win, Lose or Draw (USA).nes */
	{ 0xe6a477b2,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 3-D WorldRunner (USA).nes */
	{ 0x9ea1dc76,    2, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Rainbow Islands (USA).nes */
	{ 0x6d65cac6,    2, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Terra Cresta (Japan).nes */
	{ 0xe1b260da,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Argos no Senshi - Hachamecha Daishingeki (Japan).nes */
	{ 0x1d0f4d6b,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Black Bass, The (USA).nes */
	{ 0x266ce198,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* City Adventure Touch - Mystery of Triangle (J) [b1].nes */
	{ 0x804f898a,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Unit (Japan).nes */
	{ 0x55773880,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Adventures of Gilligan's Island, The (USA).nes */
	{ 0x6e0eb43e,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Puss 'n Boots - Pero's Great Adventure (USA).nes */
	{ 0x2bb6a0f8,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sherlock Holmes - Hakushaku Reijou Yuukai Jiken (Japan).nes */
	{ 0x28c11d24,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sukeban Deka 3 (J) [b1].nes */
	{ 0x02863604,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sukeban Deka III (Japan).nes */
	{ 0x419461d0,    2, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Cars (USA).nes */
	{ 0xdbf90772,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Alpha Mission (USA).nes */
	{ 0xd858033d,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ASO - Armored Scrum Object (J).nes */
	{ 0xf283cf58,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Colorful Dragon (Sachen).nes */
	{ 0xd858033d,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ASO - Armored Scrum Object (J).nes */
	{ 0x8f154a0d,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pu Ke Jing Ling (Asia) (Unl).nes */
	{ 0xd04a40e6,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bingo 75 (Asia) (Ja) (Unl).nes */
	{ 0x637ba508,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Adan y Eva (Spain) (Gluk Video) (Unl).nes */
	{ 0xebd0644d,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Master Chu & The Drunkard Hu (Joy Van).nes */
	{ 0x9bde3267,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Adventures of Dino Riki (USA).nes */
	{ 0xd8eff0df,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Gradius (Japan).nes */
	{ 0x1d41cc8c,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Gyruss (USA).nes */
	{ 0xcf322bb3,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* John Elway's Quarterback (USA).nes */
	{ 0xb5d28ea2,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mystery Quest (USA).nes */
	{ 0x02cc3973,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ninja Kid (USA).nes */
	{ 0xbc065fc3,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pipe Dream (U) [b1].nes */
	{ 0xc9ee15a7,    3, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Aladdin III (1995) (Unl) [hM03].nes */
	{ 0x97b6cb19,    4, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Aladdin (SuperGame) (Mapper 4) [!].nes */
	{ 0xd97c31b0,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Lasalle Ishii no Child's Quest (Japan).nes */
	{ 0x404b2e8b,    4, DEFAULT,    MI_4, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Rad Racer II (USA).nes */
	{ 0x15141401,    4, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Asmik-kun Land (Japan).nes */
	{ 0x4cccd878,    4, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kyatto Ninden Teyandee (J) [b1].nes */
	{ 0x59280bec,    4, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Jackie Chan (Japan).nes */
	{ 0x7474ac92,    4, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kabuki - Quantum Fighter (USA).nes */
	{ 0xf2594374,    4, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Matendouji (Japan).nes */
	{ 0x5337f73c,    4, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Niji no Silk Road (Japan).nes */
	{ 0x9eefb4b4,    4, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pachi-Slot Adventure 2 - Sorotta-kun no Pachi-Slot Tanteidan (Japan).nes */
	{ 0xafe03802,    4, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pachio-kun 3 (Japan) (Rev A).nes */
	{ 0x21a653c7,    4, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Sky Kid (VS).nes */
	{ 0xdb7f07be,    4, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Toki (USA) (Beta).nes */
	{ 0x9cbadc25,    5, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Just Breed (Japan).nes */
	{ 0xf518dd58,    7, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Captain Skyhawk (USA).nes */
	{ 0x6c4a9735,    7, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* WWF Wrestlemania (Europe).nes */
	{ 0x84382231,    9, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Punch-Out!! (Japan) (Gold Edition).nes */
	{ 0xbe939fce,    9, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Punch-Out!! (U) [b1].nes */
	{ 0x7b837fde,    9, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mike Tyson's Punch-Out!! (PC10) [b1].nes */
	{ 0x345d3a1a,   11, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Castle of Deceit (USA) (Unl).nes */
	{ 0xb79f2651,   11, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Chiller (USA) (Unl).nes */
	{ 0x5e66eaea,   13, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Videomation (USA).nes */
	{ 0xcd373baa,   14, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 武士魂 (8 characters).nes */
	{ 0xbfc7a2e9,   16, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ball 3 - Gokuu Den (Japan) (Rev 1).nes */
	{ 0x6e68e31a,   16, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ball 3 - Gokuu Den (Japan).nes */
	{ 0x33b899c9,   16, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ball - Daimaou Fukkatsu (Japan).nes */
	{ 0xa262a81f,   16, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Rokudenashi Blues (Japan).nes */
	{ 0x286fcd20,   21, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ganbare Goemon Gaiden 2 - Tenka no Zaihou (Japan).nes */
	{ 0x335e6339,   23, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* kung fu legend (unl)[!].nes */
	{ 0x1a8d767b,   23, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* kung fu legend (unl)[!p].nes */
	{ 0x0e263d47,   23, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* World Hero (Unl) (TV System Select) [!].nes */
	{ 0xe4a291ce,   23, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* World Hero (Unl) [!].nes */
	{ 0x51e9cd33,   23, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* World Hero (Unl) [b1].nes */
	{ 0x105dd586,   27, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mi Hun Che (Ch)(full copyrights)[!].nes */
	{ 0xbc9bb6c1,   27, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Car (Ch)(full copyrights)[!].nes */
	{ 0x43753886,   27, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mi Hun Che (Ch)(replaced copyrights)[p1][!].nes */
	{ 0x5b3de3d1,   27, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* -- */
	{ 0x511e73f8,   27, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mi Hun Che (Ch) [p1][b2].nes */
	{ 0x5555fca3,   32, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ai Sensei no Oshiete - Watashi no Hoshi (J) [b1].nes */
	{ 0x283ad224,   32, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ai Sensei no Oshiete - Watashi no Hoshi (Japan).nes */
	{ 0x243a8735,   32, DEFAULT,  0x10|4, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Major League (Japan).nes */
	{ 0x8a7d0abe,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Akira (Japan).nes */
	{ 0x376138d8,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Akira (J) [a1].nes */
	{ 0xadf606f6,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bakushou!! Jinsei Gekijou (Japan).nes */
	{ 0xbc7b1d0f,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bakushou!! Jinsei Gekijou 2 (Japan).nes */
	{ 0x7a497ae3,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Don Doko Don (Japan).nes */
	{ 0xbaca10a9,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Golfkko Open (Japan).nes */
	{ 0xf80bdc50,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Insector X (Japan).nes */
	{ 0x2a6559a1,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Operation Wolf (Japan).nes */
	{ 0xaeb7fce9,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Power Blazer (Japan).nes */
	{ 0xd920f9df,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Takeshi no Sengoku Fuuunji (Japan).nes */
	{ 0x3cd4b420,   33, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Takeshi no Sengoku Fuuunji (Japan) (Beta).nes */
	{ 0x4c7c1af3,   34, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Caesars Palace (U) [b1].nes */
	{ 0x932ff06e,   34, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Classic Concentration (U) [b1].nes */
	{ 0xf46ef39a,   37, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Super Mario Bros. + Tetris + Nintendo World Cup (Europe) (Rev 1).nes */
	{ 0x4686c5dd,   41, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Caltron - 6 in 1 (USA) (Unl).nes */
	{ 0x090c0c17,   42, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ai Senshi Nicol (FDS Conversion) [p1][!].nes */
	{ 0x4df84825,   42, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ai Senshi Nicol (FDS Conversion) [p2][!].nes */
	{ 0x579e5bc5,   42, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ai Senshi Nicol (FDS Conversion) [p3].nes */
	{ 0xc744f205,   42, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ai Senshi Nicol (FDS Conversion) [p3][t1].nes */
	{ 0x71699765,   42, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Love Warrior Nicol.nes */
	{ 0x6bf3f6a3,   42, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bio Miracle Bokutte Upa (J) (Mario Baby - FDS Conversion).nes */
	{ 0x5ba1c5cf,   42, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Green Beret (FDS Conversion) (Unl).nes */
	{ 0x50ab1ab2,   42, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ?? Green Beret (FDS Conversion, LH09) (Unl) [U][!][t1] (160K PRG */
	{ 0x7ccb12a3,   43, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ?? SMB2j */
	{ 0x6c71feae,   45, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kunio 8-in-1 [p1].nes */
	{ 0x40c0ad47,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Flintstones, The - The Rescue of Dino & Hoppy (Japan).nes */
	{ 0xaebd6549,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bakushou!! Jinsei Gekijou 3 (Japan).nes */
	{ 0x6cdc0cd9,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bubble Bobble 2 (Japan).nes */
	{ 0x99c395f9,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Captain Saver (Japan).nes */
	{ 0xa7b0536c,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Don Doko Don 2 (Japan).nes */
	{ 0xb17c828a,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Don Doko Don 2 (J) [a1].nes */
	{ 0x40c0ad47,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Flintstones, The - The Rescue of Dino & Hoppy (J).nes */
	{ 0x1500e835,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Jetsons, The - Cogswell's Caper (Japan).nes */
	{ 0xe2c94bc2,   48, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Bros 8 (Unl) [!].nes */
	{ 0xa912b064,   51, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT,   0x800 }, /* 11-in-1 Ball Games [p1][o1].nes (has CHR ROM when it shouldn't) */
	{ 0x39ab0fc7,   64, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Hard Drivin' (USA) (Proto) (Unl).nes */
	{ 0xb19a55dd,   64, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Road Runner (USA) (Unl).nes */
	{ 0xf92be3ec,   64, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Rolling Thunder (USA) (Unl).nes */
	{ 0xd114f544,   66, DEFAULT,    MI_H, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* AV Super Real Pachinko (Japan) (Unl).nes */
	{ 0xe84274c5,   66, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mississippi Satsujin Jiken (J) [h2].nes */
	{ 0xbde3ae9b,   66, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Doraemon (Japan).nes */
	{ 0x9552e8df,   66, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ball - Shen Long no Nazo (Japan).nes */
	{ 0x811f06d9,   66, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Power (USA).nes */
	{ 0xd26efd78,   66, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Mario Bros. + Duck Hunt (USA).nes */
	{ 0xdd8ed0f7,   70, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kamen Rider Club (Japan).nes */
	{ 0xbba58be5,   70, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Family Trainer 6 - Manhattan Police (Japan).nes */
	{ 0x370ceb65,   70, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Family Trainer 5 - Meiro Daisakusen (Japan).nes */
	{ 0x86be4746,    2, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dooly Bravo Land (Korea) (Unl).nes */
	{ 0xe62e3382,   71, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* MiG 29 - Soviet Fighter (USA) (Unl).nes */
	{ 0xac7b0742,   71, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Golden KTV (Ch) [!].nes */
	{ 0x054bd3e9,   74, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Di 4 Ci - Ji Qi Ren Dai Zhan (Ch).nes */
	{ 0x496ac8f7,   74, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ji Jia Zhan Shi (Ch) [b3].nes */
	{ 0xae854cef,   74, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Jia A Fung Yun (Ch).nes */
	{ 0xba51ac6f,   78, DEFAULT,    MI_4, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Holy Diver (Japan).nes */
	{ 0x3d1c3137,   78, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Uchuusen Cosmo Carrier (Japan).nes */
	{ 0xa4fbb438,   79, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* F-15 City War (AVE) (REV1.x) [b1].nes */
	{ 0xd4a76b07,   79, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* F-15 City War (AVE) (REV1.x) [b2].nes */
	{ 0x8eab381c,   79, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Deathbots (USA) (Rev 1) (Unl).nes */
	{ 0x1eb4a920,   79, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Double Strike - Aerial Attack Force (USA) (v1.1) (Unl).nes */
	{ 0x3e1271d5,   79, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tiles of Fate (USA) (Unl).nes */
	{ 0x2deb12b8,   79, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Venice Beach Volleyball (Asia) (Ja) (Super Mega) (Unl).nes */
	{ 0xd2699893,   88, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Spirit - Aratanaru Densetsu (Japan).nes */
	{ 0xbb7c5f7a,   89, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tenka no Goikenban - Mito Koumon (J) [f1].nes */
	{ 0x10119e6b,   93, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Fantasy Zone (Japan) (Sunsoft).nes */
	{ 0x2b750bf9,  101, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Urusei Yatsura - Lum no Wedding Bell (Japan) (Beta).nes */
	{ 0x0da5e32e,  101, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Urusei Yatsura - Lum no Wedding Bell (Japan).nes */
	{ 0x6096f84e,  104, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pegasus 5-in-1 (Golden Five) (Unl).nes */
	{ 0x3d3ff543,  113, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kazama Jun to Asama Yuuko no AV Dragon Mahjong (Japan) (Unl).nes */
	{ 0x68379fdb,  113, DEFAULT,    MI_V, 1, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Pipemania (Australia) (HES) (Unl).nes */
	{ 0x6a03d3f3,  114, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Lion King, The (Unl) (Mapper 114).nes */
	{ 0x0d98db53,  114, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pocahontas (Unl).nes */
	{ 0xf5676f0b,  114, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Donkey Kong (Unl) [b1].nes */
	{ 0xc5e5c5b2,  115, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bao Qing Tian (Ch).nes */
	{ 0xe40dfb7e,  116, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Somari (SOMARI-P) (NT-616) (Unl) [!].nes */
	{ 0xc9371ebb,  116, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Somari (SOMARI-W) (Unl) [!].nes */
	{ 0x78b657ac,  118, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Armadillo (Japan).nes */
	{ 0x90c773c1,  118, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Goal! Two (USA).nes */
	{ 0xb9b4d9e0,  118, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* NES Play Action Football (USA).nes */
	{ 0x07d92c31,  118, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* RPG Jinsei Game (Japan).nes */
	{ 0x37b62d04,  118, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ys III - Wanderers from Ys (Japan).nes */
	{ 0x318e5502,  121, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sonic 3D Blast 6 (Unl).nes */
	{ 0xddcfb058,  121, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Street Fighter Zero 2 '97 (Unl) [!].nes */
	{ 0xd2674b0a,  132, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Qi Wang - Chinese Chess (Asia) (Ja) (Unl).nes */
	{ 0x5aefbc94,  133, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Jovial Race (Asia) (Ja) (PAL) (Unl).nes */
	{ 0xc2df0a00,  140, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Bio Senshi Dan - Increaser Tono Tatakai (J) [hM66][b3].nes */
	{ 0xe46b1c5d,  140, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mississippi Satsujin Jiken (Japan).nes */
	{ 0x3293afea,  140, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mississippi Satsujin Jiken (Japan) (Rev A).nes */
	{ 0x6bc65d7e,  140, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Youkai Club (Japan).nes */
	{ 0x5caa3e61,  144, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Death Race (USA) (Unl).nes */
	{ 0x48239b42,  146, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mahjang Companion (Asia) (Ja) (Hacker) (Unl).nes */
	{ 0xb6a727fa,  146, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Papillon Gals (Japan) (Unl).nes */
	{ 0xa62b79e1,  146, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sidewinder (HES) [o1].nes */
	{ 0xcc868d4e,  149, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Taiwan Mahjong 16 (Sachen) [a1][!].nes */
	{ 0x29582ca1,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mei Nu Quan (Honey Peach) (Sachen) [!].nes */
	{ 0x40dbf7a2,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Olympic IQ (Asia) (Ja) (PAL) (Unl).nes */
	{ 0x73fb55ac,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Lightgun Game 2 in 1 - Cosmocop + Cyber Monster (Asia) (Ja) (Unl).nes */
	{ 0xddcbda16,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Lightgun Game 2 in 1 - Tough Cop + Super Tough Cop (Asia) (Ja) (Unl).nes */
	{ 0x47918d84,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Auto-Upturn (Asia) (Ja) (PAL) (Unl).nes */
	{ 0x471173e7,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Chinese Checkers (Asia) (Ja) (PAL) (Unl).nes */
	{ 0x2394ae1c,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Happy Pairs (Asia) (Ja) (PAL) (Unl).nes */
	{ 0xcab40a6c,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Magic Cube (Asia) (Ja) (PAL) (Unl).nes */
	{ 0xbe17e27b,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Poker III (Asia) (Ja) (Alt 2) (Unl).nes */
	{ 0x34ddf806,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Strategist (Asia) (Ja) (NTSC) (Unl).nes */
	{ 0xc06facfc,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Strategist (Asia) (Ja) (PAL) (Unl).nes */
	{ 0xa95a915a,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tasac (Asia) (Ja) (Unl).nes */
	{ 0x0f141525,  152, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Arkanoid II (Japan).nes */
	{ 0xbda8f8e4,  152, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Gegege no Kitarou 2 - Youkai Gundan no Chousen (Japan).nes */
	{ 0xb1a94b82,  152, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pocket Zaurus - Juu Ouken no Nazo (Japan).nes */
	{ 0x026c5fca,  152, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Saint Seiya - Ougon Densetsu (Japan).nes */
	{ 0x3f15d20d,  153, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Famicom Jump II - Saikyou no 7 Nin (Japan).nes */
	{ 0xd1691028,  154, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Devil Man (Japan).nes */
	{ 0xcfd4a281,  155, DEFAULT, DFAULT8, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Money Game, The (Japan).nes */
	{ 0x2f27cdef,  155, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tatakae!! Rahmen Man - Sakuretsu Choujin 102 Gei (J) [b1].nes */
	{ 0xc1719664,  155, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tatakae!! Rahmen Man - Sakuretsu Choujin 102 Gei (Japan).nes */
	{ 0xccc03440,  156, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Buzz & Waldog (USA) (Proto) (Unl).nes */
	{ 0x983d8175,  157, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Datach - Battle Rush - Build Up Robot Tournament (Japan).nes */
	{ 0x894efdbc,  157, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Datach - Crayon Shin-chan - Ora to Poi Poi (Japan).nes */
	{ 0x19e81461,  157, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Datach - Dragon Ball Z - Gekitou Tenkaichi Budoukai (Japan).nes */
	{ 0xbe06853f,  157, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Datach - J.League Super Top Players (Japan).nes */
	{ 0x0be0a328,  157, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Datach - SD Gundam - Gundam Wars (Japan).nes */
	{ 0x5b457641,  157, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Datach - Ultraman Club - Supokon Fight! (Japan).nes */
	{ 0xf51a7f46,  157, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Datach - Yu Yu Hakusho - Bakutou Ankoku Bujutsukai (Japan).nes */
	{ 0xcbf4366f,  158, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Alien Syndrome (USA) (Unl).nes */
	{ 0xe170404c,  159, DEFAULT, DEFAULT, 1,    0x10, DEFAULT, DEFAULT, NOEXTRA }, /* SD Gundam Gaiden - Knight Gundam Monogatari (Japan).nes */
	{ 0x276ac722,  159, DEFAULT, DEFAULT, 1,    0x10, DEFAULT, DEFAULT, NOEXTRA }, /* SD Gundam Gaiden - Knight Gundam Monogatari (Japan) (Rev 1).nes */
	{ 0x0cf42e69,  159, DEFAULT, DEFAULT, 1,    0x10, DEFAULT, DEFAULT, NOEXTRA }, /* Magical Taruruuto-kun - Fantastic World!! (Japan).nes */
	{ 0xdcb972ce,  159, DEFAULT, DEFAULT, 1,    0x10, DEFAULT, DEFAULT, NOEXTRA }, /* Magical Taruruuto-kun - Fantastic World!! (Japan) (Rev 1).nes */
	{ 0xb7f28915,  159, DEFAULT, DEFAULT, 1,    0x10, DEFAULT, DEFAULT, NOEXTRA }, /* Magical Taruruuto-kun 2 - Mahou Daibouken (Japan).nes */
	{ 0x183859d2,  159, DEFAULT, DEFAULT, 1,    0x10, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ball Z - Kyoushuu! Saiya Jin (Japan).nes */
	{ 0x58152b42,  160, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pipe V (Asia) (Ja) (Unl).nes */
	{ 0x1c098942,  162, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Xi You Ji Hou Zhuan (Ch).nes */
	{ 0x081caaff,  163, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Commandos (Ch).nes */
	{ 0x02c41438,  176, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Xing He Zhan Shi (Asia) (Unl).nes */
	{ 0x409601a5,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (BS-8088) [p1][!].nes */
	{ 0xa391549d,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8021) [p1][!].nes */
	{ 0x06d13d9e,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8026) [p1][!].nes */
	{ 0x2ebd5fd6,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8033) [p1][!].nes */
	{ 0x23e4906a,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8043) [p1][!].nes */
	{ 0x10155a92,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8045) [p1][!].nes */
	{ 0x8baeedc0,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8052) [p1][!].nes */
	{ 0x39307391,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8056) [p1][!].nes */
	{ 0x72ceab1e,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8078) (Ch) [p1].nes */
	{ 0x07d3f6cb,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23C8079) [p1][!].nes */
	{ 0xc6d97331,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (FK23Cxxxx, S-0210A PCB) [p1][!].nes */
	{ 0x17d43af9,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 5-in-1 (K5003) [p1][!].nes */
	{ 0x20379331,  176, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mortal Kombat 30 Peoples (DH1043) (Ch).nes */
	{ 0x558c0dc3,  178, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super 2-in-1 (Soccer Game & Crazy Dance) (Unl) [!].nes */
	{ 0xc68363f6,  180, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Crazy Climber (Japan).nes */
	{ 0x0f05ff0a,  181, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Seicross (Japan) (Rev 1).nes */
	{ 0x96ce586e,  189, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Street Fighter II - The World Warrior (Unl) [!].nes */
	{ 0x0e76e4c1,  190, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Magic Kid Googoo (Korea) (Unl).nes */
	{ 0x555a555e,  191, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sugoro Quest - Dice no Senshitachi (Ch).nes */
	{ 0x2cc381f6,  191, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sugoro Quest - Dice no Senshitachi (Ch) [o1].nes */
	{ 0xa145fae6,  192, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Young Chivalry (Ch) [b3].nes */
	{ 0xa9115bc1,  192, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Young Chivalry (Ch) [!].nes */
	{ 0x4c7bbb0e,  192, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Young Chivalry (Ch) [b2].nes */
	{ 0x98c1cd4b,  192, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ying Lie Qun Xia Zhuan (Asia) (Unl).nes */
	{ 0xee810d55,  192, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* You Ling Xing Dong (Asia) (Unl).nes */
	{ 0x442f1a29,  192, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Young Chivalry (Ch) [b1].nes */
	{ 0x637134e8,  193, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Fighting Hero (Asia) (Ja) (Unl).nes */
	{ 0xa925226c,  194, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dai-2-Ji - Super Robot Taisen (Ch) [b1].nes */
	{ 0x7f3dbf1b,  195, DEFAULT,    MI_H, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Chaos World (Ch).nes */
	{ 0xb616885c,  195, DEFAULT,    MI_H, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Chaos World (Ch) [a1].nes */
	{ 0x33c5df92,  195, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Captain Tsubasa Vol. II - Super Striker (Ch) [a1].nes */
	{ 0x1bc0be6c,  195, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Captain Tsubasa Vol. II - Super Striker (Ch) [a3].nes */
	{ 0xd5224fde,  195, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* God Slayer - Haruka Tenkuu no Sonata (Ch).nes */
	{ 0xfdec419f,  196, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Street Fighter VI 16 Peoples (Unl) [!].nes */
	{ 0x700705f4,  198, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ---- */
	{ 0x9a2cf02c,  198, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ---- */
	{ 0xd8b401a7,  198, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ---- */
	{ 0x28192599,  198, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ---- */
	{ 0x19b9e732,  198, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tenchi wo Kurau II - Shokatsu Koumei Den (J) (PRG0) [T-Chi][b6].nes */
	{ 0xdd431ba7,  198, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Tenchi wo kurau 2 (c) */
	{ 0xd871d3e6,  199, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ball Z II - Gekishin Freeza!! (Ch).nes */
	{ 0xed481b7c,  199, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Ball Z Gaiden - Saiya Jin Zetsumetsu Keikaku (Ch).nes */
	{ 0x44c20420,  199, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* San Guo Zhi 2 (Ch).nes */
	{ 0x44c20420,  199, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* San Guo Zhi 2 (Ch).nes */
	{ 0x05658ded,  201,      15, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 21-in-1 (CF-043) (2006-V) (Unl) [p1].nes */
	{ 0x276237b3,  206, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Karnov (Japan) (Rev 1).nes */
	{ 0xa5e6baf9,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dragon Slayer IV - Drasle Family (Japan).nes */
	{ 0x4f2f1846,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Famista '89 - Kaimaku Ban!! (Japan).nes */
	{ 0x22d6d5bd,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Jikuu Yuuden - Debias (Japan).nes */
	{ 0x9d21fe96,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Lupin Sansei - Pandora no Isan (Japan).nes */
	{ 0xae321339,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pro Yakyuu - Family Stadium '88 (Japan).nes */
	{ 0x96dfc776,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* R.B.I. Baseball 2 (USA) (Unl).nes */
	{ 0xfd63e7ac,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* R.B.I. Baseball 3 (USA) (Unl).nes */
	{ 0x2a01f9d1,  206, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Wagyan Land (Japan).nes */
	{ 0x7678f1d5,  207, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Fudou Myouou Den (Japan).nes */
	{ 0x07eb2c12,  208, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* FC25-台湾格斗游戏+快打传说.nes */
	{ 0xdd8ced31,  209, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Power Rangers III (Unl) [!].nes */
	{ 0x063b1151,  209, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Power Rangers IV (Unl) [!].nes */
	{ 0xdd4d9a62,  209, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Shin Samurai Spirits 2 - Haoumaru Jigoku Hen (Ch).nes */
	{ 0x0c47946d,  210,       1,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Chibi Maruko-chan - Uki Uki Shopping (Japan).nes */
	{ 0xc247cc80,  210,       1,    MI_V, 1,    0x50, DEFAULT, DEFAULT, NOEXTRA }, /* Family Circuit '91 (Japan) (En).nes */
	{ 0x808606f0,  210,       1,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Famista '91 (Japan).nes */
	{ 0x6ec51de5,  210,       2,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Famista '92 (Japan).nes */
	{ 0xadffd64f,  210,       2,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Famista '93 (Japan).nes */
	{ 0x429103c9,  210,       2,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Famista '94 (Japan).nes */
	{ 0x81b7f1a8,  210,       1,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Heisei Tensai Bakabon (Japan).nes */
	{ 0x2447e03b,  210,       2,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Top Striker (Japan).nes */
	{ 0x1dc0f740,  210,       2,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Wagyan Land 2 (Japan).nes */
	{ 0xd323b806,  210,       2,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Wagyan Land 3 (Japan).nes */
	{ 0xbd523011,  210,       2,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Namco Prism Zone - Dream Master (Japan).nes */
	{ 0x5daae69a,  211, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Aladdin - Return of Jaffar, The (Unl) [!].nes */
	{ 0x1ec1dfeb,  217, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 255-in-1 (Mapper 204) [p1].nes */
	{ 0x046d70cc,  217, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ?? 500-in-1 (Anim Splash, Alt Mapper)[p1][!] */
	{ 0x12f86a4d,  217, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 500-in-1.nes */
	{ 0xd09f778d,  217, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ?? 9999999-in-1 (Static Splash, Alt Mapper)[p1][!] */
	{ 0x62ef6c79,  232, DEFAULT, DFAULT8, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Quattro Sports (Camerica) (Aladdin) [b1].nes */
	{ 0x2705eaeb,  234, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Maxi 15 (USA) (Unl).nes */
	{ 0x80cbcacb,  235, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 100-in-1 (Unl).nes */
	{ 0x6175b9a0,  235, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 150_in_1_199x-ASp.nes */
	{ 0x745a6791,  235,       2, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 210-in-1 and Contra 4-in-1 (212-in-1,212 Hong Kong,Reset Based)(Unl).nes */
	{ 0xdf81364d,  235, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 260-in-1 [p1][!].nes */
	{ 0xa38f2f1d,  235, DEFAULT, DEFAULT, 0, DEFAULT,    0x07, DEFAULT, NOEXTRA }, /* 1500-in-1.nes */
	{ 0x6f12afc5,  235, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ?? Golden Game 150-in-1 */
	{ 0x2537b3e6,  241, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Dance Xtreme - Prima (Unl).nes */
	{ 0x11611e89,  241, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Darkseed (Unl) [p1][b1].nes */
	{ 0x81a37827,  241, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Darkseed (Unl) [p1].nes */
	{ 0xfb2b6b10,  241, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Fan Kong Jing Ying (Asia) (Unl).nes */
	{ 0xb5e83c9a,  241, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Xing Ji Zheng Ba (Asia) (Unl).nes */
	{ 0x368c19a8,  241, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* LIKO Study Cartridge 3-in-1 (Unl) [!].nes */
	{ 0x54d98b79,  241, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Titanic 1912 (Unl).nes */
	{ 0xc2730c30,   34, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Deadly Towers (USA).nes */
	{ 0xa21e675c,   34, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mashou (Japan).nes */
	{ 0x6bea1235,  245, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Yong Zhe Dou E Long - Dragon Quest VI (Ch) [a1].nes */
	{ 0x345ee51a,  245, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Yong Zhe Dou E Long - Dragon Quest VII (Ch).nes */
	{ 0x57514c6c,  245, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Yong Zhe Dou E Long - Dragon Quest VI (Ch).nes */
	{ 0xdb9d71b7,  114, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Donkey Kong (Unl) [o1].nes */
	{ 0x61fc4d20,  224, DEFAULT,    MI_H, 1,    0x70,    0x07,   DENDY, NOEXTRA }, /* (KT-1062) 口袋怪兽꞉ 水晶版.nes */
	{ 0x4d735cb1,  224, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pokemon Platinum (KT-008 PCB)(Ch)[!].nes */
	{ 0x4f427110,  224, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pokemon Platinum Alt title 1 (KT-008 PCB)(Ch)[!].nes */
	{ 0xe001de16,  224, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Pokemon Platinum Alt title 2 (KT-008 PCB)(Ch)[!].nes */
	{ 0xbdbe3c96,  238, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Contra Fighter (Unl).nes */
	{ 0xcfe02ada,    1, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT,     PAL, NOEXTRA }, /* Darkman (Europe).nes */
	{ 0xcb53c523,   11, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* King Neptune's Adventure (USA) (Unl).nes */
	{ 0x6e149729,  189, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Master Fighter II (Unl) [a1].nes */
	{ 0x60bfeb0c,   90, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mortal Kombat 2 (Unl) [!].nes */
	{ 0x247cc73d,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Poker II (Asia) (Ja) (Unl).nes */
	{ 0xe41b440f,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Sidewinder (Joy Van).nes */
	{ 0x1f1326d4,  121, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Super Sonic 5 (1997) (Unl) [!].nes */
	{ 0xb0c871c5,    3, DEFAULT,    MI_V, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Wei Lai Xiao Zi (Joy Van).nes */
	{ 0xb3be2f71,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Yanshan Chess (Unl).nes */
	{ 0x8dedea07,    3, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Shui Guo Li (Ch) [a1].nes */
	{ 0x1a71fd06,    1, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Kujaku Ou.nes */
	{ 0x99748230,  215, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* EarthWorm Jim 2 (SuperGame).nes */
	{ 0x37876ac7,  215, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Golden Card 6-in-1 (Unl) [!].nes */
	{ 0x1a3320a3,  215, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mortal Kombat 3 (SuperGame).nes */
	{ 0x80eb1839,  114, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Boogerman (Sugar Softec) (Unl) [!].nes */
	{ 0x071e4ee8,  114, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* ?? m114,submapper 1 test rom */
	{ 0xfe3e03a1,  197, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mortal Kombat III Special (YY-030) (Ch) [!].nes */
	{ 0x9151d311,  197, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mortal Kombat III 28 Peoples (NT-328) (Ch) [!].nes */
	{ 0x272709b9,  237, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Teletubbies Y2K (420-in-1).nes */
	{ 0x2e27e0af,  227, DEFAULT, DEFAULT, 1,    0x70,    0x07,   DENDY, NOEXTRA }, /* Bio Hazard.nes */
	{ 0x0e7e9309,  189, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Street Fighter II - The World Warrior (Unl) [a1].nes */
	{ 0xa3ac0095,  189, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Street Fighter II - The World Warrior (Unl) [a2].nes */
	{ 0xeced5899,  121, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Ultimate Mortal Kombat 4 (Unl) [!].nes */
	{ 0x19c1ed51,  150, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Poker III (Asia) (Ja) (Unl).nes */
	{ 0x282745c5,  141, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Q Boy (Asia) (Ja) (Unl).nes */
	{ 0x4b9ecfb2,   21, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Wai Wai World 2 - SOS!! Paseri Jou (Japan) (Virtual Console).nes */
	{ 0x33751782,    4, DEFAULT, DEFAULT, 1, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Zoda's Revenge - StarTropics II (USA, Europe) (Virtual Console).nes */
	{ 0x9bbf3e5d,   15, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 168-in-1 [p1][!].nes */

	/* TXC / Sachen / JoyVan */

	/* This cart does not work anymore with latest mapper implementation.
	 * This will be run using alternate mapper using override in m132 */
	{ 0x2a5f4c5a,  132, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Zhan Guo Si Chuan Sheng (C&E) (Unl).nes */

	{ 0x0acfc3cd,  132, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mahjong Block (MGC-008) (Unl) [!].nes */

	/* ines mappers that uses iNes 2.0 numbers */

	{ 0xf6bd8e31,  281, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 1997 Super HIK 4-in-1 (JY-052) [p1][!].nes */
	{ 0x5aa23a15,  361, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (OK-411)[p1][!].nes */
	{ 0xf6b9d088,  366, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (K-3131GS, GN-45) [p1][!].nes */
	{ 0x503566b2,  366, DEFAULT,    MI_H, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 4-in-1 (K-3131SS, GN-45) [p1][!].nes */
	{ 0xdb2d2d88,  369, DEFAULT, DEFAULT, 0,    0x07, DEFAULT, DEFAULT, NOEXTRA }, /* Super Mario Bros. Party.nes */
	{ 0x87f83ea2,  380, DEFAULT, DEFAULT, 0, DEFAULT,    0x07, DEFAULT, NOEXTRA }, /* 42 to 80,000 */
	{ 0xc4b94bd5,  389, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Caltron - 9 in 1 (USA) (Proto) (Unl).nes */

	/* ines mappers that uses unif boards */

	{ 0x0073dbd8,  260, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 2-in-1 - Mortal Kombat V Turbo 30 + Super Aladdin (Unl) [p1][!].nes */
	{ 0x4dc6107d,  260, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 2-in-1 - Boogerman + Flintstones, The (Unl) [p1][!].nes */
	{ 0xb72b2cf4,  260, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 2-in-1 - Aladdin + Lion King, The (Unl) [p1][!].nes */
	{ 0x5638ba59,  260, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Mortal Kombat Trilogy - 8 People (M1274) (Ch) [!].nes */
	{ 0xa1dc16c0,  262, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Street Heroes (Asia) (Ja) (Unl).nes */
	{ 0x1df10182,  263, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* Boogerman II (Rex-Soft) [!].nes */
	{ 0xf956fcea,  521, DEFAULT, DEFAULT, 0, DEFAULT,    0x70, DEFAULT, NOEXTRA }, /* Korean Igo (Korea) (Unl).nes */
	{ 0x2eed2e34,  289, DEFAULT, DEFAULT, 0, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }, /* 76-in-1 [p1][a1].nes 2048 PRG, 0 CHR */

	{ 0x00000000, DEFAULT, DEFAULT, DEFAULT, DEFAULT, DEFAULT, DEFAULT, DEFAULT, NOEXTRA }

#endif
