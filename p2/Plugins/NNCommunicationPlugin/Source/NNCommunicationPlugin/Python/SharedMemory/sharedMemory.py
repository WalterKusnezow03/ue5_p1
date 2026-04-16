

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

class UnrealSharedFrame:
    def __init__(self, tagname, size):
        self.size = size
        self.tagname = tagname

        self.shm = mmap.mmap(
            -1,
            size,
            tagname=tagname
        )

    def isReady(self):
        self.shm.seek(0)
        ##(inFShared memory, unreal plugin)
        flag = struct.unpack("i", self.shm.read(4))[0] ##first flag for 1 ready, 0 non ready
        return flag == 1
    
    def writeReadyFalse(self):
        self.shm.seek(0)
        self.shm.write(struct.pack("i", 0))

   
    def read_data_only(self):
        self.shm.seek(4)
        return self.shm.read(self.size - 4)
    
    ##diconnect from shared memory: managed by unreal!!!
    def close(self):
        if self.shm:
            self.shm.close()
            self.shm = None

    def __del__(self):
        self.close()

