#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char* ssid = "user";
const char* password = "88888888";

ESP8266WebServer server(80);

String ledStatus = "Tidak diketahui";
String page;

void setup(void){
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Mohon ditunggu...");
  }
  Serial.println("");
  Serial.print("Sukses terkoneksi ke ");
  Serial.println(ssid);
  Serial.print("IP address anda: ");
  Serial.println(WiFi.localIP());

  if (SPIFFS.begin()) {
    Serial.println("Proses load file");
    File file = SPIFFS.open("/web.html", "r");
    if (file) {
      page = file.readString();
      file.close();
    } else {
      Serial.println("Gagal membuka halaman web");
    }
  } else {
    Serial.println("Gagal load file");
  }

  server.on("/", HTTP_GET, [](){
    server.send(200, "text/html", page);
  });

  server.on("/LEDOn", HTTP_GET, [](){
    server.send(200, "text/plain", "LED On");
    digitalWrite(D0, HIGH);
    ledStatus = "On";
    server.send(200, "text/html", page);
  });

  server.on("/LEDOff", HTTP_GET, [](){
    server.send(200, "text/plain", "LED Off");
    digitalWrite(D0, LOW);
    ledStatus = "Off";
    server.send(200, "text/html", page);
  });

  server.begin();
  Serial.println("Web server nyala!");
}

void loop(void){
  server.handleClient();
}
