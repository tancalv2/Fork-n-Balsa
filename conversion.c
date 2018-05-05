
/* This source file contains all conversion functionalities. Sub-functions
 * included are the charToInt, and amountEncoder
 */

/********************************** Includes **********************************/
#include "conversion.h"

/********************************** Functions *********************************/

unsigned int charToInt(unsigned char c1, unsigned char c2){
    /* This function is designed convert the provided char values c1 and c2 into
     * integer values based on what c1 and c2 are. c1 and c2 are representative
     * of the tens and ones digit of a number. A precondition is c2 is only
     * NOT NULL when c1 is 1. This is important since the switch statement for
     * c2 returns a value of 10+c2, assuming that c1 is the tens and c2 is the
     * ones digit. Otherwise, the c2 is NULL and c1 is the ones digit.
     *
     * Arguments:   c1,         c1 is a char value that is between 1 and 9.
     *              c2,         c2 is a char value that is between 0 and 8.
     *
     * Returns: integer value between 1 and 18 depending on values of c1 and c2
     *
     */
    if(c2 != NULL){
        switch(c2){
            case '0':   return 10;  break;
            case '1':   return 11;  break;
            case '2':   return 12;  break;
            case '3':   return 13;  break;
            case '4':   return 14;  break;
            case '5':   return 15;  break;
            case '6':   return 16;  break;
            case '7':   return 17;  break;
            case '8':   return 18;  break;
            default:    return NULL;
        }
    }
     switch(c1){
            case '1':   return 1;  break;
            case '2':   return 2;  break;
            case '3':   return 3;  break;
            case '4':   return 4;  break;
            case '5':   return 5;  break;
            case '6':   return 6;  break;
            case '7':   return 7;  break;
            case '8':   return 8;  break;
            case '9':   return 9;  break;
            case NULL:  return 0;  break;
            default:    return NULL;
     }
}

unsigned int amountEncoder(unsigned int curr){
    /* This function is designed to encode the amount value enter by the
     * operator to an integer value that can be written onto the EEPROM. Since
     * the amount was coded in a way that cannot be written directly
     * into the EEPROM (each amount was greater than 1byte/8bits), it is encoded
     * in the code below
     *
     * Arguments:   curr,     curr is an integer value representing the current
     *                        drawer the value is being encoded from. Value
     *                        ranges from 1 to 16.
     *
     * Returns: integer value between 1 and 46, mapping each amount for a diet
     *          type to one of these values
     */

    switch(diet[curr]){
        case(1):    //R
            if(amount[curr] == 1000){    return 1;   break;}
            if(amount[curr] == 2000){    return 2;   break;}
        case(2):    //F
            if(amount[curr] == 1000){    return 3;   break;}
            if(amount[curr] == 2000){    return 4;   break;}
        case(3):    //L
            if(amount[curr] == 1000){    return 5;   break;}
            if(amount[curr] == 2000){    return 6;   break;}
            if(amount[curr] == 3000){    return 7;   break;}


        case(4):    //RF
            if(amount[curr] == 1100){    return 8;   break;}
            if(amount[curr] == 1200){    return 9;   break;}
            if(amount[curr] == 2100){    return 10;  break;}
            if(amount[curr] == 2200){    return 11;  break;}
        case(5):    //RL
            if(amount[curr] == 1100){    return 12;  break;}
            if(amount[curr] == 1200){    return 13;  break;}
            if(amount[curr] == 1300){    return 14;  break;}
            if(amount[curr] == 2100){    return 15;  break;}
            if(amount[curr] == 2200){    return 16;  break;}
        case(6):    //FL
            if(amount[curr] == 1100){    return 17;  break;}
            if(amount[curr] == 1200){    return 18;  break;}
            if(amount[curr] == 1300){    return 19;  break;}
            if(amount[curr] == 2100){    return 20;  break;}
            if(amount[curr] == 2200){    return 21;  break;}

        case(7):    //RRF
            if(amount[curr] == 1110){    return 22;  break;}
            if(amount[curr] == 1120){    return 23;  break;}
        case(8):    //RRL
            if(amount[curr] == 1110){    return 24;  break;}
            if(amount[curr] == 1120){    return 25;  break;}
        case(9):    //RFF
            if(amount[curr] == 1110){    return 26;  break;}
            if(amount[curr] == 2110){    return 27;  break;}
        case(10):   //RLL
            if(amount[curr] == 1110){    return 28;  break;}
            if(amount[curr] == 1120){    return 29;  break;}
            if(amount[curr] == 1210){    return 30;  break;}
            if(amount[curr] == 2110){    return 31;  break;}
        case(11):   //RFL
            if(amount[curr] == 1110){    return 32;  break;}
            if(amount[curr] == 1120){    return 33;  break;}
            if(amount[curr] == 1210){    return 34;  break;}
            if(amount[curr] == 2110){    return 35;  break;}
        case(12):   //FFL
            if(amount[curr] == 1110){    return 36;  break;}
            if(amount[curr] == 1120){    return 37;  break;}
        case(13):   //FLL
            if(amount[curr] == 1110){    return 38;  break;}
            if(amount[curr] == 1120){    return 39;  break;}
            if(amount[curr] == 1210){    return 40;  break;}
            if(amount[curr] == 2110){    return 41;  break;}

        case(14):   //RRFL
            if(amount[curr] == 1111){    return 42;  break;}
        case(15):   //RFFL
            if(amount[curr] == 1111){    return 43;  break;}
        case(16):   //RFLL
            if(amount[curr] == 1111){    return 44;  break;}
        case(17):   //RLLL
            if(amount[curr] == 1111){    return 45;  break;}
        case(18):   //FLLL
            if(amount[curr] == 1111){    return 46;  break;}
        default: break;
    }
}
