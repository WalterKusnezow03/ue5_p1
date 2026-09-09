


from . import NetB as NetB

class NetSwitcher:
    def __init__(self):
        self.netMap = {}
        self.activeIndex = 0
        self.autoLoadNets()

    def autoLoadNets(self):
        self.netMap[0] = NetB.NetB()
        self.netMap[1] = None

        return

    def __del__(self):

        return

    def SaveAll(self):
        for value in self.netMap.values():
            if(value != None):
                value.saveCheckpoint()
                return
    

    def findNet(self, index):
        return self.netMap.get(index) ##None if not existent

    def UpdateActiveIndexFromIntArray(self, data):
        if(len(data) > 0):
            self.UpdateActiveIndex(data[0])
        else:
            self.UpdateActiveIndex(0)

    def UpdateActiveIndex(self, index):
        self.activeIndex = index

    def GetActiveNet(self):
        print("NetSwitcher Active Index", self.activeIndex)
        return self.findNet(self.activeIndex)