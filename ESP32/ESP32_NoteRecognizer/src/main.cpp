#include <driver/i2s.h>
#include <arduinoFFT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI   19
#define OLED_CLK    18
#define OLED_DC     17
#define OLED_CS     5
#define OLED_RESET  16

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

void setup() {
  Serial.begin(115200);
  setupI2S();

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 failed");
    while (true);
  }
  display.clearDisplay();
  display.display();
}

String frequencyToNote(float freq) {
  const char* noteNames[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
  };

  if (freq < 20 || freq > 5000) return "-";

  int noteNumber = round(12 * log2(freq / 440.0) + 69);
  int noteIndex = noteNumber % 12;
  int octave = noteNumber / 12 - 1;

  return String(noteNames[noteIndex]) + String(octave);
}

void loop() {
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

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Note:");
  display.setCursor(0, 30);
  display.setTextSize(3);
  display.print(note);
  display.display();

  // Serial.printf("Freq: %.2f Hz, Note: %s\n", frequency, note.c_str());

  // delay(20);
}


