#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;

// Definindo os pinos de todos os sensores e componentes
const int DHTPIN = 12; // Pino DHT22
int potPin = 34; // Pino Potenciometro
const int botaoPin = 2; // Pino do botão de emergência

// Defina as informações do seu broker MQTT
const char *SSID = "Wokwi-GUEST"; // SSID / nome da rede WI-FI do Wokwi (Padrão)
const char *PASSWORD = "";        // Senha da rede WI-FI do Wokwi (Padrão)

// URL do broker MQTT - HiveMQ
const char *BROKER_MQTT = "broker.hivemq.com";
int BROKER_PORT = 1883;
// Definindo Tópicos
const char *ID_MQTT = "esp32_mqtt_GM";
const char *dados_topico = "gm/dados/enviar"; // Tópico para Envio dos dados
const char *emergencia_topico = "gm/dados/emergencia"; // Tópico para o botão de emergência

unsigned long publistUpdate;

// Variáveis e objetos globais
WiFiClient espClient;         // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

// Definindo a tela e a proporção
LiquidCrystal_I2C tela1(0x27, 20, 4);

DHTesp Sensor;

// Valor Inicial do Botão
bool emergenciaAcionada = false;
float t = 0;
float u = 0;

void callback(char *topic, byte *payload, unsigned int length);

PubSubClient clientMQTT(BROKER_MQTT, BROKER_PORT, callback, client);

// Conectando ao Wifi
void connectWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Conectando ao MQTT
void connectMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);

  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");

      // Inscreva-se no tópico de emergência
      MQTT.subscribe(emergencia_topico);
    } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Sensor.setup(DHTPIN, DHTesp::DHT22);
  Serial.println("Inicializando Tela 1...");
  tela1.init();
  tela1.backlight();
  Serial.println("Tela 1 inicializada!");

  // Configurando pinos
  pinMode(botaoPin, INPUT_PULLUP);

  // Configurações do display
  tela1.init();
  tela1.backlight();

  // Conectar ao Wi-Fi
  connectWiFi();

  // Conectar ao servidor MQTT
  connectMQTT();
}

void loop() {
  clientMQTT.loop();

  TempAndHumidity entrada = Sensor.getTempAndHumidity();
  t = entrada.temperature;
  u = entrada.humidity;

  int valorPot = analogRead(potPin);
  int bpm = map(valorPot, 95, 4095, 40, 120);

  // Verificar o estado do botão de emergência
  bool emergenciaPressionada = (digitalRead(botaoPin) == LOW);

  if (emergenciaPressionada && !emergenciaAcionada) {
    emergenciaAcionada = true;
    if (MQTT.connected()) {
      MQTT.publish(emergencia_topico, "EMERGENCIA ACIONADA");
      Serial.println("Mensagem de emergência enviada");
    }
  } else if (!emergenciaPressionada && emergenciaAcionada) {
    emergenciaAcionada = false;
    if (MQTT.connected()) {
      MQTT.publish(emergencia_topico, "SEM EMERGENCIA");
      Serial.println("Sem emergência");
    }
  }

  // Exibir estado do botão na linha (1,3)
  tela1.setCursor(1, 3);
  tela1.print("Botao: " + String(emergenciaAcionada ? "Acionado    " : "Nao Acionado"));

  // Imprimindo no display 1
  tela1.setCursor(1, 0);
  tela1.print("Temp: " + String(entrada.temperature) + "C");
  delay(100);

  tela1.setCursor(1, 1);
  tela1.println("Umidade: " + String(entrada.humidity) + "%");
  delay(100);

  tela1.setCursor(1, 2);
  tela1.print("BPM: " + String(bpm) + " ");
  delay(100);

  // Publicando no Tópico do MQTT Broker
  if (MQTT.connected()) {
    String data = "{\"temperature\": " + String(entrada.temperature) +
                  ", \"humidity\": " + String(entrada.humidity) +
                  ", \"bpm\": " + String(bpm) + "}";
    MQTT.publish(dados_topico, data.c_str());
    Serial.println("Data published to MQTT topic: " + String(dados_topico));
  }

  delay(800);
}

void callback(char *topic, byte *payload, unsigned int length) {
  String msg = String((char*)payload).substring(0, length);
  
  Serial.printf("Mensagem recebida via MQTT: %s do tópico: %s\n", msg.c_str(), topic);

  if (strcmp(topic, "gm/dados/emergencia") == 0) {
    // Verificar o tópico e executar a ação de emergência
    String comando = "";
    for (int i = 0; i < length; i++) {
      comando += (char)payload[i];
    }

    // Executar a ação de emergência
    Serial.println("Emergência acionada: " + comando);
    // Adicione a lógica de emergência aqui conforme necessário
  }
}
