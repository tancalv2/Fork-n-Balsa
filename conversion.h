/* 
 * File:   conversion.h
 * Author: calvi
 *
 * Created on February 25, 2018, 3:58 PM
 */

#ifndef CONVERSION_H
#define	CONVERSION_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "main.h"

/****************************** Public Interfaces *****************************/
unsigned int charToInt(unsigned char c1, unsigned char c2);
unsigned int amountEncoder(unsigned int curr);

#endif	/* CONVERSION_H */

