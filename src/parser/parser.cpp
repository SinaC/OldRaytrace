#include "parser_skel.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int initialState = 170;
int map_token(int tok) {
  switch (tok) {case E_O_F: return 1;
case RAY_INCLUDE: return 2;
case RAY_DECLARE: return 3;
case '=': return 4;
case ';': return 5;
case RAY_WORLD: return 6;
case '{': return 7;
case '}': return 8;
case RAY_SETTINGS: return 9;
case RAY_WHILE: return 10;
case '(': return 11;
case ')': return 12;
case RAY_IF: return 13;
case RAY_ELSE: return 14;
case RAY_VAR: return 15;
case '.': return 16;
case RAY_FUNCTION: return 17;
case ':': return 18;
case RAY_IDENT: return 19;
case RAY_SUPER: return 20;
case '[': return 21;
case ']': return 22;
case RAY_VECTOR2: return 23;
case '<': return 24;
case ',': return 25;
case '>': return 26;
case RAY_VECTOR3: return 27;
case RAY_VECTOR4: return 28;
case RAY_COLOR: return 29;
case '!': return 30;
case '-': return 31;
case '#': return 32;
case '|': return 33;
case '^': return 34;
case '*': return 35;
case '/': return 36;
case '+': return 37;
case RAY_LE: return 38;
case RAY_GE: return 39;
case RAY_EQ: return 40;
case RAY_NE: return 41;
case RAY_AND: return 42;
case RAY_OR: return 43;
case RAY_BOOLEAN: return 44;
case RAY_STRING: return 45;
case RAY_INTEGER: return 46;
case RAY_FLOAT: return 47;
default : 
  printf("maptoken: got unknown token: %d '%c'!\
", tok, tok);
  return 0;
}}
const int actionrow0[] = {-68, -68, -68, 0, 0, -68, 0, 0, -68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow1[] = {-4, -4, -4, 0, 0, -4, 0, 0, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
const int actionrow3[] = {0, 0, 0, -66, -66, 0, -66, 0, 0, 0, -66, -66, 0, 0, 0, -66, 0, 0, 0, 0, -66, -66, 0, -66, -66, -66, 0, 0, 0, 0, -66, -66, -66, -66, -66, -66, -66, -66, -66, -66, -66, -66, -66, 0, 0, 0, 0};
const int actionrow4[] = {0, 0, 0, -67, -67, 0, -67, 0, 0, 0, -67, -67, 0, 0, 0, -67, 0, 0, 0, 0, -67, -67, 0, -67, -67, -67, 0, 0, 0, 0, -67, -67, -67, -67, -67, -67, -67, -67, -67, -67, -67, -67, -67, 0, 0, 0, 0};
const int actionrow5[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow6[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow7[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow8[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow9[] = {0, 0, 0, -28, -28, 0, -66, 0, 0, 0, -66, -28, 0, 0, 0, -28, 0, -66, 0, 0, -28, -28, 0, -28, -28, -28, 0, 0, 0, 0, -28, -28, -28, -28, -28, -28, -28, -28, -28, -28, -28, -28, -28, 0, 0, 0, 0};
const int actionrow10[] = {0, 0, 0, -29, -29, 0, -67, 0, 0, 0, -67, -29, 0, 0, 0, -29, 0, -67, 0, 0, -29, -29, 0, -29, -29, -29, 0, 0, 0, 0, -29, -29, -29, -29, -29, -29, -29, -29, -29, -29, -29, -29, -29, 0, 0, 0, 0};
const int actionrow11[] = {0, 0, 0, -25, -25, 0, 0, 0, 0, 0, 0, -25, 0, 0, 0, -25, 0, 0, 0, 0, -25, -25, 0, -25, -25, -25, 0, 0, 0, 0, -25, -25, -25, -25, -25, -25, -25, -25, -25, -25, -25, -25, -25, 0, 0, 0, 0};
const int actionrow12[] = {0, 0, 0, -54, -54, 0, 0, 0, 0, 0, 0, -54, 0, 0, 0, -54, 0, 0, 0, 0, -54, -54, 0, -54, -54, -54, 0, 0, 0, 0, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, 0, 0, 0, 0};
const int actionrow13[] = {0, 0, 0, -55, -55, 0, 0, 0, 0, 0, 0, -55, 0, 0, 0, -55, 0, 0, 0, 0, -55, -55, 0, -55, -55, -55, 0, 0, 0, 0, -55, -55, -55, -55, -55, -55, -55, -55, -55, -55, -55, -55, -55, 0, 0, 0, 0};
const int actionrow14[] = {0, 0, 0, -56, -56, 0, 0, 0, 0, 0, 0, -56, 0, 0, 0, -56, 0, 0, 0, 0, -56, -56, 0, -56, -56, -56, 0, 0, 0, 0, -56, -56, -56, -56, -56, -56, -56, -56, -56, -56, -56, -56, -56, 0, 0, 0, 0};
const int actionrow15[] = {0, 0, 0, -57, -57, 0, 0, 0, 0, 0, 0, -57, 0, 0, 0, -57, 0, 0, 0, 0, -57, -57, 0, -57, -57, -57, 0, 0, 0, 0, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, 0, 0, 0, 0};
const int actionrow16[] = {0, 0, 0, -37, -37, 0, 0, 0, 0, 0, 0, -37, 0, 0, 0, -37, 0, 0, 0, 0, -37, -37, 0, -37, -37, -37, 0, 0, 0, 0, -37, -37, -37, -37, -37, -37, -37, -37, -37, -37, -37, -37, -37, 0, 0, 0, 0};
const int actionrow17[] = {0, 0, 0, -53, -53, 0, 0, 0, 0, 0, 0, -53, 0, 0, 0, 60, 0, 0, 0, 0, 51, -53, 0, 35, -53, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow18[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 152, 0, 0, 0, 0, 0, 149, 0, 10, 11, 101, 0, 96, 0, 0, 0, 90, 82, 72, 64, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 15, 16};
const int actionrow19[] = {0, 0, 0, -52, -52, 0, 0, 0, 0, 0, 0, -52, 0, 0, 0, 60, 0, 0, 0, 0, 51, -52, 0, 35, -52, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, -52, 0, 0, 0, 0};
const int actionrow20[] = {0, 0, 0, -51, -51, 0, 0, 0, 0, 0, 0, -51, 0, 0, 0, 60, 0, 0, 0, 0, 51, -51, 0, 35, -51, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, -51, -51, 0, 0, 0, 0};
const int actionrow21[] = {0, 0, 0, -50, -50, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0, 60, 0, 0, 0, 0, 51, -50, 0, -50, -50, -50, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, -50, -50, -50, -50, -50, -50, 0, 0, 0, 0};
const int actionrow22[] = {0, 0, 0, -45, -45, 0, 0, 0, 0, 0, 0, -45, 0, 0, 0, 60, 0, 0, 0, 0, 51, -45, 0, -45, -45, -45, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, -45, -45, -45, -45, -45, -45, 0, 0, 0, 0};
const int actionrow23[] = {0, 0, 0, -47, -47, 0, 0, 0, 0, 0, 0, -47, 0, 0, 0, 60, 0, 0, 0, 0, 51, -47, 0, -47, -47, -47, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, -47, -47, -47, -47, -47, -47, 0, 0, 0, 0};
const int actionrow24[] = {0, 0, 0, -49, -49, 0, 0, 0, 0, 0, 0, -49, 0, 0, 0, 60, 0, 0, 0, 0, 51, -49, 0, -49, -49, -49, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, -49, -49, -49, -49, -49, -49, 0, 0, 0, 0};
const int actionrow25[] = {0, 0, 0, -48, -48, 0, 0, 0, 0, 0, 0, -48, 0, 0, 0, 60, 0, 0, 0, 0, 51, -48, 0, -48, -48, -48, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, -48, -48, -48, -48, -48, -48, 0, 0, 0, 0};
const int actionrow26[] = {0, 0, 0, -46, -46, 0, 0, 0, 0, 0, 0, -46, 0, 0, 0, 60, 0, 0, 0, 0, 51, -46, 0, -46, -46, -46, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, -46, -46, -46, -46, -46, -46, 0, 0, 0, 0};
const int actionrow27[] = {0, 0, 0, -44, -44, 0, 0, 0, 0, 0, 0, -44, 0, 0, 0, 60, 0, 0, 0, 0, 51, -44, 0, -44, -44, -44, 0, 0, 0, 0, -44, 49, 47, 45, 43, 41, -44, -44, -44, -44, -44, -44, -44, 0, 0, 0, 0};
const int actionrow28[] = {0, 0, 0, -43, -43, 0, 0, 0, 0, 0, 0, -43, 0, 0, 0, 60, 0, 0, 0, 0, 51, -43, 0, -43, -43, -43, 0, 0, 0, 0, -43, 49, 47, 45, 43, 41, -43, -43, -43, -43, -43, -43, -43, 0, 0, 0, 0};
const int actionrow29[] = {0, 0, 0, -42, -42, 0, 0, 0, 0, 0, 0, -42, 0, 0, 0, 60, 0, 0, 0, 0, 51, -42, 0, -42, -42, -42, 0, 0, 0, 0, -42, 49, 47, 45, -42, -42, -42, -42, -42, -42, -42, -42, -42, 0, 0, 0, 0};
const int actionrow30[] = {0, 0, 0, -41, -41, 0, 0, 0, 0, 0, 0, -41, 0, 0, 0, 60, 0, 0, 0, 0, 51, -41, 0, -41, -41, -41, 0, 0, 0, 0, -41, 49, 47, 45, -41, -41, -41, -41, -41, -41, -41, -41, -41, 0, 0, 0, 0};
const int actionrow31[] = {0, 0, 0, -40, -40, 0, 0, 0, 0, 0, 0, -40, 0, 0, 0, 60, 0, 0, 0, 0, 51, -40, 0, -40, -40, -40, 0, 0, 0, 0, -40, 49, 47, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, 0, 0, 0, 0};
const int actionrow32[] = {0, 0, 0, -39, -39, 0, 0, 0, 0, 0, 0, -39, 0, 0, 0, 60, 0, 0, 0, 0, 51, -39, 0, -39, -39, -39, 0, 0, 0, 0, -39, -39, -39, -39, -39, -39, -39, -39, -39, -39, -39, -39, -39, 0, 0, 0, 0};
const int actionrow33[] = {0, 0, 0, -38, -38, 0, 0, 0, 0, 0, 0, -38, 0, 0, 0, 60, 0, 0, 0, 0, 51, -38, 0, -38, -38, -38, 0, 0, 0, 0, -38, -38, -38, -38, -38, -38, -38, -38, -38, -38, -38, -38, -38, 0, 0, 0, 0};
const int actionrow34[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 17, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow35[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, -60, 0, 0, 0, 60, 0, 0, 0, 0, 51, -60, 0, 35, -60, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow36[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, -61, 0, 0, 0, 60, 0, 0, 0, 0, 51, -61, 0, 35, -61, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow37[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -59, 0, 0, 0, 0, 0, 0, 0, 0, 0, -59, 0, 0, 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow38[] = {0, 0, 0, -22, -22, 0, 0, 0, 0, 0, 0, -22, 0, 0, 0, -22, 0, 0, 0, 0, -22, -22, 0, -22, -22, -22, 0, 0, 0, 0, -22, -22, -22, -22, -22, -22, -22, -22, -22, -22, -22, -22, -22, 0, 0, 0, 0};
const int actionrow39[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow40[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 152, -58, 0, 0, 0, 0, 149, 0, 10, 11, 101, 0, 96, 0, 0, 0, 90, 82, 72, 64, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 15, 16};
const int actionrow41[] = {0, 0, 0, -27, -27, 0, 0, 0, 0, 0, 58, -27, 0, 0, 0, -27, 0, 0, 0, 0, -27, -27, 0, -27, -27, -27, 0, 0, 0, 0, -27, -27, -27, -27, -27, -27, -27, -27, -27, -27, -27, -27, -27, 0, 0, 0, 0};
const int actionrow42[] = {0, 0, 0, -36, -36, 0, 0, 0, 0, 0, 0, -36, 0, 0, 0, 60, 0, 0, 0, 0, 51, -36, 0, -36, -36, -36, 0, 0, 0, 0, -36, 49, 47, -36, -36, -36, -36, -36, -36, -36, -36, -36, -36, 0, 0, 0, 0};
const int actionrow43[] = {0, 0, 0, -35, -35, 0, 0, 0, 0, 0, 0, -35, 0, 0, 0, 60, 0, 0, 0, 0, 51, -35, 0, -35, -35, -35, 0, 0, 0, 0, -35, 49, 47, -35, -35, -35, -35, -35, -35, -35, -35, -35, -35, 0, 0, 0, 0};
const int actionrow44[] = {0, 0, 0, -34, -34, 0, 0, 0, 0, 0, 152, -34, 0, 0, 0, -34, 149, 0, 10, 11, 0, -34, 96, -34, -34, -34, 90, 82, 72, 64, 62, -34, -34, -34, -34, -34, -34, -34, -34, -34, -34, -34, -34, 13, 14, 15, 16};
const int actionrow45[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 65, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow46[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 67, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow47[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 69, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow48[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow49[] = {0, 0, 0, -33, -33, 0, 0, 0, 0, 0, 152, -33, 0, 0, 0, -33, 149, 0, 10, 11, 0, -33, 96, -33, -33, -33, 90, 82, 72, 64, 62, -33, -33, -33, -33, -33, -33, -33, -33, -33, -33, -33, -33, 13, 14, 15, 16};
const int actionrow50[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 73, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow51[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 75, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow52[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 77, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow53[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 79, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow54[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow55[] = {0, 0, 0, -32, -32, 0, 0, 0, 0, 0, 152, -32, 0, 0, 0, -32, 149, 0, 10, 11, 0, -32, 96, -32, -32, -32, 90, 82, 72, 64, 62, -32, -32, -32, -32, -32, -32, -32, -32, -32, -32, -32, -32, 13, 14, 15, 16};
const int actionrow56[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 83, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow57[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 85, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow58[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 87, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow59[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow60[] = {0, 0, 0, -31, -31, 0, 0, 0, 0, 0, 152, -31, 0, 0, 0, -31, 149, 0, 10, 11, 0, -31, 96, -31, -31, -31, 90, 82, 72, 64, 62, -31, -31, -31, -31, -31, -31, -31, -31, -31, -31, -31, -31, 13, 14, 15, 16};
const int actionrow61[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 91, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow62[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 93, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow63[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 95, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow64[] = {0, 0, 0, -30, -30, 0, 0, 0, 0, 0, 0, -30, 0, 0, 0, -30, 0, 0, 0, 0, -30, -30, 0, -30, -30, -30, 0, 0, 0, 0, -30, -30, -30, -30, -30, -30, -30, -30, -30, -30, -30, -30, -30, 0, 0, 0, 0};
const int actionrow65[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow66[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow67[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 152, 0, 0, 0, 0, 0, 149, 0, 10, 11, 101, -58, 96, 0, 0, 0, 90, 82, 72, 64, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 15, 16};
const int actionrow68[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow69[] = {0, 0, 0, 0, 0, 0, 0, -10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10, -10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow70[] = {0, 0, 0, 19, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow71[] = {0, 0, 0, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow72[] = {0, 0, 0, 0, 0, 0, 0, -9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -9, -9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow73[] = {0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow74[] = {0, 0, 0, 0, 0, 0, 0, -8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -8, -8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow75[] = {0, 0, 0, 0, 0, 0, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow76[] = {0, 0, 0, -24, -24, 0, 0, 0, 0, 0, 0, -24, 0, 0, 0, -24, 0, 0, 0, 0, -24, -24, 0, -24, -24, -24, 0, 0, 0, 0, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, 0, 0, 0, 0};
const int actionrow77[] = {0, 0, 0, 0, 0, 0, 0, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow78[] = {0, 0, 0, -26, -26, 0, 0, 0, 0, 0, 0, -26, 0, 0, 0, -26, 0, 0, 0, 0, -26, -26, 0, -26, -26, -26, 0, 0, 0, 0, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, 0, 0, 0, 0};
const int actionrow79[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow80[] = {0, 0, 0, 0, 0, 0, 113, 0, 0, 0, 116, 0, 0, 0, 0, 0, 0, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow81[] = {0, 0, 0, 0, 0, 0, -12, -12, 0, -12, -12, 0, -12, -12, -12, 0, -12, 0, -12, -12, -12, 0, -12, 0, 0, 0, -12, -12, -12, -12, -12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -12, -12, -12, -12};
const int actionrow82[] = {0, 0, 0, 19, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow83[] = {0, 0, 0, 0, 0, 0, -16, -16, 0, -16, -16, 0, -16, -16, -16, 0, -16, 0, -16, -16, -16, 0, -16, 0, 0, 0, -16, -16, -16, -16, -16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -16, -16, -16, -16};
const int actionrow84[] = {0, 0, 0, 0, 0, 0, -17, -17, 0, -17, -17, 0, -17, -17, -17, 0, -17, 0, -17, -17, -17, 0, -17, 0, 0, 0, -17, -17, -17, -17, -17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -17, -17, -17, -17};
const int actionrow85[] = {0, 0, 0, 19, 121, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow86[] = {0, 0, 0, 123, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow87[] = {0, 0, 0, 0, 0, 0, -15, -15, 0, -15, -15, 0, -15, -15, -15, 0, -15, 0, -15, -15, -15, 0, -15, 0, 0, 0, -15, -15, -15, -15, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, -15, -15, -15};
const int actionrow88[] = {0, 0, 0, 0, 0, 0, 141, 0, 0, 137, 152, 0, 132, 0, 125, 0, 149, 0, 10, 11, 101, 0, 96, 0, 0, 0, 90, 82, 72, 64, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 15, 16};
const int actionrow89[] = {0, 0, 0, 0, 0, 0, -14, -14, 0, -14, -14, 0, -14, 127, -14, 0, -14, 0, -14, -14, -14, 0, -14, 0, 0, 0, -14, -14, -14, -14, -14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -14, -14, -14, -14};
const int actionrow90[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow91[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 131, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow92[] = {0, 0, 0, 0, 0, 0, -13, -13, 0, -13, -13, 0, -13, -13, -13, 0, -13, 0, -13, -13, -13, 0, -13, 0, 0, 0, -13, -13, -13, -13, -13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -13, -13, -13, -13};
const int actionrow93[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 134, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow94[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow95[] = {0, 0, 0, 0, 0, 0, -19, -19, 0, -19, -19, 0, -19, 0, -19, 0, -19, 0, -19, -19, -19, 0, -19, 0, 0, 0, -19, -19, -19, -19, -19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -19, -19, -19, -19};
const int actionrow96[] = {0, 0, 0, 0, 0, 0, -11, -11, 0, -11, -11, 0, -11, -11, -11, 0, -11, 0, -11, -11, -11, 0, -11, 0, 0, 0, -11, -11, -11, -11, -11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -11, -11, -11, -11};
const int actionrow97[] = {0, 0, 0, 0, 0, 0, 141, 139, 0, 137, 152, 0, 132, 0, 125, 0, 149, 0, 10, 11, 101, 0, 96, 0, 0, 0, 90, 82, 72, 64, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 15, 16};
const int actionrow98[] = {0, 0, 0, 0, 0, 0, -18, -18, 0, -18, -18, 0, -18, 0, -18, 0, -18, 0, -18, -18, -18, 0, -18, 0, 0, 0, -18, -18, -18, -18, -18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -18, -18, -18, -18};
const int actionrow99[] = {0, 0, 0, -20, -20, 0, 0, 0, 0, 0, 0, -20, 0, 0, 0, -20, 0, 0, 0, 0, -20, -20, 0, -20, -20, -20, 0, 0, 0, 0, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, 0, 0, 0, 0};
const int actionrow100[] = {0, 0, 0, 0, 0, 0, 141, 142, 0, 137, 152, 0, 132, 0, 125, 0, 149, 0, 10, 11, 101, 0, 96, 0, 0, 0, 90, 82, 72, 64, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 15, 16};
const int actionrow101[] = {0, 0, 0, -23, -23, 0, 0, 0, 0, 0, 0, -23, 0, 0, 0, -23, 0, 0, 0, 0, -23, -23, 0, -23, -23, -23, 0, 0, 0, 0, -23, -23, -23, -23, -23, -23, -23, -23, -23, -23, -23, -23, -23, 0, 0, 0, 0};
const int actionrow102[] = {0, 0, 0, 0, 0, 0, 144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow103[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow104[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -62, 0, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow105[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow106[] = {0, 0, 0, -21, -21, 0, 0, 0, 0, 0, 0, -21, 0, 0, 0, -21, 0, 0, 0, 0, -21, -21, 0, -21, -21, -21, 0, 0, 0, 0, -21, -21, -21, -21, -21, -21, -21, -21, -21, -21, -21, -21, -21, 0, 0, 0, 0};
const int actionrow107[] = {0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow108[] = {-5, -5, -5, 0, 0, -5, 0, 0, -5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow109[] = {0, 0, 0, 19, 153, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 51, 0, 0, 35, 0, 33, 0, 0, 0, 0, 37, 49, 47, 45, 43, 41, 39, 27, 29, 31, 25, 23, 21, 0, 0, 0, 0};
const int actionrow110[] = {0, 0, 0, 155, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow111[] = {-6, -6, -6, 0, 0, -6, 0, 0, -6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow112[] = {0, 0, 0, 0, 0, 0, 0, 158, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow113[] = {0, 0, 0, 0, 0, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow114[] = {-7, -7, -7, 0, 0, -7, 0, 0, -7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow115[] = {0, 0, 0, 0, 0, 0, 0, 162, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow116[] = {0, 0, 0, 0, 0, 0, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow117[] = {-3, -3, -3, 0, 0, -3, 0, 0, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow118[] = {-1, 3, 157, 0, 0, 161, 0, 0, 165, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow119[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow120[] = {168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int actionrow121[] = {-2, -2, -2, 0, 0, -2, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int *action_table[] = {actionrow0, actionrow1, actionrow2, actionrow3, actionrow4, actionrow5, actionrow6, actionrow7, actionrow8, actionrow9, actionrow10, actionrow11, actionrow12, actionrow13, actionrow14, actionrow15, actionrow16, actionrow17, actionrow18, actionrow19, actionrow18, actionrow20, actionrow18, actionrow21, actionrow18, actionrow22, actionrow18, actionrow23, actionrow18, actionrow24, actionrow18, actionrow25, actionrow18, actionrow26, actionrow18, actionrow27, actionrow18, actionrow28, actionrow18, actionrow29, actionrow18, actionrow30, actionrow18, actionrow31, actionrow18, actionrow32, actionrow18, actionrow33, actionrow18, actionrow34, actionrow18, actionrow35, actionrow36, actionrow18, actionrow37, actionrow38, actionrow39, actionrow40, actionrow41, actionrow7, actionrow42, actionrow18, actionrow43, actionrow18, actionrow44, actionrow45, actionrow18, actionrow46, actionrow18, actionrow47, actionrow18, actionrow48, actionrow49, actionrow50, actionrow18, actionrow51, actionrow18, actionrow52, actionrow18, actionrow53, actionrow18, actionrow54, actionrow55, actionrow56, actionrow18, actionrow57, actionrow18, actionrow58, actionrow18, actionrow59, actionrow60, actionrow61, actionrow18, actionrow62, actionrow18, actionrow63, actionrow64, actionrow65, actionrow66, actionrow67, actionrow68, actionrow69, actionrow70, actionrow18, actionrow71, actionrow72, actionrow73, actionrow74, actionrow75, actionrow7, actionrow76, actionrow77, actionrow74, actionrow78, actionrow79, actionrow40, actionrow80, actionrow81, actionrow82, actionrow83, actionrow84, actionrow85, actionrow18, actionrow86, actionrow7, actionrow87, actionrow88, actionrow89, actionrow88, actionrow90, actionrow18, actionrow91, actionrow92, actionrow88, actionrow93, actionrow18, actionrow94, actionrow95, actionrow96, actionrow97, actionrow98, actionrow99, actionrow100, actionrow98, actionrow101, actionrow102, actionrow103, actionrow104, actionrow105, actionrow106, actionrow107, actionrow18, actionrow108, actionrow109, actionrow18, actionrow110, actionrow7, actionrow111, actionrow112, actionrow74, actionrow113, actionrow114, actionrow115, actionrow74, actionrow116, actionrow117, actionrow118, actionrow119, actionrow120, actionrow121};
const int gotorow0[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};
const int gotorow2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0};
const int gotorow3[] = {0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 117, 0};
const int gotorow4[] = {0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 117, 0};
const int gotorow5[] = {0, 0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 117, 0};
const int gotorow6[] = {0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 117, 0};
const int gotorow7[] = {0, 0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 0, 117, 0};
const int gotorow8[] = {0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 117, 0};
const int gotorow9[] = {0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 117, 0};
const int gotorow10[] = {0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 117, 0};
const int gotorow11[] = {0, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0, 117, 0};
const int gotorow12[] = {0, 0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 0, 117, 0};
const int gotorow13[] = {0, 0, 0, 0, 0, 0, 0, 0, 38, 0, 0, 0, 0, 117, 0};
const int gotorow14[] = {0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 117, 0};
const int gotorow15[] = {0, 0, 0, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 117, 0};
const int gotorow16[] = {0, 0, 0, 0, 0, 0, 0, 0, 44, 0, 0, 0, 0, 117, 0};
const int gotorow17[] = {0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 0, 0, 0, 117, 0};
const int gotorow18[] = {0, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0, 117, 0};
const int gotorow19[] = {0, 0, 0, 0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 117, 0};
const int gotorow20[] = {0, 0, 0, 0, 0, 0, 0, 0, 53, 0, 0, 0, 0, 117, 0};
const int gotorow21[] = {0, 0, 0, 0, 0, 0, 0, 0, 52, 57, 55, 0, 0, 117, 0};
const int gotorow22[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 59, 0};
const int gotorow23[] = {0, 0, 0, 0, 0, 0, 0, 0, 61, 0, 0, 0, 0, 117, 0};
const int gotorow24[] = {0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 117, 0};
const int gotorow25[] = {0, 0, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 117, 0};
const int gotorow26[] = {0, 0, 0, 0, 0, 0, 0, 0, 68, 0, 0, 0, 0, 117, 0};
const int gotorow27[] = {0, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, 117, 0};
const int gotorow28[] = {0, 0, 0, 0, 0, 0, 0, 0, 74, 0, 0, 0, 0, 117, 0};
const int gotorow29[] = {0, 0, 0, 0, 0, 0, 0, 0, 76, 0, 0, 0, 0, 117, 0};
const int gotorow30[] = {0, 0, 0, 0, 0, 0, 0, 0, 78, 0, 0, 0, 0, 117, 0};
const int gotorow31[] = {0, 0, 0, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 117, 0};
const int gotorow32[] = {0, 0, 0, 0, 0, 0, 0, 0, 84, 0, 0, 0, 0, 117, 0};
const int gotorow33[] = {0, 0, 0, 0, 0, 0, 0, 0, 86, 0, 0, 0, 0, 117, 0};
const int gotorow34[] = {0, 0, 0, 0, 0, 0, 0, 0, 88, 0, 0, 0, 0, 117, 0};
const int gotorow35[] = {0, 0, 0, 0, 0, 0, 0, 0, 92, 0, 0, 0, 0, 117, 0};
const int gotorow36[] = {0, 0, 0, 0, 0, 0, 0, 0, 94, 0, 0, 0, 0, 117, 0};
const int gotorow37[] = {0, 0, 0, 0, 0, 0, 0, 0, 52, 99, 55, 0, 0, 117, 0};
const int gotorow38[] = {0, 0, 0, 0, 0, 0, 0, 0, 103, 0, 0, 0, 0, 117, 0};
const int gotorow39[] = {0, 0, 0, 0, 106, 0, 0, 0, 0, 0, 0, 0, 0, 105, 0};
const int gotorow40[] = {0, 0, 0, 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow41[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0};
const int gotorow42[] = {0, 0, 0, 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow43[] = {0, 0, 0, 0, 0, 0, 0, 0, 52, 115, 55, 0, 0, 117, 0};
const int gotorow44[] = {0, 0, 0, 0, 0, 0, 0, 0, 122, 0, 0, 0, 0, 117, 0};
const int gotorow45[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 124, 0};
const int gotorow46[] = {0, 0, 0, 0, 0, 126, 0, 0, 119, 0, 0, 0, 0, 117, 0};
const int gotorow47[] = {0, 0, 0, 0, 0, 128, 0, 0, 119, 0, 0, 0, 0, 117, 0};
const int gotorow48[] = {0, 0, 0, 0, 0, 0, 0, 0, 130, 0, 0, 0, 0, 117, 0};
const int gotorow49[] = {0, 0, 0, 0, 0, 133, 0, 0, 119, 0, 0, 0, 0, 117, 0};
const int gotorow50[] = {0, 0, 0, 0, 0, 0, 0, 0, 135, 0, 0, 0, 0, 117, 0};
const int gotorow51[] = {0, 0, 0, 0, 0, 138, 0, 0, 119, 0, 0, 0, 0, 117, 0};
const int gotorow52[] = {0, 0, 0, 0, 0, 0, 140, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow53[] = {0, 0, 0, 0, 0, 0, 143, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow54[] = {0, 0, 0, 0, 0, 0, 0, 145, 0, 0, 0, 0, 0, 0, 0};
const int gotorow55[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 9, 6, 0};
const int gotorow56[] = {0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 117, 0};
const int gotorow57[] = {0, 0, 0, 0, 0, 0, 0, 0, 154, 0, 0, 0, 0, 117, 0};
const int gotorow58[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 156, 0};
const int gotorow59[] = {0, 0, 0, 159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow60[] = {0, 0, 0, 163, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow61[] = {0, 0, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int gotorow62[] = {169, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int *goto_table[] = {gotorow0, gotorow0, gotorow1, gotorow0, gotorow0, gotorow0, gotorow0, gotorow2, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow3, gotorow0, gotorow4, gotorow0, gotorow5, gotorow0, gotorow6, gotorow0, gotorow7, gotorow0, gotorow8, gotorow0, gotorow9, gotorow0, gotorow10, gotorow0, gotorow11, gotorow0, gotorow12, gotorow0, gotorow13, gotorow0, gotorow14, gotorow0, gotorow15, gotorow0, gotorow16, gotorow0, gotorow17, gotorow0, gotorow18, gotorow0, gotorow19, gotorow0, gotorow0, gotorow20, gotorow0, gotorow0, gotorow0, gotorow21, gotorow0, gotorow22, gotorow0, gotorow23, gotorow0, gotorow24, gotorow10, gotorow0, gotorow25, gotorow0, gotorow26, gotorow0, gotorow27, gotorow0, gotorow10, gotorow0, gotorow28, gotorow0, gotorow29, gotorow0, gotorow30, gotorow0, gotorow31, gotorow0, gotorow10, gotorow0, gotorow32, gotorow0, gotorow33, gotorow0, gotorow34, gotorow0, gotorow10, gotorow0, gotorow35, gotorow0, gotorow36, gotorow0, gotorow0, gotorow0, gotorow0, gotorow37, gotorow0, gotorow0, gotorow0, gotorow38, gotorow0, gotorow0, gotorow39, gotorow40, gotorow0, gotorow41, gotorow0, gotorow39, gotorow42, gotorow0, gotorow0, gotorow43, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow0, gotorow44, gotorow0, gotorow45, gotorow0, gotorow46, gotorow0, gotorow47, gotorow0, gotorow48, gotorow0, gotorow0, gotorow49, gotorow0, gotorow50, gotorow0, gotorow0, gotorow0, gotorow51, gotorow52, gotorow0, gotorow51, gotorow53, gotorow0, gotorow54, gotorow0, gotorow55, gotorow0, gotorow0, gotorow0, gotorow56, gotorow0, gotorow0, gotorow57, gotorow0, gotorow58, gotorow0, gotorow39, gotorow59, gotorow0, gotorow0, gotorow39, gotorow60, gotorow0, gotorow0, gotorow61, gotorow0, gotorow0, gotorow62};
int goto_entry[] = {0, 1, 1, 2, 2, 2, 2, 3, 3, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14};int rule_depth_table[] = {1, 0, 2, 2, 5, 4, 4, 0, 2, 4, 3, 2, 5, 5, 7, 3, 5, 0, 2, 3, 3, 6, 5, 4, 6, 4, 3, 1, 1, 5, 6, 8, 10, 8, 2, 2, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 0, 1, 1, 3, 0, 1, 1, 3, 1, 1, 1};NonTerminal* reduce_rule(int rule_id, stack_elem* rule) {
   switch(rule_id) {
case 1: {RAYModule*  result = new RAYModule;result->instructions = (RAYCommand**) listAsArray((ListNonTerminal*) rule[0].elem, result->instructions_count);
return result;}case 2: {return NULL;
}case 3: {return ListNonTerminal::newList(rule, 1, 0);
}case 4: {RAYInclude*  result = new RAYInclude;
result->filename = (RAYStringLiteral*) rule[1].elem;
return result;}case 5: {RAYDeclare*  result = new RAYDeclare;
result->ident = (RAYIdentifier*) rule[1].elem;

result->defval = (RAYExpression*) rule[3].elem;

return result;}case 6: {RAYWorld*  result = new RAYWorld;

result->fields = (RAYField**) listAsArray((ListNonTerminal*) rule[2].elem, result->fields_count);

return result;}case 7: {RAYSettings*  result = new RAYSettings;

result->fields = (RAYField**) listAsArray((ListNonTerminal*) rule[2].elem, result->fields_count);

return result;}case 8: {return NULL;
}case 9: {return ListNonTerminal::newList(rule, 1, 0);
}case 10: {RAYField*  result = new RAYField;result->fieldName = (RAYIdentifier*) rule[0].elem;

result->value = (RAYExpression*) rule[2].elem;

return result;}case 11: {RAYCompound*  result = new RAYCompound;
result->instr = (RAYInstruction**) listAsArray((ListNonTerminal*) rule[1].elem, result->instr_count);

return result;}case 12: {RAYExprInstr*  result = new RAYExprInstr;result->expr = (RAYExpression*) rule[0].elem;

return result;}case 13: {RAYWhileInstr*  result = new RAYWhileInstr;

result->cond = (RAYExpression*) rule[2].elem;

result->body = (RAYInstruction*) rule[4].elem;
return result;}case 14: {RAYIfInstr*  result = new RAYIfInstr;

result->cond = (RAYExpression*) rule[2].elem;

result->thenpart = (RAYInstruction*) rule[4].elem;
return result;}case 15: {RAYIfInstr*  result = new RAYIfInstr;

result->cond = (RAYExpression*) rule[2].elem;

result->thenpart = (RAYInstruction*) rule[4].elem;

result->elsepart = (RAYInstruction*) rule[6].elem;
return result;}case 16: {RAYVarDecl*  result = new RAYVarDecl;
result->ident = (RAYIdentifier*) rule[1].elem;

return result;}case 17: {RAYVarDecl*  result = new RAYVarDecl;
result->ident = (RAYIdentifier*) rule[1].elem;

result->defval = (RAYExpression*) rule[3].elem;

return result;}case 18: {return NULL;
}case 19: {return ListNonTerminal::newList(rule, 1, 0);
}case 20: {RAYCompound*  result = new RAYCompound;
result->instr = (RAYInstruction**) listAsArray((ListNonTerminal*) rule[1].elem, result->instr_count);

return result;}case 21: {return (RAYExpression* ) rule[1].elem;}case 22: {RAYMethodCallExpr*  result = new RAYMethodCallExpr;result->left = (RAYExpression*) rule[0].elem;

result->ident = (RAYIdentifier*) rule[2].elem;

result->params = (RAYExpression**) listAsArray((ListNonTerminal*) rule[4].elem, result->params_count);

return result;}case 23: {RAYFunction*  result = new RAYFunction;

result->params = (RAYIdentifier**) listAsArray((ListNonTerminal*) rule[2].elem, result->params_count);

result->body = (RAYCompound*) rule[4].elem;
return result;}case 24: {RAYRecord*  result = new RAYRecord;result->handleName = (RAYIdentifier*) rule[0].elem;

result->fields = (RAYField**) listAsArray((ListNonTerminal*) rule[2].elem, result->fields_count);

return result;}case 25: {RAYRecord*  result = new RAYRecord;result->handleName = (RAYIdentifier*) rule[0].elem;

result->subHandleName = (RAYIdentifier*) rule[2].elem;

result->fields = (RAYField**) listAsArray((ListNonTerminal*) rule[4].elem, result->fields_count);

return result;}case 26: {RAYCallExpr*  result = new RAYCallExpr;result->left = (RAYIdentifier*) rule[0].elem;

result->params = (RAYExpression**) listAsArray((ListNonTerminal*) rule[2].elem, result->params_count);

return result;}case 27: {RAYQualifyExpr*  result = new RAYQualifyExpr;result->left = (RAYExpression*) rule[0].elem;

result->ident = (RAYIdentifier*) rule[2].elem;
return result;}case 28: {RAYIdentifier*  result = new RAYIdentifier;
       result->image=str_dup(rule[0].tok_image);
     
return result;}case 29: {RAYSuper*  result = new RAYSuper;
       result->image=str_dup(rule[0].tok_image);
     
return result;}case 30: {RAYList*  result = new RAYList;

result->elems = (RAYExpression**) listAsArray((ListNonTerminal*) rule[2].elem, result->elems_count);


return result;}case 31: {RAYVector2*  result = new RAYVector2;

result->X = (RAYExpression*) rule[2].elem;

result->Y = (RAYExpression*) rule[4].elem;

return result;}case 32: {RAYVector3*  result = new RAYVector3;

result->X = (RAYExpression*) rule[2].elem;

result->Y = (RAYExpression*) rule[4].elem;

result->Z = (RAYExpression*) rule[6].elem;

return result;}case 33: {RAYVector4*  result = new RAYVector4;

result->X = (RAYExpression*) rule[2].elem;

result->Y = (RAYExpression*) rule[4].elem;

result->Z = (RAYExpression*) rule[6].elem;

result->W = (RAYExpression*) rule[8].elem;

return result;}case 34: {RAYColorRGB*  result = new RAYColorRGB;

result->R = (RAYExpression*) rule[2].elem;

result->G = (RAYExpression*) rule[4].elem;

result->B = (RAYExpression*) rule[6].elem;

return result;}case 35: {RAYUnaryExpr*  result = new RAYUnaryExpr;result->op = (int) rule[0].elem;
result->expr = (RAYExpression*) rule[1].elem;
return result;}case 36: {RAYUnaryExpr*  result = new RAYUnaryExpr;result->op = (int) rule[0].elem;
result->expr = (RAYExpression*) rule[1].elem;
return result;}case 37: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;

return result;}case 38: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 39: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 40: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 41: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 42: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 43: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 44: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 45: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 46: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 47: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 48: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 49: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 50: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 51: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 52: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 53: {RAYBinaryExpr*  result = new RAYBinaryExpr;result->left = (RAYExpression*) rule[0].elem;
result->op = (int) rule[1].elem;
result->right = (RAYExpression*) rule[2].elem;
return result;}case 54: {RAYBooleanLiteral*  result = new RAYBooleanLiteral;
      if ( !str_cmp(rule[0].tok_image, "true" ) )
        result->value = 1;
      else if ( !str_cmp(rule[0].tok_image, "false" ) )
        result->value = 0;
      else
        p_error("Invalid boolean value");
    
return result;}case 55: {RAYStringLiteral*  result = new RAYStringLiteral;
       result->image=str_dup(rule[0].tok_image);
     
return result;}case 56: {RAYIntegerLiteral*  result = new RAYIntegerLiteral;
       result->value = atoi(rule[0].tok_image);
     
return result;}case 57: {RAYFloatLiteral*  result = new RAYFloatLiteral;
        result->value = (float)atof(rule[0].tok_image);
     
return result;}case 58: {return NULL;
}case 59: {return (ListNonTerminal*) rule[0].elem;
}case 60: {return ListNonTerminal::newList(rule, 0, -1);
}case 61: {return ListNonTerminal::newList(rule, 2, 0);
}case 62: {return NULL;
}case 63: {return (ListNonTerminal*) rule[0].elem;
}case 64: {return ListNonTerminal::newList(rule, 0, -1);
}case 65: {return ListNonTerminal::newList(rule, 2, 0);
}case 66: {RAYIdentifier*  result = new RAYIdentifier;
       result->image=str_dup(rule[0].tok_image);
     
return result;}case 67: {RAYSuper*  result = new RAYSuper;
       result->image=str_dup(rule[0].tok_image);
     
return result;}case 68: {RAYStringLiteral*  result = new RAYStringLiteral;
       result->image=str_dup(rule[0].tok_image);
     
return result;} default:   printf("reduce_rule: got an invalid rule id.");   exit(-1);   }}RAYModule::~RAYModule(){
for (int i=0; i<instructions_count; i++)
  delete instructions[i];
  delete[] instructions;

}void RAYModule::dump(int depth) {
  printf("%sRAYModule\n", shift(depth-1));
if (instructions) for (int i=0; i<instructions_count; i++) {
  printf("%sinstructions[%d]\n", shift(depth), i);
  instructions[i]->dump(depth+2);
}

}
RAYCommand::~RAYCommand(){
}void RAYCommand::dump(int depth) {
  printf("%sRAYCommand\n", shift(depth-1));
}
RAYInclude::~RAYInclude(){
delete filename;
}void RAYInclude::dump(int depth) {
  printf("%sRAYInclude\n", shift(depth-1));
if (filename) {printf("%sfilename\n", shift(depth));
 filename->dump(depth+2);
}

}
RAYDeclare::~RAYDeclare(){
delete ident;
delete defval;
}void RAYDeclare::dump(int depth) {
  printf("%sRAYDeclare\n", shift(depth-1));
if (ident) {printf("%sident\n", shift(depth));
 ident->dump(depth+2);
}

if (defval) {printf("%sdefval\n", shift(depth));
 defval->dump(depth+2);
}

}
RAYWorld::~RAYWorld(){
for (int i=0; i<fields_count; i++)
  delete fields[i];
  delete[] fields;

}void RAYWorld::dump(int depth) {
  printf("%sRAYWorld\n", shift(depth-1));
if (fields) for (int i=0; i<fields_count; i++) {
  printf("%sfields[%d]\n", shift(depth), i);
  fields[i]->dump(depth+2);
}

}
RAYSettings::~RAYSettings(){
for (int i=0; i<fields_count; i++)
  delete fields[i];
  delete[] fields;

}void RAYSettings::dump(int depth) {
  printf("%sRAYSettings\n", shift(depth-1));
if (fields) for (int i=0; i<fields_count; i++) {
  printf("%sfields[%d]\n", shift(depth), i);
  fields[i]->dump(depth+2);
}

}
RAYField::~RAYField(){
delete fieldName;
delete value;
}void RAYField::dump(int depth) {
  printf("%sRAYField\n", shift(depth-1));
if (fieldName) {printf("%sfieldName\n", shift(depth));
 fieldName->dump(depth+2);
}

if (value) {printf("%svalue\n", shift(depth));
 value->dump(depth+2);
}

}
RAYInstruction::~RAYInstruction(){
}void RAYInstruction::dump(int depth) {
  printf("%sRAYInstruction\n", shift(depth-1));
}
RAYCompound::~RAYCompound(){
for (int i=0; i<instr_count; i++)
  delete instr[i];
  delete[] instr;

}void RAYCompound::dump(int depth) {
  printf("%sRAYCompound\n", shift(depth-1));
if (instr) for (int i=0; i<instr_count; i++) {
  printf("%sinstr[%d]\n", shift(depth), i);
  instr[i]->dump(depth+2);
}

}
RAYExprInstr::~RAYExprInstr(){
delete expr;
}void RAYExprInstr::dump(int depth) {
  printf("%sRAYExprInstr\n", shift(depth-1));
if (expr) {printf("%sexpr\n", shift(depth));
 expr->dump(depth+2);
}

}
RAYWhileInstr::~RAYWhileInstr(){
delete body;
delete cond;
}void RAYWhileInstr::dump(int depth) {
  printf("%sRAYWhileInstr\n", shift(depth-1));
if (body) {printf("%sbody\n", shift(depth));
 body->dump(depth+2);
}

if (cond) {printf("%scond\n", shift(depth));
 cond->dump(depth+2);
}

}
RAYIfInstr::~RAYIfInstr(){
delete cond;
delete thenpart;
delete elsepart;
}void RAYIfInstr::dump(int depth) {
  printf("%sRAYIfInstr\n", shift(depth-1));
if (cond) {printf("%scond\n", shift(depth));
 cond->dump(depth+2);
}

if (thenpart) {printf("%sthenpart\n", shift(depth));
 thenpart->dump(depth+2);
}

if (elsepart) {printf("%selsepart\n", shift(depth));
 elsepart->dump(depth+2);
}

}
RAYVarDecl::~RAYVarDecl(){
delete defval;
delete ident;
}void RAYVarDecl::dump(int depth) {
  printf("%sRAYVarDecl\n", shift(depth-1));
if (defval) {printf("%sdefval\n", shift(depth));
 defval->dump(depth+2);
}

if (ident) {printf("%sident\n", shift(depth));
 ident->dump(depth+2);
}

}
RAYExpression::~RAYExpression(){
}void RAYExpression::dump(int depth) {
  printf("%sRAYExpression\n", shift(depth-1));
}
RAYMethodCallExpr::~RAYMethodCallExpr(){
for (int i=0; i<params_count; i++)
  delete params[i];
  delete[] params;

delete left;
delete ident;
}void RAYMethodCallExpr::dump(int depth) {
  printf("%sRAYMethodCallExpr\n", shift(depth-1));
if (left) {printf("%sleft\n", shift(depth));
 left->dump(depth+2);
}

if (ident) {printf("%sident\n", shift(depth));
 ident->dump(depth+2);
}

if (params) for (int i=0; i<params_count; i++) {
  printf("%sparams[%d]\n", shift(depth), i);
  params[i]->dump(depth+2);
}

}
RAYFunction::~RAYFunction(){
for (int i=0; i<params_count; i++)
  delete params[i];
  delete[] params;

delete body;
}void RAYFunction::dump(int depth) {
  printf("%sRAYFunction\n", shift(depth-1));
if (params) for (int i=0; i<params_count; i++) {
  printf("%sparams[%d]\n", shift(depth), i);
  params[i]->dump(depth+2);
}

if (body) {printf("%sbody\n", shift(depth));
 body->dump(depth+2);
}

}
RAYRecord::~RAYRecord(){
for (int i=0; i<fields_count; i++)
  delete fields[i];
  delete[] fields;

delete handleName;
delete subHandleName;
}void RAYRecord::dump(int depth) {
  printf("%sRAYRecord\n", shift(depth-1));
if (handleName) {printf("%shandleName\n", shift(depth));
 handleName->dump(depth+2);
}

if (subHandleName) {printf("%ssubHandleName\n", shift(depth));
 subHandleName->dump(depth+2);
}

if (fields) for (int i=0; i<fields_count; i++) {
  printf("%sfields[%d]\n", shift(depth), i);
  fields[i]->dump(depth+2);
}

}
RAYCallExpr::~RAYCallExpr(){
for (int i=0; i<params_count; i++)
  delete params[i];
  delete[] params;

delete left;
}void RAYCallExpr::dump(int depth) {
  printf("%sRAYCallExpr\n", shift(depth-1));
if (left) {printf("%sleft\n", shift(depth));
 left->dump(depth+2);
}

if (params) for (int i=0; i<params_count; i++) {
  printf("%sparams[%d]\n", shift(depth), i);
  params[i]->dump(depth+2);
}

}
RAYQualifyExpr::~RAYQualifyExpr(){
delete left;
delete ident;
}void RAYQualifyExpr::dump(int depth) {
  printf("%sRAYQualifyExpr\n", shift(depth-1));
if (left) {printf("%sleft\n", shift(depth));
 left->dump(depth+2);
}

if (ident) {printf("%sident\n", shift(depth));
 ident->dump(depth+2);
}

}
RAYIdentifier::~RAYIdentifier(){
}void RAYIdentifier::dump(int depth) {
  printf("%sRAYIdentifier\n", shift(depth-1));
}
RAYSuper::~RAYSuper(){
}void RAYSuper::dump(int depth) {
  printf("%sRAYSuper\n", shift(depth-1));
}
RAYList::~RAYList(){
for (int i=0; i<elems_count; i++)
  delete elems[i];
  delete[] elems;

}void RAYList::dump(int depth) {
  printf("%sRAYList\n", shift(depth-1));
if (elems) for (int i=0; i<elems_count; i++) {
  printf("%selems[%d]\n", shift(depth), i);
  elems[i]->dump(depth+2);
}

}
RAYVector2::~RAYVector2(){
delete X;
delete Y;
}void RAYVector2::dump(int depth) {
  printf("%sRAYVector2\n", shift(depth-1));
if (X) {printf("%sX\n", shift(depth));
 X->dump(depth+2);
}

if (Y) {printf("%sY\n", shift(depth));
 Y->dump(depth+2);
}

}
RAYVector3::~RAYVector3(){
delete X;
delete Y;
delete Z;
}void RAYVector3::dump(int depth) {
  printf("%sRAYVector3\n", shift(depth-1));
if (X) {printf("%sX\n", shift(depth));
 X->dump(depth+2);
}

if (Y) {printf("%sY\n", shift(depth));
 Y->dump(depth+2);
}

if (Z) {printf("%sZ\n", shift(depth));
 Z->dump(depth+2);
}

}
RAYVector4::~RAYVector4(){
delete X;
delete Y;
delete Z;
delete W;
}void RAYVector4::dump(int depth) {
  printf("%sRAYVector4\n", shift(depth-1));
if (X) {printf("%sX\n", shift(depth));
 X->dump(depth+2);
}

if (Y) {printf("%sY\n", shift(depth));
 Y->dump(depth+2);
}

if (Z) {printf("%sZ\n", shift(depth));
 Z->dump(depth+2);
}

if (W) {printf("%sW\n", shift(depth));
 W->dump(depth+2);
}

}
RAYColorRGB::~RAYColorRGB(){
delete R;
delete G;
delete B;
}void RAYColorRGB::dump(int depth) {
  printf("%sRAYColorRGB\n", shift(depth-1));
if (R) {printf("%sR\n", shift(depth));
 R->dump(depth+2);
}

if (G) {printf("%sG\n", shift(depth));
 G->dump(depth+2);
}

if (B) {printf("%sB\n", shift(depth));
 B->dump(depth+2);
}

}
RAYUnaryExpr::~RAYUnaryExpr(){
delete expr;
}void RAYUnaryExpr::dump(int depth) {
  printf("%sRAYUnaryExpr\n", shift(depth-1));
if (op) printf("%sop = %d [%c]\n", shift(depth), op, op);

if (expr) {printf("%sexpr\n", shift(depth));
 expr->dump(depth+2);
}

}
RAYBinaryExpr::~RAYBinaryExpr(){
delete left;
delete right;
}void RAYBinaryExpr::dump(int depth) {
  printf("%sRAYBinaryExpr\n", shift(depth-1));
if (op) printf("%sop = %d [%c]\n", shift(depth), op, op);

if (left) {printf("%sleft\n", shift(depth));
 left->dump(depth+2);
}

if (right) {printf("%sright\n", shift(depth));
 right->dump(depth+2);
}

}
RAYBooleanLiteral::~RAYBooleanLiteral(){
}void RAYBooleanLiteral::dump(int depth) {
  printf("%sRAYBooleanLiteral\n", shift(depth-1));
}
RAYStringLiteral::~RAYStringLiteral(){
}void RAYStringLiteral::dump(int depth) {
  printf("%sRAYStringLiteral\n", shift(depth-1));
}
RAYIntegerLiteral::~RAYIntegerLiteral(){
}void RAYIntegerLiteral::dump(int depth) {
  printf("%sRAYIntegerLiteral\n", shift(depth-1));
}
RAYFloatLiteral::~RAYFloatLiteral(){
}void RAYFloatLiteral::dump(int depth) {
  printf("%sRAYFloatLiteral\n", shift(depth-1));
}

