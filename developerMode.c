
/* This source file contains all individual mechanisms the user can use to test
 * and debug the machine without testing the full trial. It also contains the 
 * reset mechanism to reset the "robotic arm" to its initial state as well as 
 * to empty the reservoirs at the top of the machine
 */

/********************************** Includes **********************************/
#include "developerMode.h"

/********************************** Functions *********************************/

void developerMode(void){
    /* This sub-function is the home to all the functions. There are eight (8)
     * different function calls and 1 return call:
     *                  1: Horizontal Motors        (Corresponding to A)
     *                  2: Vertical Motors          (Corresponding to B)
     *                  3: Selector Motor           (Corresponding to C)
     * Next button to show a second page            (Corresponding to D)
     *                  Second Page
     *                  4: Dispenser Motors         (Corresponding to A)
     *                  5: Vibration Motor Switch   (Corresponding to B)
     *                  6: Line Sensors             (Corresponding to C)
     *                  7: Infrared Sensors         (Corresponding to D)
     *  Option 8 is not displayed and is implied to be known by the user
     *                  8: reset button             (Corresponding to 1)
     *                  -> the reset button can be accessed anytime within this
     *                     subfunction as long as 1 is pressed. When pressed,
     *                     the machine is reset to initial state 
     *                  9: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */
    unsigned char keypress;
    unsigned char temp;
    __lcd_display_control(1, 0, 0);
    while(1){
        __lcd_clear();
        __lcd_home();
        printf("Developer Mode");
        __lcd_newline();
        printf("Please wait...");
        __delay_ms(1000);
        
        __lcd_clear();
        __lcd_home();
        //Display options for page 1
        printf("A:F/B   B:U/D ");
        __lcd_newline();
        printf("C:Sel   D:next");
        
        while(PORTBbits.RB1 == 0){  continue;   }
        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }
        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //Determine which option was picked
        if(temp == 'A'){
            hMotors();
        }
        if(temp == 'B'){
            vMotors();
        }
        if(temp == 'C'){
            selMotor();
        }
        if(temp == 'D'){
            __lcd_clear();
            __lcd_home();
            //Display options for page 2
            printf("A:Dis   B:Vib");
            __lcd_newline();
            printf("C:RGB   D:Infra");

            while(PORTBbits.RB1 == 0){  continue;   }
            /* Read the 4-bit character code. */
            keypress = (PORTB & 0xF0) >> 4;
            /* Wait until the key has been released. */
            while(PORTBbits.RB1 == 1){  continue;   }
            Nop();  // Apply breakpoint here to prevent compiler optimizations
            temp = keys[keypress];     
            //Determine which option was picked
            if(temp == 'A'){
                disMotors();
            }
            if(temp == 'B'){
                vibMotors();
            }
            if(temp == 'C'){
                lineSensors();
            }
            if(temp == 'D'){
                infraSensors();
            }
        }
        //reset
        if(temp == '1'){
            __lcd_clear();
            __lcd_home();
            printf("Reseting");
            __lcd_newline();
            printf("Please wait...");
       
            unsigned int i;
            //turn vibration motors on
//            vibrationMotors(1);
            //dispense the rest of the food (maybe)
            dispenseMotorsSelector(1,0,0);
            //reset leftover global variable since machine will count while 
            //reseting
            __lcd_clear();
            __lcd_home();
            printf("R:%d, F:%d, L:%d", leftovers[1],leftovers[2],leftovers[3]);
            __delay_ms(2000);
            for(i = 0; i<4; i++){
                leftovers[i] = 0;
            }
            //Down
            for(i=0;i<200;i++){
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 1;
                __delay_ms(2);
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 1;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 1;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
            }
            __delay_ms(500);
            //Up
            for(i=0;i<600;i++){
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 1;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 1;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 1;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
            }
            //turn off vibration motors
            vibrationMotors(0);
            break;
        }
        //return
        if(temp == '*'){
             __lcd_clear();
            __lcd_home();
            printf("Returning");
            __lcd_newline();
            printf("Please wait...");
            __delay_ms(2000);
            break;
        }
    }
    return;
}

