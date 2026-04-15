# nn_server.py
import socket, struct

from InputOutput import receiver 




s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

## -- setup socket --
s.bind(("127.0.0.1", 5050))
s.listen(1)
print("Python server ready") ###printed in unreal pipe ANNSocket::Python: Python server ready
connection, addr = s.accept()
print("Connection from", addr)


## -- TICK --
def Tick():
    ReceiveSocketMessage()

def ReceiveSocketMessage():
    data = receiver.receive(connection)
    if data:
        print("received data")
        message = receiver.unpackMessageToString(data)
        ##receiver.unpackMessageToStringSplit(data, "_") #unpackMessageToString(data)
        print("received data as string: ", message)

        ProcessMessage(message)


def ProcessMessage(message):
    if(len(message) > 0):
        prefix = message[0]
        if(prefix == "FRAMEID"):
            print("received data as string: FRAME ID DATA: ", message)





## run until process finished
while True:
    Tick()
            
    

   
#### close connection after loop

connection.close()