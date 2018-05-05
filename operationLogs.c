
/* This source file contains all Log functionalities. Sub-functions 
 * included are the operationLogs and previousLogs
 */

/********************************** Includes **********************************/
#include "operationLogs.h"

/********************************** Functions *********************************/

void operationLogs(void){
    /* This sub-function provides the operator two choices:
     * either the most recent log or previous logs.
     * Either outcome returns the this sub-function where it displays the
     * following information in this order:
     *
     * LCD 1:
     * Date: __day
     * MM/DD/20YY
     * 
     * LCD 2:
     * Time it Occurred
     * HH/MM/SS
     * 
     * LCD 3:
     * Operation Time:
     * __m __s
     * 
     * LCD 4:
     * Marked Drawers:
     * D_ D_ D_ D_
     * 
     * LCD 5:
     * Leftovers:
     * R:__ F:__ L:__
     * 
     * LCD 6:
     * Drawer
     * Instructions:
     * LCD 7,8,...:
     * D__  Type:__
     * Amount:____
     * 
     * and then returns to standby mode
     * 
     * The information is read from the EEPROM by issuing the read_EEPROM 
     * command and saving it into temporary variable.
     * 
     * Arguments: none
     * 
     * Returns: none
     */
    
    unsigned char keypress;
    unsigned char temp;
    unsigned int data;
    unsigned int markedLog[17];
    unsigned int drawerLog[17];
    unsigned int dietLog[8];
    unsigned int amountLog[8];
    unsigned int inc;
    //loop variables
    unsigned int i;
    unsigned int a;
    
    //Reset which previousLog iteration it is on
    currLog = 0;
    
    enum day{Mon=1,Tu=2,Wed=3,Thu=4,Fri=5,Sat=6,Sun=7};
    enum month{Jan=1,Feb=2,Mar=3,Apr=4,May=5,Jun=6,
                Jul=7,Aug=8,Sep=9,Oct=10,Nov=11,Dec=12};
    
                
    __lcd_clear();
    __lcd_home();
    __lcd_display_control(1, 0, 0);
    /*check if there was a log
     * this is achieved by checking the fourth address
     * normally, the address is, by default, 8 bits of 1s however, when writing 
     * to the EEPROM, not all bits are used. For address 1, 2 and 3, all 8 bits
     * are used, so there is not much knowledge extracted however in address 4,
     * only the lower 6 bits are used, thus making the upper 2 bits 0, something
     * that can be checked to see whether there is something saved 
     */
    if(read_EEPROM(4)>>7 != 0){
        printf("No Logs entered!");
        __lcd_newline();
        __delay_ms(2000);
        return;
    }
                
    //Provide operator the option to choose between most recent and previous
    do{
        __lcd_clear();
        __lcd_home();
        printf("A: Most Recent");
        __lcd_newline();
        printf("B: Previous");
        while(PORTBbits.RB1 == 0){  continue;   }

        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }

        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //check if input was B
        if(temp == 'B'){
            //check the log adjacent to the first one, since it is shifted over
            if(read_EEPROM(4+memoryGap)>>7 != 0){
                __lcd_clear();
                __lcd_home();
                printf("No Previous Logs!");
                __delay_ms(2000);
                //change temp to stay in loop
                temp = NULL;
            }
        }
    }while(temp != 'A' && temp != 'B');
    __lcd_clear();
    __lcd_home();
    
    //Depending on the log was selected, variable inc was used as an increment
    if(temp == 'A'){
        inc = 0;
    }
    if(temp == 'B'){
        inc = previousLogs();
    }
    
    //read the data encoded from marked array
    //first 8 bits (1-8)
    data = read_EEPROM(2+memoryGap*inc);
    //decimal back to binary array
    markedLog[8] = data%2;
    markedLog[7] = data/2%2;
    markedLog[6] = data/4%2;
    markedLog[5] = data/8%2;
    markedLog[4] = data/16%2;
    markedLog[3] = data/32%2;
    markedLog[2] = data/64%2;
    markedLog[1] = data/128%2;
    
    //later 8 bits (9-16)
    data = read_EEPROM(3+memoryGap*inc);
    //decimal back to binary array
    markedLog[16] = data%2;
    markedLog[15] = data/2%2;
    markedLog[14] = data/4%2;
    markedLog[13] = data/8%2;
    markedLog[12] = data/16%2;
    markedLog[11] = data/32%2;
    markedLog[10] = data/64%2;
    markedLog[9]  = data/128%2;
    
    
    //read the data encoded from drawer array
    //first 8 bits (1-8)
    data = read_EEPROM(7+memoryGap*inc);    
    //decimal back to binary array
    drawerLog[8] = data%2;
    drawerLog[7] = data/2%2;
    drawerLog[6] = data/4%2;
    drawerLog[5] = data/8%2;
    drawerLog[4] = data/16%2;
    drawerLog[3] = data/32%2;
    drawerLog[2] = data/64%2;
    drawerLog[1] = data/128%2;
     
    //later 8 bits (9-16)
    data = read_EEPROM(8+memoryGap*inc);
    //decimal back to binary array
    drawerLog[16] = data%2;
    drawerLog[15] = data/2%2;
    drawerLog[14] = data/4%2;
    drawerLog[13] = data/8%2;
    drawerLog[12] = data/16%2;
    drawerLog[11] = data/32%2;
    drawerLog[10] = data/64%2;
    drawerLog[9]  = data/128%2;
    
    //read the data encoded from diet and amount array
    for(i=0;i<8;i++){
        dietLog[i] = read_EEPROM(9+i+memoryGap*inc);
        /*since the amount was coded in a way that cannot be written directly 
         * into the EEPROM (each amount was greater than 1byte/8bits), it was 
         * encoded and thus needs to be decoded here with the following below */        
        data = read_EEPROM(17+i+memoryGap*inc);
        if(data == 1 || data == 3 || data == 5){
            amountLog[i] = 1000;}
        if(data == 2 || data == 4 || data == 6){
            amountLog[i] = 2000;}
        if(data == 7){
            amountLog[i] = 3000;}
        
        if(data == 8 || data == 12 || data == 17){
            amountLog[i] = 1100;}
        if(data == 9 || data == 13 || data == 18){
            amountLog[i] = 1200;}
        if(data == 14||data == 19){
            amountLog[i] = 1300;}
        if(data == 10 || data == 15 || data == 20){
            amountLog[i] = 2100;}
        if(data == 11 || data == 16 || data == 21){
            amountLog[i] = 2200;}
                
        if(data == 22 || data == 24 || data == 26 
                || data == 28 || data == 32 || data == 36 || data == 38){
            amountLog[i] = 1110;}
        if(data == 23 || data == 25 || data == 29 
                || data == 33 || data == 37 || data == 39){
            amountLog[i] = 1120;}
        if(data == 30 || data == 34 || data == 40){
            amountLog[i] = 1210;}
        if(data == 27 || data == 31 || data == 35 || data == 41){
            amountLog[i] = 2110;}
                
        if(data == 42 || data == 43 || data == 44 || data == 45 || data == 46){
            amountLog[i] = 1111;}
    }
    
    //print the date the Operation was performed on
    __lcd_clear();
    __lcd_home();
    //Sun-Sat
    data = ((read_EEPROM(28+memoryGap*inc))>>5);
    switch(data){
        case(Mon):
            printf("Date: Monday");
            break;
        case(Tu):
            printf("Date: Tuesday");
            break;
        case(Wed):
            printf("Date: Wednesday");
            break;
        case(Thu):
            printf("Date: Thursday");
            break;
        case(Fri):
            printf("Date: Friday");
            break;
        case(Sat):
            printf("Date: Saturday");
            break;
        case(Sun):
            printf("Date: Sunday");
            break;
        default:
            break;
    }
    __lcd_newline();
    //month
    data = (read_EEPROM(28+memoryGap*inc) - (data<<5));
    switch(data){
        //MM/DD/20YY
        case(Jan):
            printf("Jan/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Feb):
            printf("Feb/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Mar):
            printf("Mar/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Apr):
            printf("Apr/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(May):
            printf("May/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Jun):
            printf("Jun/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Jul):
            printf("Jul/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Aug):
            printf("Aug/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Sep):
            printf("Sep/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Oct):
            printf("Oct/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Nov):
            printf("Nov/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        case(Dec):
            printf("Dec/%02x/20%02x",read_EEPROM(29+memoryGap*inc)
                    ,read_EEPROM(30+memoryGap*inc));
            break;
        default:
            break;
    }
    __delay_ms(2000);    
    
    //print the time the operation was performed
    __lcd_clear();
    __lcd_home();
    printf("Time it Occurred");
    __lcd_newline();
    //HH:MM:SS
    printf("    %02x:%02x:%02x    ",read_EEPROM(27+memoryGap*inc),
            read_EEPROM(26+memoryGap*inc),read_EEPROM(25+memoryGap*inc));
    __delay_ms(2000);
    
    //print the operation time
    __lcd_clear();
    __lcd_home();
    printf("Operation Time:");
    __lcd_newline();
    printf("%d m %d s",read_EEPROM(1+memoryGap*inc)/60,read_EEPROM(1+memoryGap*inc)%60);
//    printf("%d m %d s",data/60,data%60);
    __delay_ms(2000); 
        
    //print the marked drawers
    __lcd_clear();
    __lcd_home();
    printf("Marked Drawers:");
    __delay_ms(1000);
    lcd_set_cursor(0, 1);
    for(i=1;i<17;i++){    
        if(markedLog[i] == 1){
            printf("D%d ", i);
            __delay_ms(250);
        }
    }
    __delay_ms(2000);
    
    //print the leftovers
    __lcd_clear();
    __lcd_home();
    printf("Leftovers:");
    __lcd_newline();
    printf("R:%d F:%d L:%d", read_EEPROM(4+memoryGap*inc),read_EEPROM(5+memoryGap*inc),read_EEPROM(6+memoryGap*inc));
    __delay_ms(2000);
    
    //print all the drawer instructions
    __lcd_clear();
    __lcd_home();
    printf("    Drawer     ");
    __lcd_newline();
    printf(" Instructions:  ");
    __delay_ms(2000);
    for(i=0;i<8;i++){
        for(a=1;a<17;a++){
            if(drawerLog[a]==1){
                drawerLog[a]=0;
                __lcd_clear();
                __lcd_home();
                printf("D%d   Type:%d",a,dietLog[i]);
                __lcd_newline();
                printf("Amount: %d  ",amountLog[i]);
                __delay_ms(2500);
                break;
            }
        }
    }
    __lcd_clear();
    __lcd_home();
    printf("  Returning to");
    __lcd_newline();
    printf("  Standby mode");
    __delay_ms(2500);
}

unsigned int previousLogs(void){
    /* This sub-function provides the operator a choice between seven previous
     * logs that they can choose form by pressing either A, B, C or D. 
     * Arguments: none
     * 
     * Returns: integer value, 1 through 7 depending on which log was selected
     */
    
    unsigned char keypress;
    unsigned char temp;
    
    do{
        __lcd_clear();
        __lcd_home();
        //Display the different logs
        printf("A:Log 1 B:Log 2");
        __lcd_newline();
        printf("C:Log 3 D: Next");
        while(PORTBbits.RB1 == 0){  continue;   }

        /* Read the 4-bit character code. */
        keypress = (PORTB & 0xF0) >> 4;
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }

        Nop();  // Apply breakpoint here to prevent compiler optimizations
        temp = keys[keypress];
        //check if input was B
        if(temp == 'B'){
            //check the log adjacent to the first one, since it is shifted over
            if(read_EEPROM(4+memoryGap*2)>>7 != 0){
                __lcd_clear();
                __lcd_home();
                printf("   Log Empty!   ");
                __delay_ms(2000);
                //change temp to stay in loop
                temp = NULL;
            }
        }
        //check if input was C
        if(temp == 'C'){
            //check the log adjacent to the first one, since it is shifted over
            if(read_EEPROM(4+memoryGap*3)>>7 != 0){
                __lcd_clear();
                __lcd_home();
                printf("   Log Empty!   ");
                __delay_ms(2000);
                //change temp to stay in loop
                temp = NULL;
            }
        }
        //check if input was D
        if(temp == 'D'){
            //check the log adjacent to the first one, since it is shifted over
            if(read_EEPROM(4+memoryGap*4)>>7 != 0){
                __lcd_clear();
                __lcd_home();
                printf("   Logs Empty!   ");
                __delay_ms(2000);
                //change temp to stay in loop
                temp = NULL;
            }
        }
    }while(temp != 'A' && temp != 'B' && temp != 'C' &&  temp != 'D');
    if(temp == 'A'){    return 1;   }
    if(temp == 'B'){    return 2;   }
    if(temp == 'C'){    return 3;   }
    if(temp == 'D'){
        __lcd_clear();
        __lcd_home();
        //Display the different logs
        printf("A:Log 4 B:Log 5");
        __lcd_newline();
        printf("C:Log 6 D:Log 7");
        do{
            while(PORTBbits.RB1 == 0){  continue;   }

            /* Read the 4-bit character code. */
            keypress = (PORTB & 0xF0) >> 4;
            /* Wait until the key has been released. */
            while(PORTBbits.RB1 == 1){  continue;   }

            Nop();  // Apply breakpoint here to prevent compiler optimizations
            temp = keys[keypress];
            //check if input was B
            if(temp == 'B'){
                //check the log adjacent to the first one, since it is shifted over
                if(read_EEPROM(4+memoryGap*5)>>7 != 0){
                    __lcd_clear();
                    __lcd_home();
                    printf("   Log Empty!   ");
                    __delay_ms(2000);
                    //change temp to stay in loop
                    temp = NULL;
                }
            }
            //check if input was C
            if(temp == 'C'){
                //check the log adjacent to the first one, since it is shifted over
                if(read_EEPROM(4+memoryGap*6)>>7 != 0){
                    __lcd_clear();
                    __lcd_home();
                    printf("   Log Empty!   ");
                    __delay_ms(2000);
                    //change temp to stay in loop
                    temp = NULL;
                }
            }
            //check if input was D
            if(temp == 'D'){
                //check the log adjacent to the first one, since it is shifted over
                if(read_EEPROM(4+memoryGap*7)>>7 != 0){
                    __lcd_clear();
                    __lcd_home();
                    printf("   Logs Empty!   ");
                    __delay_ms(2000);
                    //change temp to stay in loop
                    temp = NULL;
                }
            }
        }while(temp != 'A' && temp != 'B' && temp != 'C' &&  temp != 'D');
        if(temp == 'A'){    return 4;   }
        if(temp == 'B'){    return 5;   }
        if(temp == 'C'){    return 6;   }
        if(temp == 'D'){    return 7;   }
    }
}
