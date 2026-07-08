#include "ONNXModel.h"

#include "Misc/FileHelper.h"


ONNXModel::ONNXModel(){
    bModelWasLoaded = false;
}

ONNXModel::~ONNXModel(){
    Reset();
}

void ONNXModel::Reset(){
    delete Session;
    delete memoryInfo;
    //delete env;
    Session = nullptr;
    memoryInfo = nullptr;
    //env = nullptr;
    bModelWasLoaded = false;
}

FString ONNXModel::MakePath(){
    FString pluginName = "PathFinderNNExtension";
    FString pluginDir = PluginDir(pluginName);

    
    FString PythonONNXPart = FString::Printf(TEXT("Python/onnxExport/netB_ONNX.onnx"));
    return FPaths::Combine(pluginDir, PythonONNXPart);
}

FString ONNXModel::PluginDir(FString pluginName){
    FString inner = pluginName; //refactured. // FString::Printf(TEXT("%s/Source/%s"), *pluginName, *pluginName);
    FString pluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        *inner
    );
    DebugHelper::logMessage("ONNXLoader::PluginDir --> ", pluginDir);
    return pluginDir;
}



void ONNXModel::LoadModel(){
    // 1. Pfad zur ONNX-Datei definieren (z.B. im Saved- oder Projekt-Ordner)
    FString FilePath = MakePath();
    LoadModel(FilePath);
}

void ONNXModel::LoadModel(FString ModelPath)
{
    try{
        DebugHelper::logMessage("ONNXModel 0) try load enviroment!: ", ModelPath);
        InitEnviroment(ModelPath);
        DebugHelper::logMessage("ONNXModel 1) loaded enviroment!: ", ModelPath);

        InitTensor();
        DebugHelper::logMessage("ONNXModel 2) loaded tensor!: ", ModelPath);

        InitInputAndOutPutNames();
        DebugHelper::logMessage("ONNXModel 3) loaded input output!: ", ModelPath);

        bModelWasLoaded = true;
        DebugHelper::logMessage("ONNXModel loaded!: ", ModelPath);

        // Ab hier kannst du mit Session.GetInputCount() etc. ganz normal arbeiten.
    }catch (const std::exception& e){
        FString ErrorMsg = FString(UTF8_TO_TCHAR(e.what()));
        UE_LOG(LogTemp, Error, TEXT("ONNX-Fehler: %s"), *ErrorMsg);
    }
}

/*
void ONNXModel::InitEnviroment(FString ModelPath){
    // 1. Microsoft Umgebung initialisieren
    //static Ort::Env Env(ORT_LOGGING_LEVEL_WARNING, "ONNX_Mac_Session");
    //env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNX_Mac_Session");

    // Übergib ORT_API_VERSION als dritten Parameter!
    env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNX_Mac_Session", ORT_API_VERSION);
   
    Ort::SessionOptions localSessionOptions;
    localSessionOptions.SetIntraOpNumThreads(2); 
    
    
    // 3. Modell direkt laden (Konvertierung von FString zu Mac-kompatiblem Pfad)
    std::string StdModelPath = TCHAR_TO_UTF8(*ModelPath);// Perfekt abgestimmt fürs M4-Air-Multithreading
    
    //// Aus der offiziellen onnxruntime_cxx_api.h
    //Env(OrtLoggingLevel default_logging_level, const char* logid, uint32_t api_version = ORT_API_VERSION);

    // Session erstellen lädt und parst das Modell direkt über Microsofts Core-Code!
    //Ort::Session Session(Env, StdModelPath.c_str(), SessionOptions);
    Session = new Ort::Session(*env, StdModelPath.c_str(), localSessionOptions);
    
    UE_LOG(LogTemp, Log, TEXT("ONNX-Model was loaded!"));
}*/
/*
void ONNXModel::InitEnviroment(FString ModelPath)
{
    // Wir nutzen exakt den vom Compiler vorgeschlagenen Konstruktor mit 2 Parametern.
    // Das 'static' sorgt dafür, dass die Umgebung im Mac-Speicher überlebt.
    static Ort::Env StaticEnv(ORT_LOGGING_LEVEL_WARNING, "ONNX_Mac_Session");
   
    Ort::SessionOptions localSessionOptions;
    localSessionOptions.SetIntraOpNumThreads(2); 
    
    // Modell-Pfad für die Microsoft C-API konvertieren
    std::string StdModelPath = TCHAR_TO_UTF8(*ModelPath);
    
    // Session erstellen und die statische Umgebung übergeben
    Session = new Ort::Session(StaticEnv, StdModelPath.c_str(), localSessionOptions);
    
    UE_LOG(LogTemp, Log, TEXT("ONNX-Model wurde erfolgreich initialisiert!"));
}*/
void ONNXModel::InitEnviroment(FString ModelPath)
{
    // 1. Safe dynamic initialization for the environment
    static Ort::Env* StaticEnv = nullptr;
    if (StaticEnv == nullptr)
    {
        // This defers creation until the execution flow safely reaches here
        StaticEnv = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNX_Mac_Session");
    }
    DebugHelper::logMessage("ONNXModel init ENV!: ", ModelPath);
   
    Ort::SessionOptions localSessionOptions;
    localSessionOptions.SetIntraOpNumThreads(2); 

    DebugHelper::logMessage("ONNXModel init SESSION OPTIONS!: ", ModelPath);
    
    // Convert Unreal FString to std::string
    std::string StdModelPath = TCHAR_TO_UTF8(*ModelPath);
    
    // 2. Protect against memory leaks if InitEnvironment is called multiple times
    if (Session != nullptr)
    {
        delete Session;
        Session = nullptr;
    }

    try 
    {
        // Pass the dereferenced pointer (*StaticEnv)
        Session = new Ort::Session(*StaticEnv, StdModelPath.c_str(), localSessionOptions);
        DebugHelper::logMessage("ONNXModel init SESSION!: ", ModelPath);
    
        UE_LOG(LogTemp, Log, TEXT("ONNX-Model wurde erfolgreich initialisiert!"));
    }
    catch (const std::exception& e)
    {
        FString ErrorMsg(e.what());
        UE_LOG(LogTemp, Error, TEXT("ONNX Session creation failed: %s"), *ErrorMsg);
    }
}


