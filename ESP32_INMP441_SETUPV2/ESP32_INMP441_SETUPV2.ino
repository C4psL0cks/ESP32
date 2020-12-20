#include <driver/i2s.h>

#define PIN_WS   15
#define PIN_SD   13
#define PIN_CLK  2

#define BUFFER_SIZE 256

int16_t soundBuffer[BUFFER_SIZE];

void i2sInit() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 128,
  };

  i2s_pin_config_t pin_config;
  pin_config.bck_io_num   = PIN_CLK;
  pin_config.ws_io_num    = PIN_WS;
  pin_config.data_out_num = I2S_PIN_NO_CHANGE;
  pin_config.data_in_num  = PIN_SD;


  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void setup() {
  Serial.begin(115200);

  i2sInit();
}

void loop() {
  size_t bytesread;
  i2s_read(I2S_NUM_0, soundBuffer, sizeof(soundBuffer), &bytesread, (100 / portTICK_RATE_MS));
  for (uint32_t inx = 0; inx < (bytesread / 2); inx++) {
    int16_t data = soundBuffer[inx];
    Serial.print(1000);
    Serial.print('\t');
    Serial.print(data);
    Serial.print('\t');
    Serial.print(-1000);
    Serial.println();
  }

  // delay(1000);
}