void hMotors(void){
    /* This sub-function is the home to all horizontal functions. There are four
     * (4) different function calls and 1 return call:
     *                  1: Forward          (Corresponding to 1)
     *                      -> full forward motion like it is during operation
     *                  2: Backward         (Corresponding to 2)
     *                      -> full backward motion like it is during operation
     *                  3: Small Forward    (Corresponding to 3)
     *                      -> 50ms delay forward motion
     *                  4: Small Backward   (Corresponding to A)
     *                      -> 50ms delay backward motion
     *                  5: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */
    unsigned char keypress;
    unsigned char temp;
    while(1){
        __lcd_clear();
        __lcd_home();
        printf("1:F     2:B");
        __lcd_newline();
        printf("3:F (s) A:B (s)");
        while(PORTBbits.RB1 == 0){  continue;   }

        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }

        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];

        //Forward
        if(temp == '1'){
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 1;
            __delay_ms(700);
        }
        //Backward
        if(temp == '2'){
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 0;
            __delay_ms(700);
        }
        //Small Forward
        if(temp == '3'){
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 1;
            __delay_ms(50);
        }
        //Small Backward
        if(temp == 'A'){
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 0;
            __delay_ms(50);
        }
        //return
        if(temp == '*'){
            break;
        }
        LATCbits.LATC1 = 0;
        LATCbits.LATC2 = 0;
    }
    return;
}

void vMotors(void){
    /* This sub-function is the home to all vertical functions. There are four
     * (4) different function calls and 1 return call:
     *                  1: Up               (Corresponding to 1)
     *                      -> big up motion like it is during operation
     *                  2: Down             (Corresponding to 2)
     *                      -> big down motion like it is during operation
     *                  3: Small Up         (Corresponding to 3)
     *                      -> Small up motion like it is during operation
     *                  4: Small Down       (Corresponding to A)
     *                      -> Small down motion like it is during operation
     *                  5: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */
    unsigned char keypress;
    unsigned char temp;
    unsigned int i;
    while(1){
        __lcd_clear();
        __lcd_home();
        printf("1:U     2:D    ");
        __lcd_newline();
        printf("3:U (s) A:D (s)");
        while(PORTBbits.RB1 == 0){  continue;   }

        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }

        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //Big Up
        if(temp == '1'){
            for(i=0;i<190;i++){
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 1;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 1;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 1;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
            } 
        }
        //Big Down
        if(temp == '2'){
            for(i=0;i<190;i++){
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 1;
                __delay_ms(2);
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 1;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 1;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
            } 
        }
        //Small Up
        if(temp == '3'){
            for(i=0;i<65;i++){
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 1;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 1;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 1;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
            } 
        }
        //Small Down
        if(temp == 'A'){
            for(i=0;i<65;i++){
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 1;
                __delay_ms(2);
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 1;
                LATEbits.LATE2 = 0;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                LATEbits.LATE2 = 1;
                LATCbits.LATC0 = 0;
                __delay_ms(2);
            }    
        }
        if(temp == '*'){
            break;
        }
        LATAbits.LATA2 = 0;
        LATAbits.LATA3 = 0;
        LATEbits.LATE2 = 0;
        LATCbits.LATC0 = 0;
    }
    return;
}

