/* 
 * File:   sensors.h
 * Author: calvi
 *
 * Created on February 15, 2018, 11:14 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "main.h"

/****************************** Public Interfaces *****************************/
unsigned int infraredSensor(void);
void lineSensor(unsigned int row);

/****************************** Local Functions *******************************/
unsigned short readADC(char channel);

/***** Constants *****/
const unsigned short lineThreshold = 0x3e7; //Threshold for Line Sensor tested on Apr/5
const unsigned short infraThreshold = 0x050; //Threshold for Infrared Sensor tested on Apr/6
#endif	/* SENSORS_H */

