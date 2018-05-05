
/* This source file contains all motorized functionalities. Sub-functions 
 * included are the motors responsible for dispensing the food pieces, selecting
 * which column to dispense to, and operation of the "robotic arm"
 * 
 * Edit: (March 22)
 * Addition of Vibration Motor as a subfunction that will be called prior to and 
 * after operations
 */

/********************************** Includes **********************************/
#include "motors.h"

/********************************** Functions *********************************/

void horizontalMotor(unsigned int size, unsigned int direction){
    /* This sub-function controls the horizontal motion of the Forklift
     * responsible for controlling the H-Bridge that is connected to 2 DC-Motors
     * connected to pins RC1 and RC2. Its motion is either moving forward or
     * backwards depending on the size of motion sent through variable size
     * Arguments:   size,       size is an integer value either 0 for small or 1
     *                          for big.
     *              direction,  direction is an integer value either 0 for 
     *                          forward, or 1 for backward.
     * 
     * Returns: none
     */

    enum hsize {Small = 0, Big = 1};
    enum hdirection {Forward = 0, Backward = 1, FCustom = 2, BCustom = 3};
    
//    __lcd_clear();
//    __lcd_home();
//    printf("Horizontal");
//    __lcd_newline();
    
    //determine whether the size is small or big
    switch(size){
        case(Small):
            if(direction == Forward){
                //small, forward
//                printf("Small Forward");
                LATCbits.LATC1 = 0;
                LATCbits.LATC2 = 1;
                __delay_ms(670); //delay is the amount of time the motor is on for 
            }else if(direction == Backward){
                //small, backward
//                printf("Small Backward");
                LATCbits.LATC1 = 1;
                LATCbits.LATC2 = 0;
                __delay_ms(650); //delay is the amount of time the motor is on for
            }   
            break;
        case(Big):
            if(direction == Forward){
                //big, forward
//                printf("Big Forward");
                LATCbits.LATC1 = 0;
                LATCbits.LATC2 = 1;
                __delay_ms(700); //delay is the amount of time the motor is on for
                LATCbits.LATC1 = 0;
                LATCbits.LATC2 = 0;
                __delay_ms(500);
                
                //forward small amount
                LATCbits.LATC1 = 0;
                LATCbits.LATC2 = 1;
                __delay_ms(75);
                LATCbits.LATC1 = 0;
                LATCbits.LATC2 = 0;
                __delay_ms(500);
                
                 //backward small amount
                LATCbits.LATC1 = 1;
                LATCbits.LATC2 = 0;
                __delay_ms(50);
                LATCbits.LATC1 = 0;
                LATCbits.LATC2 = 0;
                
            }else if(direction == Backward){
                //big, backward
//                printf("Big Backward");
                LATCbits.LATC1 = 1;
                LATCbits.LATC2 = 0;
                __delay_ms(700); //delay is the amount of time the motor is on for
            }   
            break;
        default:
            break;
    }
    LATCbits.LATC1 = 0;
    LATCbits.LATC2 = 0;
    return;
}