void selMotor(void){
    /* This sub-function is the home to all selection functions. There are four
     * (4) different function calls and 1 return call:
     *                  1: Column 1         (Corresponding to 1)
     *                      -> moves selector to column 1
     *                  2: Column 2         (Corresponding to 2)
     *                      -> moves selector to column 2
     *                  3: Column 3         (Corresponding to 3)
     *                      -> moves selector to column 3
     *                  4: Column 4         (Corresponding to A)
     *                      -> moves selector to column 4
     *                  5: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */
    unsigned char keypress;
    unsigned char temp;
    unsigned int i;
    while(1){
        __lcd_clear();
        __lcd_home();
        printf("1:C1 2:C2 3:C3");
        __lcd_newline();
        printf("A:C4 *: Return");
        while(PORTBbits.RB1 == 0){  continue;   }

        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }

        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //First make the selector in the most CCW position 
        for(i=0; i<9;i++){
            LATDbits.LATD0 = 1;
            __delay_ms(2);
            LATDbits.LATD0 = 0;
            __delay_ms(20-2);
        }
        __delay_ms(500);
        //column 2
        if(temp == '2'){
            for(i=0; i<1;i++){       
                LATDbits.LATD0 = 1;
                __delay_ms(1.7);    //delay was identified through testing
                LATDbits.LATD0 = 0;
                __delay_ms(20-1.7);
            } 
        }
        //column 3
        if(temp == '3'){        
            for(i=0; i<3;i++){       
                LATDbits.LATD0 = 1;
                __delay_ms(1.6);    //delay was identified through testing
                LATDbits.LATD0 = 0;
                __delay_ms(20-1.6);
            } 
        }
        //column 4
        if(temp == 'A'){
            for(i=0; i<4;i++){       
                LATDbits.LATD0 = 1;
                __delay_ms(1.3);
                LATDbits.LATD0 = 0;
                __delay_ms(20-1.3);
            } 
        }
        LATDbits.LATD0 = 0;
        LATDbits.LATD0 = 0;
        //return
        if(temp == '*'){
            break;
        }
    }
    return;
}

void disMotors(void){
    /* This sub-function is the home to all dispensing functions. There are three
     * (3) different function calls and 1 return call:
     *                  1: R         (Corresponding to 1)
     *                      -> dispenses R
     *                  2: L         (Corresponding to 2)
     *                      -> dispenses F
     *                  3: F         (Corresponding to 3)
     *                      -> dispenses L
     *                  4: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */
    unsigned char keypress;
    unsigned char temp;
    unsigned int i;
    while(1){
        __lcd_clear();
        __lcd_home();
        printf("1: R 2: F 3: L");
        __lcd_newline();
        printf("*: Return");
        while(PORTBbits.RB1 == 0){  continue;   }

        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }

        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //R
        if(temp == '1'){
            for(i=0; i<2;i++){//call to ensure back in neutral position
               LATCbits.LATC6 = 1;
               __delay_ms(1);
               LATCbits.LATC6 = 0;
               __delay_ms(20-1);
            }
            for(i=0; i<8;i++){
                LATCbits.LATC6 = 1;
                __delay_ms(2);
                LATCbits.LATC6 = 0;
                __delay_ms(20-2);
            }
            __delay_ms(50);    //small delay to allow food piece to drop
            for(i=0; i<9;i++){
               LATCbits.LATC6 = 1;
               __delay_ms(1);
               LATCbits.LATC6 = 0;
               __delay_ms(20-1);
            }
            for(i=0; i<2;i++){//double call to ensure back in neutral position
               LATCbits.LATC6 = 1;
               __delay_ms(1);
               LATCbits.LATC6 = 0;
               __delay_ms(20-1);
            }
        }
        //F
        if(temp == '2'){
            for(i=0; i<2;i++){//call to ensure back in neutral position
               LATCbits.LATC5 = 1;
               __delay_ms(1);
               LATCbits.LATC5 = 0;
               __delay_ms(20-1);
            }
            for(i=0; i<7;i++){
               LATCbits.LATC5 = 1;
               __delay_ms(2);
               LATCbits.LATC5 = 0;
               __delay_ms(20-2);
            }
            __delay_ms(50);    //small delay to allow food piece to drop
            for(i=0; i<9;i++){
               LATCbits.LATC5 = 1;
               __delay_ms(1);
               LATCbits.LATC5 = 0;
               __delay_ms(20-1);
            }
            for(i=0; i<2;i++){//double call to ensure back in neutral position
               LATCbits.LATC5 = 1;
               __delay_ms(1);
               LATCbits.LATC5 = 0;
               __delay_ms(20-1);
            }
        }
        //L
        if(temp == '3'){
            for(i=0; i<2;i++){//call to ensure back in neutral position
                LATCbits.LATC7 = 1;
                __delay_ms(1);
                LATCbits.LATC7 = 0;
                __delay_ms(20-1);
             }
            for(i=0; i<8;i++){
               LATCbits.LATC7 = 1;
               __delay_ms(2);
               LATCbits.LATC7 = 0;
               __delay_ms(20-2);
            }
            __delay_ms(50);    //small delay to allow food piece to drop
            for(i=0; i<9;i++){
               LATCbits.LATC7 = 1;
               __delay_ms(1);
               LATCbits.LATC7 = 0;
               __delay_ms(20-1);
            }
            for(i=0; i<2;i++){//double call to ensure back in neutral position
               LATCbits.LATC7 = 1;
               __delay_ms(1);
               LATCbits.LATC7 = 0;
               __delay_ms(20-1);
            }
        }
        if(temp == '4'){
            LATDbits.LATD1 = 1;
        }
        //off
        if(temp == '5'){
            LATDbits.LATD1 = 0;
        }
        //return
        if(temp == '*'){
            break;
        }
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
    }
    return;
}

