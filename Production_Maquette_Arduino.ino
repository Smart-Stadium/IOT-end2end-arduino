/******************************************************************************
   INCLUDES
 ******************************************************************************/

#include "grove_two_rgb_led_matrix.h"
#include <ChainableLED.h>
#include "arduino_secrets.h"
#include <LiveObjects.h>

/******************************************************************************
   USER VARIABLES
 ******************************************************************************/

uint32_t messageRate = 60000;         // stores the current data message rate in Milliseconds
unsigned long lastMessageTime = 0;   // stores the time when last data message was sent
GroveTwoRGBLedMatrixClass matrix;   // instance of MatrixClass

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

#define DISPLAY_COLOR    0X11


/******************************************************************************
   USER PROGRAM
 ******************************************************************************/
// Confirm connexion
void showConnected() {

  uint64_t printGreenSquare[] = {
    0x5252525252525252,
    0x5252525252525252,
    0x5252525252525252,
    0x5252525252525252,
    0x5252525252525252,
    0x5252525252525252,
    0x5252525252525252,
    0x5252525252525252
  };

  matrix.displayFrames(printGreenSquare, 0, true, 1);
  delay(5000);

}

// goStraight Command callback function
void goStraight(const String arguments, String &response) {


  uint64_t goStraightArray[] = {
    0xffffffadadffffff,
    0xffffffadadffffff,
    0xffffffadadffffff,
    0xffffffadadffffff,
    0xadadadadadadadad,
    0xffadadadadadadff,
    0xffffadadadadffff,
    0xffffffadadffffff
  };

  matrix.displayFrames(goStraightArray, 0, true, 1);
  delay(10000);


  response = "{\"way\":\"Vehicle is going straight.\"}";
}

// goToLoadingDock  callback function
void goToLoadingDock(const String arguments, String &response) {


  uint64_t goToLoadingDockArray[] = {
    0xfffffffffdfdffff,
    0xfffffdfffdfdffff,
    0xfffdfdfffdfdffff,
    0xfdfdfdfdfdfdffff,
    0xfdfdfdfdfdfdffff,
    0xfffdfdffffffffff,
    0xfffffdffffffffff,
    0xffffffffffffffff
  };

  matrix.displayFrames(goToLoadingDockArray, 0, true, 1);
  delay(10000);
  
  
  response = "{\"way\":\"Vehicle is stopping at the loading dock.\"}";
}

void turfOffLeds() {

  uint64_t off[] = {
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff
  };

  matrix.displayFrames(off, 0, true, 1);
  delay(5000);

} 


// turfOffLeds  callback function
void turnOffMatrixLeds(const String arguments, String &response) {

  turfOffLeds();
  response = "{\"action\":\"Turn of leds.\"}";

} 


void setup() {

  Serial.begin(115200);
  Serial.print("\n*** Live Objects for Arduino MKR boards, revision ");
  Serial.print(SW_REVISION);
  Serial.println("***");
  
  // Declaring commands.
  lo.addCommand("continueOnTheRoad", goStraight);
  lo.addCommand("stopAtLoadingDock", goToLoadingDock);
  lo.addCommand("turnOffMatrixLeds", turnOffMatrixLeds);
  // connects to the network + Live Objects
  lo.setSecurity(TLS);
  lo.begin(MQTT, TEXT, true);
 
  lo.connect();
  showConnected();
  turfOffLeds();

}

void loop() {

  if (millis() - lastMessageTime > messageRate) {
    // collect data periodically
    Serial.println("Sampling data");
    lo.addToPayload("ArduinoIsActive", true);               // adding 'uptime' value to the current payload

    Serial.println("Sending data to Live Objects");
    lo.sendData();                                   // send the data to Live Objects
    lastMessageTime = millis();
  }

  lo.loop();                             // don't forget to keep this in your main loop
}
