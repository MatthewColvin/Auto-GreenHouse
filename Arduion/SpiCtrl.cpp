#include "SpiCtrl.h"
#include <SPI.h>
#include <string.h>

SpiCtrl::SpiCtrl()
{
    pinMode(MISO, OUTPUT);      // MISO writes back to controler CIPO
    SPCR |= _BV(SPE);           // spi in slave mode
    SPI.attachInterrupt();      // call spi interrupt ISR(SPI_STC_vect)
    SPI.setDataMode(SPI_MODE0); // Set polarty and phase to 0
}

void SpiCtrl::handleInterrupt()
{
    received = SPDR;    // Value received from master if store in variable slavereceived
    spiReceived = true; // Sets received as True
}

void SpiCtrl::handleRequest()
{
    if (spiReceived)
    {
        if (isOpCode)
        {
            proccessOpcode(received);
            isOpCode = false;
        }
        handleOp();
        spiReceived = false;
    }
}

SpiCtrl::ErrorCodes SpiCtrl::proccessOpcode(uint8_t opCode)
{
    currentMode = static_cast<SpiCtrl::ControlMode>((opCode & 0b11000000) >> 6);
    isWrite = opCode & 0b00100000;
    opCodeVal = opCode & 0b00011111;
    // String OpCodeDebugMsg =
    //     "OpCode recieved: " + String(opCode) + "\n" +
    //     "Mode: " + String((int)currentMode) + "\n" +
    //     "Type: " + (isWrite ? "write" : "read") + "\n" +
    //     "Value: " + String(opCodeVal) + "\n";
    // Serial.print(OpCodeDebugMsg);
    return ErrorCodes::SUCCESS;
}

SpiCtrl::ErrorCodes SpiCtrl::handleOp()
{
    if (isWrite && byteCounter > 0)
    {
        if (byteCounter < 5)
        { // Still getting value
            retVal = (retVal << 8) | received;
        }
    }
    else // Is Read
    {
        if (byteCounter == 0) // Set the value to send on first handle
        {
            retVal = 0;
            if (opCodeVal == 14)
            {
                retVal = analogRead(A0);
            }
        }
        uint8_t spdrVal = (retVal >> byteCounter * 8) & 0xff;
        // Serial.print("    byte #");
        // Serial.print(byteCounter);
        // Serial.print(" :");
        // Serial.println(spdrVal);
        SPDR = spdrVal;
    }
    byteCounter++;

    // Reset the counter for next operation and ready class for opcode
    if (byteCounter == 5)
    {
        // if (isWrite)
        // {
        //     Serial.print("Wrote VALUE: ");
        // }
        // else
        // {
        //     Serial.print("Read VALUE: ");
        // }
        // Serial.println(retVal);
        byteCounter = 0;
        isOpCode = true;
    }
    return SpiCtrl::ErrorCodes::SUCCESS;
}

SpiCtrl::~SpiCtrl()
{
}