void vibMotors(void){
    /* This sub-function is the home to all vibration functions. There are two
     * (2) different function calls and 1 return call:
     *                  1: On         (Corresponding to 1)
     *                      -> turns on all vibration motors
     *                  2: Off        (Corresponding to 2)
     *                      -> turns off all vibration motors
     *                  3: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */
    
    unsigned char keypress;
    unsigned char temp;
    while(1){
        __lcd_clear();
        __lcd_home();
        printf("1: On 2: Off");
        __lcd_newline();
        printf("*: Return");
        while(PORTBbits.RB1 == 0){  continue;   }

        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }

        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //on
        if(temp == '1'){
            LATDbits.LATD1 = 1;
        }
        //off
        if(temp == '2'){
            LATDbits.LATD1 = 0;
        }
        //return
        if(temp == '*'){
            break;
        }
    }
    return;
}

void lineSensors(void){
    /* This sub-function is the home to all white tape detection. There is 1 
     * return call and constantly prints the Analog converted value as done in
     * the sample code:
     *                  1: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */

    unsigned char keypress;
    unsigned char temp;
    while(1){
        while(PORTBbits.RB1 == 0){  
            __lcd_clear();
            __lcd_home();
            
            //print values
            printf("%.3x     %.3x", readADC(0), readADC(1));
            __lcd_newline();
            printf("%.3x     %.3x", readADC(5), readADC(4));
            __delay_ms(200);
            continue;   
        }
        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }
        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //return
        if(temp == '*'){
            break;
        }
    }
    return;
}

void infraSensors(void){
     /* This sub-function is the home to detect anything in front of the 
      * infrared sensor. There is 1 return call and constantly prints the Analog 
      * converted value as done in the sample code:
     *                  1: return call which corresponds to * This can also be 
     *                     called anytime within this subfunction as long as *
     *                     is pressed. * will return to the original function
     *                     that called this subfunction
     * Arguments: none
     * 
     * Returns: none
     */

    unsigned char keypress;
    unsigned char temp;
    unsigned short input = readADC(6);

    while(1){
        while(PORTBbits.RB1 == 0){  
            __lcd_clear();
            __lcd_home();
            
            //print values
            printf("%.0x", readADC(6));
            __lcd_newline();
            printf("*: Return");
            __delay_ms(200);
            continue;   
        }
        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }
        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //return
        if(temp == '*'){
            break;
        }
    }
    return;
}
