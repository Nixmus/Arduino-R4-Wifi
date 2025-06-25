#include "WiFiS3.h"
#include <Servo.h>

// Configura tu WiFi aquí
char ssid[] = "Familia.Garzon_2.4G";
char pass[] = "22547995T";
int keyIndex = 0;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

// Configuración del servo
Servo miServo;
int pinServo = 9; // Pin donde está conectado el servo
int posicionActual = 90; // Posición inicial del servo (90 grados)

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Esperar a que se conecte el puerto serie
  }
  
  // Inicializar servo
  miServo.attach(pinServo);
  miServo.write(posicionActual);
  Serial.print("Servo inicializado en posición: ");
  Serial.println(posicionActual);
  
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
    String request = "";
    
    // Leer la petición HTTP completa
    while (client.connected() && !client.available()) delay(1);
    
    while (client.available()) {
      char c = client.read();
      request += c;
      if (c == '\n') break; // Solo necesitamos la primera línea
    }
    
    // Descartar el resto de la petición
    while (client.available()) client.read();
    
    Serial.println("Petición recibida: " + request);
    
    // Procesar comandos del servo
    if (request.indexOf("GET /servo/0") >= 0) {
      posicionActual = 0;
      miServo.write(posicionActual);
      Serial.println("Servo movido a 0 grados");
    }
    else if (request.indexOf("GET /servo/45") >= 0) {
      posicionActual = 45;
      miServo.write(posicionActual);
      Serial.println("Servo movido a 45 grados");
    }
    else if (request.indexOf("GET /servo/90") >= 0) {
      posicionActual = 90;
      miServo.write(posicionActual);
      Serial.println("Servo movido a 90 grados");
    }
    else if (request.indexOf("GET /servo/135") >= 0) {
      posicionActual = 135;
      miServo.write(posicionActual);
      Serial.println("Servo movido a 135 grados");
    }
    else if (request.indexOf("GET /servo/180") >= 0) {
      posicionActual = 180;
      miServo.write(posicionActual);
      Serial.println("Servo movido a 180 grados");
    }
    
    // Enviar respuesta HTML con interfaz de control
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=UTF-8");
    client.println("Connection: close");
    client.println();
    
    client.println("<!DOCTYPE html>");
    client.println("<html lang='es'>");
    client.println("<head>");
    client.println("<meta charset='UTF-8'>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<title>Control de Servo SG90</title>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f0f0; margin: 0; padding: 20px; }");
    client.println(".container { max-width: 600px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }");
    client.println("h1 { color: #333; margin-bottom: 30px; }");
    client.println(".status { background-color: #e8f5e8; padding: 15px; border-radius: 5px; margin-bottom: 30px; }");
    client.println(".controls { display: flex; flex-wrap: wrap; justify-content: center; gap: 15px; }");
    client.println(".btn { background-color: #4CAF50; color: white; padding: 15px 25px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; text-decoration: none; display: inline-block; min-width: 80px; }");
    client.println(".btn:hover { background-color: #45a049; }");
    client.println(".btn:active { transform: translateY(2px); }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    
    client.println("<div class='container'>");
    client.println("<h1>🎛️ Control de Servo SG90</h1>");
    
    client.println("<div class='status'>");
    client.print("<h3>Posición actual: ");
    client.print(posicionActual);
    client.println("°</h3>");
    client.println("</div>");
    
    client.println("<div class='controls'>");
    client.println("<a href='/servo/0' class='btn'>0°</a>");
    client.println("<a href='/servo/45' class='btn'>45°</a>");
    client.println("<a href='/servo/90' class='btn'>90°</a>");
    client.println("<a href='/servo/135' class='btn'>135°</a>");
    client.println("<a href='/servo/180' class='btn'>180°</a>");
    client.println("</div>");
    
    client.println("<p style='margin-top: 30px; color: #666;'>Haz clic en los botones para mover el servo a diferentes posiciones</p>");
    client.println("</div>");
    
    client.println("</body>");
    client.println("</html>");
    
    delay(100); // Pequeña pausa para que el servo se mueva
    client.stop();
  }
}
