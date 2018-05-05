/*
 * File:   main.c
 * Author: calvi
 *
 * Created on February 2, 2018, 2:44 PM
 */


/***** Includes *****/
#include "main.h"

void main(void){
    
    // <editor-fold defaultstate="collapsed" desc="Machine Configuration">
    /********************************* PIN I/O ********************************/
    /* Write outputs to LATx, read inputs from PORTx. Here, all latches (LATx)
     * are being cleared (set low) to ensure a controlled start-up state. */  
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    /* After the states of LATx are known, the data direction registers, TRISx
     * are configured. 0 --> output; 1 --> input. Default is  1. */
    TRISA = 0b11110011; /* RA0,1,5 input for Line Sensor
                         * RA2,3 output for Stepper Motor
                         */
    TRISB = 0xFF;       /* RB1,4-7 input for keypad
                         * for the sake of simplicity, make all RB pins input
                         */
    TRISC = 0b00011000; /* RC0 output for Stepper Motor
                         * RC1-2 output for Horizontal Motor/H-Bridge/DC Motors
                         * RC3-4 input for RTC
                         * RC5-7 output for Dispensing Motors
                         */
    TRISD = 0x00;       /* RD0 output for Selector Motor
                         * RD1 output for Vibration Motor switch
                         * RD2-7 output for LCD
                         */
    TRISE = 0b011;      /* RE0 input for Line Sensor
                         * RE1 input for Infrared Sensor
                         * RE2 output for stepper
                         */
    
    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0b1000;  // RA0-3, RA5, RE0 and RE1 are analog (pg. 224 of the PIC18F4620 datasheet)
    //only RE1,0,RA5,1,0 are used as analog inputs
    ADCON2bits.ADFM = 1; // Right justify A/D result
    ADCON2bits.ADCS = 0b010; //set T_AD as low as possible but also above minimum requirement (see pg. 231 of the PIC18F4620 datasheet)
    // </editor-fold>
    
    /* Initialize character LCD. */
    initLCD();
    __lcd_display_control(1, 0, 0);
    
    I2C_Master_Init(100000); //Initialize I2C Master with 100 kHz clock
    di(); // Disable all interrupts
    
    /* Set the time in the RTC.
     * 
     * To see the RTC keep time, comment this line out after programming the PIC
     * directly before with this line included. */
//    RTC_setTime();
    
    // <editor-fold defaultstate="collapsed" desc="Local Variable Declarations">
    /* Declare local variables. */

    unsigned char keypress;
    unsigned char temp;
        
    unsigned char drawerTemp;
    unsigned char drawerTemp2;
    unsigned int intDrawerTemp;
    
    unsigned char dietTemp;
    unsigned char dietTemp2;
    unsigned int intDietTemp;
        
    unsigned char amountTemp;
    unsigned char amountTemp2;
    unsigned char amountTemp3;
    unsigned char amountTemp4;
    unsigned int intAmountTemp;
    
    unsigned int drawerCounter;
    unsigned int maxCharLen;
    unsigned int dietAmountLen;
    
    enum options{optionDrawer=0,optionDiet=1,optionAmount=2};

    
    unsigned int year;
    unsigned int month;
    unsigned int dayNumber;
    unsigned int dayWord;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    
    unsigned int i;
    // </editor-fold>
    
    //Assumed that the initial state of machine is at top row
    currRow = 0;
    timerCounter = 0;
    
    /* Main loop. */
    __lcd_clear();
    __lcd_home();
    while(1){
        
        // <editor-fold defaultstate="collapsed" desc="Reset">
//        __lcd_clear();
//        __lcd_home();
//        printf("Resetting");
//        __lcd_newline();
//        printf("Please wait...");
       
        //Local Variable Reset
        
        keypress = NULL;
        temp = NULL;
        
        drawerTemp = NULL;
        drawerTemp2 = NULL;
        intDrawerTemp = NULL;
        
        dietTemp = NULL;
        dietTemp2 = NULL;
        intDietTemp = NULL;
        
        amountTemp = NULL;
        amountTemp2 = NULL;
        amountTemp3 = NULL;
        amountTemp4 = NULL;
        intAmountTemp = NULL;
        
        drawerCounter = NULL;
        maxCharLen = 0;
        dietAmountLen = NULL;
        
        year = 0;
        month = 0;
        dayNumber = 0;
        dayWord = 0;
        hour = 0;
        minute = 0;
        second = 0;
        
        __delay_ms(1500);
        
        //global variable reset
        resetn();
        
        // </editor-fold>
        
        INTCON3bits.INT1IE = 1; // Enable RB1 (keypad data available) interrupt
//        INT1IE = 1; // Enable RB1 (keypad data available) interrupt
        ei(); // Enable all interrupts
        do{
            calculateTime();

            /* Print received time to LCD. */
            __lcd_clear();
            __lcd_home();
            // Print date in MM/DD/YY
            printf("*:Begin %02x/%02x/%02x", time[5],time[4],time[6]);
            __lcd_newline();
            // HH:MM:SS
            printf("#:Logs  %02x:%02x:%02x", time[2],time[1],time[0]);
            __delay_ms(1000);
        }while(interruptCheck != '*' && interruptCheck != '#' && interruptCheck != 'D');
        
        INTCON3bits.INT1IE = 0; // Disable RB1 (keypad data available) interrupt
        di(); // Disable all interrupts

        __lcd_clear();
        __lcd_home();
        
        //Check if input was # or D for developer mode
        if(interruptCheck == '#'){         operationLogs(); }
        else if(interruptCheck == 'D'){    developerMode(); }
        else{
            //Welcome message
            __lcd_clear();
            __lcd_home();
            printf("Bienvenue a");
            __lcd_newline();
            printf("notre machine");
            __delay_ms(500);
            //start gathering inputs
            do{
                // <editor-fold defaultstate="collapsed" desc="Drawer">
                do{
                //reset values
                maxCharLen = 0;
                drawerTemp = NULL;
                drawerTemp2 = NULL;
                intDrawerTemp = NULL;
                //turn on blinking cursor to indicate putchable input
                __lcd_display_control(1, 1, 1);
                __lcd_clear();
                __lcd_home();
                //display message
                lcd_set_cursor(0, 1);
                printf("*:Next  C:Finish");
                lcd_set_cursor(0, 0);
                printf("Drawer:   (1-16)");
                lcd_set_cursor(7, 0);
                    do{
                        while(PORTBbits.RB1 == 0){  continue;   }

                        /* Read the 4-bit character code. */
                        keypress = (PORTB & 0xF0) >> 4;
                        /* Wait until the key has been released. */
                        while(PORTBbits.RB1 == 1){  continue;   }

                        Nop();  // Apply breakpoint here to prevent compiler optimizations
                        temp = keys[keypress];
                        //check if input was C, if so break out of while loop
                        if(temp == 'C'){
                            break;
                        }
                        //check if maximum length is reach, if so void inputs
                        if(maxCharLen == 2){
                            continue;
                        //check if inputs are A,B,D or # , if so void inputs
                        }else if(temp == 'A' || temp == 'B' || temp == 'D'|| temp == '#'){
                            continue;
                        //check if the length is already 1, if so void inputs 
                        // that are 7, 8, and 9 since inputs cannot be x7,x8,x9
                        }else if(maxCharLen == 1 && (temp == '7' || temp == '8' || temp == '9')){
                            continue;
                        //disallow inputs of 0 being the first character, 
                        //starting without any drawer inputs (* and C)
                        }else if(maxCharLen == 0 && temp != '0' && temp != '*' && temp != 'C'){
                            //if the first input is not 1, (i.e. 2-9), disable
                            //blinking cursor to indicate maximum length achieved
                            if(temp != '1'){
                                __lcd_display_control(1, 0, 0);
                            }
                            //save the input, increase the length, display the key
                            drawerTemp = temp;
                            maxCharLen++;
                            putch(temp);
                        /*If the program reaches this part of the code, it means
                         * there is something already displayed on the LCD. 
                         * Since the only two digit number can be 1x, the 
                         * first digit, or drawerTemp, must be 1. Due to the if
                         * statements prior, the only inputs that reach here are
                         * numbers ranging form 0 to 6, or the allowable values
                         * for the ones digits for the drawers, 10-16
                         */
                        }else if(drawerTemp == '1' && temp != '*'){
                            __lcd_display_control(1, 0, 0);
                            //save the input, increase the length, display the key
                            drawerTemp2 = temp;
                            maxCharLen++;
                            putch(temp);
                        }
                    //loop while inputs are not * and there is something inputed
                    }while(maxCharLen == 0 || temp != '*');
                    //convert the input into an integer value
                    intDrawerTemp = charToInt(drawerTemp,drawerTemp2);
                    //check if the input was C, if so break out of the validation
                    // of drawers loop
                    if(temp == 'C'){
                        break;
                    }
                //loop while the current input drawer is not valid (only loops
                // while the current input drawer was already selected)
                }while(validInput(optionDrawer,intDrawerTemp,0,0) != 1);
                // </editor-fold>
                //reset values, disable cursor from blinking
                maxCharLen = 0;
                __lcd_display_control(1, 1, 1);
                __lcd_clear();
                __lcd_home();
                               
                // <editor-fold defaultstate="collapsed" desc="Early Start">
                /*Here, we check whether or not the program can start early
                 * an early start can occur if there is at least one drawer 
                 * instruction saved
                 */
                if(temp == 'C'){
                    if(drawerCounter != 0){                        
                        /*here, there is at least one drawer as an input saved, 
                         *thus breaking out of the input loop and onto waiting
                         *for a command to begin packing
                         */
                        break;
                    }
                    /*here, the minimum requirement of at least one drawer was
                     * not met and thus must return to entering drawers. An error
                     * message is displayed, indicating this, and the input loop
                     * will bring the program back to the drawer input code
                     */
                    __lcd_display_control(1, 0, 0);
                    drawerTemp = NULL;
                    drawerTemp2 = NULL;
                    printf("No drawers");
                    __lcd_newline();
                    printf("entered!");
                    __delay_ms(1500);
                    __lcd_clear();
                    __lcd_home();
                    __lcd_display_control(1, 1, 1);
                }
                // </editor-fold>
                
                else{                    
                    // <editor-fold defaultstate="collapsed" desc="Diet Type">
                    //reset values
                    maxCharLen = 0;
                    dietTemp = NULL;
                    dietTemp2 = NULL;
                    intDietTemp = NULL;
                    lcd_set_cursor(0, 1);
                    printf("*:Next    (1-18)");
                    lcd_set_cursor(0, 0);
                    printf("Diet Type:");

                     do{
                        while(PORTBbits.RB1 == 0){  continue;   }

                        /* Read the 4-bit character code. */
                        keypress = (PORTB & 0xF0) >> 4;
                        /* Wait until the key has been released. */
                        while(PORTBbits.RB1 == 1){  continue;   }

                        Nop();  // Apply breakpoint here to prevent compiler optimizations
                        temp = keys[keypress];
                        //check if maximum length is reach, if so void inputs
                        if(maxCharLen == 2){
                            continue;
                        //check if inputs are A,B,C, D or # , if so void inputs
                        }else if(temp == 'A' || temp == 'B' || temp == 'C'|| temp == 'D'|| temp == '#'){
                            continue;
                        //check if the length is already 1, if so void inputs 
                        // that is 9 since inputs cannot be x9
                        }else if(maxCharLen == 1 && temp == '9'){
                            continue;
                        //disallow inputs of 0 being the first character, 
                        //starting without any drawer inputs (*)
                        }else if(maxCharLen == 0 && temp != '0' && temp != '*'){
                            //if the first input is not 1, (i.e. 2-9), disable
                            //blinking cursor to indicate maximum length achieved
                            if(temp != '1'){
                                __lcd_display_control(1, 0, 0);
                            }
                            //save the input, increase the length, display the key
                            dietTemp = temp;
                            maxCharLen++;
                            putch(temp);
                            
                         /*If the program reaches this part of the code, it means
                         * there is something already displayed on the LCD. 
                         * Since the only two digit number can be 1x, the 
                         * first digit, or dietTemp, must be 1. Due to the if
                         * statements prior, the only inputs that reach here are
                         * numbers ranging form 0 to 8, or the allowable values
                         * for the ones digits for the diet types, 10-18
                         */    
                        }else if(dietTemp == '1' && temp != '*'){
                            __lcd_display_control(1, 0, 0);
                            dietTemp2 = temp;
                            maxCharLen++;
                            putch(temp);
                        }
                    //loop while inputs are not * and there is something inputed
                    }while(maxCharLen == 0 || temp != '*');
                    
                    //convert the input into an integer value
                    intDietTemp = charToInt(dietTemp,dietTemp2);
                    //check if it is a valid input (already is, so this is just 
                    // a saving mechanism)
                    validInput(optionDiet,intDietTemp,intDrawerTemp,0);
                    //reset values
                    maxCharLen = 0;
                    __lcd_display_control(1, 1, 1);
                    __lcd_clear();
                    __lcd_home();
                    // </editor-fold>
                    
                    // <editor-fold defaultstate="collapsed" desc="Amount">
                    
                    do{
                        //reset values and allow cursor to blink
                        __lcd_clear();
                        __lcd_home();
                        __lcd_display_control(1, 1, 1);
                        maxCharLen = 0;
                        amountTemp = NULL;
                        amountTemp2 = NULL;
                        amountTemp3 = NULL;
                        amountTemp4 = NULL;
                        intAmountTemp = NULL;
                        lcd_set_cursor(0, 1);
                        
                        printf(" (1-3 per piece)");
                        lcd_set_cursor(0, 0);
                        printf("Amount:");
                        
                        //determine how many inputs are required for amount 
                        //by the length of diet type
                        if(intDietTemp<=3){          dietAmountLen = 1; //less than 3 is len 1
                        }else if(intDietTemp<=6){    dietAmountLen = 2; // 4-6 is len 2
                        }else if(intDietTemp<=13){   dietAmountLen = 3; // 7-13 is len 3
                        }else{                       dietAmountLen = 4; // rest is len 4
                        }
                        do{
                           while(PORTBbits.RB1 == 0){  continue;   }

                           /* Read the 4-bit character code. */
                           keypress = (PORTB & 0xF0) >> 4;
                           /* Wait until the key has been released. */
                           while(PORTBbits.RB1 == 1){  continue;   }

                           Nop();  // Apply breakpoint here to prevent compiler optimizations
                           temp = keys[keypress]; 
                           //limit inputs to only be 1,2 or 3 since those are the only allowable ones
                           if(dietAmountLen != 0  && (temp == '1' || temp == '2' || temp == '3')){
                               /*Here, it checks whether or not there is something
                                * saved on the lowest number temporary variable
                                * if not, save it there
                                */
                               if(amountTemp == NULL){
                                   amountTemp = temp;
                               }
                               else if(amountTemp2 == NULL){
                                   amountTemp2 = temp;
                               }
                               else if(amountTemp3 == NULL){
                                   amountTemp3 = temp;
                               }
                               else{
                                   amountTemp4 = temp;
                               }
                               //display the input and decrement the length left
                               putch(temp);
                               dietAmountLen--;
                            }
                           //disable the cursor from blinking once there is none left
                            if(dietAmountLen == 0){
                               __lcd_display_control(1, 0, 0);
                            }
                        //keep looping until there the length is 0
                        }while(dietAmountLen != 0);
                        __delay_ms(1000);
                        //convert the inputs into an integer
                        intAmountTemp = 1000*charToInt(amountTemp,NULL) 
                                + 100*charToInt(amountTemp2,NULL) 
                                + 10*charToInt(amountTemp3,NULL) 
                                + charToInt(amountTemp4,NULL);   
                    //keep looping until this input is valid
                    }while(validInput(optionAmount,intAmountTemp,intDrawerTemp,intDietTemp) != 1);
                
                    // </editor-fold>
                    //once the code reaches here, it is guaranteed to be a valid input
                    drawerCounter++;
                }
            }while(drawerCounter < 8);

            //8 drawers entered or C was pressed           
            __lcd_clear();
            __lcd_home();
            __lcd_display_control(1, 0, 0);
            lcd_set_cursor(0, 0);
            printf("Press D to Start");
            
            //keep looping until the input is D, which is to start packing
            do{
                while(PORTBbits.RB1 == 0){  continue;   }

                /* Read the 4-bit character code. */
                keypress = (PORTB & 0xF0) >> 4;
                /* Wait until the key has been released. */
                while(PORTBbits.RB1 == 1){  continue;   }

                Nop();  // Apply breakpoint here to prevent compiler optimizations
                temp = keys[keypress];
            }while(temp != 'D');
            __lcd_clear();
            __lcd_home();
            
            //save the current time
            calculateTime();
            year = time[6];
            month = time[5];
            dayNumber = time[4];
            dayWord = time[3];
            hour = time[2];
            minute = time[1];
            second = time[0];
            
            printf("  <Dispensing>");
            
            //enable interrupt bit for a timer to count for 3min just in case it fails
            //enable all interrupts
            ei();
            //see PIC18F datasheet section 11.3
            INTCONbits.TMR0IE = 1;      //enable TMR0 Interrupt
            T0CONbits.T08BIT = 0;       //set to 16 bit
            T0CONbits.T0CS = 0;         //timer mode (increment every clock edge)
            T0CONbits.TMR0ON = 1;       //enable TMR0
            T0CONbits.PSA = 0;          // clear PSA bit to assign prescaler to TMR0
            T0CONbits.T0PS = 0b111;     //Set prescaler value to 1:256, see page 123 of datasheet (Sec 11.1)
            T0CONbits.T0SE = 0;         //Set to rising edge
            interruptCheck = NULL;       //reset interruptCheck
            
            dispenseFood();
            
            INTCONbits.TMR0IE = 0;      //disable TMR0 Interrupt
            di();                       //disable all interrupts
            
            //return from dispensing food
            __lcd_clear();
            __lcd_home();
            if(interruptCheck == 1){
                printf("  TERMINATION!  ");
            }else{
                printf("   Successful   ");
                lcd_set_cursor(0, 1);
                printf("     Pack!!     ");
            }
            //calculate how long the operation was and store it into memory
            calculateTime();
            opTime = (__bcd_to_num(time[2]))*60*60 + (__bcd_to_num(time[1]))*60 + (__bcd_to_num(time[0]))
                    -((__bcd_to_num(hour))*60*60 + (__bcd_to_num(minute))*60 + (__bcd_to_num(second)));
            write_EEPROM(1,opTime);
            
            //store the date and time the operation first occurred
            write_EEPROM(25,second);
            write_EEPROM(26,minute);
            write_EEPROM(27,hour);
            write_EEPROM(28,(dayWord<<5) + month);
            write_EEPROM(29,dayNumber);
            write_EEPROM(30,year);
            
            __delay_ms(2000);
            __lcd_clear();
            __lcd_home();
            //Display how long the Operation took
            printf("Operation Time:");
            __lcd_newline();
            printf("%d m %d s",opTime/60,opTime%60);
            __delay_ms(2000);
        }
    }    
}

