
/* This source file's purpose is to read and write from the EEPROM when its
 * sub-functions are called upon. Whether it is to read or write data, 
 * sub-functions inside this source file pertains to the modification of EEPROM
 */

/********************************** Includes **********************************/
#include "EEPROM.h"

/********************************** Functions *********************************/

void write_EEPROM(unsigned int address, unsigned int data){
    /* writes the data provided into a the EEPROM at the location address
     * refer to textbook 7.4.3.2 or datasheet page 75~
     * Arguments: address, the location where the data is being stored. Goes 
     *                      from 0 to 1024, since there are 1024 bytes reserved
     *                      for the EEPROM
     *                      Each address byte contains 8 bits
     *            data,     integer value between 0 and 255, since there are 8
     *                      bits in a byte (2^8 = 256).
     * 
     * Returns: none
     */

    EEADRH = address >> 8;      //Upper bits of Data Memory Address to write
    EEADR = address;            //Lower bits of Data Memory Address to write
    EEDATA = data;              //Data Memory Value to write
    EECON1bits.EEPGD = 0;       //Point to DATA memory (Access data EEPROM memory)
    EECON1bits.CFGS = 0;        //Access EEPROM (Access Flash program or data EEPROM memory)
    EECON1bits.WREN = 1;        //Enable writes (Allows write cycles to Flash program/data EEPROM)
    di();                       //Disable Interrupts
    
    //Required Sequence
    EECON2 = 0x55;              //Write 55h
    EECON2 = 0xAA;              //Write 0xAA
    EECON1bits.WR = 1;          //Set WR bit to begin write
    //Initiates a data EEPROM erase/write cycle or a program memory erase cycle or write cycle
    //(The operation is self-timed and the bit is cleared by hardware once write is complete. The WR bit
    //can only be set (not cleared) in software.)
    
    do{
     continue;   
    }while(EECON1bits.WR == 1);
    ei();                       //Enable Interrupts
    PIR2bits.EEIF = 0;          //EEIF bit of PIR2 set high after writing, must clear
    EECON1bits.WREN = 0;        //Disable writes on write complete (EEIF set) (Inhibits write cycles to Flash program/data EEPROM)
}

unsigned int read_EEPROM(unsigned int address){
    /* read the data stored in the EEPROM at the location address
     * refer to textbook 7.4.3.2 or datasheet page 75~
     * Arguments: address, the location where the data is being stored. Goes 
     *                      from 0 to 1024, since there are 1024 bytes reserved
     *                      for the EEPROM
     *                      Each address byte contains 8 bits
     * 
     * Returns: integer value from 0-255 that was stored in the location address
     */

    EEADRH = address >> 8;      //Upper bits of Data Memory Address to read
    EEADR = address;            //Lower bits of Data Memory Address to read            
    EECON1bits.EEPGD = 0;       //Point to DATA memory (Access data EEPROM memory)
    EECON1bits.CFGS = 0;        //Access EEPROM (Access Flash program or data EEPROM memory)
    EECON1bits.RD = 1;          //EEPROM Read
    // Initiates an EEPROM read (Read takes one cycle. RD is cleared in hardware. The RD bit can only
    // be set (not cleared) in software. RD bit cannot be set when EEPGD = 1 or CFGS = 1.)
    do{
        continue;
    }while(EECON1bits.RD == 1);
    return EEDATA;              //W - EEDATA
}