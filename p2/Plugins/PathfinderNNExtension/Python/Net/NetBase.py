import torch
import torch.nn as nn
print("import torch done")

from . import NetCheckpoint as NetCheckpoint

print("import net checkpoint done")

#### BASE CLASS FOR NN (NetB, NetC...)
#### to have default functionality in export net etc
class NetBase(nn.Module):

    def __init__(self):
        super().__init__()

        print("NNServerPathfinder_NetBase: INIT!")

        ##self.memory = []
        self.latestX = None
        self.latestResult = None
        self.latestLoss = None
        self.isGpu = False

        #### MUST BE OVERRIDEN IN SUBCLASSES ! #####
        self.InitializeTensorAndPathNames()
        #### MUST BE OVERRIDEN IN SUBCLASSES ! #####
        
        self.ReInitNet()

        
        print("NNServerPathfinder_NetBase: finish construct!")

    def InitializeTensorAndPathNames(self):
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####
        self.W = 0
        self.H = 0
        self.IN_CHANNELS = 0
        self.OUT_CHANNELS = 1

        self.checkpointPath = "PyCheckpoint/netBcheckpoint.pth"
        self.ONNXPath = "Python/onnxExport/netB_ONNX.onnx"

        self.logname = "NNServerPathfinder_NetBase"
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####


    def exportNet(self):
        NetCheckpoint.ExportNet(self, self.logname, self.W, self.H, self.IN_CHANNELS, self.ONNXPath)
        return

    def saveCheckpoint(self):
        '''
        print("NNServerPathfinder_NetB: try save model!")
        torch.save({
            "model_state": self.state_dict(),
            "optimizer_state": self.optimizer.state_dict(),
            "latestLoss": self.latestLoss
        }, path)
        print("NNServerPathfinder_NetB:save model to Storage done!")
        '''
        NetCheckpoint.saveCheckpoint(self, self.logname, self.checkpointPath)
        return

    def loadCheckpoint(self):
        '''
        import os

        if not os.path.exists(path):
            print("NNServerPathfinder_NetB Checkpoint not found:", path)
            return False
        try:
            print("NNServerPathfinder_NetB Loading checkpoint:", path)
            checkpoint = torch.load(path, map_location="cpu")
            self.load_state_dict(checkpoint["model_state"], strict=True)

            # optimizer nur laden wenn kompatibel
            self.optimizer.load_state_dict(checkpoint["optimizer_state"])
            self.latestLoss = checkpoint.get("latestLoss", None)
            print("NNServerPathfinder_NetB Checkpoint loaded successfully")
            return True
        except Exception as e:
            print("NNServerPathfinder_NetB Checkpoint load failed:", e)
            return False
        '''
        print(self.logname,": TRY LOAD CHECKPOINT")
        return NetCheckpoint.loadCheckpoint(self, self.logname, self.checkpointPath)
        
    def ReInitNet():
        ##override needed!
        return
    
    def SwitchToCpu(self):
        self.isGpu = False
        self.ReInitNet()
        return
    
    def SwitchToGpu(self):
        self.isGpu = True
        self.ReInitNet()
        return

    def GetDevice(self):
        if(self.isGpu == False):
            return torch.device("cpu")
        
        isGpuAvailable = torch.backends.mps.is_available()
        if(isGpuAvailable):
            return torch.device("mps")
        
        ##device = torch.device(
        ##    "mps" if isGpuAvailable
        ##    else "cpu"
        ##)
        return torch.device("cpu")
    
    
    def __del__(self):
        self.saveCheckpoint()
        return



    #######  FORWARD SINGLE REQUEST FROM BIN ########

    def preprocessDataBinToTensor(self, data):
        size = self.H * self.W

        ##logik
        if(False):
            array = []
            for i in range (IN_CHANNELS):
                start = i * size
                end = (i+1) * size
                channelCurrent = torch.tensor(data[start : end], dtype=torch.float32).view(H, W) 
                array.append(channelCurrent)

            x = torch.stack(array, dim=0)  # (2, H, W)
            x = x.unsqueeze(0)
            return x

        ##besser wenn sowieso hintereinander gespeichert
        x = torch.tensor(data, dtype=torch.float32)
        x = x.view(self.IN_CHANNELS, self.H, self.W)
        x = x.unsqueeze(0)
        return x

    def postprocess(self, out):
        #out = out.squeeze(0).squeeze(0)  # (142, 142)
        #return out.reshape(-1)           # 20164
        return out.reshape(-1).detach() ##detach um vom net loszulösen
    

    def forwardBinData(self, data):
        x = self.preprocessDataBinToTensor(data)

        x = x.to(self.GetDevice())


        self.latestX = x.clone() ##copy incoming data

        fx = self.forward(x)
        return self.postprocess(fx)

    def forward(self, x):
        result = self.net(x) #### CAUTION: NET MUST BE DEFINED, net MUST BE GIVEN AS METHOD!
        self.latestResult = result
        return result



    ###### FOWRAD / TRAIN MULTI BATCH #######
    def trainBatchFor(self, batch_x, batch_y, iterations):
        for _ in range(iterations):
            self.trainBatch(batch_x, batch_y)

        return


    def trainBatch(self, batch_x, batch_y):
        pred = self.forward(batch_x)
        loss = self.loss_fn(pred, batch_y)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        self.latestLoss = loss.item()
        ##print("NNServerPathfinder_NetB: BACKWARD FINISH!")

    ######## call this for learning a large set ########
    def TrainFromBatchBinary(self, binary):
        self.SwitchToGpu()

        size = self.H * self.W
        ##sample: x + groundTruth
        
        sizeAll = (self.IN_CHANNELS + self.OUT_CHANNELS)
        sampleSize = size * sizeAll ## 4

        # =========================
        # 1) EINMALIG Tensor bauen
        # =========================
        data = torch.tensor(binary, dtype=torch.float32)

        numSamples = data.shape[0] // sampleSize
        data = data[:numSamples * sampleSize]

        # =========================
        # 2) Reshape in Samples
        # =========================
        ##data = data.view(numSamples, 4, H, W)
        data = data.view(numSamples, sizeAll, self.H, self.W)

        ##x = data[:, 0:3, :, :]   # input channels
        ##y = data[:, 3:4, :, :]   # target

        x = data[:, 0:self.IN_CHANNELS, :, :]   # input channels
        y = data[:, self.IN_CHANNELS:sizeAll, :, :]       # target


        # =========================
        # 3) GPU MOVE EINMAL
        # =========================
        x = x.to(self.GetDevice())
        y = y.to(self.GetDevice())

        print("samples:", numSamples)

        epochs = 50
        batch_size = 32

        for epoch in range(epochs):

            # shuffle indices (GPU-safe)
            ##perm = torch.randperm(numSamples, device=device)
            # shuffle indices CPU only
            perm = torch.randperm(numSamples)

            x_shuffled = x[perm]
            y_shuffled = y[perm]

            # batch loop
            for i in range(0, numSamples, batch_size):
                xb = x_shuffled[i:i+batch_size]
                yb = y_shuffled[i:i+batch_size]

                self.trainBatch(xb, yb)

            print(self.logname, "_RUN_NN_BATCH_EPOCH_FINISHED", epoch + 1, "_of_", epochs, " LOSS ", self.latestLoss)
        
        ##rmv from gpu
        x = None
        y = None

        ##saved by nn server.
        self.saveCheckpoint()
        self.exportNet()

        ##reload net
        self.SwitchToCpu()

        print(self.logname,"_RUN_NN_BATCH_TRAIN_FINISHED")
    
    