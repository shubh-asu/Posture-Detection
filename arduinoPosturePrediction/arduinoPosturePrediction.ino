// IMU library
#include <Arduino_LSM9DS1.h>

// Tensorflow lite libraries
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

// Change path where model header file is stored
#include <C:/Users/shubh/EmbeddedML/Miniprojects/mp4/model.h>

tflite::MicroErrorReporter tflErrorReporter;
tflite::AllOpsResolver tflOpsResolver;

// Assign model
const tflite::Model* tflModel = ::tflite::GetModel(model); 

tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM
constexpr int tensorArenaSize = 50 * 1024; // 50kb buffer size
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

float imuData[25][3]; // Array for storing IMU data
int j;
int s = 0;
float h; 
float u = 0;
int ans;

void setup() {
  Serial.begin(115200);
  if (!IMU.begin()) {
    Serial.println("Fail to initialize IMU");
    while (1);
  }

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);

  // Allocate memory for the model's tensors
  tflInterpreter->AllocateTensors();
  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}


void loop() {
  while (true) {
    j = Serial.readString().toInt();
    if (j != 1 && j != 2 && j != 3) {
      if (j != 0){Serial.println("Error, give correct input");}
      continue; // This will skip the loop if no input is given because j=0 for no input
    }

    // Calls function which reads sensor data and stores into 2D array
    sensorDataReader(j);
    // printImuData();

    //Assigns input tensor from the 2D array
    for (int a=0; a<25; a++){
      for (int b=0; b<3; b++){
        tflInputTensor->data.f[a * 3 + b] = imuData[a][b];
      }
    }
    // Run inferencing
    TfLiteStatus invokeStatus = tflInterpreter->Invoke();
    if (invokeStatus != kTfLiteOk) {
      Serial.println("Invoke failed!");
      while (1);
      return;
    }

    // Loop through the output tensor values from the model
    // for (int i = 0; i < 5; i++) {
    //   Serial.print(tflOutputTensor->data.f[i]);
    //   if (i != 5){
    //     Serial.print(",");
    //   }
    // }
    // Serial.println();
    

    //Using 2 pointer method, gets index of the maximum valued output
    for (int i = 0; i < 5; i++) {
      // i is index while iterating the output
      // h is value at index i
      h = tflOutputTensor->data.f[i];
      // s is the index of maximum value until index i
      // u is value at index s
      u = tflOutputTensor->data.f[s];
      // If current value is higher than prexious max value then it updates max value index 
      if (h>=u){
        s = i; 
      }
    }

    ans = s;
    s = 0;
    switch (ans){
      case 0:
        Serial.println("Supine");
        break;
      case 1:
        Serial.println("Prone");
        break;
      case 2:
        Serial.println("Side");
        break;
      case 3:
        Serial.println("Sitting");
        break;
      case 4:
        Serial.println("Unknown");
        break;
    }
  }
}

void sensorDataReader(int sensorNumber) {
  for (int i = 0; i<25; i++) {
    if (j == 1){
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(imuData[i][0], imuData[i][1], imuData[i][2]);  
      }
    } else if (j == 2){
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(imuData[i][0], imuData[i][1], imuData[i][2]);       
      }
    } else if (j == 3){
      if (IMU.magneticFieldAvailable()) {
        IMU.readMagneticField(imuData[i][0], imuData[i][1], imuData[i][2]);   
      }
    }
    delay(80);
  }
}

// void printImuData(){
//   for (int a = 0; a<25; a++){
//       Serial.print(imuData[a][0]);
//       Serial.print(",");
//       Serial.print(imuData[a][1]);
//       Serial.print(",");
//       Serial.println(imuData[a][2]);
//     }
// }

