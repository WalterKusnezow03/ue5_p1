#### receiver class ####
import socket, struct


def receive(connection):
    #4 Bytes from header (one int)
    numBytesHeaderData = recv_exact(connection, 4)
    numBytes = struct.unpack("i", numBytesHeaderData)[0] ##one signed integer

    #make payload
    payload = recv_exact(connection, numBytes)
    print("received payload", payload, " from numbytes", numBytes)

    return payload

def recv_exact(connection, n):
    data = b""              # 1) Anfangszustand: leeres Bytearray
    while len(data) < n:    # 2) Solange wir noch nicht genug Bytes haben
        packet = connection.recv(n - len(data))  # 3) Fordere den Rest an
        if not packet:      # 4) Verbindung wurde geschlossen (recv gibt b'' zurück)
            raise ConnectionError("Verbindung geschlossen")
        data += packet      # 5) Hänge die neuen Bytes an
    return data             # 6) Wenn genug gesammelt, gib alles zurück



def unpackCommandType(data):
    if len(data) < 4:
        print("payload command type too short")
        return -1


    ##type: int, 4 bytes
    ##command = struct.unpack("i", data[:4])[0] ##struct.unpack("i", data)[0] ##one signed integer
    command = extractFromBinary(data, 0, 4)
    print("payload command extracted ", command)
    return command




def extractFromBinary(data, startIndex, toCopyBytes):
    dataOut = struct.unpack("i", data[startIndex:startIndex+toCopyBytes])[0]
    return dataOut