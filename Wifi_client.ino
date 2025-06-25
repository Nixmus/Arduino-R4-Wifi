#include "WiFiS3.h"

// Configura tu WiFi aquí
char ssid[] = "Familia.Garzon_2.4G";
char pass[] = "22547995T";
int keyIndex = 0;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Esperar conexión del puerto serie
  }

  // Verificar módulo WiFi
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Error: Módulo WiFi no encontrado!");
    while (true); // Detener ejecución
  }

  // Verificar firmware (opcional pero informativo)
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Aviso: Considera actualizar el firmware");
  }

  // Conectar a red WiFi
  Serial.println("Iniciando conexión WiFi...");
  while (status != WL_CONNECTED) {
    Serial.print("Conectando a: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  
  // Iniciar servidor
  server.begin();
  printWifiStatus();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Cliente conectado");
    boolean currentLineIsBlank = true;
    
    // Procesar petición HTTP
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        // Detectar final de petición HTTP (línea en blanco)
        if (c == '\n' && currentLineIsBlank) {
          // Enviar respuesta HTTP
          sendHTTPResponse(client);
          break;
        }
        
        // Actualizar estado de línea
        currentLineIsBlank = (c == '\n') ? true : (c != '\r' ? false : currentLineIsBlank);
      }
    }
    
    delay(1); // Tiempo para que el navegador reciba los datos
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void sendHTTPResponse(WiFiClient& client) {
  // Headers HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=utf-8");
  client.println("Connection: close");
  client.println("Refresh: 5"); // Auto-actualización cada 5 segundos
  client.println();
  
  // Página HTML
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Arduino UNO R4 WiFi - Sensores</title>");
  client.println("<style>");
  client.println("body{font-family:Arial;margin:20px;background:#f0f0f0;}");
  client.println(".container{background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);}");
  client.println("h1{color:#333;text-align:center;}");
  client.println(".sensor{padding:10px;margin:5px 0;background:#e8f4f8;border-left:4px solid #2196F3;border-radius:5px;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<div class='container'>");
  client.println("<h1>🔌 Arduino UNO R4 WiFi</h1>");
  client.println("<h2>📊 Lecturas de Sensores Analógicos</h2>");
  
  // Leer y mostrar valores de sensores
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    client.print("<div class='sensor'>");
    client.print("<strong>Pin A");
    client.print(analogChannel);
    client.print(":</strong> ");
    client.print(sensorReading);
    client.print(" (");
    client.print(map(sensorReading, 0, 1023, 0, 100));
    client.print("%)");
    client.println("</div>");
  }
  
  client.println("<p><small>🔄 Actualización automática cada 5 segundos</small></p>");
  client.println("</div>");
  client.println("</body>");
  client.println("</html>");
}

void printWifiStatus() {
  Serial.println("\n=== INFORMACIÓN DE CONEXIÓN ===");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  IPAddress ip = WiFi.localIP();
  Serial.print("Dirección IP: ");
  Serial.println(ip);
  
  long rssi = WiFi.RSSI();
  Serial.print("Intensidad de señal (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
  
  Serial.println("================================");
  Serial.print("🌐 Accede desde tu navegador: http://");
  Serial.println(ip);
  Serial.println("================================\n");
}
