
/* This source file contains all sensor functionalities. Sub-functions 
 * included are the infrared and line sensors
 */

/********************************** Includes **********************************/
#include "sensors.h"

/********************************** Functions *********************************/

unsigned int infraredSensor(void){
    /* This sub-function controls the infrared sensor beside the dropper. Its
     * purpose is to return either a 0 or 1 when called upon depending on if an
     * object was detected. It is connected to pin RE1. RE1 is an analog pin and
     * will call the readADC() function to determine the voltage value of this 
     * pin. Once determined, it checks if the value is under the predetermined 
     * threshold and if so returns a value.
     * Arguments: none
     * 
     * Returns: 0 if nothing was detected, 1 if something was detected
     */
    unsigned short input = readADC(6);

    /* polling is used to check whether there is a dip in threshold on the RE1 
     * pin. Since the food piece moves fairly quickly, the dip in threshold must
     * be caught during the poll. There is a dip that does occur since the pin 
     * is constantly reading the value of the pin at a rate of 40Mhz (clock speed) 
     * however it must be caught by the program, thus a loop with large i and 
     * small delay interval (in the microseconds instead of milliseconds since 
     * testing showed micro seconds too small). 
     * There are two scenarios that can occur: Return early or return after loop
     * 
     * Return Early:
     * This occurs when there is a detection (input value is less than threshold 
     * value) thus returning to the function that called upon it.
     * Return after loop:
     * This occurs when there is no detection at all, otherwise known as the
     * worst case O(n) is when there. It occurs once the loop reaches the i'th 
     * iteration without iteration. This worst case should be as short as 
     * possible since this time is used polling and nothing can occur during
     * this time. The total time spent polling is: delay*i.
     */
    unsigned int i = 0;
    do{
        input = readADC(6);
        if(input < infraThreshold){    
            //There is a signal
//            __lcd_clear();
//            __lcd_home();
//            printf("DETECTION");
//            __delay_ms(2000); 
//            __lcd_clear();
//            __lcd_home();
            return 1; 
        }
        __delay_us(200); // 200microseconds was determined to be sufficiently fast
                        // enough to detect the dip in pin value
        i++;
    }while(i<10000);     // i = 10000, total time = 10000*200*10^-6 = 2s total
//    __lcd_clear();
//    __lcd_home();
//    printf("Nothing");
//    __delay_ms(2000);
//    __lcd_clear();
//    __lcd_home();
    return 0;
}

void lineSensor(unsigned int row){
    /* This sub-function controls the four line sensor located above the forks 
     * of the "robotic arm". Its purpose is to detect if their is marked drawer
     * in the current row for each of the columns. If there is, register that 
     * value onto the marked array to keep memory of this knowledge The line
     * sensors are connected to pins RA0,RA1, RE0, and RA5, where RA0 is the 
     * left-most and RA5 is the right-most sensor (left-most corresponding to 
     * face the cabinet) This is achieved through reading the analog signal 
     * (only 4 pins that are analog in the whole design). When a marked drawer 
     * is detected, the line sensor will send a voltage value much lower than 
     * that of non-marked drawer. This knowledge is then saved onto the marked 
     * array.
     * Arguments:       row,    integer value that is between 0 and 3. 
     *                          Its purpose is to keep track of which row to
     *                          write the value to in the marked array
     * 
     * Returns: none
     */
//    __lcd_clear();
//    __lcd_home();
//    printf("Line Sensor");
//    __delay_ms(2000);

    unsigned int i;
    unsigned short column1 = readADC(0);
    unsigned short column2 = readADC(1);
    unsigned short column3 = readADC(5);
    unsigned short column4 = readADC(4);
    
    //move arm customized size up
    verticalMotor(50,2);
    
    //threshold is the minimum amount of voltage change required to guarantee its a marked drawer (testing required)    
    for(i=0;i<30;i++){  //check for a total of 1.5s
//        __lcd_clear();
//        __lcd_home();
//         printf("%.3x     %.3x", readADC(0), readADC(1));
//            __lcd_newline();
//            printf("%.3x     %.3x", readADC(5), readADC(4));
//        if(readADC(0) < column1){
            column1 = readADC(0);
//        }
//        if(readADC(1) < column2){
        column2 = readADC(1);
//        }
//        if(readADC(5) < column3){
        column3 = readADC(5);
//        }
//        if(readADC(4) < column4){
        column4 = readADC(4);    
//        }
//         printf("%.3x     %.3x", column1,column2);
//            __lcd_newline();
//            printf("%.3x     %.3x", column3, column4);
        if(column1 < lineThreshold){    marked[4*row+1] = 1;    }
//        if(column2 < lineThreshold){    marked[4*row+2] = 1;    }
        if(column3 < 0x330){    marked[4*row+3] = 1;    }
        if(column4 < lineThreshold){    marked[4*row+4] = 1;    }
        __delay_ms(50); //check every 50ms
    }
    //move arm customized size down
    verticalMotor(50,3);
    return;
}

unsigned short readADC(char channel){
    /* Reads the analog input from the specified analog channel.
     *
     * Arguments: channel, the byte corresponding to the channel to read
     *
     * Returns: the 10-bit value corresponding to the voltage read from
     *          the specified channel
     */
    //From Sample Code
    
    ADCON0 = (channel & 0x0F) << 2; // Select ADC channel (i.e. pin)
    ADON = 1; // Enable module
    ADCON0bits.GO = 1; // Initiate sampling
    while(ADCON0bits.GO_NOT_DONE){  continue;   } // Poll for acquisition completion
    return (ADRESH << 8) | ADRESL; // Return result as a 16-bit value
}