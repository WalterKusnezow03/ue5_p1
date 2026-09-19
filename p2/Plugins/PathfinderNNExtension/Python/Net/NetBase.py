import torch
import torch.nn as nn
print("import torch done")

from . import NetCheckpoint as NetCheckpoint
from .TensorInput import FTensor

print("import net checkpoint done")

#### BASE CLASS FOR NN (NetB, NetC...)
#### to have default functionality in export net etc
#### SUPPORTS ONLY SINGLE BRANCH INPUT!
class NetBase(nn.Module):

    def __init__(self):
        super().__init__()
        self.epochs = 100 #300

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
        
        ##self.W = 0
        ##self.H = 0
        ##self.IN_CHANNELS = 0

        Tensor = FTensor.FTensor(144,144,4)
        self.Tensors = [Tensor]

        self.outputTensor = FTensor.FTensor(144,144,1) 

        self.OUT_CHANNELS = 1

        self.checkpointPath = "PyCheckpoint/netBcheckpoint.pth"
        self.ONNXPath = "Python/onnxExport/netB_ONNX.onnx"

        self.logname = "NNServerPathfinder_NetBase"
        #### MUST BE OVERRIDEN IN SUBCLASSES !!! #####


    def exportNet(self):
        NetCheckpoint.ExportNet(self, self.logname, self.ONNXPath)
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
        
    def ReInitNet(self):
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

    def GetInputTensor(self, index):
            return self.Tensors[index]

    #######  FORWARD SINGLE REQUEST FROM BIN ########

    '''def preprocessDataBinToTensor(self, data):
        tensor0 = self.GetInputTensor(0)

        size = tensor0.H * tensor0.W

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
        x = x.view(tensor0.Channels, tensor0.H, tensor0.W)
        x = x.unsqueeze(0)
        return x

    
    

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
    '''

    def postprocess(self, out):
        #out = out.squeeze(0).squeeze(0)  # (142, 142)
        #return out.reshape(-1)           # 20164
        return out.reshape(-1).detach() ##detach um vom net loszulösen

    def preprocessDataBinToTensor(self, data):
        # 1. Daten einmalig als 1D-Tensor laden
        flat_data = torch.tensor(data, dtype=torch.float32)

        inputs = []
        offset = 0

        # 2. Schleife über ALLE Eingangs-Tensors in self.Tensors
        for tensor in self.Tensors:
            t_size = tensor.SumSizeTensor()

            # Ausschnitt für diesen Tensor extrahieren: (C, H, W)
            t_data = flat_data[offset : offset + t_size].view(
                tensor.Channels, tensor.H, tensor.W
            )
            
            # Batch-Dimension hinzufügen: (1, C, H, W)
            t_data = t_data.unsqueeze(0)

            inputs.append(t_data)
            offset += t_size

        # Wenn nur 1 Tensor existiert, geben wir direkt den Tensor zurück.
        # Bei mehreren Tensors geben wir die Liste zurück.
        return inputs[0] if len(inputs) == 1 else inputs


    def forwardBinData(self, data):
        x = self.preprocessDataBinToTensor(data)

        # CPU/GPU-Move für Single- oder Multi-Tensor
        if isinstance(x, list):
            x = [t.to(self.GetDevice()) for t in x]
            self.latestX = [t.clone() for t in x]  # Deep-Copy der Eingabe
        else:
            x = x.to(self.GetDevice())
            self.latestX = x.clone()

        fx = self.forward(x)
        return self.postprocess(fx)


    def forward(self, x):
        # Falls x eine Liste ist, entpacken wir die Argumente für self.net(*x)
        # heisst aus: lsit(x0,x1..xn)
        # wird forward(x0, x1, ... xn) zu argumenten!
        if isinstance(x, list):
            result = self.net(*x)
        else:
            result = self.net(x)

        self.latestResult = result
        
        return result



    ###### FOWRAD / TRAIN MULTI BATCH #######
    def trainBatchFor(self, batch_x, batch_y, iterations):
        for _ in range(iterations):
            self.trainBatch(batch_x, batch_y)

        return

    def showLoss(self, gt):
        if self.latestResult is not None:
            # Convert gt to a Tensor if it isn't one already
            if not isinstance(gt, torch.Tensor):
                gt = torch.tensor(gt, dtype=torch.float32)
            
            # Ensure gt is on the same device as latestResult
            gt = gt.to(self.latestResult.device)

            # Ensure shapes match (reshape to outputTensor structure if raw binary array)
            if gt.shape != self.latestResult.shape:
                out = self.outputTensor
                gt = gt.view(1, out.Channels, out.H, out.W)

            loss = self.loss_fn(self.latestResult, gt)
            print(self.logname, "_LIVE_LOSS_", loss.item())

    def trainBatch(self, batch_x, batch_y):
        pred = self.forward(batch_x)
        loss = self.loss_fn(pred, batch_y)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        self.latestLoss = loss.item()
        ##print("NNServerPathfinder_NetB: BACKWARD FINISH!")

    ######## call this for learning a large set ########
    def InputTensorSizes(self):
        size = 0
        ##compute all sum( W * H * C )
        for tensor in self.Tensors:
            size += tensor.SumSizeTensor()

        return size

    def OutputTensorSize(self):
        return self.outputTensor.SumSizeTensor()

    def InputPlusOutputSize(self):
        return self.InputTensorSizes() + self.OutputTensorSize()


    '''def TrainFromBatchBinary(self, binary):
        self.SwitchToGpu()

        inputSize = self.InputTensorSizes()
        sampleSize = self.InputPlusOutputSize()

        ##old
        ##size = H * W
        ##sample: x + groundTruth
        
        ##sizeAll = (IN_CHANNELS + OUT_CHANNELS)
        ##sampleSize = size * sizeAll ## 4



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
        data = data.view(numSamples, sizeAll, tensor0.H, tensor0.W)

        ##stattdessen: data = data.view(numSamples, sizeAll, tensor0.H, tensor0.W)


        ##x = data[:, 0:3, :, :]   # input channels
        ##y = data[:, 3:4, :, :]   # target
        x = data[:, 0:tensor0.Channels, :, :]   # input channels
        y = data[:, tensor0.Channels:sizeAll, :, :]       # target


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

        print(self.logname,"_RUN_NN_BATCH_TRAIN_FINISHED")'''

    def TrainFromBatchBinary(self, binary):
        self.SwitchToGpu()
        device = self.GetDevice()

        sampleSize = self.InputPlusOutputSize()

        # ==========================================
        # 1) EINMALIG Tensor bauen & zuschneiden
        # ==========================================
        data = torch.tensor(binary, dtype=torch.float32)

        numSamples = data.shape[0] // sampleSize
        if numSamples == 0:
            print(f"{self.logname} WARNING: Nicht genügend Daten für ein vollständiges Sample.")
            return

        # Auf volle Samples zuschneiden und in (numSamples, sampleSize) bringen
        data = data[:numSamples * sampleSize].view(numSamples, sampleSize)

        # ==========================================
        # 2) Dynamische Extrahierung (Inputs & Target)
        # ==========================================
        x_list = []
        offset = 0

        # Alle Eingangs-Tensors verarbeiten
        for tensor in self.Tensors:
            t_size = tensor.SumSizeTensor()
            # Extrahiere flache Daten und Formate nach (Batch, C, H, W)
            t_data = data[:, offset : offset + t_size].view(
                numSamples, tensor.Channels, tensor.H, tensor.W
            )
            x_list.append(t_data)
            offset += t_size

        # Ziel-Tensor (Target) verarbeiten
        out_tensor = self.outputTensor
        out_size = out_tensor.SumSizeTensor()
        y = data[:, offset : offset + out_size].view(
            numSamples, out_tensor.Channels, out_tensor.H, out_tensor.W
        )

        # Wenn nur 1 Eingangs-Tensor vorhanden ist, packen wir ihn direkt als Tensor aus,
        # andernfalls bleibt es eine Liste von Tensors für Multi-Input Modelle.
        x = x_list[0] if len(x_list) == 1 else x_list

        # ==========================================
        # 3) GPU MOVE EINMAL
        # ==========================================
        if isinstance(x, list):
            x = [t.to(device) for t in x]
        else:
            x = x.to(device)
            
        y = y.to(device)

        print("samples:", numSamples)

        # ==========================================
        # 4) Training Loop
        # ==========================================
        
        batch_size = 32

        for epoch in range(self.epochs):
            # Indices direkt auf der GPU shuffeln (vermeidet CPU-GPU Synch)
            perm = torch.randperm(numSamples, device=device)

            # Shuffeln der Tensoren
            if isinstance(x, list):
                x_shuffled = [t[perm] for t in x]
            else:
                x_shuffled = x[perm]
                
            y_shuffled = y[perm]

            # Batch Loop
            for i in range(0, numSamples, batch_size):
                if isinstance(x_shuffled, list):
                    xb = [t[i : i + batch_size] for t in x_shuffled]
                else:
                    xb = x_shuffled[i : i + batch_size]
                    
                yb = y_shuffled[i : i + batch_size]

                self.trainBatch(xb, yb)

            print(self.logname, "_RUN_NN_BATCH_EPOCH_FINISHED", epoch + 1, "_of_", self.epochs, " LOSS ", self.latestLoss)

        # Cleanup GPU-Speicher
        del x, y, x_shuffled, y_shuffled
        if torch.cuda.is_available():
            torch.cuda.empty_cache()

        # Save & Export
        self.saveCheckpoint()
        self.exportNet()

        # Reload net
        self.SwitchToCpu()

        print(self.logname, "_RUN_NN_BATCH_TRAIN_FINISHED")
    
    