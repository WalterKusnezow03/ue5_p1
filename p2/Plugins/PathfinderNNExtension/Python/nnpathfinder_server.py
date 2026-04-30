import os
import sys


#### find NNCommunicationPlugin working dir to launch tcp listener ####
##BASE_DIR = os.path.dirname(__file__)

##COMMUNICATION_PYTHON = os.path.abspath(
##    os.path.join(BASE_DIR, "../../NNCommunicationPlugin/Python")
##)

##sys.path.insert(0, COMMUNICATION_PYTHON)

##print("COMM PATH:", COMMUNICATION_PYTHON, "\n")

##### works as expected, ignore yellow line!
import communicationPluginParent
import nn_server
from NNBase import CNNBase

##print("IMPORT OK \n")

##nn_server.Run() ##is executed!! :) 



####  custom derived
class NNServerPathfinder(nn_server.NNServer):
    def __init__(self):
        super().__init__()  # wichtig zum socket aufmachen, und shared memory
        print("NNServerPathfinder constructor")
        self.count = 0
        ##same as cpp socket
        self.frameNameRequest = "ANNPathFinderSFIN"
        self.frameNameResult = "ANNPathFinderSFRES"
        self.frameNameGroundThruth = "ANNPathFinderSFGT"


    def Tick(self):
        super().Tick()
        self.count = self.count + 1
        if(self.count > 1000):
            self.count = 0
            print("NNServerPathfinder Ticked")

    
    def ProcessSharedMemoryByTag(self, shortTag):
         if(self.sharedMemoryMap):
            page = self.sharedMemoryMap.findPage(shortTag)
            if(page != None):
                data = page.read_data_only_float_array()
                print("NNServerPathfinder shared mem Page Data Len of (",shortTag, "): ", len(data))
            else:
                print("NNServerPathfinder shared mem Page not found", shortTag)


    def ProcessSharedMemory(self):
        print("NNServerPathfinder receive -> ProcessSharedMemory")
        self.ProcessSharedMemoryByTag(self.frameNameRequest)
        self.ProcessSharedMemoryByTag(self.frameNameResult)
        self.ProcessSharedMemoryByTag(self.frameNameGroundThruth)
        return
        ##    print("hallo")

    def ProcessMessage(self, message):
        ##derived
        print("NNServerPathfinder receive message ", message) ## works as expected
        super().ProcessMessage(message)







##tick
server = NNServerPathfinder()


def RunHere():
    print("NNServerPathfinder RUN NOW")
    while True and (server.ShutDownTriggered() == False):
        server.Tick()
    
RunHere()


print("NNServerPathfinder FINISHED")