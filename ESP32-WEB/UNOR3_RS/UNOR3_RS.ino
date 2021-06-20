#define LED_1_BUTTON 3
#define LED_2_BUTTON 4
#define LED_3_BUTTON 5
#define FAN_1_BUTTON 6
#define FAN_2_BUTTON 7
#define FAN_3_BUTTON 8
#define PUMP_BUTTON 9
#define SV_BUTTON 10
String Total_Str = "" ;
String LED_1 = "" ;
String LED_2 = "" ;
String LED_3 = "" ;
String FAN_1 = "" ;
String FAN_2 = "" ;
String FAN_3 = "" ;
String PUMP = "" ;
String SV = "" ;
int LED1 = 0 ;
int LED2 = 0 ;
int LED3 = 0 ;
int FAN1 = 0 ;
int FAN2 = 0 ;
int FAN3 = 0 ;
int PUMP_ = 0 ;
int SV_ = 0 ;
String state = "";
int states;
int Last_sate_sw1 = 0;
int mode = 0;

void setup() {

  Serial.begin(9600);
  pinMode(LED_1_BUTTON, INPUT_PULLUP);
  pinMode(LED_2_BUTTON, INPUT_PULLUP);
  pinMode(LED_3_BUTTON, INPUT_PULLUP);
  pinMode(FAN_1_BUTTON, INPUT_PULLUP);
  pinMode(FAN_2_BUTTON, INPUT_PULLUP);
  pinMode(FAN_3_BUTTON, INPUT_PULLUP);
  pinMode(PUMP_BUTTON, INPUT_PULLUP);
  pinMode(SV_BUTTON, INPUT_PULLUP);
  LED_1 = "OFF";
  LED_2 = "OFF";
  LED_3 = "OFF";
  FAN_1 = "OFF";
  FAN_2 = "OFF";
  FAN_3 = "OFF";
  PUMP = "OFF";
  SV = "OFF";

}

void loop() {

  if (Serial.available() > 0) {
    state = Serial.readString();
    //    Serial.print(state);

    switch (state.toInt()) {
      case 11:
        LED1_ON();
        break;

      case 10:
        LED1_OFF();
        break;

      case 21:
        LED2_ON();
        break;

      case 20:
        LED2_OFF();
        break;

      case 31:
        LED3_ON();
        break;

      case 30:
        LED3_OFF();
        break;

      case 41:
        FAN1_ON();
        break;

      case 40:
        FAN1_OFF();
        break;

      case 51:
        FAN2_ON();
        break;

      case 50:
        FAN2_OFF();
        break;

      case 61:
        FAN3_ON();
        break;

      case 60:
        FAN3_OFF();
        break;

      case 71:
        PUMP_ON();
        break;

      case 70:
        PUMP_OFF();
        break;

      case 81:
        SV_ON();
        break;

      case 80:
        SV_OFF();
        break;

      case 91:
        mode = 1;
        break;

      case 90:
        mode = 0;
        break;
    }
  }


  //  if (mode == 1 && digitalRead(LED_1_BUTTON) == LOW) {
  //    LED1_ON();
  //  }
  //  else if (mode == 1 && digitalRead(LED_1_BUTTON) == HIGH) {
  //    LED1_OFF();
  //  }
  //
  //  if (mode == 1 && digitalRead(LED_2_BUTTON) == LOW) {
  //    LED2_ON();
  //  }
  //  else if (mode == 1 && digitalRead(LED_2_BUTTON) == HIGH) {
  //    LED2_OFF();
  //  }
  //
  //  if (mode == 1 && digitalRead(LED_3_BUTTON) == LOW) {
  //    LED3_ON();
  //  }
  //  else if (mode == 1 && digitalRead(LED_3_BUTTON) == HIGH) {
  //    LED3_OFF();
  //  }
  //
  //  if (mode == 1 && digitalRead(FAN_1_BUTTON) == LOW) {
  //    FAN1_ON();
  //  }
  //  else if (mode == 1 && digitalRead(FAN_1_BUTTON) == HIGH) {
  //    FAN1_OFF();
  //  }
  //
  //  if (mode == 1 && digitalRead(FAN_2_BUTTON) == LOW) {
  //    FAN2_ON();
  //  }
  //  else if (mode == 1 && digitalRead(FAN_2_BUTTON) == HIGH) {
  //    FAN2_OFF();
  //  }
  //
  //  if (mode == 1 && digitalRead(FAN_3_BUTTON) == LOW) {
  //    FAN3_ON();
  //  }
  //  else if (mode == 1 && digitalRead(FAN_3_BUTTON) == HIGH) {
  //    FAN3_OFF();
  //  }
  //
  //  if (mode == 1 && digitalRead(PUMP_BUTTON) == LOW) {
  //    PUMP_ON();
  //  }
  //  else if (mode == 1 && digitalRead(PUMP_BUTTON) == HIGH) {
  //    PUMP_OFF();
  //  }
  //
  //  if (mode == 1 && digitalRead(SV_BUTTON) == LOW) {
  //    SV_ON();
  //  }
  //  else if(mode == 1 && digitalRead(SV_BUTTON) == HIGH) {
  //    SV_OFF();
  //  }
  delay(200);
}
void LED1_ON() {
  digitalWrite(LED_1_BUTTON, HIGH);
  LED_1 = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void LED1_OFF() {
  digitalWrite(LED_1_BUTTON, LOW);
  LED_1 = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void LED2_ON() {
  digitalWrite(LED_2_BUTTON, HIGH);
  LED_2 = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void LED2_OFF() {
  digitalWrite(LED_2_BUTTON, LOW);
  LED_2 = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void LED3_ON() {
  digitalWrite(LED_3_BUTTON, HIGH);
  LED_3 = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void LED3_OFF() {
  digitalWrite(LED_3_BUTTON, LOW);
  LED_3 = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}

void FAN1_ON() {
  digitalWrite(FAN_1_BUTTON, HIGH);
  FAN_1 = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void FAN1_OFF() {
  digitalWrite(FAN_1_BUTTON, LOW);
  FAN_1 = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void FAN2_ON() {
  digitalWrite(FAN_2_BUTTON, HIGH);
  FAN_2 = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void FAN2_OFF() {
  digitalWrite(FAN_2_BUTTON, LOW);
  FAN_2 = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void FAN3_ON() {
  digitalWrite(FAN_3_BUTTON, HIGH);
  FAN_3 = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void FAN3_OFF() {
  digitalWrite(FAN_3_BUTTON, LOW);
  FAN_3 = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}

void PUMP_ON() {
  digitalWrite(PUMP_BUTTON, HIGH);
  PUMP = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void PUMP_OFF() {
  digitalWrite(PUMP_BUTTON, LOW);
  PUMP = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}

void SV_ON() {
  digitalWrite(SV_BUTTON, HIGH);
  SV = "ON";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
void SV_OFF() {
  digitalWrite(SV_BUTTON, LOW);
  SV = "OFF";
  Total_Str = "LED1:" + LED_1 + "/LED2:" + LED_2 + "/LED3:" + LED_3 + "/FAN1:" + FAN_1 + "/FAN2:" + FAN_2 + "/FAN3:" + FAN_3 + "/PUMP:" + PUMP + "/SV:" + SV;
  Serial.println(Total_Str);
}
