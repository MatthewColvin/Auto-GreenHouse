from abc import abstractmethod
import time

from enum import Enum


class GreenHouseLocations(Enum):
    OutsideTemp = 1
    InsideTemp = 2
    UNKNOWN = 3


# Must use GPIO.setmode(GPIO.BOARD)
class PinMap:
    TempSensor1 = 7  # GPIO4


class Peripheral:

    def __init__(
            self,
            aLocation: GreenHouseLocations = GreenHouseLocations.UNKNOWN):
        self.location = aLocation


class PollingSensor(Peripheral):

    def __init__(
            self,
            aPollRateMs=2000,
            aDataTimeoutMs=5000,
            aLocation: GreenHouseLocations = GreenHouseLocations.UNKNOWN):
        super().__init__(aLocation)
        self.lastDataRead = 0
        self.lastPoll = 0

        self.pollRateMs = aPollRateMs * 1000
        self.DataTimeoutNs = aDataTimeoutMs * 1000

    # Public

    def update(self):
        """ Used as public interface to update the data of the preferial in the DataGather thread"""
        if self.__handle():
            self.lastDataRead = time.time_ns()

    def dataValid(self) -> bool:
        return self.lastDataRead != 0 and self.lastDataRead <= time.time_ns() + self.DataTimeoutNs

    # Protected Override when you wish
    @abstractmethod
    def _readSensor(self) -> bool:
        """Update Private members then return true if it was successful"""

    def _needRefresh(self) -> bool:
        """Can also be updated to have different check for when to rerun __readSensor"""
        return time.time_ns() > self.lastPoll + self.pollRateMs

    # Private Careful when overriding
    def __handle(self) -> bool:
        self.lastPoll = time.time_ns()
        if (self._needRefresh()):
            return self._readSensor()
