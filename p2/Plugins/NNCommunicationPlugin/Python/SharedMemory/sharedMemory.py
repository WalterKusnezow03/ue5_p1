

#----- PYTHON ACESS TO SHARED FRAME ------
#pythonCode.py

import mmap
import struct

#SIZE = 1920 * 1080 * 3 + 16

#shm = mmap.mmap(
#    -1,
#    SIZE,
#    tagname="/unreal_nn_shared"
#)





import mmap
import struct
import posix_ipc

##isReady()
##writeReadyFalse()

class UnrealSharedFrame:
    def __init__(self, tagname, size):
        self.size = size
        self.tagname = tagname
        self.closed = False

        self.shm = posix_ipc.SharedMemory(tagname)
        self.map = mmap.mmap(self.shm.fd, size)
        self.shm.close_fd()

        print("UnrealSharedFrame Opened", tagname)

    def isReady(self):
        self.map.seek(0)
        flag = struct.unpack("i", self.map.read(4))[0]
        print("UnrealSharedFrame PAGE READY", flag)
        return flag == 1

    def writeReadyFalse(self):
        self.map.seek(0)
        self.map.write(struct.pack("i", 0))
        print("Write Ready False: -> ", self.isReady())

    def writeReadyTrue(self):    
        self.map.seek(0)
        self.map.write(struct.pack("i", 1))
        print("Write Ready True: -> ", self.isReady())

    ##read as uint8
    def read_data_only(self):
        self.map.seek(4)
        return self.map.read(self.size - 4)
    
    ##read as float 
    def read_data_only_float_array(self):
        self.map.seek(4)  # nach dem Ready-Flag
        raw = self.map.read(self.size - 4)

        float_count = len(raw) // 4
        return struct.unpack(f"{float_count}f", raw[:float_count * 4])

    def write_data_only_float_array(self, values):
        self.map.seek(4)  # nach dem Ready-Flag

        packed = struct.pack(f"{len(values)}f", *values)

        max_bytes = self.size - 4
        if len(packed) > max_bytes:
            raise ValueError("write_data_only_float_array DATA TOO LARGE!")

        self.map.write(packed)

        print("NNServerPathfinder_RequestFinishAvailable shared memory page: write finish")

        # Optional: restlichen Bereich mit Nullen füllen
        remaining = max_bytes - len(packed)
        if remaining > 0:
            self.map.write(b"\x00" * remaining)



    def close(self):
        if self.closed:
            return
        self.closed = True

        try:
            if hasattr(self, "map") and self.map:
                self.map.close()
                self.map = None
        except:
            pass

        try:
            if hasattr(self, "shm") and self.shm:
                self.shm.close_fd()
                self.shm = None
        except:
            pass

    def __del__(self):
        self.close()


