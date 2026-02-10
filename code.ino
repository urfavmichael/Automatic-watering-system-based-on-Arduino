int current_humidity = 0, min_humidity = 200; // для мин сред ариф между сух и влаж почвы
int percent = 0;
int MOSpin = 9;
unsigned long time1 = 0;
unsigned long time2 = 0;
int t1 = 5000; // частота обращения к датчику влажности
int t2 = 1000; // время на сколько вкл насос
bool flag = false;
bool pumpRunning = false;

void setup() {
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(MOSpin, OUTPUT);
  analogWrite(MOSpin, 0); // Насос выключен при старте
}

void loop() {
  // Проверяем влажность почвы каждые t1 миллисекунд
  if (millis() - time1 >= t1) {
    // Читать аналоговый сигнал с датчика влажности
    current_humidity = analogRead(A1);
    // Запомнить когда проверили влажность почвы
    time1 = millis();
    // Вывод информации на экран
    Serial.print(current_humidity);
    Serial.print(" | ");
    percent = map(current_humidity, 0, 1023, 0, 100);
    Serial.print(100 - percent); // Инвертируем, чтобы показывать влажность, а не сухость
    Serial.println("%");
    
    // Если текущая влажность меньше минимальной
    if (current_humidity < min_humidity && !pumpRunning) {
      flag = true; // надо вкл насос
      time2 = millis(); // сохр время начала полива
    }
  }

  // Если надо включить насос и он еще не работает
  if (flag && !pumpRunning) {
    pumpRunning = true;
    analogWrite(MOSpin, 255);  // Включить насос на полную мощность
    Serial.println("Насос ВКЛЮЧЕН");
    time2 = millis(); // Запоминаем время включения
    flag = false;
  }

  // Если насос работает, проверяем время
  if (pumpRunning && millis() - time2 >= t2) {
    analogWrite(MOSpin, 0);  // Выключить насос
    pumpRunning = false;
    Serial.println("Насос ВЫКЛЮЧЕН");
  }

  // Небольшая задержка для стабильности
  delay(50);
}
