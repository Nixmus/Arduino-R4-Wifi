#include "WiFiS3.h"

// Configura tu WiFi aquí
char ssid[] = "TU_RED_WIFI";
char pass[] = "TU_PASSWORD";
int keyIndex = 0;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Esperar a que se conecte el puerto serie
  }
  
  // Verificar módulo WiFi
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Error: No se encuentra el módulo WiFi!");
    while (true);
  }
  
  // Conectar a WiFi
  Serial.print("Conectando a: ");
  Serial.println(ssid);
  
  while (status != WL_CONNECTED) {
    Serial.print("Intentando conexión a SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000); // Esperar 10 segundos entre intentos
  }
  
  Serial.println();
  Serial.println("¡Conectado!");
  
  server.begin();
  Serial.print("Servidor iniciado en: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    // Esperar petición HTTP
    while (client.connected() && !client.available()) delay(1);
    
    // Leer y descartar la petición
    while (client.available()) client.read();
    
    // Enviar respuesta HTML
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<html><body>");
    client.println("<h1>Sensores Arduino</h1>");
    
    // Mostrar valores de sensores
    for (int i = 0; i < 6; i++) {
      client.print("<p>A");
      client.print(i);
      client.print(": ");
      client.print(analogRead(i));
      client.println("</p>");
    }
    
    client.println("</body></html>");
    client.stop();
  }
}
