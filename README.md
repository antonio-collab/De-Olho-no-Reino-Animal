Aqui está a documentação do código com explicações e emojis:

---

### 📝 **Documentação do Código: Sistema de Câmera com Detecção de Movimento com Sensor PIR**

#### 📦 **Objetivo**
Este código configura uma câmera ESP32 para capturar uma foto automaticamente quando um sensor PIR (infravermelho passivo) detecta movimento. A imagem é salva no cartão SD com um nome sequencial, e o número da foto é armazenado na EEPROM para garantir que o nome da próxima imagem seja incrementado corretamente.

---

#### 🔧 **Configurações do Hardware**

- **Câmera ESP32**: A câmera é configurada com os pinos da `AI Thinker Camera` para capturar imagens.
- **Sensor PIR**: O sensor PIR detecta movimento e aciona a captura da foto. O pino de saída do PIR é conectado ao pino `GPIO13` do ESP32.
- **Cartão SD**: As fotos são salvas em um cartão SD inserido no ESP32, que é montado na inicialização do código.
- **EEPROM**: Usada para armazenar o número da foto, garantindo que cada imagem tenha um nome único e sequencial.

---

#### 📋 **Passos do Código**

1. **Inicialização da Câmera 📷**:
   - O código configura a câmera com as definições de pinos e qualidade de imagem.
   - A câmera usa a interface SPI (DIO) para capturar imagens JPEG.

2. **Configuração do Sensor PIR 🚶‍♂️**:
   - O sensor PIR está conectado ao pino `GPIO13`. Quando movimento é detectado, o estado do pino é `HIGH` e o código captura uma foto.
   - O sensor PIR é lido constantemente em um loop até que o movimento seja detectado.

3. **Cartão SD 💾**:
   - O cartão SD é montado usando a biblioteca `SD_MMC` e se a montagem for bem-sucedida, ele é preparado para armazenar as imagens.
   - A cada foto, o número da imagem é salvo no cartão SD com o formato `pictureX.jpg`, onde `X` é o número da foto.

4. **Uso da EEPROM 🔑**:
   - A EEPROM é usada para salvar o número da última foto tirada. O número da foto é armazenado em dois bytes e lido ao iniciar o código.
   - Após cada foto capturada, o número é incrementado e salvo novamente na EEPROM.

5. **Captura da Foto 📸**:
   - Quando o movimento é detectado, o código captura a foto e a salva no cartão SD.
   - O arquivo é aberto e os dados da imagem são gravados. O número da foto é atualizado na EEPROM para garantir que a próxima foto tenha um número sequencial.

6. **Modo de Sono 😴**:
   - Após a captura da foto, o ESP32 é configurado para entrar em modo de sono profundo, economizando energia.
   - O modo de sono profundo é ativado e o código vai dormir até o próximo movimento detectado pelo sensor PIR.

---

#### 🔄 **Fluxo do Programa**

1. **Início**: O código começa inicializando a câmera e o cartão SD.
2. **Sensor PIR**: O código aguarda o sensor PIR detectar movimento.
3. **Captura da Foto**: Quando o movimento é detectado:
   - A foto é capturada pela câmera.
   - O número da foto é incrementado e salvo na EEPROM.
   - A imagem é salva no cartão SD.
4. **Modo de Sono**: Após capturar a foto, o ESP32 entra em modo de sono profundo para economizar energia até o próximo movimento detectado.
5. **Repetição**: O ciclo se repete sempre que o sensor PIR detecta movimento.

---

#### 🛠 **Pinos e Configuração**

- **Pinos da Câmera**:
  - Pinos `Y2_GPIO_NUM`, `Y3_GPIO_NUM`, `Y4_GPIO_NUM`, etc., são configurados conforme o modelo da câmera.
- **Pino PIR**: 
  - O sensor PIR é conectado ao `GPIO13` (definido como `PIR_PIN`).
- **EEPROM**:
  - O número da foto é armazenado em dois bytes na EEPROM (endereço 0 e 1).
- **Cartão SD**:
  - O cartão SD é montado e utilizado para salvar as imagens.

---

#### ⚠️ **Possíveis Erros**

- **Falha na Câmera**: Se a câmera não for inicializada corretamente, o código exibirá uma mensagem de erro no Serial Monitor.
- **Falha na Montagem do Cartão SD**: Se o cartão SD não for montado corretamente, uma mensagem de erro será exibida.
- **Falha na Captura da Imagem**: Se não for possível capturar a foto, o código exibirá uma mensagem de erro no Serial Monitor.

---

#### 📝 **Considerações Finais**

- O código foi projetado para ser eficiente em termos de uso de memória e processamento, aproveitando a PSRAM (caso disponível) e utilizando a EEPROM para armazenamento persistente.
- Após detectar movimento, o ESP32-CAM captura a foto, salva no cartão SD, e entra em modo de sono profundo até o próximo evento de movimento.
- A implementação do sensor PIR ajuda a criar um sistema de segurança básico ou um dispositivo de captura automática de imagens.

---

### 📌 **Resumo do Fluxo de Ação**:

1. **Configurações iniciais**: Câmera e cartão SD.
2. **Aguardar movimento**: O sistema aguarda até que o PIR detecte movimento.
3. **Captura e salvamento da foto**: Após o movimento, o código tira a foto e a salva com um nome único.
4. **Economia de energia**: Após a captura, o ESP32-CAM entra em modo de sono até o próximo movimento.

---

🔧 **Feito por**: [Antonio Carlos]  
🌐 **Contato**: [antonioac3522@gmail.com]

---

Espero que essa documentação seja útil! 😊
