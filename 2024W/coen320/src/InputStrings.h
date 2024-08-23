#ifndef SRC_INPUTSTRINGS_H_
#define SRC_INPUTSTRINGS_H_

// Time ID PX PY PZ VX VY VZ
// Low load is specifically curated to demonstrate various aspects of the system
// Med and high load were randomly generated

constexpr auto LOW_LOAD = "1 0 50000 20000 1000 0 0\n"
		"2 50000 0 20000 0 1000 0\n"
		"3 0 0 15000 250 250 500\n";

constexpr auto MED_LOAD =
    "1 0 0 20000 500 500 0\n" // Heading towards the center from the left
    "2 10000 0 20000 -500 500 0\n" // Heading towards the center from the right
    "3 5000 5000 20000 0 -500 0\n" // Heading downwards towards the center
    "4 -5000 5000 20000 0 -500 0\n" // Heading downwards towards the center from a negative X
    "5 0 -10000 20000 0 500 0\n" // Heading upwards towards the center
    "6 10000 10000 20000 -500 -500 0\n" // Heading towards the center from top-right
    "7 -10000 10000 20000 500 -500 0\n" // Heading towards the center from top-left
    "8 0 10000 20000 0 -500 0\n" // Heading straight down
    "9 5000 -5000 20000 0 500 0\n" // Heading upwards from the bottom-right
    "10 -5000 -5000 20000 0 500 0\n"; // Heading upwards from the bottom-left


constexpr auto HIGH_LOAD = "1 70222 0 16393 900 534 0\n"
		"2 0 43499 19036 868 461 0\n"
		"3 18093 0 15704 982 365 0\n"
		"4 0 84580 15289 492 859 0\n"
		"5 0 29453 19133 445 993 0\n"
		"6 37590 0 18898 802 585 0\n"
		"7 64380 0 16069 641 303 0\n"
		"8 75801 0 20148 779 1196 0\n"
		"9 5190 0 17901 572 330 0\n"
		"10 0 92659 15300 406 278 0\n"
		"11 24558 0 21642 257 329 0\n"
		"12 9925 0 16691 586 986 0\n"
		"13 0 78386 19920 1118 622 0\n"
		"14 89727 0 23645 1143 1039 0\n"
		"15 93776 0 20390 949 821 0\n";

#endif /* SRC_INPUTSTRINGS_H_ */
