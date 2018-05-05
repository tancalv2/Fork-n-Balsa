/* 
 * File:   RTC.h
 * Author: calvi
 *
 * Created on February 18, 2018, 2:11 PM
 */

#ifndef RTC_H
#define	RTC_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "main.h"
#include "I2C.h"

/****************************** Public Interfaces *****************************/
void RTC_setTime(void);
void calculateTime(void);

#endif	/* RTC_H */

