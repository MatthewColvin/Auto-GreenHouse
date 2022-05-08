/////////////////////////////////
// The basic concept
/////////////////////////////////
// Spi will respond with a 32 bit signed int based on the first byte sent to the device.

// So this means controller will need to send opcode and
// then 4 dummy bytes for a read or the 32bit value to write

// Negative values will be error return codes See ErrorCodes Enum

///////////////////////////////////
// The op Code
///////////////////////////////////
// ??00 0000
// The two MSBs will represent ControlMode
//    Mode 0 is a simple mode used read and write pins
//    Mode 1 is a speical mode that takes a number that will link to a get and set routine for specific routine
//    Mode 2 and 3 are RESERVED for further dev.

// 00?0 0000
// The next bit is used as the READ/WRITE or GET/SET bit

// 000? ????
// The next 5 bits will be used to determine what to do based on the current mode
// ControlMode 0
//    Read or write the given pin
//    If the given value does not match the return will be INVALID_PIN
// ControlMode 1
//    Get/Read or Set/Write for a specific component

#pragma once
#include <string.h>

class SpiCtrl
{

public:
    enum class ErrorCodes
    {
        SUCCESS = -1,
        INVALID_PIN = -2
    };

    enum class ControlMode
    {
        READ_WRITE = 0,
        SPEICAL_FUNCTION = 1,
        NOTHING_YET = 2,
        NOTHING_YET1 = 3
    };

    SpiCtrl();
    ~SpiCtrl();

    void handleInterrupt();
    void handleRequest();

private:
    volatile boolean spiReceived = false;
    volatile byte received = 0x00;

    bool isOpCode = true;
    // OpCode portions
    ControlMode currentMode = ControlMode::READ_WRITE; // top 2 bits
    bool isWrite = false;                              // bit 3
    uint8_t opCodeVal = 0;                             // lower 5 bits
    ErrorCodes proccessOpcode(uint8_t opCode);

    // Read/Write or Get/Set Value
    uint8_t byteCounter = 0;
    int retVal = 0;
    ErrorCodes handleOp();
};
