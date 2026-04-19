# nn_server.py
import socket, struct

from InputOutput import receiver
from SharedMemory.sharedMemory import UnrealSharedFrame


print("NN_SERVER MODULE LOADED")

### import globally into other plugins ###
class NNServer:
    def __init__(self):
        self.openedconnection = True
        self.sharedMemoryObj = None

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ## -- setup socket --
        self.s.bind(("127.0.0.1", 5050))
        self.s.listen(1)
        print("NNServer Python server ready") ###printed in unreal pipe ANNSocket::Python: Python server ready
        self.connection, self.addr = self.s.accept()
        print("NNServer Connection from", self.addr)

        
        
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
        if(len(message) >= 3):
            prefix = message[0]
            if(prefix == "FRAMEID"):
                tagname, size = message[1], int(message[2])
                if(len(tagname) < 2):
                    return
                if(int(size) < 1):
                    return

                if(self.sharedMemoryObj):
                    self.sharedMemoryObj.close()
                    self.sharedMemoryObj = None
                
                tagname, size = message[1], int(message[2])
                print("NNServer FRAME ID: REOPEN SHARED MEMORY ", tagname, " ", size)
                self.sharedMemoryObj = UnrealSharedFrame(tagname, size)

    

    def ProcessSharedMemory(self):
        print("NNServer receive -> ProcessSharedMemory")
        if(self.sharedMemoryObj):
            if(self.sharedMemoryObj.isReady()):
                print("NNServer Shared Mem is ready")
            
            data = self.sharedMemoryObj.read_data_only()
            print("NNServer shared mem Data ", data)
            
            return 
            ##print("tick")
        else:
            print("NNServer NO memory Object! ")



def Run():
    ## run until process finished
    serverVar = NNServer()
    while True:
        serverVar.Tick()
    serverVar.closeConnect()

##Run()












