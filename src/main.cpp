#include "config.h"

void setup() 
{
  Serial.begin(115200);
  dht.setup(DHTPIN, DHTesp::DHT11);
  lcd.init();
  lcd.backlight();
  pinMode(lightPin, INPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);

  /* Connect Wifi*/
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  /* Connect to Adafruit Server*/
  mqtt.connect();
}

void reconnectMQTT()
{
  while ( !mqtt.connected() ) 
  {
    if ( mqtt.connect() ) 
    {
      Serial.println("Connected !");
    } 
    else 
    {
      delay(5000); 
    }
  }
}

void enterDeepSleep() 
{
  Serial.println("Entering Deep Sleep...");
  esp_sleep_enable_timer_wakeup(15 * 60 * 1000000); 
  esp_deep_sleep_start();
}

void loop() 
{
  int temperature = dht.getTemperature();
  int humidity = dht.getHumidity();
  int lightValue = analogRead(lightPin);
  int gasValue = digitalRead(gasPin);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Gas: ");
  Serial.println(gasValue);

  if( !mqtt.ping(3) ) 
  {
    if(! mqtt.connected())
      reconnectMQTT();
  }

  /* Publish to Adafruit */
  if ( !Temperature1.publish(temperature) ) {                     
    Serial.println(F("Failed"));
  } 
  if ( !Humidity1.publish(humidity) ) {                   
    Serial.println(F("Failed"));
  }
  else {
    Serial.println(F("Sent!"));
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:  ");
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(humidity);

  if( lightValue > 3800 )
  {
    lcd.setCursor(10,0);
    lcd.print("Night");
  }
  else
  {
    lcd.setCursor(10,0);
    lcd.print("Day  ");
  }

  if( gasValue == 1 )
  {
    digitalWrite(REDPIN,1);
    lcd.setCursor(10,1);
    lcd.print("WARNING");
  }
  else
  {
    digitalWrite(GREENPIN,1);
    lcd.setCursor(10,1);
    lcd.print("GOOD");
  }
  delay(15000);
  enterDeepSleep();
}