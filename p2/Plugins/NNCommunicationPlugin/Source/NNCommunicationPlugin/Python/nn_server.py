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
            self.ProcessSharedMemory()

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
                if(self.sharedMemoryObj):
                    self.sharedMemoryObj.close()
                
                tagname, size = message[1], int(message[2])
                self.sharedMemoryObj = UnrealSharedFrame(tagname, size)

    

    def ProcessSharedMemory(self):
        if(self.sharedMemoryObj):
            return 
            ##print("tick")



def Run():
    ## run until process finished
    serverVar = NNServer()
    while True:
        serverVar.Tick()
    serverVar.closeConnect()

##Run()












