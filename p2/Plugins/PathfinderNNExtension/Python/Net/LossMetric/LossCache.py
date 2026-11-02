import matplotlib.pyplot as plt
import os

class LossCache:
    def __init__(self):
        self.CacheTrainSet = []
        self.CacheTestSet = []

    def addLossItem(self, lossValue, isTrain):
        if isTrain:
            self.CacheTrainSet.append(lossValue)
        else:
            self.CacheTestSet.append(lossValue)

    def save_plot(self, filename="loss_curve.png"):
        """Erzeugt und speichert den Graphen im selben Ordner wie diese Python-Datei."""
        if not self.CacheTrainSet and not self.CacheTestSet:
            print("Keine Loss-Daten zum Plotten vorhanden.")
            return

        # 1. Den exakten Ordner des Skripts ermitteln
        script_dir = os.path.dirname(os.path.abspath(__file__))
        
        # 2. Den Dateinamen mit dem Ordner verknüpfen
        full_path = os.path.join(script_dir, filename)

        plt.figure(figsize=(10, 5))

        # Train & Test Kurven einzeichnen
        if self.CacheTrainSet:
            plt.plot(self.CacheTrainSet, label="Train Loss", color="blue", linewidth=2)
        if self.CacheTestSet:
            plt.plot(self.CacheTestSet, label="Test Loss", color="red", linewidth=2)

        plt.title("Training & Test Loss")
        plt.xlabel("Steps / Epoch")
        plt.ylabel("Loss")
        plt.legend()
        plt.grid(True)

        # 3. Unter dem vollständigen Pfad speichern
        plt.savefig(full_path)
        plt.close() # Speicher freigeben
        print(f"Loss-Graph erfolgreich gespeichert unter: {full_path}")

    def __del__(self):
        # Optional: Versucht beim Löschen des Objekts automatisch zu speichern
        try:
            self.save_plot("loss_curve_final.png")
        except Exception:
            pass


######  DEBUG  ######
def TEST():
    cache = LossCache()

    for i in range(0,100):
        cache.addLossItem(1 / (i + 1) + i * 3, True)
    for i in range(0,100):
        cache.addLossItem(1 /  (i+1) + 2, False)
    cache.save_plot()

#TEST()