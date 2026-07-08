# nn_server.py
print("NNServer NN_SERVER MODULE START")
import socket, struct

from InputOutput import receiver
from SharedMemory.sharedMemory import UnrealSharedFrame
from SharedMemory.sharedMemoryMap import UnrealSharedMemoryMap
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

        ##NEW
        ##self.connection.settimeout(0.01)


        ##new
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind(("127.0.0.1", 5050))
        self.s.listen(1)
        print("NNServer Python server ready")
        print("NNServer Waiting for connection...")
        self.connection, self.addr = self.s.accept()
        print("NNServer Connection from", self.addr)







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
                
                
                
                

                

    

    def ProcessSharedMemory(self):
        print("NNServer receive -> ProcessSharedMemory")
        if(self.sharedMemoryMap):
            ##read
            return 
        else:
            print("NNServer NO memory Object! ")


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



def Run():
    ## run until process finished
    serverVar = NNServer()
    while True and (serverVar.SHUTDOWNFLAG != True):
        serverVar.Tick()
    serverVar.closeConnect()

##Run()












