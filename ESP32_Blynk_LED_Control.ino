#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

#define LED_PIN 2
#define BUTTON_PIN 19

int ledState = 0;
int buttonCount = 0;
bool lastButtonState = HIGH;

BlynkTimer timer;

// SWITCH CONTROL FROM APP (V0)
BLYNK_WRITE(V0)
{
  int value = param.asInt();

  Serial.print("Received V0 = ");
  Serial.println(value);

  ledState = value;

  digitalWrite(LED_PIN, ledState);

  // Update LED status in app
  Blynk.virtualWrite(V2, ledState);
}

// BUTTON PHYSICAL PRESS
void checkButton()
{
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW)
  {
    delay(30); // debounce

    if (digitalRead(BUTTON_PIN) == LOW)
    {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);

      buttonCount++;

      Blynk.virtualWrite(V1, buttonCount); // Counter
      Blynk.virtualWrite(V2, ledState);    // LED Status

      Serial.print("Button Count = ");
      Serial.println(buttonCount);
    }
  }

  lastButtonState = currentButtonState;
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(50L, checkButton);
}

void loop()
{
  Blynk.run();
  timer.run();
}
