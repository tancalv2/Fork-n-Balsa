/* 
 * File:   operationLogs.h
 * Author: calvi
 *
 * Created on February 15, 2018, 1:20 PM
 */

#ifndef OPERATIONLOGS_H
#define	OPERATIONLOGS_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "main.h"
#include "EEPROM.h"

/****************************** Public Interfaces *****************************/
void operationLogs(void);

/****************************** Local Functions *******************************/
unsigned int previousLogs(void);

/****************************** Global Variables ******************************/
unsigned int currLog;   //keep track how far in the recursion is for PreviousLog

/***** Constants *****/
const char memoryGap = 30;    //how much each log differs by

#endif	/* OPERATIONLOGS_H */

