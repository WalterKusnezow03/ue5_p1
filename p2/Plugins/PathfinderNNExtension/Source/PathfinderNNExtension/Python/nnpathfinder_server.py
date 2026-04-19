import os
import sys


#### find NNCommunicationPlugin working dir to launch tcp listener ####
BASE_DIR = os.path.dirname(__file__)

COMMUNICATION_PYTHON = os.path.abspath(
    os.path.join(BASE_DIR, "../../../../NNCommunicationPlugin/Source/NNCommunicationPlugin/Python")
)

sys.path.insert(0, COMMUNICATION_PYTHON)

print("COMM PATH:", COMMUNICATION_PYTHON, "\n")

##### works as expected, ignore yellow line!
import nn_server

print("IMPORT OK \n")

##nn_server.Run() ##is executed!! :) 



####  custom derived
class NNServerPathfinder(nn_server.NNServer):
    def __init__(self):
        super().__init__()  # wichtig zum socket aufmachen, und shared memory
        print("NNServerPathfinder constructor")
        self.count = 0

    def Tick(self):
        super().Tick()
        self.count = self.count + 1
        if(self.count > 1000):
            self.count = 0
            print("NNServerPathfinder Ticked")

    def ProcessSharedMemory(self):
        print("NNServerPathfinder receive -> ProcessSharedMemory")
        if(self.sharedMemoryObj):
            if(self.sharedMemoryObj.isReady()):
                print("NNServerPathfinder Shared Mem is ready")
            
            data = self.sharedMemoryObj.read_data_only()
            print("NNServerPathfinder shared mem Data ", data)
            
            return 
            ##print("tick")
        else:
            print("NNServerPathfinder NO memory Object! ")


        return
        ##    print("hallo")

    def ProcessMessage(self, message):
        ##derived
        print("NNServerPathfinder receive message ", message) ## works as expected
        super().ProcessMessage(message)







##tick
server = NNServerPathfinder()

def RunHere():
    while True:
        server.Tick()
RunHere()
