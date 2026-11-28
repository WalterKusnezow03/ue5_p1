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
print("nnmovement_server nn_server import ok \n")

##debug only
##from Base.NNBase import CNNBase
from Base.NNBase import TorchDebug
print("nnmovement_server TorchDebug import ok \n")


import Net.NetB as NetB
print("nnmovement_server NetB import ok \n")

import Net.NetSwitcher as NetSwitcher
print("nnmovement_server NetSwitcher import ok \n")


print("nnmovement_server ALL IMPORT OK \n")




####  custom derived
class NNServerMovement(nn_server.NNServer):
    def __init__(self):
        self.PORT = 5051
        super().__init__()  # wichtig zum socket aufmachen, und shared memory
        print("nnmovement_server constructor")
        
        

        self.netSwitcher = NetSwitcher.NetSwitcher()
        
        print("nnmovement_server constructor end")

    def setupLogName(self):
        self.logName = "NNServerMovement"

    def setupFrameNames(self):
        ##same as cpp socket (ANNPathFinderSocket)
        self.frameNameRequest = "ANNMovementSFIN"
        self.frameNameResult = "ANNMovementSFRES"
        self.frameNameGroundThruth = "ANNMovementSFGT"
        self.frameNameNNType = "ANNMovementSFNN"
        self.frameNameBatch = "ANNMovementSFB"
    


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
            ##switch to index 0 by default if no page is found
            self.netSwitcher.UpdateActiveIndex(0)
        return










##tick
print("NNServerMovement BEFORE INIT")
server = NNServerMovement()
print("NNServerMovement END INIT")


def RunHere():
    print("NNServerMovement RUN NOW")
    while True:
        server.Tick()
        print("NNServerMovement Tick NOW")
        if(server.ShutDownTriggered() == True):
            print("NNServerMovement Tick NOW - SHUT DOWN")
            break

RunHere()
##server.SaveNet()

print("NNServerMovement FINISHED")

print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
print("PYTHON_SAFE_TO_EXIT")