//to be refractured into PathfinderNN ONNX
void ONNXModel::InitTensor(){
    InitTensor(144, 144, 4);
}

void ONNXModel::InitTensor(int W, int H, int channels){
    //batch 1
    //channels 4
    //dim x,y = 142

    /*
    Python analog:

    x = torch.tensor(data, dtype=torch.float32)
    x = x.view(IN_CHANNELS, H, W)
    x = x.unsqueeze(0)

    */

    // 1. Die finale Shape definieren: [Batch, Channels, Height, Width]
    // Entspricht exakt: x.view(4, 142, 142) -> unsqueeze(0)
    //std::vector<int64_t> InputDims = { 1, 4, 142, 142 };
    //std::vector<int64_t> inputDimensions = { 1, channels, H, W };
    inputDimensions = { 1, channels, H, W };
    int BufferSize = 1 * channels * H * W;

    // 2. Den flachen C++ Buffer mit der passenden Gesamtgröße reservieren
    // 1 * 4 * 142 * 142 = 80656 Elemente
    inputTensorValues = std::vector<float>(BufferSize, 0.0f);

    // Speicherplatz für den Tensor zuweisen (MemoryInfo)
    //memoryInfo = new Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
    // Ersetze Zeile 118 durch:
    memoryInfo = new Ort::MemoryInfo(
        Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault)
    );

    // Den eigentlichen Input-Tensor erstellen (Das bindet deine C++ Daten an ONNX)
    inputTensor = Ort::Value::CreateTensor<float>(
        *memoryInfo, 
        inputTensorValues.data(), //direkter pointer <- hier können dann MemCpy operations stattfinden um 
        //einen neuen forward pass zu erzeugen
        inputTensorValues.size(), 
        inputDimensions.data(), 
        inputDimensions.size()
    );

}

void ONNXModel::InitInputAndOutPutNames(){
    /*
    //PYTHON: Onnx export wie folgt:
    torch.onnx.export(
        net,
        dummy_input,
        path,           # <--- Hier die Variable statt des festen Strings nutzen!
        opset_version=12,
        input_names=['input'],   # Hilft Unreal Engine, den Eingang zu identifizieren
        output_names=['output']  # Hilft Unreal Engine, den Ausgang zu identifizieren
    )*/
    //const char* InputNames[] = { "input_name" };  // Ändere das zu deinem echten Input-Namen
    //const char* OutputNames[] = { "output_name" }; // Ändere das zu deinem echten Output-Namen

    InputNames = { "input" };  // Ändere das zu deinem echten Input-Namen
    OutputNames = { "output" }; // Ändere das zu deinem echten Output-Namen
}

bool ONNXModel::CopyDataToTensor(TArray<float> &buffer){
    TArrayView<float> FullViewBuffer(buffer);
    return CopyDataToTensor(FullViewBuffer);
}

bool ONNXModel::CopyDataToTensor(const TArrayView<float> &buffer){
    if(bModelWasLoaded){
        if(buffer.Num() > 0 && buffer.Num() == inputTensorValues.size()){
            //copy buffer in into input tensor std vector
            //FMemory::Memcpy(*Target, *Src, sizeT)
            FMemory::Memcpy(
                inputTensorValues.data(),  // Ziel: Die direkte RAM-Adresse des std::vector, als ref in tensor drin!
                buffer.GetData(),          // Quelle: Die direkte RAM-Adresse des Unreal TArray
                buffer.Num() * sizeof(float) // Größe in Bytes: Anzahl der Floats * 4 Bytes
            );
            return true;
        }
    }
    return false;
}

