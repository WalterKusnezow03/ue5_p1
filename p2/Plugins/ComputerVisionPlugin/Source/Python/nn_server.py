# nn_server.py
import socket, struct

from InputOutput import receiver 




s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 5050))
s.listen(1)
print("Python server ready") ###printed in unreal pipe ANNSocket::Python: Python server ready
connection, addr = s.accept()
print("Connection from", addr)

while True:
    ##print("PythonTick")
    print("Python::PythonTick", flush=True) ##works with flush.
    data = receiver.receive(connection)
    if data:
        print("received data", flush=True)
        typecommand = receiver.unpackCommandType(data)
        if typecommand == -1:
            print("Python::received connection close!", flush=True)
            break
        else:
            print("received command other", typecommand)
            
    

   
#### close connection after loop
connection.close()