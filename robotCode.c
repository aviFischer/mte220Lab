#include "mte220.c"  // C functions needed for labs

/*
;**********************************************************************
;
;    Filename:	    robotCode.c
;    Date:          Nov 23/21
;    File Version:  1.1
;
;    Author:        A. Fischer and A. Davidan
; Based on lab6.c by C.C.W. Hulls, P.Eng and D.L Wight, 2005
;
;**********************************************************************
;
;    Files required: mte220.c
;
;**********************************************************************
;
;    History:
;
;    1.0  Oct 9/21  original release
;    1.1  Oct 23/21 Bugfixes and cleaning up
;
;**********************************************************************
;**********************************************************************
;
;    Description:
;
;   The robot code controls the robot made for mte 220 labs by having
;   the robot follow a black path on white paper and stop when a magnet
;   is deteceted and either blink an LED or turn on an LED depending
;   on the magnet's polarity
;
;**********************************************************************
*/

//;**********************************************************************

// Magnet detection Thresholds
#define HALL_EFFECT_SENSOR_HIGH_THRESHOLD 175
#define HALL_EFFECT_SENSOR_LOW_THRESHOLD 125

// Infared Light Thresholds to turn
#define PHOTODIODE_TURN_LEFT_READING 0x8d
#define PHOTODIODE_TURN_RIGHT_READING 0x72

void handleMagnets(int polarity)
{
    if (polarity)
    {
        int i;
        for (i = 0; i < 7; i++)
        {
            // Turn on LED for fraction of time corresponding to analog value
            OnLED
            LongDelay(16);

            // Turn off LED
            OffLED
            LongDelay(16);
        }
    }
    else {
        OnLED
        LongDelay(274);
    }
    OffLED
}

void main(void)
{
    uns8 analog_value;  // current ADC value
    uns8 hall_effect_reading;

    Initialization();
    WaitForButton();
    //both servos are on after button press
    UseServos         // (syntax of "call" is correct without () or ;)
    BothServosOn

    int magnetDetected = 0;

    while (1)  // loop forever
    {
        analog_value = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp
        hall_effect_reading = AnalogConvert(ADC_HALL_EFFECT);

        if(hall_effect_reading > HALL_EFFECT_SENSOR_HIGH_THRESHOLD) 
        {
            if (magnetDetected == 0) {
                magnetDetected = 1;
                BothServosOff
                handleMagnets(1);
            }
        }
        else if (magnetDetected == 0 && hall_effect_reading < HALL_EFFECT_SENSOR_LOW_THRESHOLD)
        {
            if (magnetDetected == 0)
            {
                magnetDetected = 1;
                BothServosOff
                handleMagnets(0);
            }
        }
        else {
            magnetDetected = 0;
        }
        // no magnet detected
        if ( analog_value < PHOTODIODE_TURN_RIGHT_READING )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
        {
            // left servo only
            LeftServoOn
            RightServoOff
        }
        else if ( analog_value > PHOTODIODE_TURN_LEFT_READING )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
        {
            // right servo only
            RightServoOn
            LeftServoOff
        }
        else
        {
            // both servos on
            BothServosOn
        }        
    }
}
