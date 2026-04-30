

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
        return flag == 1

    def writeReadyFalse(self):
        self.map.seek(0)
        self.map.write(struct.pack("i", 0))

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


