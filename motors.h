/* 
 * File:   motors.h
 * Author: calvi
 *
 * Created on February 14, 2018, 10:15 PM
 */

#ifndef MOTORS_H
#define	MOTORS_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "main.h"

/***** Constants *****/
const int SMALL_SIZE = 65; //size of vertical motors
const int BIG_SIZE = 195;

/****************************** Public Interfaces *****************************/
void horizontalMotor(unsigned int size, unsigned int direction);
void verticalMotor(unsigned int size, unsigned int direction);
void selectorMotor(unsigned int choice);
void dispenseMotorsSelector(unsigned int mode, unsigned int dietType, unsigned int Amount);

/****************************** Local Functions *******************************/
void dispenseMotors(unsigned int R, unsigned int F, unsigned int L);
void vibrationMotors(unsigned int state);

#endif	/* MOTORS_H */

