import torch
import torch.nn as nn
import torch.nn.functional as F

class TwoStreamConvNet(nn.Module):
    def __init__(self, in_ch1=4, in_ch2=3, num_outputs=360, out_ch=2):
        super().__init__()
        self.num_outputs = num_outputs  # 360
        self.out_ch = out_ch            # 2

        self.branch1 = self.conv_branch(in_ch1, 16, 32)
        self.branch2 = self.conv_branch(in_ch2, 16, 32)
        self.fusion_head = self.conv_fusionHead(32, 64, 128)
        
        # Mappt die 128 Features pro Schritt auf out_ch (2)
        ##self.fc = nn.Conv1d(128, out_ch, kernel_size=1)

        self.out = nn.Sequential(
            nn.Conv1d(128, 64, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.Conv1d(64, out_ch, kernel_size=1)  # <-- Hier endet es direkt linear ohne ReLU!
        )

    def conv_fusionHead(self, a, b, c):
        return nn.Sequential(
            ##nn.Conv2d(in_channels, out_channels, image_kernel_size, ...)
            nn.Conv1d(a + a, b, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.Conv1d(b, c, kernel_size=3, padding=1),
            nn.ReLU()
        )
    
    def conv_branch(self, a, b, c):
        return nn.Sequential(
            nn.Conv1d(a, b, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.Conv1d(b, c, kernel_size=3, padding=1),
            nn.ReLU()
        )

    def forward(self, x1, x2):
        B = x1.size(0)

        x1 = x1.view(B, 4, 360)  
        feat1 = self.branch1(x1)  # (B, 32, 360)
        feat2 = self.branch2(x2)  # (B, 32, 10)

        if feat2.size(2) != feat1.size(2):
            feat2 = F.interpolate(feat2, size=feat1.size(2), mode='linear', align_corners=False)

        fused = torch.cat([feat1, feat2], dim=1)  # (B, 64, 360)
        fusion_head_out = self.fusion_head(fused)        # (B, 128, 360)

        # 1. Conv1d anwenden -> Shape: (B, 2, 360)
        out = self.out(fusion_head_out)

        # 3. Exakt an das C++ Target-Format anpassen: (B, 2, 1, 360)
        # Fügt die H=1 Dimension an Index 2 ein
        out = out.unsqueeze(2)

        return out