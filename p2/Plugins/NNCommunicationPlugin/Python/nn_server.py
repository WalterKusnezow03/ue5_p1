# nn_server.py
import socket, struct

from InputOutput import receiver
from SharedMemory.sharedMemory import UnrealSharedFrame
from SharedMemory.sharedMemoryMap import UnrealSharedMemoryMap
from NNBase import CNNBase

print("NNServer NN_SERVER MODULE LOADED")

### import globally into other plugins ###
class NNServer:
    def __init__(self):
        self.openedconnection = True
        ##self.sharedMemoryObj = None

        self.sharedMemoryMap = UnrealSharedMemoryMap()

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ## -- setup socket --
        self.s.bind(("127.0.0.1", 5050))
        self.s.listen(1)
        print("NNServer Python server ready") ###printed in unreal pipe ANNSocket::Python: Python server ready
        self.connection, self.addr = self.s.accept()
        print("NNServer Connection from", self.addr)

        CNNBase.debugTorch()
        self.SHUTDOWNFLAG = False

    def ShutDownTriggered(self):
        return self.SHUTDOWNFLAG == True
        
        
    def Tick(self):
        if(self.openedconnection):
            self.ReceiveSocketMessage()
            print("NNServer receive finish")
            self.ProcessSharedMemory()
            print("NNServer receive finish process shared mem")

    def ReceiveSocketMessage(self):
        data = receiver.receive(self.connection)
        if data:
            print("NNServer received data")
            ##message = receiver.unpackMessageToString(data)
            message = receiver.unpackMessageToStringSplit(data, "_") #unpackMessageToString(data)
            print("NNServer received data as string: ", message)

            self.ProcessMessage(message)


    def ProcessMessage(self, message):
        if(len(message) > 0):
            prefix = message[0]
            if(prefix == "SHUTDOWN"):
                self.closeConnect()
                self.SHUTDOWNFLAG = True

            if(prefix == "FRAMEID"):
                print("NNServer received data as string: FRAME ID DATA: ", message)
                self.CloseAndReopenSharedMemoryCommand(message)
                

    def closeConnect(self):
        self.connection.close()
        self.s.close()
        self.openedconnection = False
        print("SHUTDOWN_CONFIRMED")

    ##### not tested ! #####
    def CloseAndReopenSharedMemoryCommand(self, message):
        if(len(message) >= 4):
            prefix = message[0]
            if(prefix == "FRAMEID"):
                tagname, size, shortTag = message[1], int(message[2]), message[3]
                if(len(tagname) < 2):
                    return
                if(int(size) < 1):
                    return
                

                if(self.sharedMemoryMap):
                    self.sharedMemoryMap.CloseAndReopenPage(tagname, size, shortTag)
                    print("NNServer FRAME ID: REOPEN SHARED MEMORY ", tagname, " ", size)

    

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












