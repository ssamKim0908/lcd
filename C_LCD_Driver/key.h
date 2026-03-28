#ifndef __KEY_H
#define __KEY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//define the key button pin in BCM number
#define K1_PIN      17
#define K2_PIN      18
#define K3_PIN      22
#define K4_PIN      23
#define R_PIN       20
#define L_PIN       21
#define UP_PIN      13
#define LEFT_PIN    5
#define DOWN_PIN    26
#define RIGHT_PIN   19
#define CENTER_PIN  6

void Key_Init(void);
void Key_Task(void);


#endif

