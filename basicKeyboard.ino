/*
Hey there! This code was originally written for our YouTube channel, https://www.youtube.com/RKadeGaming
We're glad that you've taken an interest in our project, and we hope that you have a good time building it!
We've made this code public, and you're free to use it however you like. If you end up sharing the project with others though, 
we'd appreciate some attribution so that people know where to find more stuff like this.
Thanks, and have fun! :)
*/


#include <Keyboard.h>
#include <movingAvg.h>
// “Sketch” menu > Include Library > Manage Libraries… > Search for “movingavg” > Install
// the Arduino must be an HID type; Micro, Leonardo, or Due. The computer doesn't recognize the Uno as a keyboard.


// the resistance thresholds when using 22M ohm resistors
const int PressedMaxThreshold = 200; // this is the maximum reading when the button is pressed. Anything underneath this value will register as a touch (a higher number will allow inputs with a higher natural resistance at the risk of false positives)
const int ReleasedMinThreshold = 300; // this is the minimum reading when there is no connection. Any reading higher than this will register as no longer touching
const byte PinCount = 4;

// you can have as many keys as you have analog pins
const byte InputPins[PinCount] = {A0, A1, A2, A3};
// how to program arbitrary letters
const char KeyCodes[PinCount] = {'w', 'a', 's','d'};
// how to use special keys
//const char KeyCodes[PinCount] = {KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW};


// every character/key has these attributes
struct TouchInput {
    byte analogPin;
    char keycode;
    movingAvg filter = movingAvg(20);
    boolean wasPressed = false;
};

// this is an array of TouchInput structs called "Pins" that contains "PinCount" items in it
TouchInput Pins[PinCount];


void setup(){
    Serial.begin(115200);

    // loop through each of the keys and initialize them in the order defined in InputPins[] KeyCodes[]
    for(int i = 0; i < PinCount; i++){
        Pins[i].analogPin = InputPins[i];
        Pins[i].keycode = KeyCodes[i];
        Pins[i].filter.begin();
    }
}


void loop(){
    // evaluate each of the pins defined earlier
    for(int i = 0; i < PinCount; i++){

        float currentAverage = Pins[i].filter.reading(analogRead(Pins[i].analogPin));
        boolean previousState = Pins[i].wasPressed;
        boolean currentState = previousState;   // Default if in the dead zone

        if(currentAverage < PressedMaxThreshold){
            currentState = true;        // this means that the circuit has been completed, so the key should be pressed
        }
        else if(currentAverage > ReleasedMinThreshold){
            currentState = false;       // the circuit has been broken, so the key should be released
        }

        // if the state of the pin has changed since the last iteration
        if(currentState != previousState){
            // then start or stop pressing the key (the opposite of what it was before)
            if(currentState){
                Keyboard.press(Pins[i].keycode);
            }else{
                Keyboard.release(Pins[i].keycode);
            }         
        }
        Pins[i].wasPressed = currentState;
    }
}

