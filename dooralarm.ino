/*
 * Door alarm by cone
 * Power is applied to nodemcu when the door is opened. When this happens it should:
 * - Chirp every two minutes, for 30 mins as a reminder that the door is open
 * - Alarm after 30 minutes. Each alarm cycle is: on for 30s, off for 3 for maximum annoy
 * - After > 60 iterations of this, return to a longer chirp to reduce noise pollution risk in case of failure of switch when noone is in.
 * 
 * TODO: MQTT can be integrated with init for door open, and a last will and testament for when power is removed. 
 * TODO: LEDs for maximum shine
 */

const int ALARM_PIN = 5; //Pin the alarm is connected to
const int GRACE_PERIOD = 1800000; //How long can the door be opened for until the alarm goes off
const int CHIRP_PERIOD = 120000; //How often should the alarm chirp when the door is first opened?

long startMillis = 0;
long startChirpMillis = 0;

int alarmFlag = 0;
int alarmCount = 0;


void setup() {
  pinMode(ALARM_PIN, OUTPUT);
  startMillis = millis(); //START THE CLOCK
  chirp(); //Indicate the alarm is working when opened
}

/*
 * Produces a single chirp. 
 * Realistic minimum time is 100ms due to the sounder circuitry.
 */
void chirp() {
    digitalWrite(ALARM_PIN, HIGH);
    delay(100);
    digitalWrite(ALARM_PIN, LOW);
}

// the loop function runs over and over again forever
void loop() {
  long currentMillis = millis();

  if(alarmFlag == 1){
    /*
     * GRACE PERIOD HAS BEEN PASSED
     */
    if(alarmCount > 60){
      /*
       * Prevent noise pollution caused from a continually sounding alarm. 
       */
      digitalWrite(ALARM_PIN, HIGH);
      delay(500);
      digitalWrite(ALARM_PIN, LOW);
      delay(60000);
    }else{
      alarmCount ++;
      digitalWrite(ALARM_PIN, HIGH);
      delay(30000);
      digitalWrite(ALARM_PIN, LOW);
      delay(3000);
    }
    
  }else{
    /**
     * In the grace period - chirp when neded and keep track of timer for alarm
     */

     //If we have passed the grace period, set alarm flag to true
    if(currentMillis - startMillis >= GRACE_PERIOD) {
      alarmFlag = 1;
      startMillis = currentMillis;
    }
    
    //chirp
    if(currentMillis - startChirpMillis >= CHIRP_PERIOD) {
      chirp();
      startChirpMillis = currentMillis;
    }
  }
  
  
}
