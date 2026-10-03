


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
        self.epochs = 300
        print("NNServerPathfinder_NetC: finish construct!")

    ##override
    ##override
    ##override
    def forward(self, *args):
        # Wenn nur 1 Argument übergeben wurde, und das eine Liste oder ein Tupel ist (von NetBase)
        if len(args) == 1 and isinstance(args[0], (list, tuple)):
            inputs = args[0]
        else:
            inputs = args

        print("NNServerPathfinder_NetC: forward!")

        # Universelle Bereinigung: Entfernt jegliche Dimensionen der Größe 1 (außer Batch und Channels)
        unpacked_args = []
        for t in inputs:
            while t.dim() > 3:
                squeezed = False
                for i in range(2, t.dim()):
                    if t.shape[i] == 1:
                        t = t.squeeze(i)
                        squeezed = True
                        break
                if not squeezed:
                    break
            unpacked_args.append(t)
        
        # An das eigentliche Netzwerk übergeben
        result = self.net(*unpacked_args)
                
        self.latestResult = result
        return result

    ##override
    def InitializeTensorAndPathNames(self):

        ## hitpoints total are (360 for player and 360 for all bots in total (360 * 4 (x,y split)))
        W = 360 
        H = 1
        C = 4 ## seperate channels for x and y (x1...xn, y1...yn)
        self.tensorA = FTensor.FTensor(W,H,C)

        W = 10 ##player trajectories (x,y,t) are always 10 
        H = 1
        C = 3
        self.tensorB = FTensor.FTensor(W,H,C)

        self.Tensors = [self.tensorA, self.tensorB]

        
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####
        self.OUT_CHANNELS = 2

        self.outputTensor = FTensor.FTensor(360, 1, self.OUT_CHANNELS) ##output tensor

        self.checkpointPath = "PyCheckpoint/netCcheckpoint.pth"
        self.ONNXPath = "Python/onnxExport/netC_ONNX.onnx"

        self.logname = "NNServerPathfinder_NetC"
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####

    

    

    ##override - called on construct aswell
    def ReInitNet(self):
        channelsInA = self.tensorA.Channels 
        channelsInB = self.tensorB.Channels 

        valuesOut = self.outputTensor.W
        channelsOut = self.OUT_CHANNELS ##1
        self.net = Net.TwoStreamConvNet(channelsInA, channelsInB, valuesOut, channelsOut)

        ##move to gpu if available
        self.net = self.net.to(self.GetDevice())

        ###bei falscher distanz eine besonders harte bestrafung, sonst gering
        ##increaseLoss = 20
        ##self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * increaseLoss)).mean()



        
        # Berechnet den euklidischen Abstand pro Sample: sqrt(dx^2 + dy^2)
        ##self.loss_fn = lambda pred, target: torch.sqrt(torch.sum((pred - target) ** 2, dim=-1) + 1e-8).mean()        
        
        self.loss_fn = nn.MSELoss()

        ##self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-3) ## lr=1e-4
        self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-4) ## lr=1e-5
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









