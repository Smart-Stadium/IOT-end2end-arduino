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

  // An example of a simple command callback function
void blinkLED5times(const String arguments, String &response) {
  pinMode(LED_BUILTIN, OUTPUT);
  for (byte i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
  response = "{\"blinked\":\"5 times\"}";
}
// goStraight Command
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

  matrix.displayFrames(goStraightArray, 2000, true, 1);
  delay(10000);


  response = "{\"way\":\"Vehicle is going straight.\"}";
}

// goToLoadingDock Command
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

  matrix.displayFrames(goToLoadingDockArray, 2000, true, 1);
  delay(10000);
  
  
  response = "{\"way\":\"Vehicle is stopping at the loading dock.\"}";
}

void setup() {
  Serial.begin(115200);
  Serial.print("\n*** Live Objects on Arduino MKR boards, revision ");
  Serial.print(SW_REVISION);
  Serial.println("***");

  // Declaring a simple commands hadled by the function 'blinkLED5times'.
  lo.addCommand("continueOnTheRoad", goStraight);
  lo.addCommand("stopAtLoadingDock", goToLoadingDock);
  lo.setSecurity(TLS);
  lo.begin(MQTT, TEXT, true);
  lo.connect();                          // connects to the network + Live Objects
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
