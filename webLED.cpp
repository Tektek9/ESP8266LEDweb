#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "user";
const char *password = "88888888";

ESP8266WebServer server(80);

String ledStatus = "Tidak diketahui";

String page = R"(
      <!DOCTYPE html>
      <html>

      <head>
        <title>Web Server ESP8266 Sederhana</title>
      </head>

      <body>
        <h1>ESP8266 Web Server Sederhana</h1>
        <p>LED Status: <span id="ledStatus">Tidak diketahui</span></p>
        <form action="/LEDOn">
          <button type="submit">LED On</button>
        </form>
        <form action="/LEDOff">
          <button type="submit">LED Off</button>
        </form>
        <script>
          function updateStatus(status) {
            document.getElementById('ledStatus').innerText = status;
          }
        </script>
      </body>

      </html>
      )";

#define LEDPin D0

void setup(void)
{
    delay(1000);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Sukses terkoneksi ke ");
    Serial.println(ssid);
    Serial.print("IP address anda: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](){ 
        server.send(200, "text/html", page); 
    });

    server.on("/LEDOn", HTTP_GET, [](){
        server.send(200, "text/plain", "LED On");
        digitalWrite(LEDPin, HIGH);
        ledStatus = "On";
        updateStatus(ledStatus); 
    });

    server.on("/LEDOff", HTTP_GET, [](){
        server.send(200, "text/plain", "LED Off");
        digitalWrite(LEDPin, LOW);
        ledStatus = "Off";
        updateStatus(ledStatus); 
    });

    server.begin();
    Serial.println("Web server nyala!");
}

void loop(void)
{
    server.handleClient();
}