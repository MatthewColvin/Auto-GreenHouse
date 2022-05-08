from enum import IntEnum
import spidev
import time


class OpcodeMode(IntEnum):
    READ_WRITE_PIN = 0
    GET_SET_FUNCTIONS = 1
    NOTHING_YET = 2
    NOTHING_YET1 = 3


class ArduinoPin(IntEnum):
    D0 = 0
    D1 = 1
    D2 = 2
    D3 = 3
    D4 = 4
    D5 = 5
    D6 = 6
    D7 = 7
    D8 = 8
    D9 = 9
    D10 = 10
    D11 = 11
    D12 = 12
    D13 = 13
    A0 = 14
    A1 = 15
    A2 = 16
    A3 = 17
    A4 = 18
    A5 = 19
    A6 = 20
    A7 = 21


spi = spidev.SpiDev()


def setupSpi():
    bus = 0
    device = 0

    spi.open(bus, device)
    spi.max_speed_hz = 5000
    spi.mode = 0b00  # CPOL|CPHA Polarity: 0 Phase:0 This is arduion SPI_MODE0


def createOpCode(mode: OpcodeMode, isWrite: bool, value: int) -> int:
    if (value > pow(5, 2)):
        print("ERROR opcode Value out of Range")
        exit(-1)
    else:
        return (int(mode) << 6) | (isWrite << 5) | value


def ReadPin(pin: ArduinoPin) -> int:
    # Send the opCode
    to_send = [createOpCode(OpcodeMode.READ_WRITE_PIN, False, pin)]
    spi.xfer(to_send)
    # Send the dummy bits to get value back
    val_bytes = spi.xfer([0x00, 0x00, 0x00, 0x00])
    value = int.from_bytes(val_bytes, "little")
    #print("da bytes {} \n daBits: {}".format(val_bytes, bin(value)))
    return value


def WritePin(pin: ArduinoPin, value: int):
    # Send the opCode and the value to write
    to_send = [createOpCode(OpcodeMode.READ_WRITE_PIN, True, pin)]
    to_send.extend(value.to_bytes(4, "big"))
    spi.xfer(to_send)
