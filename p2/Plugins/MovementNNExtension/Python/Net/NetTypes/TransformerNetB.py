
import torch
import torch.nn as nn
import torch.nn.functional as F
import math

class TransformerNetB(nn.Module):

    def __init__(self, num_heads, embeddingDimension, ffn_hidden_dim, inTensor, outTensor):
        super().__init__()

        self.InTensor = inTensor
        self.OutTensor = outTensor

        d_model = inTensor.Channels

        ###new
        d_model = embeddingDimension
        self.input_projection = nn.Linear(inTensor.Channels, d_model)

        #if d_model != 4:
        #    raise ValueError("Movement tokens require d_model=4")
        assert d_model % num_heads == 0, "d_model muss durch num_heads teilbar sein"
        
        self.d_model = d_model
        self.num_heads = num_heads
        self.d_head = d_model // num_heads

        # Q, K, V und Output-Projektion als normale skalarprodukte (lin modell)
        self.WQ = nn.Linear(d_model, d_model, bias=False)
        self.WK = nn.Linear(d_model, d_model, bias=False)
        self.WV = nn.Linear(d_model, d_model, bias=False)
        self.Sum = nn.Linear(d_model, d_model, bias=False)

        self.norm1 = nn.LayerNorm(d_model)
        self.norm2 = nn.LayerNorm(d_model)

        self.ffn = nn.Sequential(
            nn.Linear(d_model, ffn_hidden_dim),
            nn.ReLU(),
            nn.Linear(ffn_hidden_dim, ffn_hidden_dim),
            nn.ReLU(),
            nn.Linear(ffn_hidden_dim, ffn_hidden_dim),
            nn.ReLU(),
            nn.Linear(ffn_hidden_dim, d_model),
        )

        #self.prediction = nn.Linear(d_model, outTensor.W * d_model)
        self.prediction = nn.Linear(d_model, outTensor.W * outTensor.Channels)

        ##positional encoding
        self.max_seq_len = 512
        position = torch.arange(self.max_seq_len).unsqueeze(1)
        div_term = torch.exp(
            torch.arange(0, d_model, 2) *
            (-math.log(10000.0) / d_model)
        )
        pe = torch.zeros(self.max_seq_len, d_model)
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term[:pe[:, 1::2].shape[1]])
        self.register_buffer("pos_encoding", pe)

    def GetQueryKeyValue(self, E):
         # x hat Shape: (batch_size, seq_len, d_model) und dtype=torch.float32
        B, T, C = E.shape

        # In Multi-Head aufteilen: (B, num_heads, T, d_head)
        #vorher:   (B, T, num_heads, d_head)
        #nachher:  (B, num_heads, T, d_head)
        q = self.WQ(E).view(B, T, self.num_heads, self.d_head).transpose(1, 2)
        k = self.WK(E).view(B, T, self.num_heads, self.d_head).transpose(1, 2)
        v = self.WV(E).view(B, T, self.num_heads, self.d_head).transpose(1, 2)
        return q,k,v

    ##oberes dreieck der matrix löschen um attention zu bereinigen, attention nur nach hinten
    #           Key 0  1  2
    #    Query 0:   y  n  n
    #    Query 1:   y  y  n
    #    Query 2:   y  y  y
    def RemoveAttentionToFutureKeys(self, x, attentionScores, sequenceLenght):
        # Causal Mask (oberes Dreieck auf -inf setzen)
        causal_mask = torch.triu(
            torch.ones(
                sequenceLenght, 
                sequenceLenght, 
                device=x.device, 
                dtype=torch.bool
            ), 
            diagonal=1
        )
        scores = attentionScores.masked_fill(causal_mask, float("-inf"))
        return scores

    def GetAttentionScores(self, Q, K):
        K_T = K.transpose(-2, -1)
        attentionScores = (Q @ K_T) / math.sqrt(self.d_head)
        return attentionScores

    def GetDeltaE(self, attentionScores, V):
        attention = F.softmax(attentionScores, dim=-1)
        deltaE = attention @ V
        return deltaE

    def InShape(self, x: torch.Tensor):
        #if x.ndim != 4 or tuple(x.shape[1:]) not in ((4, 1, 20), (4, 20, 1)):
        inTensor = self.InTensor
        #out_tensor.Channels, out_tensor.H, out_tensor.W
        if (
            x.ndim != 4 or
            tuple(x.shape[1:]) not in (
                (inTensor.Channels, inTensor.H, inTensor.W), 
                (inTensor.Channels, inTensor.W, inTensor.H)
            )
        ):
            return False
        return True

    def SkipConnection(self, x, deltaE, B, T, C):
        deltaE = deltaE.transpose(1, 2).contiguous().view(B, T, C)
        outE = self.norm1(x + self.Sum(deltaE))
        return outE

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        if not self.InShape(x):
            raise ValueError(
                f"Expected input shape (batch, {self.InTensor.Channels}, "
                f"{self.InTensor.H}, {self.InTensor.W}) or (batch, "
                f"{self.InTensor.Channels}, {self.InTensor.W}, {self.InTensor.H})"
            )

        ###new with Embedding Dimension
        x = x.reshape(x.shape[0], self.InTensor.W, self.InTensor.Channels)
        x = self.input_projection(x)  # (Batch, 20, 32)
        

        # NetBase preserves the token-major float buffer when reshaping.
        #x = x.reshape(x.shape[0], 20, self.d_model)
        #x = x.reshape(x.shape[0], self.InTensor.W, self.d_model)
        B, T, C = x.shape
        x = x + self.pos_encoding[:T].unsqueeze(0)

        # In Multi-Head aufteilen: (B, num_heads, T, d_head)
        q,k,v = self.GetQueryKeyValue(x)
        attentionScores = self.GetAttentionScores(q,k)
        attentionScores = self.RemoveAttentionToFutureKeys(x, attentionScores, T)

        deltaE = self.GetDeltaE(attentionScores, v)
        outE = self.SkipConnection(x, deltaE, B, T, C)

        outX = self.ffn(outE[:, -1, :])

        #return self.prediction(outX).reshape(B, 4, 1, 5)
        out_tensor = self.OutTensor
        return self.prediction(outX).reshape(B, out_tensor.Channels, out_tensor.H, out_tensor.W)

       
        #out_size = out_tensor.SumSizeTensor()
        #y = data[:, offset : offset + out_size].view(
        #    numSamples, out_tensor.Channels, out_tensor.H, out_tensor.W
        #)






