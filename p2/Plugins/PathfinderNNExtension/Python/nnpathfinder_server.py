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
import NetB

print("NNServerPathfinder IMPORT OK \n")



##debug
##print("NNServerPathfinder make Net B instance debug \n")
##localNet = NetB.NetB() ##NetA.NetA()
##print("NNServerPathfinder make Net B instance finish \n")



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

        print("NNServerPathfinder_NetB: INIT!")
        self.Net = NetB.NetB()

        self.bWaitingForGroundTruth = False
        self.resultData = None

        self.frameNameBatch = "ANNPathFinderSFB"
        self.bWaitingForBatch = True

        self.doBatchTraining = True

        self.trainLive = False
        print("NNServerPathfinder constructor end")


    def TagIsGroundTruthButDontTrainLive(self, tag):
        return tag == self.frameNameGroundThruth and (self.trainLive == False)
    
    def ProcessSharedMemoryByTag(self, shortTag):
        if(self.sharedMemoryMap):
            page = self.sharedMemoryMap.findPage(shortTag)
            if(page != None):
                ##if data ready: read, flag read / false
                    ##process data, wait for result.
                
                ##isReady()
                ##writeReadyFalse()
                if(page.isReady()):
                    
                    

                    ## default process data
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

        ##process frame data ground truth -> if life training enabled: do
        if(shortTag == self.frameNameGroundThruth):
            if(self.bWaitingForGroundTruth):

                self.bWaitingForGroundTruth = False ##ready for a new request
                if(self.trainLive):
                    print("NNServerPathfinder_RUN_NN_BACKPROP ", len(data))
                    self.Net.learnData(data)
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
                self.bResultDataWritten = True ##mark as true
                return
            else:
                print("NNServerPathfinder_RequestFinishAvailable BUT RESULT PAGE NOT SETUP")
            
        return


    



    ##override
    def ProcessSharedMemory(self):
        ##self.sharedMemoryMap.ShowMap() ##debug
        self.ProcessBatchOnce()
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
    


    ##### batch process only once! #####
    ##batch process single time
    def ProcessBatchOnce(self):

        if self.bWaitingForBatch:
            print("NNServerPathfinder_ProcessBatch_Waiting")
            if(self.sharedMemoryMap):
                page = self.sharedMemoryMap.findPage(self.frameNameBatch)
                if(page != None):
                    if(page.isReady()):
                        print("NNServerPathfinder_ProcessBatch")
                        print("ANNServerPathfinder_ProcessBatch")
                        data = page.read_data_only_float_array()
                        ##print("NNServerPathfinder shared mem Page Data Len of (",shortTag, "): ", len(data))
                        page.writeReadyFalse()

                        ##block - testing needed
                        if self.doBatchTraining == False:
                            self.bWaitingForBatch = False
                            return
                    
                        ##feed
                        self.Net.TrainFromBatchBinary(data)

                        self.bWaitingForBatch = False

        
                

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
