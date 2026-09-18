


import torch
import torch.nn as nn

print("import torch done")

from .NetTypes import TwoStreamConv as Net
from .TensorInput import FTensor

from . import NetBase as NetBase

print("import unet done")



class NetC(NetBase.NetBase):

    def __init__(self):
        print("NNServerPathfinder_NetC: INIT!")
        super().__init__()        
        print("NNServerPathfinder_NetC: finish construct!")


    ##override
    def InitializeTensorAndPathNames(self):

        ## hitpoints total are (360 for player and 360 for all bots in total (360 * 2))
        W = 360 * 2 
        H = 1
        C = 2 ## seperate channels for x and y (x1...xn, y1...yn)
        self.tensorA = FTensor.FTensor(W,H,C)

        W = 10 ##player trajectories (x,y,t) are always 10 
        C = 3
        self.tensorB = FTensor.FTensor(W,H,C)

        self.Tensors = [self.tensorA, self.tensorB]

        
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####
        self.OUT_CHANNELS = 1

        self.checkpointPath = "PyCheckpoint/netCcheckpoint.pth"
        self.ONNXPath = "Python/onnxExport/netC_ONNX.onnx"

        self.logname = "NNServerPathfinder_NetC"
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####

    ##override - called on construct aswell
    def ReInitNet(self):
        channelsInA = self.tensorA.Channels 
        channelsInB = self.tensorB.Channels 

        channelsOut = self.OUT_CHANNELS ##1
        self.net = Net.TwoStreamConvNet1D_Classifier(channelsInA, channelsInB, channelsOut)

        ##move to gpu if available
        self.net = self.net.to(self.GetDevice())

        ### todo einfach distanz 

        ##MSE aber bei falschem peak: 50 mal mehr loss, 1.0 + ... grund signal, 0 ist 0 aber nicht gut.
        ##self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * 50.0)).mean()
        increaseLoss = 1 # 50, 100
        self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * increaseLoss)).mean()


        self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-3) ## lr=1e-4
        ##self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-4) ## lr=1e-4
 
        if(self.loadCheckpoint()):
            print(self.logname,": loaded model from Storage!")
            ##ANNPathFinderSocket::ReceivePythonPrint NNServerPathfinder_NetA: loaded model from Storage!
            ##IS PRINTED.

            #debug
            super().exportNet()
    

    ### ----- not anymore! -----
    ### update forward data!!!
    ### update train function!!!









