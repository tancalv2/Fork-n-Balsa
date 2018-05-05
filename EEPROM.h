/* 
 * File:   EEPROM.h
 * Author: calvi
 *
 * Created on February 17, 2018, 5:48 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "main.h"

/****************************** Public Interfaces *****************************/
void write_EEPROM(unsigned int address, unsigned int data);
unsigned int read_EEPROM(unsigned int address);

#endif	/* EEPROM_H */

