import RPi.GPIO as GPIO
import time
import threading


from tempfile import TemporaryDirectory
from I2CCtrl import UnoCrtl, ArduinoPin
from BasePeripheral import PinMap, BasePeripheral
from TempSensor import TempSensor


# GPIO.setmode(GPIO.BCM)
# GPIO.setup(TempSensorPin, GPIO.IN)


# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.cleanup()

Ts1 = TempSensor(PinMap.TempSensor1, 20, 50)

peripherals = [Ts1]


def updatePeripherals():
    while 1:
        for p in peripherals:
            p.update()


DataGather = threading.Thread(target=updatePeripherals)
DataGather.start()

try:

    while 1:
        if Ts1.dataValid():
            print("Temp: {} Humidity: {}".format(
                Ts1.getTemp(), Ts1.getHumidity()))
        else:
            print("invlaid tmp")

        time.sleep(.25)

except KeyboardInterrupt:
    print('Interrupted')

    DataGather.join(5)
    sys.exit(0)
