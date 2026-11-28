


import torch
import torch.nn as nn

print("import torch done")

from .NetTypes import TransformerNet as TransformerNet
from .NetTypes import TransformerNetB as TransformerNetB
from Base.Net.TensorInput import FTensor

from Base.Net import NetBase as NetBase

print("import unet done")



class NetB(NetBase.NetBase):

    def __init__(self):
        print("NNServerMovement_NetB: INIT!")
        super().__init__()        
        print("NNServerMovement_NetB: finish construct!")


    ##override
    def InitializeTensorAndPathNames(self):
        
        #### MUST BE OVERRIDEN IN SUBCLASSES - mirror unreal samples !!! #####
        Tensor = FTensor.FTensor(20,1,4)
        self.Tensors = [Tensor]

        self.outputTensor = FTensor.FTensor(5,1,4) ##output tensor

        self.OUT_CHANNELS = 1

        self.checkpointPath = "PyCheckpoint/netBcheckpoint.pth"
        self.ONNXPath = "Python/onnxExport/netB_ONNX.onnx"

        self.logname = "NNServerMovement_NetB"
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####

    ##override - called on construct aswell
    def ReInitNet(self):
        Tensor = self.GetInputTensor(0)


        numAttentionHeads = 4
        fnnHiddenDim = 64

        ##old
        #self.net = TransformerNet.TransformerNet(numAttentionHeads, fnnHiddenDim, Tensor, self.outputTensor)

        ##new
        embeddingDim = 128
        self.net = TransformerNetB.TransformerNetB(
            numAttentionHeads, 
            embeddingDim, 
            fnnHiddenDim, 
            Tensor, 
            self.outputTensor
        )
        
        
        
        ##move to gpu if available
        self.net = self.net.to(self.GetDevice())


        self.loss_fn = self.LossFunction


        self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-3) ## lr=1e-4
        ##self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-4) ## lr=1e-4
 
        if(self.loadCheckpoint()):
            print(self.logname,": loaded model from Storage!")
            ##ANNPathFinderSocket::ReceivePythonPrint NNServerPathfinder_NetA: loaded model from Storage!
            ##IS PRINTED.

            #debug
            super().exportNet()
    
    
    def LossFunction(self, pred, target):
        pred = pred.reshape(pred.shape[0], -1, 4)
        target = target.reshape(target.shape[0], -1, 4)

        position_loss = ((pred[..., :3] - target[..., :3]) ** 2).mean()
        time_loss = ((pred[..., 3] - target[..., 3]) ** 2).mean()
        
        position_weight = 0.75
        time_weight = 0.25
        
        return position_weight * position_loss + time_weight * time_loss










