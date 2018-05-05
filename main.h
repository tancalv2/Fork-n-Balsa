/* 
 * File:   main.h
 * Author: calvi
 *
 * Created on February 15, 2018, 11:06 PM
 */

#ifndef MAIN_H
#define	MAIN_H


/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "I2C.h"
#include "RTC.h"
#include "EEPROM.h"
#include "conversion.h"
#include "motors.h"
#include "sensors.h"
#include "operationLogs.h"
#include "developerMode.h"

/***** Macros *****/
#define __bcd_to_num(num) (num & 0x0F) + ((num & 0xF0)>>4)*10

/***** Constants *****/
const char keys[] = "123A456B789C*0#D";
const char happynewyear[7] = {  0x00, // 00 Seconds 
                                0x30, // 30 Minutes
                                0x10, // 24 hour mode, set to 011:00
                                0x03, // Wednesday
                                0x11, // 11th
                                0x04, // April
                                0x18  // 2018
};
//const char happynewyear[7] = {  0x00, // 00 Seconds 
//                                0x30, // 30 Minutes
//                                0x11, // 24 hour mode, set to 11:00
//                                0x03, // Wednesday
//                                0x11, // 11th
//                                0x04, // April
//                                0x18  // 2018
//};

/****************************** Local Functions *******************************/
void resetn(void);
unsigned int validInput(unsigned int option, unsigned int temp, unsigned int currDrawer, unsigned int currDiet);
void dispenseFood(void);

/****************************** Global Variables ******************************/
unsigned int drawer[17];
unsigned int diet[17];
unsigned int amount[17];
unsigned int marked[17];
unsigned int opTime;
unsigned int leftovers[4];
unsigned char interruptCheck;
unsigned int time[7]; // Create a byte array to hold time read from RTC
unsigned int timerCounter; // Timer counter
unsigned int currRow;

#endif	/* MAIN_H */

