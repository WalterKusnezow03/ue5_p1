# nn_server.py
print("NNServer NN_SERVER MODULE START")
import socket, struct

from .InputOutput import receiver
from .SharedMemory.sharedMemory import UnrealSharedFrame
from .SharedMemory.sharedMemoryMap import UnrealSharedMemoryMap
##from NNBase import CNNBase

print("NNServer NN_SERVER MODULE LOADED")

########## always send empty messages if nothing happens: otherwise TCP will be blocking! ##########
########## Fixing attempts not sucessfull yet, is a workaround! ##########

### import globally into other plugins ###
class NNServer:
    def __init__(self):
        self.openedconnection = True
        ##self.sharedMemoryObj = None

        self.sharedMemoryMap = UnrealSharedMemoryMap()

        ##self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ## -- setup socket --
        ##self.s.bind(("127.0.0.1", 5050))
        ##self.s.listen(1)
        ##print("NNServer Python server ready") ###printed in unreal pipe ANNSocket::Python: Python server ready
        ##self.connection, self.addr = self.s.accept()
        ##print("NNServer Connection from", self.addr)

        ##CNNBase.debugTorch()
        self.SHUTDOWNFLAG = False




        ##setup connection
        self.setupConnection()

        self.setupFrameNames()
        self.setupFlags()

    def setupConnection(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind(("127.0.0.1", 5050))
        self.s.listen(1)
        print("NNServer Python server ready")
        print("NNServer Waiting for connection...")
        self.connection, self.addr = self.s.accept()
        print("NNServer Connection from", self.addr)

    def setupFrameNames(self):
        ##same as cpp socket (ANNPathFinderSocket)
        self.frameNameRequest = "ANNPathFinderSFIN"
        self.frameNameResult = "ANNPathFinderSFRES"
        self.frameNameGroundThruth = "ANNPathFinderSFGT"
        self.frameNameNNType = "ANNPathFinderSFNN"

    def setupFlags(self):
        self.bWaitingForGroundTruth = False
        self.resultData = None
        self.frameNameBatch = "ANNPathFinderSFB"
        self.bWaitingForBatch = True
        self.doBatchTraining = True
        self.trainLive = False

    def ShutDownTriggered(self):
        return self.SHUTDOWNFLAG == True
        
        
    def Tick(self):
        if(self.openedconnection):
            self.ReceiveSocketMessage()
            ##print("NNServer receive finish")
            self.ProcessSharedMemory()
            ##print("NNServer receive finish process shared mem")

    def ReceiveSocketMessage(self):
        data = receiver.receive(self.connection)
        if data:
            ##print("NNServer received data")
            ##message = receiver.unpackMessageToString(data)
            message = receiver.unpackMessageToStringSplit(data, "_") #unpackMessageToString(data)
            ##print("NNServer received data as string: ", message)

            self.ProcessMessage(message)

    

    def OnShutDown(self):
        print("NNServerPathfinder OnShutDown Save net")
        self.SaveNet()
        print("NNServerPathfinder OnShutDown FINISHED")
        print("PYTHON_SAFE_TO_EXIT")
        return

    def ProcessMessage(self, message):
        if(len(message) > 0):
            prefix = message[0]
            if(prefix == "SHUTDOWN"):
                self.OnShutDown()
                self.closeConnect()
                self.SHUTDOWNFLAG = True

            if(prefix == "FRAMEID"):
                print("NNServer received data as string: FRAME ID DATA: ", message)
                self.CloseAndReopenSharedMemoryCommand(message)
                

    def closeConnect(self):

        try:
            self.connection.shutdown(socket.SHUT_RDWR)
        except:
            pass

        try:
            self.connection.close()
        except:
            pass

        try:
            self.s.close()
        except:
            pass

        ##self.connection.close()
        ##self.s.close()
        self.openedconnection = False
        print("SHUTDOWN_CONFIRMED")

    ##### not tested ! #####
    def CloseAndReopenSharedMemoryCommand(self, message):
        if(len(message) >= 4):
            prefix = message[0]
            if(prefix == "FRAMEID"):
                
                if(len(message) == 4):
                    tagname, size, shortTag = message[1], int(message[2]), message[3]
                    if(len(tagname) < 2):
                        return
                    if(int(size) < 1):
                        return
                    if(self.sharedMemoryMap):
                        self.sharedMemoryMap.CloseAndReopenPage(tagname, size, shortTag)
                        print("NNServer FRAME ID: REOPEN SHARED MEMORY ", tagname, " ", size)
                
                if(len(message) == 5):
                    tagname, size, semMutexName, shortTag = message[1], int(message[2]), message[3], message[4]
                    if(len(tagname) < 2):
                        return
                    if(int(size) < 1):
                        return
                    if(self.sharedMemoryMap):
                        self.sharedMemoryMap.CloseAndReopenPageSemaphore(tagname, size, semMutexName, shortTag)
                        print("NNServer FRAME ID: REOPEN SHARED MEMORY ", tagname, " ", size, " ", semMutexName)
                
                

    def TagIsGroundTruthButDontTrainLive(self, tag):
        return tag == self.frameNameGroundThruth and (self.trainLive == False)           

                

    

    def ProcessSharedMemory(self):
        ##self.sharedMemoryMap.ShowMap() ##debug
        #self.ProcessSharedMemorySwitchNNType()
        self.ProcessBatchOnce()
        self.WriteResultDataIfPending()
        
        if(self.bWaitingForGroundTruth):
            self.ProcessSharedMemoryByTag(self.frameNameGroundThruth)
            return
    

        self.ProcessSharedMemoryByTag(self.frameNameRequest)
        
        
        return
        
        ##old
        #print("NNServer receive -> ProcessSharedMemory")
        #if(self.sharedMemoryMap):
            ##read
        #    return 
        #else:
        #    print("NNServer NO memory Object! ")


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
            NetRef = self.GetNet()
            if(NetRef != None):
                self.resultData = NetRef.forwardBinData(data)
                ##self.resultData = self.Net.forwardBinData(data)
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

                NetRef = self.GetNet()
                if(NetRef != None):
                    NetRef.showLoss(data)


                if(self.trainLive):
                    print("NNServerPathfinder_RUN_NN_BACKPROP ", len(data))

                    if(NetRef != None):
                        NetRef.Net.learnData(data)
                    

                    ##self.Net.learnData(data)
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

    def GetNet(self):
        #if(self.netSwitcher != None):
        #    return self.netSwitcher.GetActiveNet()
        return None

    def SaveNet(self):
        if(self.netSwitcher != None):
            self.netSwitcher.SaveAll()

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
                        ##self.Net.TrainFromBatchBinary(data)
                        NetRef = self.GetNet()
                        if(NetRef != None):
                            NetRef.TrainFromBatchBinary(data)

                        self.bWaitingForBatch = False
        return


    ##### DESTRUKTOR
    def __del__(self):
        try:
            if self.connection:
                self.connection.close()
        except:
            pass

        try:
            if self.s:
                self.s.close()
        except:
            pass

        print("NNServer sockets closed on delete")






#######  RUNNER ########

def Run():
    ## run until process finished
    serverVar = NNServer()
    while True and (serverVar.SHUTDOWNFLAG != True):
        serverVar.Tick()
    serverVar.closeConnect()

##Run()











