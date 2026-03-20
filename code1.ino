// Калибровочные значения
const int DRY_SOIL_VALUE = 50;    // Сухая почва (нужен полив)
const int WET_SOIL_VALUE = 840;   // Влажная почва (после полива)

// Если значение датчика ниже 200 - включаем полив
const int WATERING_THRESHOLD = 200;

// Пины
const int MOISTURE_PIN = A1;
const int PUMP_PIN = 9;

// Тайминги
unsigned long lastCheck = 0;
const unsigned long CHECK_INTERVAL = 5000;  // Проверка каждые 5 сек
const unsigned long PUMP_TIME = 1000;       // Полив 1 сек

// Состояние насоса
bool pumpRunning = false;
unsigned long pumpStartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  Serial.println("Система полива запущена");
}

void loop() {
  // Периодическая проверка датчика
  if (millis() - lastCheck >= CHECK_INTERVAL) {
    int sensorValue = analogRead(MOISTURE_PIN);
    
    // Если почва сухая (значение ниже порога) и насос не работает - включаем
    if (sensorValue < WATERING_THRESHOLD && !pumpRunning) {
      Serial.println("Сухо, полив...");
      startPump();
    }
    
    lastCheck = millis();
  }
  
  // Управление насосом
  if (pumpRunning && millis() - pumpStartTime >= PUMP_TIME) {
    stopPump();
    Serial.println("Полив завершен");
  }
  
  delay(50);
}

void startPump() {
  pumpRunning = true;
  pumpStartTime = millis();
  digitalWrite(PUMP_PIN, HIGH);
}

void stopPump() {
  pumpRunning = false;
  digitalWrite(PUMP_PIN, LOW);
}