from enum import IntEnum
import time


class OpcodeMode(IntEnum):
    READ_WRITE_PIN = 0
    GET_SET_FUNCTIONS = 1


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


class UnoCrtl:

    def __init__(self):
        self.thing = "test"
