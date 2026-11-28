from .sharedMemory import UnrealSharedFrame


class UnrealSharedMemoryMap:
    def __init__(self):
        self.pagesMap = {}

    def findPage(self, shortTag):
        return self.pagesMap.get(shortTag) ##None if not existent

    def openPage(self, tagname, size, shortTag):
        self.pagesMap[shortTag] = (UnrealSharedFrame(tagname, size))
        print("UnrealSharedMemoryMap open page", tagname, " ", shortTag)

    def openPageSempaphore(self, tagname, size, semWriteName, semReadName, shortTag):
        self.pagesMap[shortTag] = (UnrealSharedFrame(tagname, size, semWriteName, semReadName))
        print("UnrealSharedMemoryMap open page", tagname, " ", shortTag)

    ##by short tag. (map key)
    def closePage(self, shortTag):
        page = self.findPage(shortTag)
        
        if(page != None):
            page.close()
            self.pagesMap[shortTag] = None

    def CloseAndReopenPage(self, tagname, size, shortTag):
        self.closePage(shortTag)
        self.openPage(tagname, size, shortTag)

    


    def CloseAndReopenPageSemaphore(self, tagname, size, semMutexName, shortTag):
        self.closePage(shortTag)
        self.openPageSempaphore(tagname, size, semMutexName, shortTag)

    def ShowMap(self):
        for key, value in self.pagesMap.items():
            print("UnrealSharedMemoryMap: ", key)