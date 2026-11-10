import os
import sys
print("NNServerPathfinder LAUNCHED .PY \n")

#### find from other folder / plugin
#### ----- NOT USED ANYMORE! -----
#### find NNCommunicationPlugin working dir to launch tcp listener ####
##BASE_DIR = os.path.dirname(__file__)

##COMMUNICATION_PYTHON = os.path.abspath(
##    os.path.join(BASE_DIR, "../../NNCommunicationPlugin/Python")
##)

##sys.path.insert(0, COMMUNICATION_PYTHON)

##print("COMM PATH:", COMMUNICATION_PYTHON, "\n")
#### ----- NOT USED ANYMORE! -----

##### works as expected, ignore yellow line!

from Base import nn_server
print("NNServerPathfinder nn_server import ok \n")

##debug only
##from Base.NNBase import CNNBase
from Base.NNBase import TorchDebug
print("NNServerPathfinder TorchDebug import ok \n")

import Net.NetA as NetA
print("NNServerPathfinder NetA import ok \n")

import Net.NetB as NetB
print("NNServerPathfinder NetB import ok \n")

import Net.NetSwitcher as NetSwitcher
print("NNServerPathfinder NetSwitcher import ok \n")


print("NNServerPathfinder ALL IMPORT OK \n")


#### ----- DEBUG - NOT NEEDED ANYMORE! -----
##debug
##print("NNServerPathfinder make Net B instance debug \n")
##localNet = NetB.NetB() ##NetA.NetA()
##print("NNServerPathfinder make Net B instance finish \n")
##nn_server.Run() ##is executed!! :) 
#### ----- DEBUG - NOT NEEDED ANYMORE! -----



####  custom derived
class NNServerPathfinder(nn_server.NNServer):
    def __init__(self):
        super().__init__()  # wichtig zum socket aufmachen, und shared memory
        print("NNServerPathfinder constructor")
        
        

        self.netSwitcher = NetSwitcher.NetSwitcher()
        
        print("NNServerPathfinder constructor end")

    def setupFrameNames(self):
        ##same as cpp socket (ANNPathFinderSocket)
        self.frameNameRequest = "ANNPathFinderSFIN"
        self.frameNameResult = "ANNPathFinderSFRES"
        self.frameNameGroundThruth = "ANNPathFinderSFGT"
        self.frameNameNNType = "ANNPathFinderSFNN"
    


    def GetNet(self):
        if(self.netSwitcher != None):
            return self.netSwitcher.GetActiveNet()
        return None
    



    ##override
    def ProcessSharedMemory(self):
        ##self.sharedMemoryMap.ShowMap() ##debug
        self.ProcessSharedMemorySwitchNNType()
        super().ProcessSharedMemory()
        
        


    def SaveNet(self):
        if(self.netSwitcher != None):
            self.netSwitcher.SaveAll()

        ##if(self.Net != None):
        ##    self.Net.saveCheckpoint()
        ##    return

    


    #### switch nn type by shared memory written number
    def ProcessSharedMemorySwitchNNType(self):
        if(self.sharedMemoryMap):
            page = self.sharedMemoryMap.findPage(self.frameNameNNType)
            if(page != None):
                if(page.isReady()):
                    if(self.netSwitcher != None):
                        self.netSwitcher.UpdateActiveIndexFromIntArray(page.read_data_only_int_array())
                        return
            self.netSwitcher.UpdateActiveIndex(0)
        return










##tick
print("NNServerPathfinder BEFORE INIT")
server = NNServerPathfinder()
print("NNServerPathfinder END INIT")


def RunHere():
    print("NNServerPathfinder RUN NOW")
    while True:
        server.Tick()
        print("NNServerPathfinder Tick NOW")
        if(server.ShutDownTriggered() == True):
            print("NNServerPathfinder Tick NOW - SHUT DOWN")
            break

RunHere()
##server.SaveNet()

print("NNServerPathfinder FINISHED")

print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
