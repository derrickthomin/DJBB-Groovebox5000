#pragma once
#include <Arduino.h>
#include <vector>

/*
*************************** Color Definitions **************************

        Display file got big... lets move the color biz here

 It goes:      COLOR_LEVEL  =    Color_brightness (higher > brighter)
               colors[5]    =    {darkest, ... , brightest}

************************************************************************
*/

// BASICS
#define BLACK 0x0000
#define WHITE 0xFFFF

// RED
#define RED_1 0x4824
#define RED_2 0x5824
#define RED_3 0x8847
#define RED_4 0xC06A
#define RED_5 0xF06C
#define RED_NEOPIX_1 0x290b0d
#define RED_NEOPIX_2 0x5e191d
#define RED_NEOPIX_3 0x96262d
#define RED_NEOPIX_4 0xcf343e
#define RED_NEOPIX_5 0x0FF404C  
const std::vector<uint16_t> reds {RED_1, RED_2, RED_3, RED_4, RED_5};
const std::vector<uint32_t> reds_neopix {RED_NEOPIX_1, RED_NEOPIX_2, RED_NEOPIX_3, RED_NEOPIX_4, RED_NEOPIX_5};

// BLUE
#define BLUE_1 0x0124
#define BLUE_2 0x0208
#define BLUE_3 0x032C
#define BLUE_4 0x04D3
#define BLUE_5 0x0679
#define BLUE_NEOPIX_1 0x013333
#define BLUE_NEOPIX_2 0x025c5c
#define BLUE_NEOPIX_3 0x019191
#define BLUE_NEOPIX_4 0x00CCCC
#define BLUE_NEOPIX_5 0x002fafa
const std::vector<uint16_t> blues{BLUE_1, BLUE_2, BLUE_3, BLUE_4, BLUE_5};
const std::vector<uint32_t> blues_neopix {BLUE_NEOPIX_1, BLUE_NEOPIX_2, BLUE_NEOPIX_3, BLUE_NEOPIX_4, BLUE_NEOPIX_5};

// GREEN
#define GREEN_1 0x09E1
#define GREEN_2 0x1BC3
#define GREEN_3 0x3666
#define GREEN_4 0x5EAB
#define GREEN_5 0x9F33
#define GREEN_NEOPIX_1 0x002412
#define GREEN_NEOPIX_2 0x01572c
#define GREEN_NEOPIX_3 0x018a46
#define GREEN_NEOPIX_4 0x02bf61
#define GREEN_NEOPIX_5 0x00FF7F
const std::vector<uint16_t> greens {GREEN_1, GREEN_2, GREEN_3, GREEN_4, GREEN_5};
const std::vector<uint32_t> greens_neopix {GREEN_NEOPIX_1, GREEN_NEOPIX_2, GREEN_NEOPIX_3, GREEN_NEOPIX_4, GREEN_NEOPIX_5};

// PINK
#define PINK_1 0x2042
#define PINK_2 0x4885
#define PINK_3 0x80E8
#define PINK_4 0xB12B
#define PINK_5 0xF9D1
#define PINK_NEOPIX_1 0x2e2430
#define PINK_NEOPIX_2 0x5d4861
#define PINK_NEOPIX_3 0xb98fc2
#define PINK_NEOPIX_4 0xb98fc2
#define PINK_NEOPIX_5 0xF4BBFF
const std::vector<uint16_t> pinks {PINK_1, PINK_2, PINK_3, PINK_4, PINK_5};
const std::vector<uint32_t> pinks_neopix {PINK_NEOPIX_1, PINK_NEOPIX_2, PINK_NEOPIX_3, PINK_NEOPIX_4, PINK_NEOPIX_5};

// YELLOW
#define YELLOW_1 0x3141
#define YELLOW_2 0x6282
#define YELLOW_3 0x8B83
#define YELLOW_4 0xBCC4
#define YELLOW_5 0xF606
#define YELLOW_NEOPIX_1 0x292108
#define YELLOW_NEOPIX_2 0x594712
#define YELLOW_NEOPIX_3 0x8c701c
#define YELLOW_NEOPIX_4 0xbf9926
#define YELLOW_NEOPIX_5 0xF4C430
const std::vector<uint16_t> yellows {YELLOW_1, YELLOW_2, YELLOW_3, YELLOW_4, YELLOW_5};
const std::vector<uint32_t> yellows_neopix {YELLOW_NEOPIX_1, YELLOW_NEOPIX_2, YELLOW_NEOPIX_3, YELLOW_NEOPIX_4, YELLOW_NEOPIX_5};

// ORANGE
#define ORANGE_1 0x3081
#define ORANGE_2 0x6122
#define ORANGE_3 0x99C4
#define ORANGE_4 0xCA65
#define ORANGE_5 0xFAE6
#define ORANGE_NEOPIX_1 0x36140b
#define ORANGE_NEOPIX_2 0x4a1b10
#define ORANGE_NEOPIX_3 0x82311d
#define ORANGE_NEOPIX_4 0xc44a2b
#define ORANGE_NEOPIX_5 0xFF6037
const std::vector<uint16_t> oranges {ORANGE_1, ORANGE_2, ORANGE_3, ORANGE_4, ORANGE_5};
const std::vector<uint32_t> oranges_neopix {ORANGE_NEOPIX_1, ORANGE_NEOPIX_2, ORANGE_NEOPIX_3, ORANGE_NEOPIX_4, ORANGE_NEOPIX_5};

// PURPLE
#define PURPLE_1 0x2006
#define PURPLE_2 0x480C
#define PURPLE_3 0x7013
#define PURPLE_4 0x9819
#define PURPLE_5 0xB81F
#define PURPLE_NEOPIX_1 0x024011d
#define PURPLE_NEOPIX_2 0x590248
#define PURPLE_NEOPIX_3 0x940377
#define PURPLE_NEOPIX_4 0xc702a0
#define PURPLE_NEOPIX_5 0xFF00CC
const std::vector<uint16_t> purples {PURPLE_1, PURPLE_2, PURPLE_3, PURPLE_4, PURPLE_5};
const std::vector<uint32_t> purples_neopix {PURPLE_NEOPIX_1, PURPLE_NEOPIX_2, PURPLE_NEOPIX_3, PURPLE_NEOPIX_4, PURPLE_NEOPIX_5};

//const std::vector<std::vector<uint32_t>> all_colors_neopix;