bool ONNXModel::Forward(std::vector<Ort::Value> &output){
    //auto OutputTensors = 
    if(!Session){
        return false;
    }
    

    output = Session->Run(
        Ort::RunOptions{nullptr}, 
        InputNames.data(), 
        &inputTensor, 
        1, 
        OutputNames.data(), 
        1
    );
    if (!output.empty() && output[0].IsTensor()){
        return true;
    }
    return false;
}

bool ONNXModel::Forward(TArray<float> &bufferPredictionOut){
    std::vector<Ort::Value> outputTensors;
    if(Forward(outputTensors)){
        
        Ort::Value& SingleOutput = outputTensors[0];
        
        // 1. Gesamte Anzahl an Elementen bestimmen 
        // Bei H=142, W=142 und Out_Channels=1 sind das genau 142 * 142 * 1 = 20164 Elemente
        size_t TotalElements = SingleOutput.GetTensorTypeAndShapeInfo().GetElementCount();
        
        if (TotalElements > 0){
            // 2. Das Unreal-Ausgabe-Array auf die exakte Größe bringen (entspricht dem Ziel-Reshaping)
            bufferPredictionOut.SetNumUninitialized(TotalElements);
            
            // 3. Den rohen Float-Pointer aus dem Tensor extrahieren
            float* SourceData = SingleOutput.GetTensorMutableData<float>();
            
            // 4. Daten ohne Schleife direkt in das Unreal-Array kopieren (das entspricht .detach())
            FMemory::Memcpy(
                bufferPredictionOut.GetData(),       // Ziel: Unser flaches Unreal-TArray
                SourceData,                          // Quelle: Der flache Microsoft-Speicher block
                TotalElements * sizeof(float)        // Größe in Bytes (20164 * 4 Bytes)
            );
            
            return true; 
        }
        
    }
    return false;
}

bool ONNXModel::RunModel(
    TArray<float> &bufferIn,
    TArray<float> &bufferPredictionOut
){
    TArrayView<float> FullViewBufferIn(bufferIn);
    return RunModel(FullViewBufferIn, bufferPredictionOut);
}

bool ONNXModel::RunModel(
    TArray<uint8> &bufferIn, //binary like python / shared mem
    TArray<float> &bufferPredictionOut
){
    if(Uint8ToFloatConvertable(bufferIn)){
        TArrayView<float> bufferInFloatView(
            reinterpret_cast<float*>(bufferIn.GetData()), //ptr start
            bufferIn.Num() / sizeof(float) //ptr end / element end, converted to a float end.
        );
        return RunModel(bufferInFloatView, bufferPredictionOut);
    }
    return false;
}

bool ONNXModel::Uint8ToFloatConvertable(const TArray<uint8> &buffer) const {
    if(buffer.Num() > 0){
        return buffer.Num() % sizeof(float) == 0;
    }
    return false;
}






bool ONNXModel::RunModel(
    const TArrayView<float> &bufferIn,
    TArray<float> &bufferPredictionOut
){
    try
    {
        if(!bModelWasLoaded){
            LoadModel();
        }
        if(CopyDataToTensor(bufferIn)){
            if(Forward(bufferPredictionOut)){
                DebugHelper::logMessage("ONNXModel RunModel Success");
                return true;
            }
        }
    }
    catch (const std::exception& e)
    {
        FString ErrorMsg = FString(UTF8_TO_TCHAR(e.what()));
        DebugHelper::logMessage("ONNXModel RunModel Fail: ", ErrorMsg);
        return false;
    }
    DebugHelper::logMessage("ONNXModel RunModel Fail: Other Issue.");
    return false;
}




bool ONNXModel::RunModelAsync(TArray<uint8> &bufferIn, TFunction<void(TArray<float>)> OnComplete)
{
    if(Uint8ToFloatConvertable(bufferIn)){
        TArrayView<float> bufferInFloatView(
            reinterpret_cast<float*>(bufferIn.GetData()), //ptr start
            bufferIn.Num() / sizeof(float) //ptr end / element end, converted to a float end.
        );
        // 1. Schiebe die schwere Berechnung auf einen Hintergrund-Thread (Worker Thread)
        AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [
            this, bufferInFloatView, OnComplete](){
            TArray<float> PredictionOut;
            // Berechnung im Hintergrund
            if (this->RunModel(bufferInFloatView, PredictionOut)){
                // 2. Zurück auf den GameThread springen, um das Ergebnis sicher zu übergeben
                AsyncTask(ENamedThreads::GameThread, [PredictionOut = MoveTemp(PredictionOut), OnComplete]()
                {
                    OnComplete(PredictionOut);
                });
            }
        });
        return true;
    }
    return false;
}






