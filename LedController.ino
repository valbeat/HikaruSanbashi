#include <Adafruit_NeoPixel.h>

#define THRESHOLD 300
#define LED_NUM 15
#define LED_PIN 6
#define LED_BRIGHTNESS 32

const int analogInPin = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);
int sensorValue = 0;
int sensorValueArray[100] = {};
boolean flag = false;

int yellow[] = {127, 100, 16};
int white[] = {255, 255, 255};

int arraySize = sizeof(sensorValueArray) / sizeof(sensorValueArray[0]);
  
void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
  strip.setBrightness(LED_BRIGHTNESS);
}

int count = 0;
void loop() {
  sensorValue = (int)analogRead(analogInPin);
//  Serial.println(sensorValue);
  if (count < arraySize -1) {
    sensorValueArray[count] = sensorValue;
    count++;
    return;
  }
  count = 0;
  float value = median(sensorValueArray, arraySize);
  Serial.println(value);
  if (THRESHOLD < value && flag == false){
    flag = true;
    fade(yellow,white,100);
    return;
  }
  if (THRESHOLD >= value && flag == true){
    flag = false;
    fade(white,yellow,100);
    return;
  }
  if (flag == true) {
    setColor(strip.Color(white[0], white[1], white[2]), 0);
  } else {
    setColor(strip.Color(yellow[0], yellow[1], yellow[2]), 0);
  }
}

// LEDの色をセットする
void setColor(uint32_t c, uint8_t wait) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

// LEDの色をフェードさせる
void fade(int *in_c, int *out_c, int wait ) {
    int diff[3];
    float color[3];
    for (int i=0; i < 3; i++) {
      color[i] = in_c[i];
      diff[i] = out_c[i] - in_c[i];

    }
    int count = 0;
    while(count < 10) {
      for (int i=0; i < 3; i++) {
        color[i] += diff[i] * 0.1;
      }
      for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(color[0], color[1], color[2]));
      }
      strip.show();
      delay(100);
      count++; 
    }
    delay(wait);
}

// 配列の平均値を返す
float mean(int *array, int arraySize) {
    int sum = 0;
    for (int i = 0; i < arraySize; i++){
      sum += array[i];
    }
    return sum / arraySize;
}

// 配列の中央値を返す
float median(int *array, int arraySize) {
    int _array[arraySize];
    memcpy(_array, array, arraySize);
    int tmp;
    for (int i = 0; i < arraySize - 1; i++){
      for (int j = i + 1; j < arraySize; j++) {
        if (_array[i] > _array[j]){ 
          tmp = _array[i];
          _array[i] = _array[j];
          _array[j] = tmp;
        }
      }
    }
    if (arraySize % 2 == 1) {
      return _array[(arraySize - 1) / 2];
    } else {
      return (_array[(arraySize / 2) - 1] + _array[arraySize / 2]) / 2;
    }
}
