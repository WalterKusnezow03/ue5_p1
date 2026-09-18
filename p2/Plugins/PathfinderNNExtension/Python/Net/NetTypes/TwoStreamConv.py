import torch
import torch.nn as nn

class TwoStreamConvNet1D_Classifier(nn.Module):
    def __init__(self, in_ch1=2, in_ch2=3, num_outputs=2):
        super().__init__()

        # Branch 1: Für den 2-Kanal Input
        self.branch1 = self.conv_branch(in_ch1, 16, 32)

        # Branch 2: Für den 3-Kanal Input
        self.branch2 = self.conv_branch(in_ch2, 16, 32)

        # Gemeinsamer Conv-Head nach Late Fusion
        self.fusion_head = self.conv_fusionHead(32, 64, 128)

        # 1. Global Pooling: Reduziert egal welche Sequenzlänge auf (Batch, 128, 1)
        self.global_pool = nn.AdaptiveAvgPool1d(1)

        # 2. Linear Layer: Mappt 128 Features auf genau 2 Output-Werte
        self.fc = nn.Linear(128, num_outputs)

    def conv_fusionHead(self, a, b, c):

        return nn.Sequential(
            nn.Conv1d(a + a, b, kernel_size=3, padding=1),
            nn.BatchNorm1d(b),
            nn.ReLU(),
            nn.Conv1d(b, c, kernel_size=3, padding=1),
            nn.BatchNorm1d(c),
            nn.ReLU()
        )


        #return nn.Sequential(
        #    nn.Conv1d(32 + 32, 64, kernel_size=3, padding=1),
        #    nn.BatchNorm1d(64),
        #    nn.ReLU(),
        #    nn.Conv1d(64, 128, kernel_size=3, padding=1),
        #    nn.BatchNorm1d(128),
        #    nn.ReLU()
        #)


    def conv_branch(self, a, b, c):
        ##Padding = (KernelSize - 1) / 2
        ##return nn.Sequential(
        #    ##nn.Conv2d(in_channels, out_channels, image_kernel_size, ...)
        #    nn.Conv2d(a, b, 3, padding=1),
        #    nn.ReLU(),
        #    nn.Conv2d(b, b, 3, padding=1),
        #    nn.ReLU()
        #)

        # Kommentar zu "BatchNorm1d"
        # Batch Normalization (BatchNorm) normalisiert die 
        # Aktivierungen innerhalb eines Batches während des Trainings. 
        # Konkret sorgt es dafür, dass der Mittelwert nahe 0 
        # und die Standardabweichung nahe 1 liegen.
        

        return nn.Sequential(
            nn.Conv1d(a, b, kernel_size=3, padding=1),
            nn.BatchNorm1d(b),
            nn.ReLU(),
            nn.Conv1d(b, c, kernel_size=3, padding=1),
            nn.BatchNorm1d(c),
            nn.ReLU()
        )

        #return nn.Sequential(
        #    nn.Conv1d(in_ch1, 16, kernel_size=3, padding=1),
        #    nn.BatchNorm1d(16),
        #    nn.ReLU(),
        #    nn.Conv1d(16, 32, kernel_size=3, padding=1),
        #    nn.BatchNorm1d(32),
        #    nn.ReLU()
        #)


    



    def forward(self, x1, x2):
        # Input-Shapes: (B, 2, L) und (B, 3, L)
        
        # Branch Feature Extraction
        feat1 = self.branch1(x1)  # (B, 32, L)
        feat2 = self.branch2(x2)  # (B, 32, L)

        # Late Fusion
        fused = torch.cat([feat1, feat2], dim=1)  # (B, 64, L)
        features = self.fusion_head(fused)        # (B, 128, L)

        # Zeitdimension auf 1 reduzieren & flachklopfen
        pooled = self.global_pool(features)       # (B, 128, 1)
        flat = torch.flatten(pooled, 1)           # (B, 128)

        # Finale 2 Werte berechnen
        out = self.fc(flat)                       # (B, 2)

        return out