void verticalMotor(unsigned int size, unsigned int direction){
    /* This sub-function controls the vertical motion of the forklift
     * responsible for controlling the stepper motor connected to pins RE2, RC0,
     * and RC3-2
     * Arguments:       size,   size is an integer value either 0 for small or 1
     *                          for big. A small value corresponds to moving 
     *                          the "robotic arm"'s forks up or down by a small 
     *                          motion to hook onto the drawers
     *                          Apr/3/2018
     *                          integer value of how long the motor is on for
     *                          if custom motion
     *              direction,  direction is an integer value either 0 for up,
     *                          or 1 for down
     *                          Apr/3/2018
     *                          2 for custom up, 3 for custom down
     * 
     * SMALL_SIZE and BIG_SIZE  are constants that control how long the stepper 
     * motor runs for. 
     * 
     * Returns: none
     */

    enum vsize {Small = 0, Big = 1};
    enum vdirection {Up = 0, Down = 1, UCustom = 2, DCustom = 3};
    
    unsigned int i;
    
//    __lcd_clear();
//    __lcd_home();
//    printf("Vertical");
//    __lcd_newline();
    
    if(size == Small && direction == Up){
//        printf("Small, Up");
        for(i=0;i<SMALL_SIZE;i++){
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
    }else if(size == Small && direction == Down){
//        printf("Small, Down");
        for(i=0;i<SMALL_SIZE;i++){
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
    }else if(size == Big && direction == Up){
//        printf("Big, Up");
        for(i=0;i<BIG_SIZE;i++){
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
 
    }else if(size == Big && direction == Down){
//        printf("Big, Down");
        for(i=0;i<BIG_SIZE;i++){
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
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            LATEbits.LATE2 = 0;
            LATCbits.LATC0 = 1;
            __delay_ms(2);
        }
    }  
    
    if(direction == UCustom){
//        printf("Custom, Up");
        for(i=0;i<size;i++){
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
    else if(direction ==  DCustom){
//        printf("Custom, Down");
        for(i=0;i<size;i++){
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
    //Stop the motors
    LATAbits.LATA2 = 0;
    LATAbits.LATA3 = 0;
    LATEbits.LATE2 = 0;
    LATCbits.LATC0 = 0;
    return;
}

void selectorMotor(unsigned int column){
    /* This sub-function controls a positional servo motor that selects which 
     * column to dispense to connected to pin RD0
     * Arguments:     column,   column is an integer from 1 to 4, depending on 
     *                          which column was selected. The servo motor 
     *                          rotates the amount of distance required to 
     *                          "select" the designated column
     *              
     * Returns: none
     */
    //RD0
    unsigned int i;
//    __lcd_clear();
//    __lcd_home();
//    printf("Selector");
//    __lcd_newline();
//    printf("Col: %d", column);
//    __delay_ms(2000);
    
    //First make the selector in the most CCW position 
    for(i=0; i<9;i++){
        LATDbits.LATD0 = 1;
        __delay_ms(2);
        LATDbits.LATD0 = 0;
        __delay_ms(20-2);
    }
    //provide some delay to prevent an overload on the motor
    __delay_ms(500);
    
    //column 2
    if(column == 2){
        for(i=0; i<1;i++){       
            LATDbits.LATD0 = 1;
            __delay_ms(1.7);    //delay was identified through testing
            LATDbits.LATD0 = 0;
            __delay_ms(20-1.7);
        } 
    }
    //column 3
    if(column == 3){        
        for(i=0; i<3;i++){       
            LATDbits.LATD0 = 1;
            __delay_ms(1.6);    //delay was identified through testing
            LATDbits.LATD0 = 0;
            __delay_ms(20-1.6);
        } 
    }
    //column 4
    if(column == 4){
        for(i=0; i<4;i++){       
            LATDbits.LATD0 = 1;
            __delay_ms(1.3);
            LATDbits.LATD0 = 0;
            __delay_ms(20-1.3);
        } 
    }
    return;
}

void dispenseMotorsSelector(unsigned int mode, unsigned int dietType, unsigned int Amount){
    /* This sub-function is a precursor to the main dispenseMotors sub-function
     * In this sub-function, it either decodes the dietType and Amount into the
     * specific R, F, and L values that is needed to be dispensed and sends the
     * values to dispenseMotors or it continuously dispenses until no food is
     * left
     * Arguments:     mode,   integer value either 0 or 1. When mode is 0, this
     *                        sub-function decodes the dietType and Amount into 
     *                        its R, F and L values and sends it to the 
     *                        dispenseMotors sub-function to commence dispensing
     *                        If mode is 1, this sub-function loops the 
     *                        dispenseMotors sub-function for each food-piece
     *                        until there is no food pieces left in each.
     *            dietType,   integer value of the diet type entered by operator
     *            Amount,     integer value of the amount entered by operator
     * Returns: none
     */
    
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
    unsigned int c4;
    
    enum currMode {single = 0, continuous = 1};
    
    //say Amount is 1234
    c1 = Amount/1000;                                   //1
    c2 = ((Amount-Amount/1000*1000)/100);               //2
    c3 = ((Amount-Amount/100*100)/10);                  //3
    c4 = ((Amount-Amount/10*10));                       //4 
    
    if(mode == continuous){
        /*dispensing the remainder of the foods. Calls upon infraredSensor() to 
         * check if any object has dropped for each food piece. If there is, 
         * keep looping for that food piece until there is no food piece left. 
         * Cycle for all three and increment while it is not zero*/
        selectorMotor(3);
        do{
            //check if there's an interrupt, if so, skip the dispensing
            if(interruptCheck == 1){
                break;
            }
            //Dispense R first
            dispenseMotors(1,0,0);
            leftovers[1] += 1;
        }while(infraredSensor() != 0);
        //decrease by 1 since it over counts
        leftovers[1] -= 1;
        selectorMotor(2);
        do{
            //check if there's an interrupt, if so, skip the dispensing
            if(interruptCheck == 1){
                break;
            }
            //Dispense F next
            dispenseMotors(0,1,0);
            leftovers[2] += 1;
        }while(infraredSensor() != 0);
        //decrease by 1 since it over counts
        leftovers[2] -= 1;
        selectorMotor(1);
        do{
            //check if there's an interrupt, if so, skip the dispensing
            if(interruptCheck == 1){
                break;
            }
            //Dispense L last
            dispenseMotors(0,0,1);
            leftovers[3] += 1;
        }while(infraredSensor() != 0);
        //decrease by 1 since it over counts
        leftovers[3] -= 1;
        return;
    }else if(mode == single){
        /*dispensing the food pieces according to diet type and amount
         * The diet type and amount are coded according to how the information
         * was saved into its variables. Due to this, it must be decoded as
         * follows:
            case 1    R
            case 2    F
            case 3    L
         
            case 4    RF
            case 5    RL
            case 6    FL

            c1,c2,c3 all are either 1 or 2 (2)
            case 7    RRF
            case 8    RRL
            case 9    RFF
            case 10   RLL
            case 11   RFL
            case 12   FFL
            case 13   FLL

            c1 to c4 all 1
            case 14   RRFL
            case 15   RFFL
            case 16   RFLL
            case 17   RLLL
            case 18   FLLL
         */
        switch(dietType){
            case 1:            dispenseMotors(c1,0,0);                 break;
            case 2:            dispenseMotors(0,c1,0);                 break;
            case 3:            dispenseMotors(0,0,c1);                 break;

            case 4:            dispenseMotors(c1,c2,0);                break;
            case 5:            dispenseMotors(c1,0,c2);                break;
            case 6:            dispenseMotors(0,c1,c2);                break;

            case 7:            dispenseMotors(c1+c2,c3,0);             break;
            case 8:            dispenseMotors(c1+c2,0,c3);             break;
            case 9:            dispenseMotors(c1,c2+c3,0);             break;
            case 10:            dispenseMotors(c1,0,c2+c3);            break;
            case 11:            dispenseMotors(c1,c2,c3);              break;
            case 12:            dispenseMotors(0,c1+c2,c3);            break;
            case 13:            dispenseMotors(0,c1,c2+c3);            break;

            case 14:            dispenseMotors(c1+c2,c3,c4);           break;
            case 15:            dispenseMotors(c1,c2+c3,c4);           break;
            case 16:            dispenseMotors(c1,c2,c3+c4);           break;
            case 17:            dispenseMotors(c1,0,c2+c3+c4);         break;
            case 18:            dispenseMotors(0,c1,c2+c3+c4);         break;
            default:                                                   break;
        }
        return;
    }
}

void dispenseMotors(unsigned int R, unsigned int F, unsigned int L){
    /* This sub-function is main dispenseMotors sub-function that controls 3 
     * positional servo motors, one for each food piece. Pin RC6 corresponds to 
     * R, RC5 with F, and RC7 with L.
     * Arguments:       R,  integer value that represents how many R pieces are
     *                      needed to be dispensed
     *                  F,  integer value that represents how many F pieces are
     *                      needed to be dispensed
     *                  L,  integer value that represents how many L pieces are
     *                      needed to be dispensed                      
     * Returns: none
     */
    
    unsigned int i;
    enum motorState {off = 0, on = 1};
    
//    __lcd_clear();
//    __lcd_home();
//    printf("R:%d F:%d L:%d", R,F,L);
//    __delay_ms(2000);
    
    //for non zeros R, F, or L, turn the appropriate servo motor 
    while(R > 0){
        //RC6
        __delay_ms(1000);
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
        R = R - 1;
//        __lcd_clear();
//        __lcd_home();
//        printf("R:%d F:%d L:%d", R,F,L);
//        __delay_ms(2000);
        
    }
    while(F > 0){
        //RC5
        __delay_ms(1000);
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
        F = F - 1;
//        __lcd_clear();
//        __lcd_home();
//        printf("R:%d F:%d L:%d", R,F,L);
//        __delay_ms(2000);
        
    }
    while(L > 0){
        //RC7
        __delay_ms(1000);
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
        L = L - 1;
//        __lcd_clear();
//        __lcd_home();
//        printf("R:%d F:%d L:%d", R,F,L);
//        __delay_ms(2000);
    }
    //Disable motors
    LATCbits.LATC5 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;

    return;
}

void vibrationMotors(unsigned int state){
    /* This sub-function is a switch that turns on and off all the vibration
     * motors depending on what type of input it receives. The switch is
     * associated with pin RD1.
     * 
     * Arguments: state     state is an integer value either 0 or 1, 0 being off
     *                      and 1 being on.
     * 
     * Returns: none
     */
    //RD1
    enum motorState {off = 0, on = 1};
    if(state==on){     
        LATDbits.LATD1 = 1;
    }else{
        LATDbits.LATD1 = 0;
    } 
    return;
}