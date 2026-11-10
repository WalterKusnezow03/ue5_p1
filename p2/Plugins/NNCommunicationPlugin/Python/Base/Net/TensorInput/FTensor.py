


class FTensor:
    def __init__(self, Width, Height, Channels):
        super().__init__()
        self.W = Width
        self.H = Height
        self.Channels = Channels

    def SumSizeTensor(self):
        return self.W * self.H * self.Channels