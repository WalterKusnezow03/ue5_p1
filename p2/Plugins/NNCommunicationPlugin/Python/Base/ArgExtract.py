import sys



class ArgumentExtractor:

    def __init__():
        return

    @staticmethod
    def GetArgsAsStringArray():
        #print(sys.argv[0])   #Pfad zum Skript
        return sys.argv[1:]   #Alle zusätzlichen Argumente

    @staticmethod
    def GetArg(prefix):
        args = ArgumentExtractor.GetArgsAsStringArray()
        for current in args:
            extracted, found = ArgumentExtractor.RemovePrefixFrom(prefix, current)
            if found:
                return extracted
            
        return None

    @staticmethod
    def GetArgAsInt(prefix):
        arg = ArgumentExtractor.GetArg(prefix)
        if arg != None:
            return int(arg)
        return None


    @staticmethod 
    def RemovePrefixFrom(prefix, arg):
        if prefix in arg:
            return arg.replace(prefix, "", 1), True
        return arg, False