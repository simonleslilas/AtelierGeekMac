#define CANDLELED 11
#define REDLED 7
#define BUZZER 4
#define PROXIMITY_THRESHOLD 800 
#define PROXIMITY_CONSECUTIVE_READINGS 3
#define BUZZER_FREQUENCY 38
#define FLICKER_INTERVAL 25

long previousMillis = 0;
long closeReadings = 0;

/* Constantes pour les broches */
const byte TRIGGER_PIN = 3; // Broche TRIGGER
const byte ECHO_PIN = 2;    // Broche ECHO
 
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

void setup() {
  pinMode(CANDLELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  //pinMode(SENSOR, INPUT);
  //delay(1000); // Allow the proximity sensor to initialize
    /* Initialise le port série */
  Serial.begin(115200);
   
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
}
void loop() {


   
  if (getDistance() < PROXIMITY_THRESHOLD) // Is someone close?
  {
    closeReadings++;
    if (closeReadings >= PROXIMITY_CONSECUTIVE_READINGS) // require n consecutive "close" readings before going into EVIL mode. This prevents little blips of the buzzer.
      {
        digitalWrite(CANDLELED, LOW);
        digitalWrite(REDLED, HIGH);
        tone(BUZZER, BUZZER_FREQUENCY);
      }
  }
  else
  { //regular candle flicker. Based on Arduino example BlinkWithoutDelay.
    noTone(BUZZER);
    digitalWrite(REDLED, LOW);
    closeReadings = 0;
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > FLICKER_INTERVAL)
    {
      previousMillis = currentMillis;
      analogWrite(CANDLELED, random(0, 256));
    }
  }
}

float getDistance(){
      digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
   digitalWrite(TRIGGER_PIN, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;

  

    /* Affiche les résultats en mm, cm et m */
  Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(distance_mm / 1000.0, 2);
  Serial.println(F("m)"));
   
  return distance_mm;
}


