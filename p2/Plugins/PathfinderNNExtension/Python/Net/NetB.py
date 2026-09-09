


import torch
import torch.nn as nn

print("import torch done")

from . import UNet as UNet

from . import NetBase as NetBase

print("import unet done")



class NetB(NetBase.NetBase):

    def __init__(self):
        print("NNServerPathfinder_NetB: INIT!")
        super().__init__()        
        print("NNServerPathfinder_NetB: finish construct!")


    ##override
    def InitializeTensorAndPathNames(self):
        
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####
        self.W = 144
        self.H = 144
        self.IN_CHANNELS = 4
        self.OUT_CHANNELS = 1

        self.checkpointPath = "PyCheckpoint/netBcheckpoint.pth"
        self.ONNXPath = "Python/onnxExport/netB_ONNX.onnx"

        self.logname = "NNServerPathfinder_NetB"
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####

    ##override
    def ReInitNet(self):
        channelsIn = self.IN_CHANNELS ##4
        channelsOut = self.OUT_CHANNELS ##1
        self.net = UNet.UNet(channelsIn, channelsOut)
        ##move to gpu if available
        self.net = self.net.to(self.GetDevice())


        ##MSE aber bei falschem peak: 50 mal mehr loss, 1.0 + ... grund signal, 0 ist 0 aber nicht gut.
        ##self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * 50.0)).mean()
        self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * 100.0)).mean()


        self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-3) ## lr=1e-4
        ##self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-4) ## lr=1e-4
 
        if(self.loadCheckpoint()):
            print(self.logname,": loaded model from Storage!")
            ##ANNPathFinderSocket::ReceivePythonPrint NNServerPathfinder_NetA: loaded model from Storage!
            ##IS PRINTED.

            #debug
            super().exportNet()
    

    









