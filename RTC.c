
/* This source file's purpose is to contain the sample code provided for the RTC
 * It is used to set and calculate the current time.
 */

/********************************** Includes **********************************/
#include "RTC.h"

/********************************** Functions *********************************/

void RTC_setTime(void){
    /* Writes the happynewyear array to the RTC memory.
     *
     * Arguments: none
     *
     * Returns: none
     */
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    /* Write array. */
    for(char i=0; i<7; i++){
        I2C_Master_Write(happynewyear[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
}

void calculateTime(void){
    /* Calculates the current time onto the time array. Code from sample code
     *
     * Arguments: none
     *
     * Returns: none
     */
    
    unsigned char i; // Loop counter
    /* Reset RTC memory pointer. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition

    /* Read current time. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
    for(i = 0; i < 6; i++){
        time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    time[6] = I2C_Master_Read(NACK); // Final Read with NACK
    I2C_Master_Stop(); // Stop condition
}