


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
        old_model_state = self.state_dict()
        old_optimizer_state = self.optimizer.state_dict()
        old_latest_loss = self.latestLoss
        
        try:
            checkpoint = torch.load(path)

            self.load_state_dict(checkpoint["model_state"])
            self.optimizer.load_state_dict(checkpoint["optimizer_state"])
            self.latestLoss = checkpoint["latestLoss"]
            return True
        except Exception as e:
            print(f"NNServerPathfinder_NetA error while loading model: {e}")
            # rollback
            self.load_state_dict(old_model_state)
            self.optimizer.load_state_dict(old_optimizer_state)
            self.latestLoss = old_latest_loss
            print("NNServerPathfinder_NetA State restored after failed load.")
            return False
    

    def __init__(self):
        super().__init__()

        ##self.memory = []
        self.latestX = None
        self.latestResult = None
        self.latestLoss = None

        channelsIn = 2
        channelsOut = 1
        

        ##out = in + 2p - k + 1


        self.net = nn.Sequential(
            ##nn.Conv2d(in_channels, out_channels, image_kernel_size, ...)
            nn.Conv2d(channelsIn, 16, 11, padding=5), ##out = in142 + 2*5 - 11 + 1 = 142
            nn.ReLU(),
            ## 16 -> 32 -> 16
            nn.Conv2d(16, 32, 7, padding=3), ##out = in142 + 2*3 - 7 + 1 = 142
            nn.ReLU(),

            ##32 -> 32
            nn.Conv2d(32, 32, 7, padding=3), ##out = in142 + 2*3 - 7 + 1 = 142
            nn.ReLU(),
            ##32 -> 32
            nn.Conv2d(32, 32, 7, padding=3), ##out = in142 + 2*3 - 7 + 1 = 142
            nn.ReLU(),
            ##32 -> 32
            nn.Conv2d(32, 32, 7, padding=3), ##out = in142 + 2*3 - 7 + 1 = 142
            nn.ReLU(),

            ##32 -> 16
            nn.Conv2d(32, 16, 3, padding=1), ##out = in142 + 2*1 - 3 + 1 = 142
            nn.ReLU(),
            
            ## 16 -> 1 (142 * 142)
            nn.Conv2d(16, channelsOut, 3, padding=1) ##out = in142 + 2*1 - 3 + 1 = 142 -> in out 142
            
            
            ##only between 0 and 1 values.
            ##,nn.Sigmoid()
        )


        ##out = in + 2p - k + 1
        self.net = nn.Sequential(
            ##11 is very washed out
            ##nn.Conv2d(channelsIn, 16, 11, padding=5), ##out = in142 + 2*5 - 11 + 1 = 142
            ##nn.ReLU(),
            nn.Conv2d(channelsIn, 16, 3, padding=1), ##out = in142 + 2*1 - 3 + 1 = 142
            nn.ReLU(),
            nn.Conv2d(16, 16, 3, padding=1), ##out = in142 + 2*1 - 3 + 1 = 142
            nn.ReLU(),
            nn.Conv2d(16, 32, 7, padding=3), ##out = in142 + 2*3 - 7 + 1 = 142
            nn.ReLU(),
            nn.Conv2d(32, 32, 7, padding=3), ##out = in142 + 2*3 - 7 + 1 = 142
            nn.ReLU(),
            nn.Conv2d(32, 16, 3, padding=1), ##out = in142 + 2*1 - 3 + 1 = 142
            nn.ReLU(),
            nn.Conv2d(16, channelsOut, 3, padding = 1) ##out = in142 + 2*1 - 3 + 1 = 142
        )

        ##self.loss_fn = nn.L1Loss() ##nn.MSELoss()
        ##self.loss_fn = nn.BCELoss()
        ##self.loss_fn = nn.MSELoss()

        ## nicht agressiv genug!
        ##der mse loss ist aber nicht gut wenn 0 auf 0 matched oft, obwohl 1 nicht richtig ist!!!!!
        ##self.loss_fn = nn.MSELoss()

        ##kein 0,1 problem.
        ##pos_weight = torch.tensor([2.0]) ##tut 30 mal mehr weh wenn falsch.
        ##self.loss_fn = nn.BCEWithLogitsLoss(pos_weight=pos_weight)


        ##distanz ^2
        ##wenn 0 dist * 1 - 0 = 0 yay
        ##wenn 1 dist * 1 //full loss

        ##lossPenetration = 1 
        ##self.loss_fn = lambda pred, target: (((pred - target) ** 2) * ((lossPenetration - target) ** 2)).mean()


        ##MSE
        ##self.loss_fn = lambda pred, target: ((pred - target) ** 2).mean()

        ##MSE aber bei falschem peak: 50 mal mehr loss, 1.0 + ... grund signal, 0 ist 0 aber nicht gut.
        self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1.0 + target * 50.0)).mean()


        ##wie kann man peaks hevorstechen lassen?
        ##ein exponential am ende
        ##softmax aber ungeeignet, mehrere sind richtig! Heat gewollt! nicht ein pixel! ggf mehrere



        ### mit sigmoid: bleibt stecken bei 0.25
        ###### From CPP REsult data: player location 1 (gaussian 5x5), else 0
        ##factor = 30.0 ##10
        ##self.loss_fn = lambda pred, target: (((pred - target) ** 2) * (1 + target * factor)).mean()


        ##negativ beispiel
        ##((pred - target).abs() -> 0 - 1 = -1
        ##(1 + target * 10) -> 1 + 1 = 11
        ##-1 * 11 = abs(-11) loss, bestrafung

        ##positiv beispiel
        ##((pred - target).abs() -> 1 - 1 = 0
        ##-1 * 11 = 0 loss, super gemacht



        self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-3) ## lr=1e-4
        ##self.optimizer = torch.optim.Adam(self.parameters(), lr=1e-4) ## lr=1e-4
 

        if(self.loadCheckpoint()):
            print("NNServerPathfinder_NetA: loaded model from Storage!")
            ##ANNPathFinderSocket::ReceivePythonPrint NNServerPathfinder_NetA: loaded model from Storage!
            ##IS PRINTED.

    def __del__(self):
        ##self.saveCheckpoint()
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

        channel0 = torch.tensor(data[:size], dtype=torch.float32).view(H, W)
        channel1 = torch.tensor(data[size:2*size], dtype=torch.float32).view(H, W)

        ##ergebnis:
        ##channel0.shape = (142, 142)
        ##channel1.shape = (142, 142)

        x = torch.stack([channel1, channel0], dim=0)  # (2, H, W)
        ##ergebnis 2,142,142

        x = x.unsqueeze(0) # (1, 2, H, W) -> (batch, dim, H, W)

        return x

    def forwardTwoChannelData(self, data):
        x = self.preprocessDataTwoChannel(data)
        self.latestX = x.clone() ##copy incoming data

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
    
    

    ############################ BACKWARD PASS SINGLE BATCH ############################
    ############################ BACKWARD PASS SINGLE BATCH ############################
    ############################ BACKWARD PASS SINGLE BATCH ############################

    def preprocessDataSingleChannel(self, data):
        x = torch.tensor(list(data[:H*W]), dtype=torch.float32)

        # reshape in 2D
        x = x.view(1, 1, H, W)  # (batch, channel, height, width)

        return x
    
    def learnData(self, data):
        y = self.preprocessDataSingleChannel(data)
        self.learnGroundTruth(y)

    def learnGroundTruth(self, real_y):
        ##train iterative
        if(self.trainSample(real_y, 1)):
            self.latestResult = None
            return



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

        print("NNServerPathfinder_NetA_RUN_NN_BACKPROP_END")

        self.latestResult = None

    #####  TESTING NEEDED  #####
    def trainSample(self, real_y, iterations=1):
        if(self.latestX == None):
            return False
        if(iterations <= 0):
            return False

        x = self.latestX
        y = real_y

        print("NNServerPathfinder_NetA_RUN_NN_BACKPROP_START_ITERATIONS ", iterations)
        for i in range(iterations):
            print("NNServerPathfinder_NetA_RUN_NN_BACKPROP_ITERATION ", i)
            pred = self.forward(x)
            loss = self.loss_fn(pred, y)

            


            self.optimizer.zero_grad()
            loss.backward()
            self.optimizer.step()

            self.latestLoss = loss.item()

            print("NNServerPathfinder_NetA pred mean:", pred.mean().item())
            print("NNServerPathfinder_NetA target mean:", real_y.mean().item())
            print("NNServerPathfinder_NetA loss:", loss.item())

        print("NNServerPathfinder_NetA_RUN_NN_BACKPROP_END_ITERATIONS ", iterations)

        self.latestX = None

        return True
    




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

    ######## call this for learning a large set ########
    def TrainFromBatchBinary(self, binary):
        xBatch, yBatch = self.DecomposeIntoBatches(binary)







        #### single all ####
        epochs = 50

        flag = False
        if(flag):
            print("NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_START", "epochs: ",epochs, " samples: ", len(xBatch))
            for i in range(epochs):
                self.trainBatch(xBatch, yBatch) ##batch completly faster than each sample on its own
                print("NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED", i + 1, " of ", epochs, " LOSS ", self.latestLoss)
        else:
            print("NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_START", "epochs: ",epochs, " samples: ", len(xBatch))
            sizeBatch = 32
            batches_x, batches_y = self.SplitIntoMiniBatches(xBatch, yBatch, sizeBatch)
            for i in range(epochs):
                for j in range(len(batches_x)):
                    self.trainBatch(batches_x[j], batches_y[j])
                
                print("NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED", i + 1, " of ", epochs, " LOSS ", self.latestLoss)

        
        return

    def SplitIntoMiniBatches(self, x, y, batch_size):
        # --- SHUFFLE ---
        perm = torch.randperm(x.shape[0])
        x = x[perm]
        y = y[perm]
        
        # --- SPLIT ---
        batches_x = []
        batches_y = []

        total = x.shape[0]

        for i in range(0, total, batch_size):
            xb = x[i:i+batch_size]
            yb = y[i:i+batch_size]

            batches_x.append(xb)
            batches_y.append(yb)

        return batches_x, batches_y

    def DecomposeIntoBatches(self, binary):
        size = H * W
        sampleSize = size * 3  # 2 input channels + 1 target

        xBatch = []
        yBatch = []

        numSamples = len(binary) // sampleSize

        for i in range(numSamples):
            offset = i * sampleSize

            # --- INPUT ---
            channel0 = torch.tensor(binary[offset : offset + size], dtype=torch.float32).view(H, W)
            channel1 = torch.tensor(binary[offset + size : offset + 2*size], dtype=torch.float32).view(H, W)

            x = torch.stack([channel1, channel0], dim=0)  # (2, H, W)
            xBatch.append(x)

            # --- TARGET ---
            y = torch.tensor(binary[offset + 2*size : offset + 3*size], dtype=torch.float32).view(1, H, W)
            yBatch.append(y)

        # --- STACK TO BATCH ---
        xBatch = torch.stack(xBatch, dim=0)  # (B, 2, H, W)
        yBatch = torch.stack(yBatch, dim=0)  # (B, 1, H, W)

        return xBatch, yBatch

    ######### BACKWARD PASS MULTI BATCH #########
    ######### BACKWARD PASS MULTI BATCH #########
    ######### BACKWARD PASS MULTI BATCH #########
