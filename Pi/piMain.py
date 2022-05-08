from multiprocessing.connection import wait
import time
import SpiCtrl

SpiCtrl.setupSpi()

#SpiCtrl.WritePin(SpiCtrl.ArduinoPin.D0, 500)

while 1:
    potVal = SpiCtrl.ReadPin(SpiCtrl.ArduinoPin.A0)
    print("potVal: {}".format(potVal))