void resetn(void){
    
    unsigned int i;
    
    enum vMotorSize{Small = 0, Big = 1};
    enum vMotorDirection{Up = 0, Down = 1};
    
    //reset global variables
    interruptCheck = NULL;
    opTime = 0;
    for(i = 0; i<17; i++){
        drawer[i] = 0;
        diet[i] = 0;
        amount[i] = 0;
        marked[i] = 0;
    }
    
    for(i = 0; i<4; i++){
        leftovers[i] = 0;
    }
    
    currRow = 0;
    
    //set the timer counter to 0
    timerCounter = 0;

}

unsigned int validInput(unsigned int option, unsigned int temp, unsigned int currDrawer, unsigned int currDiet){
    /* This sub-function is called from the main function. It is used to check
     * the validity of the drawer, diet and amount inputs based on what was
     * previously entered and constraints placed on the input.
     * 
     * Arguments:   option,         integer value that is between 0 and 2. 
     *                              Three options are drawer, diet, or amount
     *              temp,           integer value that stores the temporary 
     *                              value whether it being the drawer, diet or 
     *                              amount.
     *              currDrawer,     integer value that represents the current
     *                              drawer the is being checked against
     *              currDiet,       integer value that represents the current
     *                              diet the is being checked against
     * Returns: integer value, 1 through 7 depending on which log was selected
     */
    
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
    unsigned int c4;
    unsigned int valid = 0;
    
    enum options{optionDrawer=0,optionDiet=1,optionAmount=2};
    enum validity{invalidInput=0,validInput=1};
    
    switch(option){
        case optionDrawer:
            if(drawer[temp] == 1){
                __lcd_clear();
                __lcd_home();
                printf("    Invalid!    ");
               __delay_ms(500); 
               return invalidInput;
            }
            else{
                drawer[temp] = 1;
            }
            return validInput;
            
        //option was Diet
        case optionDiet:
            //save diet into current drawer
            diet[currDrawer] = temp;
            return validInput;
            
        //option was Amount 
        case optionAmount:
            //disassemble amount input into individual number
            //say temp is 1234
            c1 = temp/1000;                                 //1
            c2 = ((temp-temp/1000*1000)/100);               //2
            c3 = ((temp-temp/100*100)/10);                  //3
            c4 = ((temp-temp/10*10));                       //4      
            //print drawer instructions to display for operator
            __lcd_clear();
            __lcd_home();
            printf("Your Input: D%d,", currDrawer);
            lcd_set_cursor(0, 1);
            printf("Diet:%d A:%d%d%d%d", currDiet,c1,c2,c3,c4);
            __delay_ms(500);
            
            //check if sum of each individual number is within maximum 4 limit
            if((c1+c2+c3+c4) <= 4){
                /*break down each diet type case into the what each value is
                 * allowed to be. See below to see breakdown of each diet type.
                 */
                switch(currDiet){
                    /*
                     1. c1 to c4 must be from 1 to 3
                     2. c1 to c4 is already <= 4 (due to if statement above)
                     R, F cannot be 3
                     L can be anything (since inputs are from 1 to 3)
                     blank symbolizes no restraints
                     case 1     cant be 3                       R
                     case 2     cant be 3                       F
                     case 3                                     L
                     
                     case 4     both cant be 3                  RF
                     case 5     c1 cant be 3                    RL
                     case 6     c1 cant be 3                    FL

                    c1,c2,c3 all are either 1 or 2 (2)
                     case 7     c1,c2 cant be 2                 RRF
                     case 8     c1,c2 cant be 2                 RRL
                     case 9     c2,c3 cant be 2                 RFF
                     case 10                                    RLL
                     case 11                                    RFL
                     case 12    c1,c2 cant be 2                 FFL
                     case 13                                    FLL
                     
                     c1 to c4 all 1
                     case 14                                    RRFL
                     case 15                                    RFFL
                     case 16                                    RFLL
                     case 17                                    RLLL
                     case 18                                    FLLL
                     */
                    //single
                    case 1:
                        if(c1 == 3){                break;  }
                        valid = validInput;
                    case 2:
                        if(c1 == 3){                break;  }
                        valid = validInput;
                        
                    //double
                    case 4:
                        if(c1 == 3 || c2 == 3){     break;  }
                        valid = validInput;
                    case 5:
                        if(c1 == 3){                break;  }
                        valid = validInput;
                    case 6:
                        if(c1 == 3){                break;  }
                        valid = validInput;
                        
                    //triple  
                    case 7:
                        if(c1 != 1 || c2 != 1){     break;  }
                        valid = validInput;
                    case 8:
                        if(c1 != 1 || c2 != 1){     break;  }
                        valid = validInput;
                    case 9:
                        if(c2 != 1 || c3 != 1){     break;  }
                        valid = validInput;
                    case 12:
                        if(c1 != 1 || c2 != 1){     break;  }
                        valid = validInput;
                        
                    //quadruple
                    default:
                        valid = validInput;
                        break;
                }
                //check if valid variable was valid
                if(valid == 1){
                    amount[currDrawer] = temp;
                    __lcd_clear();
                    __lcd_home();
                    printf("   Successful   ");
                    lcd_set_cursor(0, 1);
                    printf("     Input!     ");
                    __delay_ms(500); 
                    return validInput;
                }
            }
            //valid variable was invalid
            __lcd_clear();
            __lcd_home();
            printf("    Invalid!    ");
            __delay_ms(500); 
            return invalidInput;
            break;
            
        default:
            break;
    }       
}

