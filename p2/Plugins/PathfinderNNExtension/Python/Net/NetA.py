from .. import communicationPluginParent
import CNNBase
import torch.nn as nn

##Input : 1 × 142 × 142
##Output: 1 × 142 × 142
H = 142
W = 142

class NetA(nn.Module):

    ###### noch alles sehr unklar #######

    def __init__(self):
        super().__init__()

        self.memory = []

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

    
    def postprocess(self, out):
        #out = out.squeeze(0).squeeze(0)  # (142, 142)
        #return out.reshape(-1)           # 20164
        return out.view(-1)



    #def preprocessDataSingleChannel(self, data):
    #    x = torch.tensor(list(data[:H*W]), dtype=torch.float32)

        # reshape in 2D
    #    x = x.view(1, 1, H, W)  # (batch, channel, height, width)

    #    return x
    
    def preprocessDataTwoChannel(self, data):
        size = H * W

        channel0 = torch.tensor(data[:size], dtype=torch.float32).view(H, W)
        channel1 = torch.tensor(data[size:2*size], dtype=torch.float32).view(H, W)

        x = torch.stack([channel1, channel0], dim=0)  # (2, H, W)
        x = x.unsqueeze(0)                  # (1, 2, H, W)

        return x




    def forward(self, x):
        result = self.net(x)

        self.memory.append({
            "x": x,
            "pred": result
        })

        return result
    
    
    def learn(self, real_y):
        sample = self.memory.pop(0)

        pred = sample["pred"]

        loss = self.loss_fn(pred, real_y)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        return loss.item()