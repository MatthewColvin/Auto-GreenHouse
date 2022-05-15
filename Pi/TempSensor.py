from cmath import e
import dht11
import RPi.GPIO as GPIO
import time
from Peripheral import PollingSensor, GreenHouseLocations


class TempSensor(PollingSensor):

    def __init__(self,
                 aPin,
                 aPollRateMs=2000,
                 aDataTimeoutMs=5000):

        super().__init__(aPollRateMs, aDataTimeoutMs)

        self.DHT11 = dht11.DHT11(pin=aPin)
        self.temp = 0
        self.humidity = 0

    # Public
    def getTemp(self):
        return self.temp

    def getHumidity(self):
        return self.humidity

    # Protected
    def _readSensor(self) -> bool:
        """ OverRide from base to update internal data state"""
        result = self.DHT11.read()
        if result.is_valid():
            self.humidity = result.humidity
            self.temp = result.temperature
            return True
        else:
            return False
