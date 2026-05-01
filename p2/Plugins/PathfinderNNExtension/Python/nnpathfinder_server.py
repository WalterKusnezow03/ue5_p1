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
import NetA

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
        self.Net = NetA.NetA()

        self.bWaitingForGroundTruth = False
        self.resultData = None



    
    def ProcessSharedMemoryByTag(self, shortTag):
        if(self.sharedMemoryMap):
            page = self.sharedMemoryMap.findPage(shortTag)
            if(page != None):
                ##if data ready: read, flag read / false
                    ##process data, wait for result.
                
                ##isReady()
                ##writeReadyFalse()
                if(page.isReady()):
                    data = page.read_data_only_float_array()
                    ##print("NNServerPathfinder shared mem Page Data Len of (",shortTag, "): ", len(data))
                    page.writeReadyFalse()
                    self.ProcessDataByMemoryTag(data, shortTag)
            else:
                print("NNServerPathfinder shared mem Page not found", shortTag)

    def ProcessDataByMemoryTag(self, data, shortTag):
        print("NNServerPathfinder Data Process, Len of (",shortTag, "): ", len(data))

        if(shortTag == self.frameNameRequest):
            if(self.bWaitingForGroundTruth):
                print("NNServerPathfinder_RUN_NN_RETURNED: WAIT GT")
                return
            print("NNServerPathfinder_RUN_NN")
            ##run net
            self.resultData = self.Net.forwardTwoChannelData(data)
            self.bWaitingForGroundTruth = True
            self.bResultDataWritten = False

            ##write result
            print("NNServerPathfinder_RUN_NN_Finish_A", len(self.resultData))
            ##self.WriteResultDataIfPending()

            return

        if(shortTag == self.frameNameGroundThruth):
            if(self.bWaitingForGroundTruth):
                print("NNServerPathfinder_RUN_NN_BACKPROP ", len(data))
                self.Net.learnData(data)
                self.bWaitingForGroundTruth = False ##ready for a new request
                print("NNServerPathfinder_RUN_NN_BACKPROP_FINISH", len(data))
                return

    def WriteResultDataIfPending(self):
        if(self.resultData != None and self.bResultDataWritten == False):
            print("NNServerPathfinder_RequestFinishAvailable Start")
            resultPage = self.sharedMemoryMap.findPage(self.frameNameResult)
            if(resultPage != None):
                resultPage.write_data_only_float_array(self.resultData)
                resultPage.writeReadyTrue()
                print("NNServerPathfinder_RequestFinishAvailable Write! float:", len(self.resultData), " uint8 ", len(self.resultData) * 4)
                self.resultData = None
                self.bResultDataWritten == True
                return
            else:
                print("NNServerPathfinder_RequestFinishAvailable BUT RESULT PAGE NOT SETUP")
            
        return

    ##override
    def ProcessSharedMemory(self):
        self.sharedMemoryMap.ShowMap()
        self.WriteResultDataIfPending()
        
        if(self.bWaitingForGroundTruth):
            self.ProcessSharedMemoryByTag(self.frameNameGroundThruth)
            return
    

        self.ProcessSharedMemoryByTag(self.frameNameRequest)
        
        
        return
        ##    print("hallo")

    def ProcessMessage(self, message):
        ##derived
        print("NNServerPathfinder receive message ", message) ## works as expected
        super().ProcessMessage(message)

    def SaveNet(self):
        if(self.Net != None):
            self.Net.saveCheckpoint()
            return

    def OnShutDown(self):
        print("NNServerPathfinder OnShutDown Save net")
        self.SaveNet()
        print("NNServerPathfinder OnShutDown FINISHED")
        print("PYTHON_SAFE_TO_EXIT")
        super().OnShutDown()
        return





##tick
server = NNServerPathfinder()


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
