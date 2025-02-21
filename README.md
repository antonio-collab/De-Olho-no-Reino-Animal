# Documentação do Código: Sistema de Captura de Imagem com Detecção de Movimento

Este código foi desenvolvido para capturar imagens usando uma **ESP32-CAM** sempre que um **sensor PIR (passivo infravermelho)** detectar movimento. A imagem é salva em um **cartão SD** e o número da foto é armazenado na **EEPROM** para garantir que cada foto tenha um número único. A ESP32 entra em modo de sono profundo até que o movimento seja detectado.

---

## **Funcionalidades Principais**

1. **Detecção de Movimento com Sensor PIR:**
   - O código usa um sensor PIR (conectado ao pino GPIO 13) para detectar movimento.
   - Quando o sensor PIR detecta movimento, a câmera captura uma imagem e a salva no cartão SD.
   
2. **Captura de Imagem com a Câmera ESP32:**
   - Utiliza a biblioteca `esp_camera` para configurar e capturar imagens com a câmera ESP32-CAM.
   - A imagem é salva no formato JPEG.

3. **Armazenamento de Fotos no Cartão SD:**
   - O número da foto é gerenciado utilizando a EEPROM para garantir que as imagens sejam salvas com um nome único (ex: `picture1.jpg`, `picture2.jpg` etc.).
   - A foto é salva no cartão SD com o número correto.

4. **Uso de Modo de Sono Profundo:**
   - Após capturar a imagem, o ESP32 entra em modo de sono profundo até que o sensor PIR detecte movimento novamente.

---

## **Componentes Necessários**

- **ESP32-CAM**: Placa de desenvolvimento com câmera embutida.
- **Sensor PIR**: Sensor de movimento para acionar a captura da foto.
- **Cartão SD**: Para armazenar as imagens capturadas.
- **Fonte de Alimentação**: Para alimentar a ESP32-CAM.

---

## **Pinagem**

A ESP32-CAM usa os seguintes pinos para se comunicar com os periféricos:

| Pino           | Função                    |
|----------------|---------------------------|
| GPIO 32        | Pino de alimentação da câmera (PWDN) |
| GPIO 0         | Pino de relógio (XCLK)     |
| GPIO 26        | Pino de dados da câmera (SIOD) |
| GPIO 27        | Pino de dados da câmera (SIOC) |
| GPIO 25        | Pino de sincronização vertical (VSYNC) |
| GPIO 23        | Pino de referência horizontal (HREF) |
| GPIO 22        | Pino de clock (PCLK)       |
| GPIO 21        | Pino de dados da câmera (Y5) |
| GPIO 19        | Pino de dados da câmera (Y4) |
| GPIO 18        | Pino de dados da câmera (Y3) |
| GPIO 17        | Pino de dados da câmera (Y2) |
| GPIO 13        | Pino do Sensor PIR         |

---

## **Descrição do Código**

### **1. Definição de Pinos e Configuração da Câmera**

- O código começa configurando os pinos da **ESP32-CAM** e a câmera.
- A biblioteca `esp_camera` é utilizada para configurar a câmera (resolução, formato de imagem e pinos de conexão).

### **2. Configuração do Sensor PIR**

- O **sensor PIR** é conectado ao pino GPIO 13.
- O código configura esse pino como **entrada** e aguarda a detecção de movimento.

### **3. Inicialização do Cartão SD**

- O código tenta montar o cartão SD usando a biblioteca `SD_MMC`.
- Caso o cartão SD não seja encontrado, o código informa o erro na saída serial.

### **4. Armazenamento do Número da Foto na EEPROM**

- A EEPROM é usada para armazenar o número da última foto tirada. 
- O número é recuperado, incrementado e salvo de volta na EEPROM, garantindo que as fotos tenham nomes únicos.

### **5. Modo de Sono Profundo**

- O ESP32 entra em **modo de sono profundo** enquanto aguarda a detecção de movimento pelo sensor PIR.
- Quando o sensor PIR detecta movimento, o ESP32 sai do modo de sono profundo e captura a imagem.

### **6. Captura da Imagem e Salvamento no Cartão SD**

- Quando o movimento é detectado, o código captura a imagem usando a função `esp_camera_fb_get()`.
- A imagem é salva com um nome único (por exemplo, `picture1.jpg`) no cartão SD.

### **7. Retorno ao Modo de Sono Profundo**

- Após salvar a imagem, o código desliga o flash (pino GPIO 4), retorna a câmera ao modo de espera e coloca o ESP32 no **modo de sono profundo** novamente.

---

## **Fluxo de Execução**

1. O código inicializa a câmera e o cartão SD.
2. A EEPROM é lida para obter o número da última foto salva.
3. O ESP32 entra em **modo de sono profundo** e aguarda a detecção de movimento pelo sensor PIR.
4. Quando o movimento é detectado:
   - O ESP32 sai do sono profundo.
   - A câmera captura uma foto.
   - A foto é salva no cartão SD com o número correto.
   - O número da foto é incrementado e armazenado na EEPROM.
   - O ESP32 retorna ao modo de sono profundo.

---


## **Como Usar**

1. Conecte a **ESP32-CAM** ao seu computador.
2. Carregue o código para a ESP32.
3. Conecte o **sensor PIR** ao pino GPIO 13.
4. Insira um **cartão SD** formatado no slot da ESP32-CAM.
5. O sistema estará pronto para capturar imagens quando houver movimento detectado.

---

## **Exemplo de Saída Serial**

- **Inicialização**:
  ```
  Iniciando o Cartão SD
  ```

- **Esperando por Movimento**:
  ```
  Aguardando movimento...
  ```

- **Captura de Imagem**:
  ```
  Nome do arquivo da foto: /picture1.jpg
  Foto salva no caminho: /picture1.jpg
  ```

- **Entrando no Modo de Sono**:
  ```
  Entrando no modo de sono...
  ```

---

Essa documentação fornece uma visão geral sobre como o código funciona, seus principais componentes, e como utilizá-lo para capturar imagens automaticamente sempre que um movimento for detectado.
