/* 
 * File:   developerMode.h
 * Author: calvi
 *
 * Created on March 29, 2018, 11:37 PM
 */

#ifndef DEVELOPERMODE_H
#define	DEVELOPERMODE_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "main.h"

/****************************** Public Interfaces *****************************/
void developerMode(void);
void hMotors(void);
void vMotors(void);
void selMotor(void);
void disMotors(void);
void vibMotors(void);
void lineSensors(void);
void infraSensors(void);

#endif	/* DEVELOPERMODE_H */