void dispenseFood(void){
    /* This sub-function calls all the sub-functions from the motor and sensor 
     * source file through an iterative pattern. Initially, the function turns
     * on all the vibration motors to start vibrating. Then, starting at the top
     * column, this function calls upon the horizontal motors to move forward,
     * sense for marked drawers, activate the vertical motors to move up by a 
     * small amount,  move the horizontal motors back by a small amount (to open
     * the drawers if necessary), compare the sensed marked drawers with any drawer 
     * instructions and determines whether it can dispense to that column, 
     * move vertical motors down by a small amount, and finally move the 
     * horizontal motors back. It repeats this process three times until the 
     * "robotic arm" is at the bottom, then starts dispensing the remainder of
     * the food-pieces while counting. Once all the packaging has occurred, the
     * vibration motors are turned off, the information of logs is shifted and 
     * stored into the EEPROM
     * 
     * Arguments: none
     * 
     * Returns: none
     */
       
    unsigned int n;
    unsigned int i;
    unsigned int currColumn;
    unsigned int drawerLog;
    unsigned int markedLog;
    
    enum vibrationMotorState {off = 0, on = 1};
    enum motorSize{Small = 0, Big = 1};
    enum hMotorDirection{Forward = 0, Backward = 1};
    enum vMotorDirection{Up = 0, Down = 1};
    enum dispenseMotor{single = 0, continuous = 1};
    
    /* definition of coordinate system  
     *             Column (currColumn)
     *          1   2   3   4
     *        0
     *  R     1
     *  o     2
     *  w     3
     * (currRow)
     */
    //turn on vibration motors
    vibrationMotors(on);
    //start at the row 0, move down to row 3
    for(currRow=0;currRow<4;currRow++){
        //always check if the time is over the limit, if so, exit main loop
        if(interruptCheck == 1){
                break;
            }
        
        // <editor-fold defaultstate="collapsed" desc="Marked Drawer Sequence">
            /*The goal is to move the "robotic arm" towards the cabinet and sense 
             * the marked drawers 
             */
        
        //horizontal motors big, forward  
        horizontalMotor(Big, Forward);
        __delay_ms(500);
        //Line Sensor read and save into marked[] for current row
        lineSensor(currRow);
        __delay_ms(500);
    
        // </editor-fold>   
       
        // <editor-fold defaultstate="collapsed" desc="Opening Drawer Sequence">
        /*The goal is to open the cabinet
         */
         //always check if the time is over the limit, if so, exit main loop
        if(interruptCheck == 1){
            break;
        }
        // check if the current row has any drawers at all (reduce opening it if
        // empty)
        if(drawer[4*currRow+1]==1&& marked[4*currRow+1]==0||
            drawer[4*currRow+2]==1&& marked[4*currRow+2]==0||
            drawer[4*currRow+3]==1&& marked[4*currRow+3]==0||
            drawer[4*currRow+4]==1&& marked[4*currRow+4]==0){ 
            //Vertical motors up small
            verticalMotor(Small, Up);
            __delay_ms(500);
            //horizontal motors small, backward
            horizontalMotor(Small, Backward);
            __delay_ms(500);
        
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Dispensing Sequence">
            //loop from column 1 to column 4 to check if a drawer instruction is in
            // current row
            for(currColumn=1;currColumn<5;currColumn++){
                //always check if the time is over the limit, if so, exit this loop
                if(interruptCheck == 1){
                    break;
                }
                
                //check if drawer instruction is true and not marked drawer
                if(drawer[4*currRow+currColumn]==1 && marked[4*currRow+currColumn]==0){
                    //drawer input was not marked
                    //move selectorMotor to current column
                    selectorMotor(currColumn);
                    __delay_ms(500);

                    //send diet and amount instruction for this drawer to dispenser
                    dispenseMotorsSelector(single,
                            diet[4*currRow+currColumn],amount[4*currRow+currColumn]);
                    
                    //time required for all food pieces to get into drawer
                    __delay_ms(3000);
                }
            }
        // </editor-fold>   

    // <editor-fold defaultstate="collapsed" desc="Closing Drawer Sequence">
            /*Now that the drawers have been sensed and filled, it is time to close
             * the drawers and move the "robotic arm" away from the cabinet
             */

            //horizontal motors small, forwards
            horizontalMotor(Small, Forward);
            __delay_ms(500);
            //Vertical motors, small down
            verticalMotor(Small,Down);
            __delay_ms(500);
        }
        else{
            //add a small delay between detecting and approaching the drawer
            __delay_ms(500);
        }
        //horizontal motors Big, backwards    
        horizontalMotor(Big, Backward);
        __delay_ms(500);
    // </editor-fold>
        
        /*Here, we check whether the "robotic arm" has hit the bottom row, as 
         * described in the definition of the coordinate system
         */
        if(currRow != 3){
            //always check if the time is over the limit, if so, do not move down
            if(interruptCheck != 1){
                //Vertical motors, big down
                verticalMotor(Big,Down);
                __delay_ms(500);
            }
        }
    }
    //The packing is now complete OR time will exceed maximum allowed duration
    
    //check if there's an interrupt, if so, skip the dispensing leftovers
    if(interruptCheck == 1){
        //There was an interrupt, do nothing
    }else{
        //Dispense the rest of the food pieces and start counting
        dispenseMotorsSelector(continuous,0,0);
    }
    //every operation requiring vibration motors are completed, set state to off
    vibrationMotors(off);
    
    // <editor-fold defaultstate="collapsed" desc="Memory Storage">

    //shift the logs to the next slots (every 30 bytes are the storages)
    for(i=7;i>0;i--){
        for(n=1;n<memoryGap+1;n++){
            write_EEPROM(n+memoryGap*i,read_EEPROM(n+(memoryGap*(i-1))));
        }
    }
    
    //storage of information from current operation into EEPROM
    
    //markedDrawers
    //binary array to decimal
    markedLog = marked[8] + marked[7]*2 + marked[6]*4 + marked[5]*8 +
            marked[4]*16 + marked[3]*32 + marked[2]*64 + marked[1]*128;
    write_EEPROM(2,markedLog);
    markedLog = marked[16] + marked[15]*2 + marked[14]*4 + marked[13]*8 +
            marked[12]*16 + marked[11]*32 + marked[10]*64 + marked[9]*128;
    write_EEPROM(3,markedLog);
        
    //R,F,L
    write_EEPROM(4,leftovers[1]);
    write_EEPROM(5,leftovers[2]);
    write_EEPROM(6,leftovers[3]);
        
    //Drawers
    //binary array to decimal
    drawerLog = drawer[8] + drawer[7]*2 + drawer[6]*4 + drawer[5]*8 +
            drawer[4]*16 + drawer[3]*32 + drawer[2]*64 + drawer[1]*128;
    write_EEPROM(7,drawerLog);
    drawerLog = drawer[16] + drawer[15]*2 + drawer[14]*4 + drawer[13]*8 +
            drawer[12]*16 + drawer[11]*32 + drawer[10]*64 + drawer[9]*128;
    write_EEPROM(8,drawerLog);
    
    //diet and amount
    unsigned int c = 0;
    for(i=1;i<17;i++){
        if(drawer[i]==1){
            write_EEPROM(9+c,diet[i]);
            write_EEPROM(17+c,amountEncoder(i));
            c++;
        }
    }
    // </editor-fold>
    
    return;
}

