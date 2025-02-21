#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <EEPROM.h>          

#define EEPROM_SIZE 2 

RTC_DATA_ATTR int bootCount = 0;

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define PIR_SENSOR_PIN    13  // Pino onde o sensor PIR está conectado

int pictureNumber = 0;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Desabilita o detetor de queda de tensão
  Serial.begin(115200);

  Serial.setDebugOutput(true);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(4, INPUT);
  digitalWrite(4, LOW);
  rtc_gpio_hold_dis(GPIO_NUM_4);

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Inicializa a câmera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Erro ao iniciar câmera 0x%x", err);
    return;
  }

  Serial.println("Iniciando o Cartão SD");

  delay(500);
  if (!SD_MMC.begin()) {
    Serial.println("Falha ao montar o cartão SD");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Nenhum cartão SD encontrado");
    return;
  }

  // Inicializa EEPROM com tamanho definido
  EEPROM.begin(EEPROM_SIZE);

  // Lê o número da foto armazenado na EEPROM
  pictureNumber = (EEPROM.read(0) << 8) | EEPROM.read(1);  // Combina os dois bytes

  // Incrementa o número da foto
  pictureNumber++;

  // Armazena o número da foto de volta na EEPROM (dois bytes)
  EEPROM.write(0, (pictureNumber >> 8) & 0xFF);  // Parte alta do número
  EEPROM.write(1, pictureNumber & 0xFF);         // Parte baixa do número
  EEPROM.commit();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);  // Habilita o modo de sono com o PIR como gatilho

  // Espera por movimento
  Serial.println("Aguardando movimento...");
  while (digitalRead(PIR_SENSOR_PIN) == LOW) {
    // Enquanto o PIR não detectar movimento, o ESP32 vai entrar em modo de sono
    esp_deep_sleep_start();  // Coloca o ESP32 no modo de sono profundo
  }

  // Captura a foto após movimento ser detectado
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Falha ao capturar a imagem");
    return;
  }

  // Caminho onde a foto será salva no cartão SD
  String path = "/picture" + String(pictureNumber) + ".jpg";

  fs::FS &fs = SD_MMC;
  Serial.printf("Nome do arquivo da foto: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Falha ao abrir arquivo para escrita");
  } else {
    file.write(fb->buf, fb->len);  // Escreve o conteúdo da imagem no arquivo
    Serial.printf("Foto salva no caminho: %s\n", path.c_str());
  }
  file.close();
  esp_camera_fb_return(fb);

  // Desliga o flash da ESP32-CAM
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);

  Serial.println("Entrando no modo de sono...");
  esp_deep_sleep_start();  // Entra em sono profundo após salvar a foto
}

void loop() {
 
}
