


import torch
import torch.nn as nn

##Input : 1 × 142 × 142
##Output: 1 × 142 × 142
H = 142
W = 142

class NetA(nn.Module):

    ##def saveModel(self, path="netA_weights.pth"):
    ##    torch.save(self.state_dict(), path)
    ##    print(f"Model NetA saved: : {path}")

    ##def loadModel(self, path="netA_weights.pth"):
    ##    try:
    ##        state = torch.load(path)
    ##        self.load_state_dict(state)
    ##        self.eval()
    ##        print(f"Model NetA loaded: {path}")
    ##        return True
    ##    except Exception as e:
    ##        print(f"error while loading model: {e}")
    ##        return False

    def saveCheckpoint(self, path="netAcheckpoint.pth"):
        print("NNServerPathfinder_NetA: try save model!")
        torch.save({
            "model_state": self.state_dict(),
            "optimizer_state": self.optimizer.state_dict(),
            "latestLoss": self.latestLoss
        }, path)
        print("NNServerPathfinder_NetA:saved_model_to_Storage!")

    def loadCheckpoint(self, path="netAcheckpoint.pth"):
        try:
            checkpoint = torch.load(path)

            self.load_state_dict(checkpoint["model_state"])
            self.optimizer.load_state_dict(checkpoint["optimizer_state"])
            self.latestLoss = checkpoint["latestLoss"]
            return True
        except Exception as e:
            print(f"NNServerPathfinder_NetA error while loading model: {e}")
            return False 
    

    def __init__(self):
        super().__init__()

        ##self.memory = []
        self.latestResult = None
        self.latestLoss = None

        channelsIn = 2
        channelsOut = 1
        self.net = nn.Sequential(
            ##nn.Conv2d(in_channels, out_channels, image_kernel_size, ...)
            nn.Conv2d(channelsIn, 16, 3, padding=1),
            nn.ReLU(),

            nn.Conv2d(16, 32, 3, padding=1),
            nn.ReLU(),

            nn.Conv2d(32, 16, 3, padding=1),
            nn.ReLU(),

            nn.Conv2d(16, channelsOut, 3, padding=1)
        )

        self.loss_fn = nn.MSELoss()
        self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-3)


        if(self.loadCheckpoint()):
            print("NNServerPathfinder_NetA: loaded model from Storage!")
            ##ANNPathFinderSocket::ReceivePythonPrint NNServerPathfinder_NetA: loaded model from Storage!
            ##IS PRINTED.

    def __del__(self):
        ##self.saveCheckpoint()
        return

    ############################ FORWARD PASS ############################
    ############################ FORWARD PASS ############################
    ############################ FORWARD PASS ############################
    def postprocess(self, out):
        #out = out.squeeze(0).squeeze(0)  # (142, 142)
        #return out.reshape(-1)           # 20164
        return out.reshape(-1).detach() ##detach um vom net loszulösen

    
    def preprocessDataTwoChannel(self, data):
        size = H * W

        channel0 = torch.tensor(data[:size], dtype=torch.float32).view(H, W)
        channel1 = torch.tensor(data[size:2*size], dtype=torch.float32).view(H, W)

        x = torch.stack([channel1, channel0], dim=0)  # (2, H, W)
        x = x.unsqueeze(0)                  # (1, 2, H, W)

        return x

    def forwardTwoChannelData(self, data):
        x = self.preprocessDataTwoChannel(data)
        fx = self.forward(x)
        return self.postprocess(fx)


    def forward(self, x):
        result = self.net(x)

        ##self.memory.append({
        ##    "x": x,
        ##    "pred": result
        ##})
        self.latestResult = result

        return result
    
    

    ############################ BACKWARD PASS ############################
    ############################ BACKWARD PASS ############################
    ############################ BACKWARD PASS ############################

    def preprocessDataSingleChannel(self, data):
        x = torch.tensor(list(data[:H*W]), dtype=torch.float32)

        # reshape in 2D
        x = x.view(1, 1, H, W)  # (batch, channel, height, width)

        return x
    
    def learnData(self, data):
        y = self.preprocessDataSingleChannel(data)
        self.learn(y)

    def learn(self, real_y):
        ##sample = self.memory.pop(0)
        ##pred = sample["pred"]
        if self.latestResult is None:
            return
        
        if(self.latestResult == None):
            return
        ##print("NNServerPathfinder_RUN_NN_BACKPROP_START")

        pred = self.latestResult
        loss = self.loss_fn(pred, real_y)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        self.latestLoss = loss.item()

        ##print("NNServerPathfinder_RUN_NN_BACKPROP_END")

        self.latestResult = None