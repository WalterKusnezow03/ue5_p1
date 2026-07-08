
import torch
import torch.nn as nn
import torch.nn.functional as F

class UNet(nn.Module):
    def __init__(self, in_ch=3, out_ch=1):
        super().__init__()

        ##channel nach importance gewichten
        ##self.channel_weight = nn.Parameter(torch.tensor([1.0, 1.0, 10.0]))

        # Encoder
        self.enc1 = self.conv_block(in_ch, 16) 
        self.enc2 = self.conv_block(16, 32)
        self.enc3 = self.conv_block(32, 64)

        self.pool = nn.MaxPool2d(2)

        # Bottleneck
        self.bottleneck = self.conv_block(64, 128)

        # Decoder
        self.up3 = nn.ConvTranspose2d(128, 64, 2, stride=2)
        self.dec3 = self.conv_block(128, 64)

        self.up2 = nn.ConvTranspose2d(64, 32, 2, stride=2)
        self.dec2 = self.conv_block(64, 32)

        self.up1 = nn.ConvTranspose2d(32, 16, 2, stride=2)
        self.dec1 = self.conv_block(32, 16)

        # Output
        self.out = nn.Conv2d(16, out_ch, 1)







    def conv_block(self, a, b):
        ##Padding = \frac{Kernel\_Size - 1}{2}
        return nn.Sequential(
            ##nn.Conv2d(in_channels, out_channels, image_kernel_size, ...)
            nn.Conv2d(a, b, 3, padding=1),
            nn.ReLU(),
            nn.Conv2d(b, b, 3, padding=1),
            nn.ReLU()
        )

    def forward(self, x):
        input_size = x.shape[2:]

        ##extra weight
        ##x = x * self.channel_weight.unsqueeze(0).unsqueeze(-1).unsqueeze(-1)



        e1 = self.enc1(x)
        e2 = self.enc2(self.pool(e1))
        e3 = self.enc3(self.pool(e2))

        b = self.bottleneck(self.pool(e3))

        d3 = self.up3(b)
        #d3 = self.match_tensor(d3, e3)
        d3 = torch.cat([d3, e3], dim=1) ##concatenate encoded to decoder
        d3 = self.dec3(d3)

        d2 = self.up2(d3)
        #d2 = self.match_tensor(d2, e2)
        d2 = torch.cat([d2, e2], dim=1)
        d2 = self.dec2(d2)

        d1 = self.up1(d2)
        #d1 = self.match_tensor(d1, e1)
        d1 = torch.cat([d1, e1], dim=1)
        d1 = self.dec1(d1)

        out = self.out(d1)

        # HARD GUARANTEE OUTPUT SIZE
        ##out = F.interpolate(out, size=input_size, mode="bilinear", align_corners=False)
        out = F.interpolate(out, size=input_size, mode="nearest") ##kein rand generieren, nur 0

        return out
    

    def match_tensor(self, x, ref):
        diffY = ref.size(2) - x.size(2)
        diffX = ref.size(3) - x.size(3)

        return F.pad(
            x,
            [diffX // 2, diffX - diffX // 2,
            diffY // 2, diffY - diffY // 2]
        )