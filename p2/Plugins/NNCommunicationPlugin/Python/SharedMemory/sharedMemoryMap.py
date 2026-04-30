from .sharedMemory import UnrealSharedFrame


class UnrealSharedMemoryMap:
    def __init__(self):
        self.pagesMap = {}

    def findPage(self, shortTag):
        return self.pagesMap.get(shortTag) ##None if not existent

    def openPage(self, tagname, size, shortTag):
        self.pagesMap[shortTag] = (UnrealSharedFrame(tagname, size))
        print("UnrealSharedMemoryMap open page", tagname, " ", shortTag)

    def closePage(self, shortTag):
        page = self.findPage(shortTag)
        
        if(page != None):
            page.close()
            self.pagesMap[shortTag] = None

    def CloseAndReopenPage(self, tagname, size, shortTag):
        self.closePage(shortTag)
        self.openPage(tagname, size, shortTag)