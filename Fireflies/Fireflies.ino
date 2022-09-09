/**
 * @mainpage Fireflies Project
 *
 * @section description_main Description
 * An Arduino sketch that simulates fireflies.
 *
 * @section circuit_main Circuit
 * - 8 LEDs, via 330 ohm resistors, are connected to pins D2-D9.
 *
 * @section notes_main Notes
 * - Use the Serial Monitor to view output while in DEBUG mode.
 *
 * Copyright (c) 2022 Woolsey Workshop.  All rights reserved.
 */


/**
 * @file Fireflies.ino
 *
 * @brief The primary Arduino sketch implementation file.
 *
 * @section description_fireflies_ino Description
 * An Arduino sketch that simulates fireflies.
 *
 * @section libraries_fireflies_ino Libraries
 * - None.
 *
 * @section notes_fireflies_ino Notes
 * - Comments are Doxygen compatible.
 *
 * @section todo_fireflies_ino TODO
 * - None.
 *
 * @section author_fireflies_ino Author(s)
 * - Created by John Woolsey on 07/29/2021.
 * - Modified by John Woolsey on 09/07/2022.
 *
 * Copyright (c) 2022 Woolsey Workshop.  All rights reserved.
 */


// Defines
#define DEBUG 0  ///< The mode of operation; 0 = normal, 1 = debug.


// Types
/**
 * The Firefly structure.
 *
 * Contains the attributes needed for each simulated firefly.
 */
struct Firefly {
   uint8_t pin;                 ///< The pin controlling the LED.
   bool isLit;                  ///< The current lit status; true = on, false = off.
   unsigned long triggerTime;   ///< The last time the firefly was lit.
   unsigned long triggerDelay;  ///< The delay between successive firefly light times.
};


// Pin Mapping
const uint8_t LEDs[] = {2, 3, 4, 5, 6, 7, 8, 9};  ///< The LED pins representing the fireflies.


// Global Constants
const unsigned long LightTime = 500;                     ///< The time that a firefly is lit in milliseconds.
const unsigned long MinDarkTime = 5000;                  ///< The minimum time that a firefly is not lit in milliseconds.
const unsigned long MaxDarkTime = 10000;                 ///< The maximum time that a firefly is not lit in milliseconds.
const uint8_t LEDsNum = sizeof(LEDs) / sizeof(LEDs[0]);  ///< The number of LEDs (fireflies).


// Global Instances
Firefly fireflies[LEDsNum];  ///< The array of all firefly instances.


/**
 * The standard Arduino setup function used for setup and configuration tasks.
 */
void setup() {
   if (DEBUG) {
      // Serial Monitor
      Serial.begin(9600);  // initialize serial bus
      while (!Serial);     // wait for serial connection
      Serial.println(F("Running in DEBUG mode.  Turn off for normal operation."));
   }

   // Configure all fireflies
   for (uint8_t i = 0; i < LEDsNum; i++) {
      pinMode(LEDs[i], OUTPUT);
      digitalWrite(LEDs[i], LOW);
      fireflies[i].pin = LEDs[i];
      fireflies[i].isLit = false;
      fireflies[i].triggerTime = 0;
      fireflies[i].triggerDelay = random(MinDarkTime, MaxDarkTime);
   }
}


/**
 * The standard Arduino loop function used for repeating tasks.
 */
void loop() {
   // Process all fireflies
   for (uint8_t i = 0; i < LEDsNum; i++) {
      processFirefly(fireflies[i]);
   }
}


/**
 * Prints the current time and the status of a firefly.
 *
 * @param currentTime  The current time in milliseconds.
 * @param firefly      The firefly instance.
 */
void printFireflyStatus(unsigned long currentTime, Firefly firefly) {
   Serial.print(F("Firefly: currentTime = "));
   Serial.print(currentTime);
   Serial.print(F(", triggerTime = "));
   Serial.print(firefly.triggerTime);
   Serial.print(F(", triggerDelay = "));
   Serial.print(firefly.triggerDelay);
   Serial.print(F(", pin = "));
   Serial.print(firefly.pin);
   Serial.print(F(", isLit = "));
   Serial.println(firefly.isLit);
}


/**
 * Processes the lighting timing of a firefly.
 *
 * @param firefly  The firefly instance.
 */
void processFirefly(Firefly &firefly) {
   unsigned long currentTime = millis();
   // Light firefly at appropriate trigger time
   if (!firefly.isLit && (currentTime - firefly.triggerTime >= firefly.triggerDelay)) {
      if (DEBUG) {
         printFireflyStatus(currentTime, firefly);
         Serial.println(F("  Turning on firefly."));
      }
      digitalWrite(firefly.pin, HIGH);
      firefly.isLit = true;
      firefly.triggerTime = currentTime;
   // Turn off firefly after appropriate lit time
   } else if (firefly.isLit && (currentTime - firefly.triggerTime >= LightTime)) {
      if (DEBUG) {
         printFireflyStatus(currentTime, firefly);
         Serial.println(F("  Turning off firefly."));
      }
      digitalWrite(firefly.pin, LOW);
      firefly.isLit = false;
      firefly.triggerDelay = LightTime + random(MinDarkTime, MaxDarkTime);
   }
}
