#include <driver/i2s.h>
#include <arduinoFFT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <vector>
#include <algorithm>

String getRandomNote();
int getRandomString();

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI   19
#define OLED_CLK    18
#define OLED_DC     17
#define OLED_CS     5
#define OLED_RESET  16

#define ADC_X 34
#define ADC_Y 35
#define JOYSTICK_SW 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC,
                         OLED_RESET, OLED_CS);

#define I2S_NUM       I2S_NUM_0
#define I2S_WS        25
#define I2S_SCK       26
#define I2S_SD        23

const int SAMPLES = 2048;
const double SAMPLING_FREQUENCY = 11025;
float vReal[SAMPLES];
float vImag[SAMPLES];
int32_t samples[SAMPLES];
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY); 

enum class AppState {
    NoteChooser,
    InGame,
    None,
};
AppState currentState = AppState::NoteChooser;

void setupI2S() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLING_FREQUENCY,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM);
}

esp_adc_cal_characteristics_t *adc_chars;

void setupADC() {
  // Configure ADC1 channel 6 (GPIO34)
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  
  // Configure ADC1 channel 7 (GPIO35)
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);

  // Setup ADC calibration
  adc_chars = (esp_adc_cal_characteristics_t*) calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
      ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);
  
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.println("Using eFuse Vref");
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    Serial.println("Using Two Point Calibration");
  } else {
    Serial.println("Using Default Vref");
  }
}

int readADC(int channel) {
  int raw = adc1_get_raw((adc1_channel_t)channel);
  // Convert raw reading to voltage in mV
  int voltage = esp_adc_cal_raw_to_voltage(raw, adc_chars);
  return voltage;
}

void setup() {
  Serial.begin(115200);
  setupI2S();

  setupADC();

  pinMode(JOYSTICK_SW, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 failed");
    while (true);
  }
  display.clearDisplay();
  display.display();
}

