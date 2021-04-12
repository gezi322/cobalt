// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Flags: --allow-natives-syntax

import * as m1 from "modules-skip-large1.mjs";
import * as m2 from "modules-skip-large2.mjs";

assertFalse(%HasFastProperties(m1));
assertFalse(%HasFastProperties(m2));
assertFalse(%HaveSameMap(m1, m2));

function verify(m) {
  assertEquals(m.a0, 0);
  assertEquals(m.a1, 1);
  assertEquals(m.a2, 2);
  assertEquals(m.a3, 3);
  assertEquals(m.a4, 4);
  assertEquals(m.a5, 5);
  assertEquals(m.a6, 6);
  assertEquals(m.a7, 7);
  assertEquals(m.a8, 8);
  assertEquals(m.a9, 9);
  assertEquals(m.a10, 10);
  assertEquals(m.a11, 11);
  assertEquals(m.a12, 12);
  assertEquals(m.a13, 13);
  assertEquals(m.a14, 14);
  assertEquals(m.a15, 15);
  assertEquals(m.a16, 16);
  assertEquals(m.a17, 17);
  assertEquals(m.a18, 18);
  assertEquals(m.a19, 19);
  assertEquals(m.a20, 20);
  assertEquals(m.a21, 21);
  assertEquals(m.a22, 22);
  assertEquals(m.a23, 23);
  assertEquals(m.a24, 24);
  assertEquals(m.a25, 25);
  assertEquals(m.a26, 26);
  assertEquals(m.a27, 27);
  assertEquals(m.a28, 28);
  assertEquals(m.a29, 29);
  assertEquals(m.a30, 30);
  assertEquals(m.a31, 31);
  assertEquals(m.a32, 32);
  assertEquals(m.a33, 33);
  assertEquals(m.a34, 34);
  assertEquals(m.a35, 35);
  assertEquals(m.a36, 36);
  assertEquals(m.a37, 37);
  assertEquals(m.a38, 38);
  assertEquals(m.a39, 39);
  assertEquals(m.a40, 40);
  assertEquals(m.a41, 41);
  assertEquals(m.a42, 42);
  assertEquals(m.a43, 43);
  assertEquals(m.a44, 44);
  assertEquals(m.a45, 45);
  assertEquals(m.a46, 46);
  assertEquals(m.a47, 47);
  assertEquals(m.a48, 48);
  assertEquals(m.a49, 49);
  assertEquals(m.a50, 50);
  assertEquals(m.a51, 51);
  assertEquals(m.a52, 52);
  assertEquals(m.a53, 53);
  assertEquals(m.a54, 54);
  assertEquals(m.a55, 55);
  assertEquals(m.a56, 56);
  assertEquals(m.a57, 57);
  assertEquals(m.a58, 58);
  assertEquals(m.a59, 59);
  assertEquals(m.a60, 60);
  assertEquals(m.a61, 61);
  assertEquals(m.a62, 62);
  assertEquals(m.a63, 63);
  assertEquals(m.a64, 64);
  assertEquals(m.a65, 65);
  assertEquals(m.a66, 66);
  assertEquals(m.a67, 67);
  assertEquals(m.a68, 68);
  assertEquals(m.a69, 69);
  assertEquals(m.a70, 70);
  assertEquals(m.a71, 71);
  assertEquals(m.a72, 72);
  assertEquals(m.a73, 73);
  assertEquals(m.a74, 74);
  assertEquals(m.a75, 75);
  assertEquals(m.a76, 76);
  assertEquals(m.a77, 77);
  assertEquals(m.a78, 78);
  assertEquals(m.a79, 79);
  assertEquals(m.a80, 80);
  assertEquals(m.a81, 81);
  assertEquals(m.a82, 82);
  assertEquals(m.a83, 83);
  assertEquals(m.a84, 84);
  assertEquals(m.a85, 85);
  assertEquals(m.a86, 86);
  assertEquals(m.a87, 87);
  assertEquals(m.a88, 88);
  assertEquals(m.a89, 89);
  assertEquals(m.a90, 90);
  assertEquals(m.a91, 91);
  assertEquals(m.a92, 92);
  assertEquals(m.a93, 93);
  assertEquals(m.a94, 94);
  assertEquals(m.a95, 95);
  assertEquals(m.a96, 96);
  assertEquals(m.a97, 97);
  assertEquals(m.a98, 98);
  assertEquals(m.a99, 99);
  assertEquals(m.a100, 100);
  assertEquals(m.a101, 101);
  assertEquals(m.a102, 102);
  assertEquals(m.a103, 103);
  assertEquals(m.a104, 104);
  assertEquals(m.a105, 105);
  assertEquals(m.a106, 106);
  assertEquals(m.a107, 107);
  assertEquals(m.a108, 108);
  assertEquals(m.a109, 109);
  assertEquals(m.a110, 110);
  assertEquals(m.a111, 111);
  assertEquals(m.a112, 112);
  assertEquals(m.a113, 113);
  assertEquals(m.a114, 114);
  assertEquals(m.a115, 115);
  assertEquals(m.a116, 116);
  assertEquals(m.a117, 117);
  assertEquals(m.a118, 118);
  assertEquals(m.a119, 119);
  assertEquals(m.a120, 120);
  assertEquals(m.a121, 121);
  assertEquals(m.a122, 122);
  assertEquals(m.a123, 123);
  assertEquals(m.a124, 124);
  assertEquals(m.a125, 125);
  assertEquals(m.a126, 126);
  assertEquals(m.a127, 127);
  assertEquals(m.a128, 128);
  assertEquals(m.a129, 129);
  assertEquals(m.a130, 130);
  assertEquals(m.a131, 131);
  assertEquals(m.a132, 132);
  assertEquals(m.a133, 133);
  assertEquals(m.a134, 134);
  assertEquals(m.a135, 135);
  assertEquals(m.a136, 136);
  assertEquals(m.a137, 137);
  assertEquals(m.a138, 138);
  assertEquals(m.a139, 139);
  assertEquals(m.a140, 140);
  assertEquals(m.a141, 141);
  assertEquals(m.a142, 142);
  assertEquals(m.a143, 143);
  assertEquals(m.a144, 144);
  assertEquals(m.a145, 145);
  assertEquals(m.a146, 146);
  assertEquals(m.a147, 147);
  assertEquals(m.a148, 148);
  assertEquals(m.a149, 149);
  assertEquals(m.a150, 150);
  assertEquals(m.a151, 151);
  assertEquals(m.a152, 152);
  assertEquals(m.a153, 153);
  assertEquals(m.a154, 154);
  assertEquals(m.a155, 155);
  assertEquals(m.a156, 156);
  assertEquals(m.a157, 157);
  assertEquals(m.a158, 158);
  assertEquals(m.a159, 159);
  assertEquals(m.a160, 160);
  assertEquals(m.a161, 161);
  assertEquals(m.a162, 162);
  assertEquals(m.a163, 163);
  assertEquals(m.a164, 164);
  assertEquals(m.a165, 165);
  assertEquals(m.a166, 166);
  assertEquals(m.a167, 167);
  assertEquals(m.a168, 168);
  assertEquals(m.a169, 169);
  assertEquals(m.a170, 170);
  assertEquals(m.a171, 171);
  assertEquals(m.a172, 172);
  assertEquals(m.a173, 173);
  assertEquals(m.a174, 174);
  assertEquals(m.a175, 175);
  assertEquals(m.a176, 176);
  assertEquals(m.a177, 177);
  assertEquals(m.a178, 178);
  assertEquals(m.a179, 179);
  assertEquals(m.a180, 180);
  assertEquals(m.a181, 181);
  assertEquals(m.a182, 182);
  assertEquals(m.a183, 183);
  assertEquals(m.a184, 184);
  assertEquals(m.a185, 185);
  assertEquals(m.a186, 186);
  assertEquals(m.a187, 187);
  assertEquals(m.a188, 188);
  assertEquals(m.a189, 189);
  assertEquals(m.a190, 190);
  assertEquals(m.a191, 191);
  assertEquals(m.a192, 192);
  assertEquals(m.a193, 193);
  assertEquals(m.a194, 194);
  assertEquals(m.a195, 195);
  assertEquals(m.a196, 196);
  assertEquals(m.a197, 197);
  assertEquals(m.a198, 198);
  assertEquals(m.a199, 199);
  assertEquals(m.a200, 200);
  assertEquals(m.a201, 201);
  assertEquals(m.a202, 202);
  assertEquals(m.a203, 203);
  assertEquals(m.a204, 204);
  assertEquals(m.a205, 205);
  assertEquals(m.a206, 206);
  assertEquals(m.a207, 207);
  assertEquals(m.a208, 208);
  assertEquals(m.a209, 209);
  assertEquals(m.a210, 210);
  assertEquals(m.a211, 211);
  assertEquals(m.a212, 212);
  assertEquals(m.a213, 213);
  assertEquals(m.a214, 214);
  assertEquals(m.a215, 215);
  assertEquals(m.a216, 216);
  assertEquals(m.a217, 217);
  assertEquals(m.a218, 218);
  assertEquals(m.a219, 219);
  assertEquals(m.a220, 220);
  assertEquals(m.a221, 221);
  assertEquals(m.a222, 222);
  assertEquals(m.a223, 223);
  assertEquals(m.a224, 224);
  assertEquals(m.a225, 225);
  assertEquals(m.a226, 226);
  assertEquals(m.a227, 227);
  assertEquals(m.a228, 228);
  assertEquals(m.a229, 229);
  assertEquals(m.a230, 230);
  assertEquals(m.a231, 231);
  assertEquals(m.a232, 232);
  assertEquals(m.a233, 233);
  assertEquals(m.a234, 234);
  assertEquals(m.a235, 235);
  assertEquals(m.a236, 236);
  assertEquals(m.a237, 237);
  assertEquals(m.a238, 238);
  assertEquals(m.a239, 239);
  assertEquals(m.a240, 240);
  assertEquals(m.a241, 241);
  assertEquals(m.a242, 242);
  assertEquals(m.a243, 243);
  assertEquals(m.a244, 244);
  assertEquals(m.a245, 245);
  assertEquals(m.a246, 246);
  assertEquals(m.a247, 247);
  assertEquals(m.a248, 248);
  assertEquals(m.a249, 249);
  assertEquals(m.a250, 250);
  assertEquals(m.a251, 251);
  assertEquals(m.a252, 252);
  assertEquals(m.a253, 253);
  assertEquals(m.a254, 254);
  assertEquals(m.a255, 255);
  assertEquals(m.a256, 256);
  assertEquals(m.a257, 257);
  assertEquals(m.a258, 258);
  assertEquals(m.a259, 259);
  assertEquals(m.a260, 260);
  assertEquals(m.a261, 261);
  assertEquals(m.a262, 262);
  assertEquals(m.a263, 263);
  assertEquals(m.a264, 264);
  assertEquals(m.a265, 265);
  assertEquals(m.a266, 266);
  assertEquals(m.a267, 267);
  assertEquals(m.a268, 268);
  assertEquals(m.a269, 269);
  assertEquals(m.a270, 270);
  assertEquals(m.a271, 271);
  assertEquals(m.a272, 272);
  assertEquals(m.a273, 273);
  assertEquals(m.a274, 274);
  assertEquals(m.a275, 275);
  assertEquals(m.a276, 276);
  assertEquals(m.a277, 277);
  assertEquals(m.a278, 278);
  assertEquals(m.a279, 279);
  assertEquals(m.a280, 280);
  assertEquals(m.a281, 281);
  assertEquals(m.a282, 282);
  assertEquals(m.a283, 283);
  assertEquals(m.a284, 284);
  assertEquals(m.a285, 285);
  assertEquals(m.a286, 286);
  assertEquals(m.a287, 287);
  assertEquals(m.a288, 288);
  assertEquals(m.a289, 289);
  assertEquals(m.a290, 290);
  assertEquals(m.a291, 291);
  assertEquals(m.a292, 292);
  assertEquals(m.a293, 293);
  assertEquals(m.a294, 294);
  assertEquals(m.a295, 295);
  assertEquals(m.a296, 296);
  assertEquals(m.a297, 297);
  assertEquals(m.a298, 298);
  assertEquals(m.a299, 299);
  assertEquals(m.a300, 300);
  assertEquals(m.a301, 301);
  assertEquals(m.a302, 302);
  assertEquals(m.a303, 303);
  assertEquals(m.a304, 304);
  assertEquals(m.a305, 305);
  assertEquals(m.a306, 306);
  assertEquals(m.a307, 307);
  assertEquals(m.a308, 308);
  assertEquals(m.a309, 309);
  assertEquals(m.a310, 310);
  assertEquals(m.a311, 311);
  assertEquals(m.a312, 312);
  assertEquals(m.a313, 313);
  assertEquals(m.a314, 314);
  assertEquals(m.a315, 315);
  assertEquals(m.a316, 316);
  assertEquals(m.a317, 317);
  assertEquals(m.a318, 318);
  assertEquals(m.a319, 319);
  assertEquals(m.a320, 320);
  assertEquals(m.a321, 321);
  assertEquals(m.a322, 322);
  assertEquals(m.a323, 323);
  assertEquals(m.a324, 324);
  assertEquals(m.a325, 325);
  assertEquals(m.a326, 326);
  assertEquals(m.a327, 327);
  assertEquals(m.a328, 328);
  assertEquals(m.a329, 329);
  assertEquals(m.a330, 330);
  assertEquals(m.a331, 331);
  assertEquals(m.a332, 332);
  assertEquals(m.a333, 333);
  assertEquals(m.a334, 334);
  assertEquals(m.a335, 335);
  assertEquals(m.a336, 336);
  assertEquals(m.a337, 337);
  assertEquals(m.a338, 338);
  assertEquals(m.a339, 339);
  assertEquals(m.a340, 340);
  assertEquals(m.a341, 341);
  assertEquals(m.a342, 342);
  assertEquals(m.a343, 343);
  assertEquals(m.a344, 344);
  assertEquals(m.a345, 345);
  assertEquals(m.a346, 346);
  assertEquals(m.a347, 347);
  assertEquals(m.a348, 348);
  assertEquals(m.a349, 349);
  assertEquals(m.a350, 350);
  assertEquals(m.a351, 351);
  assertEquals(m.a352, 352);
  assertEquals(m.a353, 353);
  assertEquals(m.a354, 354);
  assertEquals(m.a355, 355);
  assertEquals(m.a356, 356);
  assertEquals(m.a357, 357);
  assertEquals(m.a358, 358);
  assertEquals(m.a359, 359);
  assertEquals(m.a360, 360);
  assertEquals(m.a361, 361);
  assertEquals(m.a362, 362);
  assertEquals(m.a363, 363);
  assertEquals(m.a364, 364);
  assertEquals(m.a365, 365);
  assertEquals(m.a366, 366);
  assertEquals(m.a367, 367);
  assertEquals(m.a368, 368);
  assertEquals(m.a369, 369);
  assertEquals(m.a370, 370);
  assertEquals(m.a371, 371);
  assertEquals(m.a372, 372);
  assertEquals(m.a373, 373);
  assertEquals(m.a374, 374);
  assertEquals(m.a375, 375);
  assertEquals(m.a376, 376);
  assertEquals(m.a377, 377);
  assertEquals(m.a378, 378);
  assertEquals(m.a379, 379);
  assertEquals(m.a380, 380);
  assertEquals(m.a381, 381);
  assertEquals(m.a382, 382);
  assertEquals(m.a383, 383);
  assertEquals(m.a384, 384);
  assertEquals(m.a385, 385);
  assertEquals(m.a386, 386);
  assertEquals(m.a387, 387);
  assertEquals(m.a388, 388);
  assertEquals(m.a389, 389);
  assertEquals(m.a390, 390);
  assertEquals(m.a391, 391);
  assertEquals(m.a392, 392);
  assertEquals(m.a393, 393);
  assertEquals(m.a394, 394);
  assertEquals(m.a395, 395);
  assertEquals(m.a396, 396);
  assertEquals(m.a397, 397);
  assertEquals(m.a398, 398);
  assertEquals(m.a399, 399);
  assertEquals(m.a400, 400);
  assertEquals(m.a401, 401);
  assertEquals(m.a402, 402);
  assertEquals(m.a403, 403);
  assertEquals(m.a404, 404);
  assertEquals(m.a405, 405);
  assertEquals(m.a406, 406);
  assertEquals(m.a407, 407);
  assertEquals(m.a408, 408);
  assertEquals(m.a409, 409);
  assertEquals(m.a410, 410);
  assertEquals(m.a411, 411);
  assertEquals(m.a412, 412);
  assertEquals(m.a413, 413);
  assertEquals(m.a414, 414);
  assertEquals(m.a415, 415);
  assertEquals(m.a416, 416);
  assertEquals(m.a417, 417);
  assertEquals(m.a418, 418);
  assertEquals(m.a419, 419);
  assertEquals(m.a420, 420);
  assertEquals(m.a421, 421);
  assertEquals(m.a422, 422);
  assertEquals(m.a423, 423);
  assertEquals(m.a424, 424);
  assertEquals(m.a425, 425);
  assertEquals(m.a426, 426);
  assertEquals(m.a427, 427);
  assertEquals(m.a428, 428);
  assertEquals(m.a429, 429);
  assertEquals(m.a430, 430);
  assertEquals(m.a431, 431);
  assertEquals(m.a432, 432);
  assertEquals(m.a433, 433);
  assertEquals(m.a434, 434);
  assertEquals(m.a435, 435);
  assertEquals(m.a436, 436);
  assertEquals(m.a437, 437);
  assertEquals(m.a438, 438);
  assertEquals(m.a439, 439);
  assertEquals(m.a440, 440);
  assertEquals(m.a441, 441);
  assertEquals(m.a442, 442);
  assertEquals(m.a443, 443);
  assertEquals(m.a444, 444);
  assertEquals(m.a445, 445);
  assertEquals(m.a446, 446);
  assertEquals(m.a447, 447);
  assertEquals(m.a448, 448);
  assertEquals(m.a449, 449);
  assertEquals(m.a450, 450);
  assertEquals(m.a451, 451);
  assertEquals(m.a452, 452);
  assertEquals(m.a453, 453);
  assertEquals(m.a454, 454);
  assertEquals(m.a455, 455);
  assertEquals(m.a456, 456);
  assertEquals(m.a457, 457);
  assertEquals(m.a458, 458);
  assertEquals(m.a459, 459);
  assertEquals(m.a460, 460);
  assertEquals(m.a461, 461);
  assertEquals(m.a462, 462);
  assertEquals(m.a463, 463);
  assertEquals(m.a464, 464);
  assertEquals(m.a465, 465);
  assertEquals(m.a466, 466);
  assertEquals(m.a467, 467);
  assertEquals(m.a468, 468);
  assertEquals(m.a469, 469);
  assertEquals(m.a470, 470);
  assertEquals(m.a471, 471);
  assertEquals(m.a472, 472);
  assertEquals(m.a473, 473);
  assertEquals(m.a474, 474);
  assertEquals(m.a475, 475);
  assertEquals(m.a476, 476);
  assertEquals(m.a477, 477);
  assertEquals(m.a478, 478);
  assertEquals(m.a479, 479);
  assertEquals(m.a480, 480);
  assertEquals(m.a481, 481);
  assertEquals(m.a482, 482);
  assertEquals(m.a483, 483);
  assertEquals(m.a484, 484);
  assertEquals(m.a485, 485);
  assertEquals(m.a486, 486);
  assertEquals(m.a487, 487);
  assertEquals(m.a488, 488);
  assertEquals(m.a489, 489);
  assertEquals(m.a490, 490);
  assertEquals(m.a491, 491);
  assertEquals(m.a492, 492);
  assertEquals(m.a493, 493);
  assertEquals(m.a494, 494);
  assertEquals(m.a495, 495);
  assertEquals(m.a496, 496);
  assertEquals(m.a497, 497);
  assertEquals(m.a498, 498);
  assertEquals(m.a499, 499);
  assertEquals(m.a500, 500);
  assertEquals(m.a501, 501);
  assertEquals(m.a502, 502);
  assertEquals(m.a503, 503);
  assertEquals(m.a504, 504);
  assertEquals(m.a505, 505);
  assertEquals(m.a506, 506);
  assertEquals(m.a507, 507);
  assertEquals(m.a508, 508);
  assertEquals(m.a509, 509);
  assertEquals(m.a510, 510);
  assertEquals(m.a511, 511);
  assertEquals(m.a512, 512);
  assertEquals(m.a513, 513);
  assertEquals(m.a514, 514);
  assertEquals(m.a515, 515);
  assertEquals(m.a516, 516);
  assertEquals(m.a517, 517);
  assertEquals(m.a518, 518);
  assertEquals(m.a519, 519);
  assertEquals(m.a520, 520);
  assertEquals(m.a521, 521);
  assertEquals(m.a522, 522);
  assertEquals(m.a523, 523);
  assertEquals(m.a524, 524);
  assertEquals(m.a525, 525);
  assertEquals(m.a526, 526);
  assertEquals(m.a527, 527);
  assertEquals(m.a528, 528);
  assertEquals(m.a529, 529);
  assertEquals(m.a530, 530);
  assertEquals(m.a531, 531);
  assertEquals(m.a532, 532);
  assertEquals(m.a533, 533);
  assertEquals(m.a534, 534);
  assertEquals(m.a535, 535);
  assertEquals(m.a536, 536);
  assertEquals(m.a537, 537);
  assertEquals(m.a538, 538);
  assertEquals(m.a539, 539);
  assertEquals(m.a540, 540);
  assertEquals(m.a541, 541);
  assertEquals(m.a542, 542);
  assertEquals(m.a543, 543);
  assertEquals(m.a544, 544);
  assertEquals(m.a545, 545);
  assertEquals(m.a546, 546);
  assertEquals(m.a547, 547);
  assertEquals(m.a548, 548);
  assertEquals(m.a549, 549);
  assertEquals(m.a550, 550);
  assertEquals(m.a551, 551);
  assertEquals(m.a552, 552);
  assertEquals(m.a553, 553);
  assertEquals(m.a554, 554);
  assertEquals(m.a555, 555);
  assertEquals(m.a556, 556);
  assertEquals(m.a557, 557);
  assertEquals(m.a558, 558);
  assertEquals(m.a559, 559);
  assertEquals(m.a560, 560);
  assertEquals(m.a561, 561);
  assertEquals(m.a562, 562);
  assertEquals(m.a563, 563);
  assertEquals(m.a564, 564);
  assertEquals(m.a565, 565);
  assertEquals(m.a566, 566);
  assertEquals(m.a567, 567);
  assertEquals(m.a568, 568);
  assertEquals(m.a569, 569);
  assertEquals(m.a570, 570);
  assertEquals(m.a571, 571);
  assertEquals(m.a572, 572);
  assertEquals(m.a573, 573);
  assertEquals(m.a574, 574);
  assertEquals(m.a575, 575);
  assertEquals(m.a576, 576);
  assertEquals(m.a577, 577);
  assertEquals(m.a578, 578);
  assertEquals(m.a579, 579);
  assertEquals(m.a580, 580);
  assertEquals(m.a581, 581);
  assertEquals(m.a582, 582);
  assertEquals(m.a583, 583);
  assertEquals(m.a584, 584);
  assertEquals(m.a585, 585);
  assertEquals(m.a586, 586);
  assertEquals(m.a587, 587);
  assertEquals(m.a588, 588);
  assertEquals(m.a589, 589);
  assertEquals(m.a590, 590);
  assertEquals(m.a591, 591);
  assertEquals(m.a592, 592);
  assertEquals(m.a593, 593);
  assertEquals(m.a594, 594);
  assertEquals(m.a595, 595);
  assertEquals(m.a596, 596);
  assertEquals(m.a597, 597);
  assertEquals(m.a598, 598);
  assertEquals(m.a599, 599);
  assertEquals(m.a600, 600);
  assertEquals(m.a601, 601);
  assertEquals(m.a602, 602);
  assertEquals(m.a603, 603);
  assertEquals(m.a604, 604);
  assertEquals(m.a605, 605);
  assertEquals(m.a606, 606);
  assertEquals(m.a607, 607);
  assertEquals(m.a608, 608);
  assertEquals(m.a609, 609);
  assertEquals(m.a610, 610);
  assertEquals(m.a611, 611);
  assertEquals(m.a612, 612);
  assertEquals(m.a613, 613);
  assertEquals(m.a614, 614);
  assertEquals(m.a615, 615);
  assertEquals(m.a616, 616);
  assertEquals(m.a617, 617);
  assertEquals(m.a618, 618);
  assertEquals(m.a619, 619);
  assertEquals(m.a620, 620);
  assertEquals(m.a621, 621);
  assertEquals(m.a622, 622);
  assertEquals(m.a623, 623);
  assertEquals(m.a624, 624);
  assertEquals(m.a625, 625);
  assertEquals(m.a626, 626);
  assertEquals(m.a627, 627);
  assertEquals(m.a628, 628);
  assertEquals(m.a629, 629);
  assertEquals(m.a630, 630);
  assertEquals(m.a631, 631);
  assertEquals(m.a632, 632);
  assertEquals(m.a633, 633);
  assertEquals(m.a634, 634);
  assertEquals(m.a635, 635);
  assertEquals(m.a636, 636);
  assertEquals(m.a637, 637);
  assertEquals(m.a638, 638);
  assertEquals(m.a639, 639);
  assertEquals(m.a640, 640);
  assertEquals(m.a641, 641);
  assertEquals(m.a642, 642);
  assertEquals(m.a643, 643);
  assertEquals(m.a644, 644);
  assertEquals(m.a645, 645);
  assertEquals(m.a646, 646);
  assertEquals(m.a647, 647);
  assertEquals(m.a648, 648);
  assertEquals(m.a649, 649);
  assertEquals(m.a650, 650);
  assertEquals(m.a651, 651);
  assertEquals(m.a652, 652);
  assertEquals(m.a653, 653);
  assertEquals(m.a654, 654);
  assertEquals(m.a655, 655);
  assertEquals(m.a656, 656);
  assertEquals(m.a657, 657);
  assertEquals(m.a658, 658);
  assertEquals(m.a659, 659);
  assertEquals(m.a660, 660);
  assertEquals(m.a661, 661);
  assertEquals(m.a662, 662);
  assertEquals(m.a663, 663);
  assertEquals(m.a664, 664);
  assertEquals(m.a665, 665);
  assertEquals(m.a666, 666);
  assertEquals(m.a667, 667);
  assertEquals(m.a668, 668);
  assertEquals(m.a669, 669);
  assertEquals(m.a670, 670);
  assertEquals(m.a671, 671);
  assertEquals(m.a672, 672);
  assertEquals(m.a673, 673);
  assertEquals(m.a674, 674);
  assertEquals(m.a675, 675);
  assertEquals(m.a676, 676);
  assertEquals(m.a677, 677);
  assertEquals(m.a678, 678);
  assertEquals(m.a679, 679);
  assertEquals(m.a680, 680);
  assertEquals(m.a681, 681);
  assertEquals(m.a682, 682);
  assertEquals(m.a683, 683);
  assertEquals(m.a684, 684);
  assertEquals(m.a685, 685);
  assertEquals(m.a686, 686);
  assertEquals(m.a687, 687);
  assertEquals(m.a688, 688);
  assertEquals(m.a689, 689);
  assertEquals(m.a690, 690);
  assertEquals(m.a691, 691);
  assertEquals(m.a692, 692);
  assertEquals(m.a693, 693);
  assertEquals(m.a694, 694);
  assertEquals(m.a695, 695);
  assertEquals(m.a696, 696);
  assertEquals(m.a697, 697);
  assertEquals(m.a698, 698);
  assertEquals(m.a699, 699);
  assertEquals(m.a700, 700);
  assertEquals(m.a701, 701);
  assertEquals(m.a702, 702);
  assertEquals(m.a703, 703);
  assertEquals(m.a704, 704);
  assertEquals(m.a705, 705);
  assertEquals(m.a706, 706);
  assertEquals(m.a707, 707);
  assertEquals(m.a708, 708);
  assertEquals(m.a709, 709);
  assertEquals(m.a710, 710);
  assertEquals(m.a711, 711);
  assertEquals(m.a712, 712);
  assertEquals(m.a713, 713);
  assertEquals(m.a714, 714);
  assertEquals(m.a715, 715);
  assertEquals(m.a716, 716);
  assertEquals(m.a717, 717);
  assertEquals(m.a718, 718);
  assertEquals(m.a719, 719);
  assertEquals(m.a720, 720);
  assertEquals(m.a721, 721);
  assertEquals(m.a722, 722);
  assertEquals(m.a723, 723);
  assertEquals(m.a724, 724);
  assertEquals(m.a725, 725);
  assertEquals(m.a726, 726);
  assertEquals(m.a727, 727);
  assertEquals(m.a728, 728);
  assertEquals(m.a729, 729);
  assertEquals(m.a730, 730);
  assertEquals(m.a731, 731);
  assertEquals(m.a732, 732);
  assertEquals(m.a733, 733);
  assertEquals(m.a734, 734);
  assertEquals(m.a735, 735);
  assertEquals(m.a736, 736);
  assertEquals(m.a737, 737);
  assertEquals(m.a738, 738);
  assertEquals(m.a739, 739);
  assertEquals(m.a740, 740);
  assertEquals(m.a741, 741);
  assertEquals(m.a742, 742);
  assertEquals(m.a743, 743);
  assertEquals(m.a744, 744);
  assertEquals(m.a745, 745);
  assertEquals(m.a746, 746);
  assertEquals(m.a747, 747);
  assertEquals(m.a748, 748);
  assertEquals(m.a749, 749);
  assertEquals(m.a750, 750);
  assertEquals(m.a751, 751);
  assertEquals(m.a752, 752);
  assertEquals(m.a753, 753);
  assertEquals(m.a754, 754);
  assertEquals(m.a755, 755);
  assertEquals(m.a756, 756);
  assertEquals(m.a757, 757);
  assertEquals(m.a758, 758);
  assertEquals(m.a759, 759);
  assertEquals(m.a760, 760);
  assertEquals(m.a761, 761);
  assertEquals(m.a762, 762);
  assertEquals(m.a763, 763);
  assertEquals(m.a764, 764);
  assertEquals(m.a765, 765);
  assertEquals(m.a766, 766);
  assertEquals(m.a767, 767);
  assertEquals(m.a768, 768);
  assertEquals(m.a769, 769);
  assertEquals(m.a770, 770);
  assertEquals(m.a771, 771);
  assertEquals(m.a772, 772);
  assertEquals(m.a773, 773);
  assertEquals(m.a774, 774);
  assertEquals(m.a775, 775);
  assertEquals(m.a776, 776);
  assertEquals(m.a777, 777);
  assertEquals(m.a778, 778);
  assertEquals(m.a779, 779);
  assertEquals(m.a780, 780);
  assertEquals(m.a781, 781);
  assertEquals(m.a782, 782);
  assertEquals(m.a783, 783);
  assertEquals(m.a784, 784);
  assertEquals(m.a785, 785);
  assertEquals(m.a786, 786);
  assertEquals(m.a787, 787);
  assertEquals(m.a788, 788);
  assertEquals(m.a789, 789);
  assertEquals(m.a790, 790);
  assertEquals(m.a791, 791);
  assertEquals(m.a792, 792);
  assertEquals(m.a793, 793);
  assertEquals(m.a794, 794);
  assertEquals(m.a795, 795);
  assertEquals(m.a796, 796);
  assertEquals(m.a797, 797);
  assertEquals(m.a798, 798);
  assertEquals(m.a799, 799);
  assertEquals(m.a800, 800);
  assertEquals(m.a801, 801);
  assertEquals(m.a802, 802);
  assertEquals(m.a803, 803);
  assertEquals(m.a804, 804);
  assertEquals(m.a805, 805);
  assertEquals(m.a806, 806);
  assertEquals(m.a807, 807);
  assertEquals(m.a808, 808);
  assertEquals(m.a809, 809);
  assertEquals(m.a810, 810);
  assertEquals(m.a811, 811);
  assertEquals(m.a812, 812);
  assertEquals(m.a813, 813);
  assertEquals(m.a814, 814);
  assertEquals(m.a815, 815);
  assertEquals(m.a816, 816);
  assertEquals(m.a817, 817);
  assertEquals(m.a818, 818);
  assertEquals(m.a819, 819);
  assertEquals(m.a820, 820);
  assertEquals(m.a821, 821);
  assertEquals(m.a822, 822);
  assertEquals(m.a823, 823);
  assertEquals(m.a824, 824);
  assertEquals(m.a825, 825);
  assertEquals(m.a826, 826);
  assertEquals(m.a827, 827);
  assertEquals(m.a828, 828);
  assertEquals(m.a829, 829);
  assertEquals(m.a830, 830);
  assertEquals(m.a831, 831);
  assertEquals(m.a832, 832);
  assertEquals(m.a833, 833);
  assertEquals(m.a834, 834);
  assertEquals(m.a835, 835);
  assertEquals(m.a836, 836);
  assertEquals(m.a837, 837);
  assertEquals(m.a838, 838);
  assertEquals(m.a839, 839);
  assertEquals(m.a840, 840);
  assertEquals(m.a841, 841);
  assertEquals(m.a842, 842);
  assertEquals(m.a843, 843);
  assertEquals(m.a844, 844);
  assertEquals(m.a845, 845);
  assertEquals(m.a846, 846);
  assertEquals(m.a847, 847);
  assertEquals(m.a848, 848);
  assertEquals(m.a849, 849);
  assertEquals(m.a850, 850);
  assertEquals(m.a851, 851);
  assertEquals(m.a852, 852);
  assertEquals(m.a853, 853);
  assertEquals(m.a854, 854);
  assertEquals(m.a855, 855);
  assertEquals(m.a856, 856);
  assertEquals(m.a857, 857);
  assertEquals(m.a858, 858);
  assertEquals(m.a859, 859);
  assertEquals(m.a860, 860);
  assertEquals(m.a861, 861);
  assertEquals(m.a862, 862);
  assertEquals(m.a863, 863);
  assertEquals(m.a864, 864);
  assertEquals(m.a865, 865);
  assertEquals(m.a866, 866);
  assertEquals(m.a867, 867);
  assertEquals(m.a868, 868);
  assertEquals(m.a869, 869);
  assertEquals(m.a870, 870);
  assertEquals(m.a871, 871);
  assertEquals(m.a872, 872);
  assertEquals(m.a873, 873);
  assertEquals(m.a874, 874);
  assertEquals(m.a875, 875);
  assertEquals(m.a876, 876);
  assertEquals(m.a877, 877);
  assertEquals(m.a878, 878);
  assertEquals(m.a879, 879);
  assertEquals(m.a880, 880);
  assertEquals(m.a881, 881);
  assertEquals(m.a882, 882);
  assertEquals(m.a883, 883);
  assertEquals(m.a884, 884);
  assertEquals(m.a885, 885);
  assertEquals(m.a886, 886);
  assertEquals(m.a887, 887);
  assertEquals(m.a888, 888);
  assertEquals(m.a889, 889);
  assertEquals(m.a890, 890);
  assertEquals(m.a891, 891);
  assertEquals(m.a892, 892);
  assertEquals(m.a893, 893);
  assertEquals(m.a894, 894);
  assertEquals(m.a895, 895);
  assertEquals(m.a896, 896);
  assertEquals(m.a897, 897);
  assertEquals(m.a898, 898);
  assertEquals(m.a899, 899);
  assertEquals(m.a900, 900);
  assertEquals(m.a901, 901);
  assertEquals(m.a902, 902);
  assertEquals(m.a903, 903);
  assertEquals(m.a904, 904);
  assertEquals(m.a905, 905);
  assertEquals(m.a906, 906);
  assertEquals(m.a907, 907);
  assertEquals(m.a908, 908);
  assertEquals(m.a909, 909);
  assertEquals(m.a910, 910);
  assertEquals(m.a911, 911);
  assertEquals(m.a912, 912);
  assertEquals(m.a913, 913);
  assertEquals(m.a914, 914);
  assertEquals(m.a915, 915);
  assertEquals(m.a916, 916);
  assertEquals(m.a917, 917);
  assertEquals(m.a918, 918);
  assertEquals(m.a919, 919);
  assertEquals(m.a920, 920);
  assertEquals(m.a921, 921);
  assertEquals(m.a922, 922);
  assertEquals(m.a923, 923);
  assertEquals(m.a924, 924);
  assertEquals(m.a925, 925);
  assertEquals(m.a926, 926);
  assertEquals(m.a927, 927);
  assertEquals(m.a928, 928);
  assertEquals(m.a929, 929);
  assertEquals(m.a930, 930);
  assertEquals(m.a931, 931);
  assertEquals(m.a932, 932);
  assertEquals(m.a933, 933);
  assertEquals(m.a934, 934);
  assertEquals(m.a935, 935);
  assertEquals(m.a936, 936);
  assertEquals(m.a937, 937);
  assertEquals(m.a938, 938);
  assertEquals(m.a939, 939);
  assertEquals(m.a940, 940);
  assertEquals(m.a941, 941);
  assertEquals(m.a942, 942);
  assertEquals(m.a943, 943);
  assertEquals(m.a944, 944);
  assertEquals(m.a945, 945);
  assertEquals(m.a946, 946);
  assertEquals(m.a947, 947);
  assertEquals(m.a948, 948);
  assertEquals(m.a949, 949);
  assertEquals(m.a950, 950);
  assertEquals(m.a951, 951);
  assertEquals(m.a952, 952);
  assertEquals(m.a953, 953);
  assertEquals(m.a954, 954);
  assertEquals(m.a955, 955);
  assertEquals(m.a956, 956);
  assertEquals(m.a957, 957);
  assertEquals(m.a958, 958);
  assertEquals(m.a959, 959);
  assertEquals(m.a960, 960);
  assertEquals(m.a961, 961);
  assertEquals(m.a962, 962);
  assertEquals(m.a963, 963);
  assertEquals(m.a964, 964);
  assertEquals(m.a965, 965);
  assertEquals(m.a966, 966);
  assertEquals(m.a967, 967);
  assertEquals(m.a968, 968);
  assertEquals(m.a969, 969);
  assertEquals(m.a970, 970);
  assertEquals(m.a971, 971);
  assertEquals(m.a972, 972);
  assertEquals(m.a973, 973);
  assertEquals(m.a974, 974);
  assertEquals(m.a975, 975);
  assertEquals(m.a976, 976);
  assertEquals(m.a977, 977);
  assertEquals(m.a978, 978);
  assertEquals(m.a979, 979);
  assertEquals(m.a980, 980);
  assertEquals(m.a981, 981);
  assertEquals(m.a982, 982);
  assertEquals(m.a983, 983);
  assertEquals(m.a984, 984);
  assertEquals(m.a985, 985);
  assertEquals(m.a986, 986);
  assertEquals(m.a987, 987);
  assertEquals(m.a988, 988);
  assertEquals(m.a989, 989);
  assertEquals(m.a990, 990);
  assertEquals(m.a991, 991);
  assertEquals(m.a992, 992);
  assertEquals(m.a993, 993);
  assertEquals(m.a994, 994);
  assertEquals(m.a995, 995);
  assertEquals(m.a996, 996);
  assertEquals(m.a997, 997);
  assertEquals(m.a998, 998);
  assertEquals(m.a999, 999);
  assertEquals(m.a1000, 1000);
  assertEquals(m.a1001, 1001);
  assertEquals(m.a1002, 1002);
  assertEquals(m.a1003, 1003);
  assertEquals(m.a1004, 1004);
  assertEquals(m.a1005, 1005);
  assertEquals(m.a1006, 1006);
  assertEquals(m.a1007, 1007);
  assertEquals(m.a1008, 1008);
  assertEquals(m.a1009, 1009);
  assertEquals(m.a1010, 1010);
  assertEquals(m.a1011, 1011);
  assertEquals(m.a1012, 1012);
  assertEquals(m.a1013, 1013);
  assertEquals(m.a1014, 1014);
  assertEquals(m.a1015, 1015);
  assertEquals(m.a1016, 1016);
  assertEquals(m.a1017, 1017);
  assertEquals(m.a1018, 1018);
  assertEquals(m.a1019, 1019);
  assertEquals(m.a1020, 1020);
  assertEquals(m.a1021, 1021);
  assertEquals(m.a1022, 1022);
  assertEquals(m.a1023, 1023);
  assertEquals(m.a1024, 1024);
  assertEquals(m.a1025, 1025);
  assertEquals(m.a1026, 1026);
  assertEquals(m.a1027, 1027);
  assertEquals(m.a1028, 1028);
  assertEquals(m.a1029, 1029);
  assertEquals(m.a1030, 1030);
  assertEquals(m.a1031, 1031);
  assertEquals(m.a1032, 1032);
  assertEquals(m.a1033, 1033);
  assertEquals(m.a1034, 1034);
  assertEquals(m.a1035, 1035);
  assertEquals(m.a1036, 1036);
  assertEquals(m.a1037, 1037);
  assertEquals(m.a1038, 1038);
  assertEquals(m.a1039, 1039);
  assertEquals(m.a1040, 1040);
  assertEquals(m.a1041, 1041);
  assertEquals(m.a1042, 1042);
  assertEquals(m.a1043, 1043);
  assertEquals(m.a1044, 1044);
  assertEquals(m.a1045, 1045);
  assertEquals(m.a1046, 1046);
  assertEquals(m.a1047, 1047);
  assertEquals(m.a1048, 1048);
  assertEquals(m.a1049, 1049);
  assertEquals(m.a1050, 1050);
  assertEquals(m.a1051, 1051);
  assertEquals(m.a1052, 1052);
  assertEquals(m.a1053, 1053);
  assertEquals(m.a1054, 1054);
  assertEquals(m.a1055, 1055);
  assertEquals(m.a1056, 1056);
  assertEquals(m.a1057, 1057);
  assertEquals(m.a1058, 1058);
  assertEquals(m.a1059, 1059);
  assertEquals(m.a1060, 1060);
  assertEquals(m.a1061, 1061);
  assertEquals(m.a1062, 1062);
  assertEquals(m.a1063, 1063);
  assertEquals(m.a1064, 1064);
  assertEquals(m.a1065, 1065);
  assertEquals(m.a1066, 1066);
  assertEquals(m.a1067, 1067);
  assertEquals(m.a1068, 1068);
  assertEquals(m.a1069, 1069);
  assertEquals(m.a1070, 1070);
  assertEquals(m.a1071, 1071);
  assertEquals(m.a1072, 1072);
  assertEquals(m.a1073, 1073);
  assertEquals(m.a1074, 1074);
  assertEquals(m.a1075, 1075);
  assertEquals(m.a1076, 1076);
  assertEquals(m.a1077, 1077);
  assertEquals(m.a1078, 1078);
  assertEquals(m.a1079, 1079);
  assertEquals(m.a1080, 1080);
  assertEquals(m.a1081, 1081);
  assertEquals(m.a1082, 1082);
  assertEquals(m.a1083, 1083);
  assertEquals(m.a1084, 1084);
  assertEquals(m.a1085, 1085);
  assertEquals(m.a1086, 1086);
  assertEquals(m.a1087, 1087);
  assertEquals(m.a1088, 1088);
  assertEquals(m.a1089, 1089);
  assertEquals(m.a1090, 1090);
  assertEquals(m.a1091, 1091);
  assertEquals(m.a1092, 1092);
  assertEquals(m.a1093, 1093);
  assertEquals(m.a1094, 1094);
  assertEquals(m.a1095, 1095);
  assertEquals(m.a1096, 1096);
  assertEquals(m.a1097, 1097);
  assertEquals(m.a1098, 1098);
  assertEquals(m.a1099, 1099);
}
verify(m1);  // Uninitialized.
verify(m1);  // Premonomorphic.
verify(m2);  // Monomorphic.
