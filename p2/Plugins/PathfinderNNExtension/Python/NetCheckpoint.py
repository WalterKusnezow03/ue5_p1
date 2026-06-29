import torch

def saveCheckpoint(net, logName, path="netBcheckpoint.pth"):
        print("NNServerPathfinder_NetB: try save model!")
        torch.save({
            "model_state": net.state_dict(),
            "optimizer_state": net.optimizer.state_dict(),
            "latestLoss": net.latestLoss
        }, path)
        print(logName,":save model to Storage done!")



def loadCheckpoint(net, logName, path="netBcheckpoint.pth"):
    import os

    if not os.path.exists(path):
        print(logName," Checkpoint not found:", path)
        return False
    try:
        print(logName," Loading checkpoint:", path)
        checkpoint = torch.load(path, map_location="cpu")
        net.load_state_dict(checkpoint["model_state"], strict=True)

        # optimizer nur laden wenn kompatibel
        net.optimizer.load_state_dict(checkpoint["optimizer_state"])
        net.latestLoss = checkpoint.get("latestLoss", None)
        print(logName," Checkpoint loaded successfully")
        return True
    except Exception as e:
        print(logName," Checkpoint load failed:", e)
        return False