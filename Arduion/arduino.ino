
#define ENABLE_PRINT
#ifndef ENABLE_PRINT
// disable Serial output
#define Serial SomeOtherwiseUnusedName
static class
{
public:
  void begin(...) { delay(20); }
  void print(...) { delay(20); }
  void println(...) { delay(20); }
} Serial;
#endif

#include <SPI.h>
#include "SpiCtrl.h"
void spiSetup();
int ledPin = 7;
int LLshiftTestPin = 4;

unsigned long lastPotDebugPrint;
const int potDebugPrintDelay = 5000; // print pot logs every second

unsigned long lightlastToggled;
bool lightOn;
bool TestLit = false;

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
  digitalWrite(ledPin, LOW);
  lightOn = false;
  // POT setup
  lastPotDebugPrint = millis();
  // ll setup
  pinMode(LLshiftTestPin, OUTPUT);
  digitalWrite(LLshiftTestPin, LOW);
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

  if (millis() > 500 && !TestLit)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(LLshiftTestPin, HIGH);
    TestLit = true;
  }

  handleLed(potVal * 10);
}

void handleLed(unsigned long resetTime)
{
  bool lightToggled = lightOn != digitalRead(ledPin);
  if (lightToggled)
  {
    lightlastToggled = millis();
  }

  lightOn = digitalRead(ledPin);
  if (millis() > lightlastToggled + resetTime && lightOn)
  {
    // Serial.println("write low");
    digitalWrite(ledPin, LOW);
    digitalWrite(LLshiftTestPin, LOW);
  }
}

uint16_t readPot()
{
  return analogRead(A0);
}
