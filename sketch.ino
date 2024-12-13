#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

const int pino_botao = 15;  // Pino relacionado ao botão
int estado_botao = 0;  // Variável que armazena o estado atual do botão
int ultimo_estado_botao = 0;  // Variável que armazena o último estado do botão

const int pino_ldr = 4;  // Pino relacionado ao sensor LDR
int limite_luz = 600; // Define o valor limite entre a detecção de claro ou escuro do sensor

bool semaforoAberto = false;

unsigned long ultimo_tempo_debounce;

// Define função que faz o LED piscar quando está escuro
void modoNoturno(){
  Serial.println("Modo Noturno");
  // Desliga todas os outros LEDs
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  // Pisca o LED amarelo
  digitalWrite(led_amarelo, HIGH);
  delay(2000);
  digitalWrite(led_amarelo, LOW);
  delay(2000);
}

void modoDiurno(){
  Serial.println("Modo Diurno");
  // acende o LED verde por 3 segundo
  digitalWrite(led_verde, HIGH);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);
  semaforoAberto = true;
  delay(3000);

  // acende o LED amarelo por 2 segundos
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, HIGH);
  digitalWrite(led_vermelho, LOW);
  delay(2000);

  // acende o LED vermelho por 5 segundos
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, HIGH);

  // Enquanto o LED vermelho está aceso verifica o botão 
  unsigned long tempoQueFechou = millis();
  unsigned long tempoFechado = millis();
  while (tempoFechado - tempoQueFechou < 5000){
    tempoFechado = millis();
    int leituraBotao = digitalRead(pino_botao);

    if (leituraBotao != ultimo_estado_botao) {
        ultimo_tempo_debounce = millis();
    }

    if ((millis() - ultimo_tempo_debounce) > 200) {
        if (leituraBotao == LOW && estado_botao == HIGH) {
            delay(1000);
            tempoFechado = tempoQueFechou + 6000;
        }
        estado_botao = leituraBotao;
    }
  }

}

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde, OUTPUT);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_amarelo, OUTPUT);

  // Inicialização das entradas
  pinMode(pino_botao, INPUT); // Inicializa o pino do botão como input (entrada)

  // Inicizaliza os leds em LOW
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    WiFi.reconnect();
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String caminhoServidor = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(caminhoServidor.c_str());

    int codigoRespostaHttp = http.GET(); // Código do Resultado da Requisição HTTP

    if (codigoRespostaHttp>0) {
      Serial.print("Código de resposta HTTP: ");
      Serial.println(codigoRespostaHttp);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Código de erro: ");
      Serial.println(codigoRespostaHttp);
      }
      http.end();
    }

  else {
    Serial.println("WiFi disconectado");
  }
}

void loop() {

  // leitura do sensor LDR
  int estado_ldr = analogRead(pino_ldr);

  // verfica a leitura do ldr 
  if(estado_ldr >= limite_luz){
    Serial.print("Está escuro, ligue o LED");
    Serial.println(estado_ldr);
    modoNoturno(); // chama a função que faz o LED amarelo piscar
  }else{
    Serial.print("Está claro, desligue as luzes");
    Serial.println(estado_ldr);
    modoDiurno();
  }
}