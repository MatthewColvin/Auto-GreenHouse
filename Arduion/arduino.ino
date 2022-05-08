#include <SPI.h>
#include "SpiCtrl.h"
void spiSetup();
int ledPin = 7;

unsigned long lastPotDebugPrint;
const int potDebugPrintDelay = 5000; // print pot logs every second

unsigned long lightlastToggled;
bool lightOn;

SpiCtrl spiControl;

ISR(SPI_STC_vect)
{
  spiControl.handleInterrupt();
}

void setup()
{
  Serial.begin(115200);
  // LED setup
  lightlastToggled = millis();
  pinMode(ledPin, OUTPUT);
  lightOn = false;
  // POT setup
  lastPotDebugPrint = millis();
}

void loop()
{
  spiControl.handleRequest();

  uint16_t potVal = readPot();
  if (millis() > lastPotDebugPrint + potDebugPrintDelay)
  {
    Serial.print("potval: ");
    Serial.println(potVal);
    lastPotDebugPrint = millis();
  }

  handleLed(potVal);
}

void handleLed(unsigned long blinkDelay)
{
  if (millis() > lightlastToggled + blinkDelay)
  {
    if (!lightOn)
    {
      // Serial.write("write high");
      digitalWrite(ledPin, HIGH);
      lightOn = true;
    }
    else
    {
      // Serial.println("write low");
      digitalWrite(ledPin, LOW);
      lightOn = false;
    }
    lightlastToggled = millis();
  }
}

uint16_t readPot()
{
  return analogRead(A0);
}
