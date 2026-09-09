import torch
'''
def saveCheckpoint(net, logName, path="PyCheckpoint/netBcheckpoint.pth"):
        print("NNServerPathfinder_NetB: try save model!")
        torch.save({
            "model_state": net.state_dict(),
            "optimizer_state": net.optimizer.state_dict(),
            "latestLoss": net.latestLoss
        }, path)
        print(logName,":save model to Storage done!")



def loadCheckpoint(net, logName, path="PyCheckpoint/netBcheckpoint.pth"):
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
'''
def saveCheckpoint(net, logName, path="PyCheckpoint/netBcheckpoint.pth"):
    import os
    # Den absoluten Pfad auflösen
    absolute_path = os.path.abspath(path)
    
    print(f"NNServerPathfinder_NetB: try save model! Target: {absolute_path}")
    
    # WICHTIG FÜR DEN BUILD: 
    # Wenn der Ordner "PyCheckpoint" im Arbeitsverzeichnis noch nicht existiert,
    # stürzt torch.save ab. Wir erstellen ihn hier sicherheitshalber:
    os.makedirs(os.path.dirname(absolute_path), exist_ok=True)
    
    try:
        torch.save({
            "model_state": net.state_dict(),
            "optimizer_state": net.optimizer.state_dict(),
            "latestLoss": net.latestLoss
        }, absolute_path)
        print(logName, ":save model to Storage done!")
    
    except Exception as e:
        print(logName, f":CRITICAL save failed to {absolute_path}. Error: {e}")



def loadCheckpoint(net, logName, path="PyCheckpoint/netBcheckpoint.pth"):
    import os
    # Den absoluten Pfad auflösen
    absolute_path = os.path.abspath(path)
    
    print(f"{logName} Looking for checkpoint at: {absolute_path}")

    if not os.path.exists(absolute_path):
        print(logName, " Checkpoint not found at:", absolute_path)
        return False
    try:
        print(logName, " Loading checkpoint:", absolute_path)
        checkpoint = torch.load(absolute_path, map_location="cpu")
        net.load_state_dict(checkpoint["model_state"], strict=True)

        net.optimizer.load_state_dict(checkpoint["optimizer_state"])
        net.latestLoss = checkpoint.get("latestLoss", None)
        print(logName, " Checkpoint loaded successfully")
        return True
    except Exception as e:
        print(logName, " Checkpoint load failed:", e)
        return False
    






def ExportNet(
    net, 
    logName, 
    WIDTH, HEIGHT,
    Channels,
    path="Python/onnxExport/netB_ONNX.onnx"
):
    
    ##debug
    ##return
   
    import os

    # 1. Sicherstellen, dass der Zielordner (z.B. "PyONNX") existiert
    # Falls er nicht existiert, erstellt os.makedirs ihn automatisch, um Crashs zu vermeiden
    dir_name = os.path.dirname(path)
    if dir_name and not os.path.exists(dir_name):
        os.makedirs(dir_name, exist_ok=True)

    # 2. Dummy Input erstellen
    # Batchgröße = 1
    # Kanäle = 1 (z. B. Graustufen/Hindernis-Map)
    # Höhe = 256, Breite = 256
    # dummy_input = torch.randn(1, 1, 256, 256)
    dummy_input = torch.randn(1, Channels, WIDTH, HEIGHT)

    fastExport = True
    if(fastExport == False):
        torch.onnx.export(
            net, 
            dummy_input, 
            path,                         # Übergabe der Pfad-Variablen
            ##opset_version=13,             # Exakt auf 13 gesetzt (Maximum für deine UE-Library)
            input_names=['input'],        # Identifikator für den Eingang in C++
            output_names=['output'],      # Identifikator für den Ausgang in C++
            do_constant_folding=True      # Optimiert das Modell beim Exportieren
        )
    else:
        print(logName, " EXPORT NET ONNX FAST", path)
        ##nein
        '''torch.onnx.export(
            net, 
            dummy_input, 
            path,                         
            opset_version=18,             # Nutze die native, moderne Version
            input_names=['input'],        
            output_names=['output'],      
            do_constant_folding=False     # <--- HIER AUF FALSE SETZEN! Spart enorm viel Zeit.
        )'''
        # Erzwinge den klassischen, schnellen Export-Modus

        ##nein
        '''
        torch.onnx.export(
            net, 
            dummy_input, 
            path,                         
            opset_version=18,             
            input_names=['input'],        
            output_names=['output'],      
            do_constant_folding=False,
            # WICHTIG: Verhindert, dass PyTorch die neue, langsame Dynamo-Engine startet
            export_params=True,
            training=torch.onnx.TrainingMode.EVAL
        )'''
        try:
            torch.onnx.utils.export(
                net, 
                dummy_input, 
                path,                         
                opset_version=13,             
                input_names=['input'],        
                output_names=['output'],      
                do_constant_folding=False
            )
        except Exception as e:
            print(logName, f":CRITICAL EXPORT NET ONNX save failed to {path}. Error: {e}")
        print(logName, " EXPORT NET ONNX FAST FINSHED ! ", path)
    


    print(logName, " EXPORT ONNX NET", path)
    import onnx  # <--- HIER IMPORTIEREN!
    print(logName, " DEBUG LOAD ONNX NET", path)
    # Lade dein exportiertes Modell
    model = onnx.load(path)

    # 1. IR-Version auslesen (Das ist das grundlegende Dateiformat)
    print(f"ONNX IR-Version der Datei: {model.ir_version}")

    # 2. Opset-Version auslesen (Die mathematischen Operatoren)
    opset_version = model.opset_import[0].version if len(model.opset_import) > 0 else "Unbekannt"
    print(f"ONNX Opset-Version der Datei: {opset_version}")
