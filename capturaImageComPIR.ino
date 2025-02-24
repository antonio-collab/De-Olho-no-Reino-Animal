#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"              
#include "SD_MMC.h"            
#include "soc/soc.h"          
#include "soc/rtc_cntl_reg.h"  
#include "driver/rtc_io.h"
#include <EEPROM.h>            

// Define a quantidade de bytes que deseja acessar (2 bytes para número da foto)
#define EEPROM_SIZE 2

// Pinos para a câmera CAMERA_MODEL_AI_THINKER
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

#define PIR_PIN 13         // Pino conectado ao sensor PIR (pino OUT do PIR)

unsigned int pictureNumber = 0;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Desativa detector de queda de energia
  Serial.begin(115200);

  Serial.setDebugOutput(true);

  // Configuração da câmera
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

  pinMode(4, INPUT);
  digitalWrite(4, LOW);
  rtc_gpio_hold_dis(GPIO_NUM_4);

  // Configure as configurações da câmera dependendo da disponibilidade da PSRAM
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // Tamanho de quadro de alta resolução
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA; // Resolução mais baixa para memória limitada
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Inicia a câmera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Inicia o SD
  Serial.println("Starting SD Card");
  delay(500);
  if (!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD Card attached");
    return;
  }

  // Lê número da foto atual da EEPROM (2 bytes)
  EEPROM.begin(EEPROM_SIZE);
  byte highByte = EEPROM.read(0);
  byte lowByte = EEPROM.read(1);
  pictureNumber = (highByte << 8) | lowByte;  // Combina 2 bytes

  // Aguarda a detecção de movimento pelo PIR
  Serial.println("Aguardando movimento...");
  while (true) {
    int pirState = digitalRead(PIR_PIN);  // Lê o estado do sensor PIR
    
    if (pirState == HIGH) {  // Se movimento detectado
      Serial.println("Movimento detectado!");
      
      // Incrementa o número da foto
      pictureNumber++;

      // Caminho onde a nova imagem será salva no cartão SD
      String path = "/picture" + String(pictureNumber) + ".jpg";
      fs::FS &fs = SD_MMC;
      Serial.printf("Nome do arquivo da foto: %s\n", path.c_str());

      // Tira foto com a câmera
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Falha ao capturar a foto");
        return;
      }

      // Abre o arquivo e salve a imagem
      File file = fs.open(path.c_str(), FILE_WRITE);
      if (!file) {
        Serial.println("Falha ao abrir o arquivo para escrita");
      } else {
        file.write(fb->buf, fb->len);  // Escreve os dados da imagem
        Serial.printf("Foto salva no caminho: %s\n", path.c_str());

        // Salva o número da imagem atualizado na EEPROM (2 bytes)
        EEPROM.write(0, (pictureNumber >> 8) & 0xFF);  // Salva byte alto
        EEPROM.write(1, pictureNumber & 0xFF);         // Salva byte baixo
        EEPROM.commit();
      }
      file.close();  // Fecha o arquivo
      esp_camera_fb_return(fb);  // Libera o buffer de quadros

      break;  // Sai do loop após capturar a foto
    }
  }

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);

  // Define o ESP32 para o modo de suspensão profunda após capturar a foto
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);
  Serial.println("Indo para o modo de sono agora");
  delay(1000);
  esp_deep_sleep_start();  // Inicia o modo sleep
  Serial.println("This will never be printed");
}

void loop() {
  
}