const char* noteNames[] = {
  "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

std::vector<int> choosenNoteIndexes;

String frequencyToNote(float freq) {

  if (freq < 20 || freq > 5000) return "-";

  int noteNumber = round(12 * log2(freq / 440.0) + 69);
  int noteIndex = noteNumber % 12;
  int octave = noteNumber / 12 - 1;

  return String(noteNames[noteIndex]) + String(octave);
}

int points = 0;
String randomNote;
int randomString = 1;
int noteRecognizedInARow = 0;
String prevRecognizedNote;
#define NOTES_IN_A_ROW_RECOGNITION_THRESHOLD 5

void handleInGameState();
void handleNoteChooserState();

void loop() {

  switch(currentState)
  {
    case AppState::InGame:
      handleInGameState();
      break;
    case AppState::NoteChooser:
      handleNoteChooserState();
      break;
    default:
      Serial.print("Unrecognized state");
  }
}

void handleInGameState() {
  size_t bytes_read;

  i2s_read(I2S_NUM, &samples, sizeof(samples), &bytes_read, portMAX_DELAY);
  int sampleCount = bytes_read / sizeof(int32_t);

  for (int i = 0; i < sampleCount; i++) {
    int32_t raw = samples[i];
    raw >>= 8;
    vReal[i] = (float)raw;
    vImag[i] = 0.0f;
  }

  float mean = 0;
  for (int i = 0; i < sampleCount; i++) {
    mean += vReal[i];
  }
  mean /= sampleCount;

  for (int i = 0; i < sampleCount; i++) {
    vReal[i] -= mean;
  }

  FFT.windowing(FFT_WIN_TYP_BLACKMAN, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  double maxVal = 0;
  int maxIndex = 0;
  for (int i = 1; i < SAMPLES / 2; i++) {
    if (vReal[i] > maxVal) {
      maxVal = vReal[i];
      maxIndex = i;
    }
  }
  double frequency = (maxIndex * SAMPLING_FREQUENCY) / SAMPLES;
  String note = frequencyToNote(frequency);
  String noteWithoutOctave = note.substring(0, note.length() - 1);

  Serial.print("Note: ");
  Serial.println(note);


  if (prevRecognizedNote == noteWithoutOctave) {
    ++noteRecognizedInARow;
  } else {
    noteRecognizedInARow = 0;
    prevRecognizedNote = noteWithoutOctave;
  }

  if (noteWithoutOctave == randomNote &&
      noteRecognizedInARow > NOTES_IN_A_ROW_RECOGNITION_THRESHOLD) {
    randomNote = getRandomNote();
    points++;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);

  display.setCursor(0, 0);
  display.print("Note:");
  display.print(randomNote);

  display.setCursor(0,20);
  display.print("String: ");
  display.print(randomString);

  display.setCursor(0, 50);
  display.print("Points:");
  display.print(points);
  display.display();
}

int noteCursorPos = 0;
bool didSwipeRight = false; 
bool didSwipeLeft = false; 
bool didSwipeUp = false; 
bool didSwipeDown = false; 
bool didClick = false; 

bool didSwipeRightLock = true; 
bool didSwipeLeftLock = true; 
bool didSwipeUpLock = true; 
bool didSwipeDownLock = true; 
bool didClickLock = true; 

void handleNoteChooserState() {

  didSwipeRight = false; 
  didSwipeLeft = false; 
  didSwipeUp = false; 
  didSwipeDown = false; 
  didClick = false;

  uint32_t x_channel = readADC(ADC1_CHANNEL_7);
  uint32_t y_channel = readADC(ADC1_CHANNEL_6);
  Serial.printf("ch7: %d, ch6: %d\n\r", x_channel, y_channel);

  //Right swiping
  if(didSwipeRightLock && x_channel < 300) {
    didSwipeRight = true;
    didSwipeRightLock = false;
  }
  if(x_channel > 300) {
    didSwipeRightLock = true;
  }

  //Left swiping
  if(didSwipeLeftLock && x_channel > 3000) {
    didSwipeLeft = true;
    didSwipeLeftLock = false;
  }
  if(x_channel < 3000) {
    didSwipeLeftLock = true;
  }

  //Down swiping
  if(didSwipeDownLock && y_channel > 3000) {
    didSwipeDown = true;
    didSwipeDownLock = false;
  }
  if(y_channel < 3000) {
    didSwipeDownLock = true;
  }

  //Up swiping
  if(didSwipeUpLock && y_channel < 300) {
    didSwipeUp = true;
    didSwipeUpLock = false;
  }
  if(y_channel > 300) {
    didSwipeUpLock = true;
  }
   
  //Clicking 
  if(didClickLock && !digitalRead(JOYSTICK_SW)) {
    didClick = true;
    didClickLock = false;
  }
  if(digitalRead(JOYSTICK_SW)) {
    didClickLock = true;
  }

  if(didClick) {
    auto it = std::find(choosenNoteIndexes.begin(), choosenNoteIndexes.end(), noteCursorPos); 
    if(it != choosenNoteIndexes.end()) {
      choosenNoteIndexes.erase(it);
    } else {
      choosenNoteIndexes.push_back(noteCursorPos);
    }
  }

  if(didSwipeRight) {
    noteCursorPos = (++noteCursorPos)%12;
  }
  if(didSwipeLeft) {
    noteCursorPos = (--noteCursorPos)%12;
  }

  if(didSwipeUp) {
    currentState = AppState::InGame;
    randomNote = getRandomNote();
    randomString = getRandomString();
  }


  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 3);
  display.setTextSize(1);
  display.print("All notes: ");
  display.setCursor(0, 16);
  for(int i = 0 ; i < 11 ; ++i) {
    display.print(noteNames[i]);
    if(noteCursorPos == i) {
      display.print("*");
    }
    display.print(" ");
  }

  display.setCursor(0, 35);
  display.print("Choosen notes: ");
  for(int i = 0 ; i < 11 ; ++i) {
    if(std::find(choosenNoteIndexes.begin(), choosenNoteIndexes.end(), i) != choosenNoteIndexes.end()) {
      display.print(noteNames[i]);
      display.print(" ");
    }
  }

  display.display();
}

String getRandomNote() {
  int64_t time_us = esp_timer_get_time();
  int randomIndex = choosenNoteIndexes[time_us%choosenNoteIndexes.size()];
  return String(noteNames[randomIndex]);
}

int getRandomString() {
  int64_t time_us = esp_timer_get_time();
  int randomString = time_us%7;
  if(randomString == 0) {
    randomString = 1;
  }
  return randomString;
}