void interrupt interruptHandler(void){
    /* This function is mapped to the interrupt vector, and so is called when
     * any interrupt occurs to handle it. Note that if any interrupt is enabled,
     * and it does not have a handler to clear it, then unexpected behavior will
     * occur, perhaps even causing the PIC to reset.
     *
     * Arguments: none
     * 
     * Returns: none
     */
    
    //from sample code, though there are some edits to include extra interrupts
    
    if(INTCONbits.TMR0IF && INTCONbits.TMR0IE){
        INTCONbits.TMR0IE = 0;      //disable interrupt
        T0CONbits.TMR0ON = 0;       //disable TMR0
        timerCounter++;             //increment the counter
        //___s has passed, trigger an alert, set a flag
        if(timerCounter > 102){      //the value is 1.67s * the number for counting
            interruptCheck = 1;         //trigger a global interrupt flag variable
        }
        else{
        INTCONbits.TMR0IE = 1;      //enable interrupt
        T0CONbits.TMR0ON = 1;       //enable TMR0
        }
        INTCONbits.TMR0IF = 0;  // Clear interrupt flag bit to signify it's been handled 
    }
    
    
    //Keypad interrupt
    if(INTCON3bits.INT1IF && INTCON3bits.INT1IE){
        /* Interrupt on change handler for RB1. */
        
        /* Write key press data to bottom line of LCD. */
        __lcd_newline();
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        unsigned char temp = keys[keypress];
                
        if(temp == '*' || temp == '#' || temp == 'D'){
            interruptCheck = temp;
        }
        INTCON3bits.INT1IF = 0;  // Clear interrupt flag bit to signify it's been handled        
    }
}