#include "config.h"

void setup() {
  Serial.begin(115200);
  dht.setup(DHTPIN, DHTesp::DHT11);
  lcd.init();
  lcd.backlight();
  pinMode(lightPin, INPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);

  /* Connect Wifi */
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  /* Connect MQTT Broker */
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (mqtt.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(mqtt.state());
         delay(2000);
     }
 }
}

/*Set up Deep Sleep Mode*/
void enterDeepSleep() {
  Serial.println("Entering Deep Sleep...");
  // Thiết lập chế độ Deep Sleep
  esp_sleep_enable_timer_wakeup(15 * 60 * 1000000); // Đánh thức sau 30 phút (15 * 1000000 micro giây)

  // Khởi động Deep Sleep
  esp_deep_sleep_start();
}

void loop() {
    int temperature = dht.getTemperature();
    int humidity = dht.getHumidity();
    int lightValue = analogRead(lightPin);
    int gasValue = digitalRead(gasPin);
    char temperatureStr[8];
    char humidityStr[8];

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\t");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Gas: ");
    Serial.println(gasValue);

    /* Num to string and publish data to mqtt broker */
    snprintf(temperatureStr, sizeof(temperatureStr), "%d", temperature);
    snprintf(humidityStr, sizeof(humidityStr), "%d", humidity);
    mqtt.publish(MQTT_TOPIC_TEMPERATURE,temperatureStr);
    mqtt.publish(MQTT_TOPIC_HUMIDITY, humidityStr);
    Serial.println("Sent");

    /* Display LCD */
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:  ");
    lcd.print(temperature);
    lcd.setCursor(0, 1);
    lcd.print("Humid: ");
    lcd.print(humidity);

    if(lightValue > 3800){
      lcd.setCursor(10,0);
      lcd.print("Night");
    }
    else{
      lcd.setCursor(10,0);
      lcd.print("Day  ");
    }
    if(gasValue == 1){
      digitalWrite(REDPIN,1);
      lcd.setCursor(10,1);
      lcd.print("WARNING");
    }
    else{
      digitalWrite(GREENPIN,1);
      lcd.setCursor(10,1);
      lcd.print("Stable");
    }
    delay(5000);
    enterDeepSleep();
}
