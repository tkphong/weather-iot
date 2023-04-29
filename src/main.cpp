#include "config.h"

void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void setup() {
  Serial.begin(115200);
  dht.setup(DHTPIN, DHTesp::DHT11);
  lcd.init();
  lcd.backlight();
  pinMode(lightPin, INPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);

  Serial.println(F("Adafruit IO Example"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
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

  // connect to adafruit io
  connect();
}

void loop() {
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  int soundValue = analogRead(soundPin);
  int lightValue = analogRead(lightPin);
  int gasValue = digitalRead(gasPin);

  // Hiển thị nhiệt độ và độ ẩm trên Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Sound: ");
  Serial.println(soundValue);
  Serial.print("Gas: ");
  Serial.println(gasValue);
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }
  delay(3000);
/* Publish to Adafruit */
  if (! Temperature1.publish(temperature)) {                     
    Serial.println(F("Failed"));
  } 
  if (! Humidity1.publish(humidity)) {                   
    Serial.println(F("Failed"));
  }
  if (! Sound1.publish(soundValue)){
    Serial.println(F("Failed"));
  }
  else {
    Serial.println(F("Sent!"));
  }
  // Hiển thị nhiệt độ và độ ẩm trên màn hình LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tem:");
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(humidity);

  if(lightValue > 3800){
    lcd.setCursor(10,0);
    lcd.print("Night");
  }
  else{
    lcd.setCursor(10,0);
    lcd.print("Day  ");
  }

  if(soundValue > 2000){
    lcd.setCursor(10,1);
    lcd.print("Windy ");
  }
  else{
    lcd.setCursor(10,1);
    lcd.print("Normal");
  }
  if(gasValue == 1){
    digitalWrite(REDPIN,1);
  }
  else{
    digitalWrite(GREENPIN,1);
  }
  delay(2000);
}
