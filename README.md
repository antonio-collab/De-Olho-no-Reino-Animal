# Projeto ESP32-CAM: Captura e Armazenamento de Imagens no Cartão SD

Este projeto foi desenvolvido para capturar uma imagem com o módulo ESP32-CAM e armazená-la em um cartão SD, utilizando a biblioteca `esp_camera.h`. Além disso, o código implementa o uso de memória EEPROM para salvar o número sequencial das imagens capturadas.

## Funcionalidades
- Captura de imagens em formato JPEG usando o ESP32-CAM.
- Armazenamento das imagens capturadas no cartão SD.
- Numeração sequencial das imagens com persistência em EEPROM.
- Modo Deep Sleep para economia de energia.
- Desativação do detector de brownout para maior estabilidade.

## Requisitos
- Módulo ESP32-CAM AI Thinker.
- Cartão SD (formatado em FAT32).
- Ambiente de desenvolvimento Arduino IDE configurado para ESP32.
- Bibliotecas necessárias:  
  - `esp_camera.h`
  - `FS.h`
  - `SD_MMC.h`
  - `EEPROM.h`

## Pinagem
A configuração de pinos utilizada é compatível com o modelo **AI Thinker**:
- `PWDN_GPIO_NUM`: 32  
- `RESET_GPIO_NUM`: -1  
- `XCLK_GPIO_NUM`: 0  
- `SIOD_GPIO_NUM`: 26  
- `SIOC_GPIO_NUM`: 27  
- `Y9_GPIO_NUM`: 35  
- `Y8_GPIO_NUM`: 34  
- `Y7_GPIO_NUM`: 39  
- `Y6_GPIO_NUM`: 36  
- `Y5_GPIO_NUM`: 21  
- `Y4_GPIO_NUM`: 19  
- `Y3_GPIO_NUM`: 18  
- `Y2_GPIO_NUM`: 5  
- `VSYNC_GPIO_NUM`: 25  
- `HREF_GPIO_NUM`: 23  
- `PCLK_GPIO_NUM`: 22  

## Como Usar
1. Conecte o módulo ESP32-CAM ao computador.
2. Faça o upload do código para o ESP32-CAM utilizando o Arduino IDE.
3. Insira o cartão SD no módulo ESP32-CAM.
4. A imagem será capturada automaticamente, salva no cartão SD com o nome `pictureX.jpg`, onde `X` é o número da imagem sequencial.
5. O ESP32-CAM entra em modo Deep Sleep após a captura.


## Notas
- Caso o cartão SD não seja detectado, a mensagem `SD Card Mount Failed` será exibida.
- Para acessar as imagens, retire o cartão SD e insira-o em um leitor de cartão no computador.

## Licença
Este projeto é de código aberto e pode ser modificado livremente para atender às suas necessidades.
