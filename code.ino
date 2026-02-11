// Калибровочные значения
const int DRY_SOIL_VALUE = 50;    // Сухая почва (нужен полив)
const int WET_SOIL_VALUE = 840;   // Влажная почва (после полива)

// Порог полива (можно менять)
const int WATERING_THRESHOLD = 200;  // Начать полив ниже этого знач

// Пины
const int MOISTURE_PIN = A1;  // Датчик влажности
const int PUMP_PIN = 9;       // Насос

// Тайминги
unsigned long lastCheck = 0;
const unsigned long CHECK_INTERVAL = 5000;  // Проверка кажд 5 сек
const unsigned long PUMP_TIME = 1000;       // Полив 1 сек

// Состояние насоса
bool pumpRunning = false;
unsigned long pumpStartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  Serial.println("=== Система полива запущена ===");
  Serial.print("Сухая почва: "); Serial.println(DRY_SOIL_VALUE);
  Serial.print("Влажная почва: "); Serial.println(WET_SOIL_VALUE);
  Serial.print("Порог полива: "); Serial.println(WATERING_THRESHOLD);
  Serial.println();
}

void loop() {
  // Периодическая проверка датчика
  if (millis() - lastCheck >= CHECK_INTERVAL) {
    int sensorValue = analogRead(MOISTURE_PIN);
    
    // Считаем процент влажности (для инверсного датчика)
    int moisturePercent = map(sensorValue, DRY_SOIL_VALUE, WET_SOIL_VALUE, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);
    
    Serial.print("Сырое: ");
    Serial.print(sensorValue);
    Serial.print(" | Влажность: ");
    Serial.print(moisturePercent);
    Serial.print("%");
    
    // Проверка нужен ли полив
    if (sensorValue < WATERING_THRESHOLD && !pumpRunning) {
      Serial.println(" - СУХО, включаем насос!");
      startPump();
    } else if (sensorValue >= WATERING_THRESHOLD && !pumpRunning) {
      Serial.println(" - ОК");
    }
    
    lastCheck = millis();
  }
  
  // Управление насосом
  if (pumpRunning) {
    if (millis() - pumpStartTime >= PUMP_TIME) {
      stopPump();
      Serial.println("Насос выкл");
    }
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
