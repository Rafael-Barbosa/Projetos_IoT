#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>



const char* ssid = "HOME";
const char* password = "senhasenha";




#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiServer server(80);

void setup() {

  IPAddress local_IP(192,168,1,133); //Troque o IP
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);

  Serial.begin(9600);

  dht.begin();

  
  Serial.println();
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
}

  WiFi.begin(ssid, password);
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

Serial.println("");
  Serial.println("Conexão WiFi estabelecida");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Umidade: "); Serial.println(h);
    Serial.print("Temperatura: "); Serial.println(t);

    
    WiFiClient client;

    String url = "/get-sensor-data?temperature=" + String(t) + "&humidity=" + String(h);
    
    if (client.connect("192.168.1.191", 5000)) { 
        Serial.println("Conectado ao servidor");
        
       
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: 192.168.1.191\r\n" + 
                     "Connection: close\r\n\r\n");
        while(client.connected()) {
            if(client.available()) {
                String line = client.readStringUntil('\r');
                Serial.print(line);
            }
        }
        client.stop();
    } else {
        Serial.println("Connection to server failed");
    }

    delay(1000); 
}