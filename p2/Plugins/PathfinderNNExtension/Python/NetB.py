


import torch
import torch.nn as nn

print("import torch done")

import UNet
import NetCheckpoint

print("import unet done")

##Input : 1 × 142 × 142
##Output: 1 × 142 × 142
H = 144 #142
W = 144 #142

IN_CHANNELS = 4
OUT_CHANNELS = 1

class NetB(nn.Module):

    def exportNet(self):
        NetCheckpoint.ExportNet(self, "NNServerPathfinder_NetB", W, H, IN_CHANNELS)
        return

    def saveCheckpoint(self, path="PyCheckpoint/netBcheckpoint.pth"):
        '''
        print("NNServerPathfinder_NetB: try save model!")
        torch.save({
            "model_state": self.state_dict(),
            "optimizer_state": self.optimizer.state_dict(),
            "latestLoss": self.latestLoss
        }, path)
        print("NNServerPathfinder_NetB:save model to Storage done!")
        '''
        NetCheckpoint.saveCheckpoint(self, "NNServerPathfinder_NetB", path)
        return

    def loadCheckpoint(self, path="PyCheckpoint/netBcheckpoint.pth"):
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
        print("NNServerPathfinder_NetB: TRY LOAD CHECKPOINT")
        return NetCheckpoint.loadCheckpoint(self, "NNServerPathfinder_NetB", path)
        

   
    

    def __init__(self):
        super().__init__()

        print("NNServerPathfinder_NetB: INIT!")

        ##self.memory = []
        self.latestX = None
        self.latestResult = None
        self.latestLoss = None
        self.isGpu = False

        
        
        self.ReInitNet()

        
        print("NNServerPathfinder_NetB: finish construct!")


    


    
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

    def ReInitNet(self):
        channelsIn = IN_CHANNELS ##3
        channelsOut = OUT_CHANNELS ##1
        self.net = UNet.UNet(channelsIn, channelsOut)
        ##move to gpu if available
        self.net = self.net.to(self.GetDevice())


        ##MSE aber bei falschem peak: 50 mal mehr loss, 1.0 + ... grund signal, 0 ist 0 aber nicht gut.
        ##self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * 50.0)).mean()
        self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * 100.0)).mean()


        self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-3) ## lr=1e-4
        ##self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-4) ## lr=1e-4
 
        if(self.loadCheckpoint()):
            print("NNServerPathfinder_NetB: loaded model from Storage!")
            ##ANNPathFinderSocket::ReceivePythonPrint NNServerPathfinder_NetA: loaded model from Storage!
            ##IS PRINTED.

            #debug
            self.exportNet()
        

    def __del__(self):
        self.saveCheckpoint()
        return

    ############################ FORWARD PASS SINGLE BATCH ############################
    ############################ FORWARD PASS SINGLE BATCH ############################
    ############################ FORWARD PASS SINGLE BATCH ############################

    def postprocess(self, out):
        #out = out.squeeze(0).squeeze(0)  # (142, 142)
        #return out.reshape(-1)           # 20164
        return out.reshape(-1).detach() ##detach um vom net loszulösen

    ##channel 0: polygon map
    ##channel 1: player last visited time (relative to 0 (-4,-3,-2...))
    def preprocessDataTwoChannel(self, data):
        size = H * W

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
        x = x.view(IN_CHANNELS, H, W)
        x = x.unsqueeze(0)
        return x




        #channel0 = torch.tensor(data[ : size], dtype=torch.float32).view(H, W) ##polygon channel
        #channel1 = torch.tensor(data[size : 2*size], dtype=torch.float32).view(H, W) ##player trajectory channel
        #channel2 = torch.tensor(data[size*2 : 3*size], dtype=torch.float32).view(H, W) ##vision cone channel

        ##ergebnis:
        ##channel0.shape = (142, 142)
        ##channel1.shape = (142, 142)

        #x = torch.stack([channel2, channel1, channel0], dim=0)  # (2, H, W)
        ##ergebnis 2,142,142

        #x = x.unsqueeze(0) # (1, 2, H, W) -> (batch, dim, H, W)

        #return x

    def forwardTwoChannelData(self, data):
        x = self.preprocessDataTwoChannel(data)

        x = x.to(self.GetDevice())


        self.latestX = x.clone() ##copy incoming data

        fx = self.forward(x)
        return self.postprocess(fx)


    def forward(self, x):
        print("NNServerPathfinder_NetB: FOWARD!")
        result = self.net(x)

        ##self.memory.append({
        ##    "x": x,
        ##    "pred": result
        ##})
        self.latestResult = result
        print("NNServerPathfinder_NetB: FOWARD FINISH!")

        return result
    
    

    

    
    
    




    ######### FORWARD PASS MULTI BATCH #########
    ######### FORWARD PASS MULTI BATCH #########
    ######### FORWARD PASS MULTI BATCH #########
    

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
        print("NNServerPathfinder_NetB: BACKWARD FINISH!")

    ######## call this for learning a large set ########
    def TrainFromBatchBinary(self, binary):
        self.SwitchToGpu()

        size = H * W
        ##sample: x + groundTruth
        
        sizeAll = (IN_CHANNELS + OUT_CHANNELS)
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
        data = data.view(numSamples, sizeAll, H, W)

        ##x = data[:, 0:3, :, :]   # input channels
        ##y = data[:, 3:4, :, :]   # target

        x = data[:, 0:IN_CHANNELS, :, :]   # input channels
        y = data[:, IN_CHANNELS:sizeAll, :, :]       # target


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

            print("NNServerPathfinder_NetB_RUN_NN_BATCH_EPOCH_FINISHED", epoch + 1, "_of_", epochs, " LOSS ", self.latestLoss)
        
        ##rmv from gpu
        x = None
        y = None

        ##saved by nn server.
        self.saveCheckpoint()
        self.exportNet()

        self.SwitchToCpu()
    
    
    









