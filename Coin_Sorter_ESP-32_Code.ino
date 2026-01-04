#include <WiFi.h>
#include <ESP32Servo.h>

//Done By Yazan Ayash

//WiFi Setup code
const char* ssid = "GJU_STUDENT";
const char* password = "GJUstudent";

//Initializing the Laptop Port
IPAddress laptopIP(10, 12, 14, 43);
const uint16_t laptopPort = 6000;
WiFiClient client;


//Initializing IR sensor
#define IR_PIN 13
#define DARK_LEVEL HIGH

//Initializing the Servo Motors
Servo gateServo1;
#define SERVO_PIN 14
#define CLOSED_POS 100
#define OPEN_POS 45
Servo gateServo2;
#define SERVO2_PIN 5
#define CLOSED2_POS 90
#define OPEN2_POS 180
Servo rampServo;
#define RAMPSERVO_PIN 8
#define FIFTYQ 120
#define TWENTYFIVEQ 90
#define TENQ 60
#define FIVEQ 30

bool sent = false;

void setup() {

  for (int i = 0; i < 50; i++) {
  Serial.println();
}
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);

  // Servo setup
  gateServo1.setPeriodHertz(50);
  gateServo1.attach(SERVO_PIN, 500, 2400);
  gateServo1.write(CLOSED_POS);
  
  gateServo2.setPeriodHertz(50);
  gateServo2.attach(SERVO2_PIN, 500, 2400);
  gateServo2.write(CLOSED2_POS);
  
  rampServo.setPeriodHertz(50);
  rampServo.attach(RAMPSERVO_PIN, 500, 2400);

  // WiFi setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nESP32 connected");
  Serial.println(WiFi.localIP());

}

void loop() {

  // Ensure connection to laptop is always alive
  if (!client.connected()) {
    Serial.println("Connecting to laptop...");

    while (!client.connect(laptopIP, laptopPort)) {
      Serial.print(".");
      delay(500);
    }

    Serial.println("\nConnected to laptop");
  }

  // Read IR sensor
  int ir = digitalRead(IR_PIN);

  // Trigger only once per coin
  if (ir == DARK_LEVEL && !sent) {

    // Send trigger
    client.println("TAKE_PIC");
    Serial.println("TAKE_PIC sent");

    // Wait for reply
    unsigned long startTime = millis();
    while (!client.available()) {
      if (!client.connected()) {
        Serial.println("Laptop disconnected!");
        sent = false;
        return;
      }
      // if (millis() - startTime > 8000) {
      //   Serial.println("Timeout waiting for reply");
      //   return;
      // }
    }

    int reply = client.parseInt();
    Serial.print("Received reply: ");
    Serial.println(reply);

    // -------- Motor logic --------
    

    //Moving motors if the coin is 50 Piasters
      if (reply == 0) {
        Serial.println("Opening interior gate");
        gateServo1.write(OPEN_POS);
        delay(2000);
        gateServo1.write(CLOSED_POS);
        delay(3000);
        Serial.println("Moving exteerior Ramp");
        rampServo.write(FIFTYQ);
        Serial.println("Opening exterior gate");
        gateServo2.write(OPEN2_POS);
        delay(2000);
        gateServo2.write(CLOSED2_POS);
      }

      //Moving the motors if the coin is 25 Piasters
      if (reply == 1) {
        Serial.println("Opening interior gate");
        gateServo1.write(OPEN_POS);
        delay(2000);
        gateServo1.write(CLOSED_POS);
        delay(3000);
        Serial.println("Moving exteerior Ramp");
        rampServo.write(TWENTYFIVEQ);
        Serial.println("Opening exterior gate");
        gateServo2.write(OPEN2_POS);
        delay(2000);
        gateServo2.write(CLOSED2_POS);
      }

      //Moving the motors if the coin is 10 piasters
      if (reply == 2) {
        Serial.println("Opening interior gate");
        gateServo1.write(OPEN_POS);
        delay(2000);
        gateServo1.write(CLOSED_POS);
        delay(3000);
        Serial.println("Moving exteerior Ramp");
        rampServo.write(TENQ);
        Serial.println("Opening exterior gate");
        gateServo2.write(OPEN2_POS);
        delay(2000);
        gateServo2.write(CLOSED2_POS);
      }

      //Moving the motors if the coin is 5 piasters
      if (reply == 3) {
        Serial.println("Opening interior gate");
        gateServo1.write(OPEN_POS);
        delay(2000);
        gateServo1.write(CLOSED_POS);
        delay(3000);
        Serial.println("Moving exteerior Ramp");
        rampServo.write(TENQ);
        Serial.println("Opening exterior gate");
        gateServo2.write(OPEN2_POS);
        delay(2000);
        gateServo2.write(CLOSED2_POS);
      }

    sent = true;
  }

  // Reset trigger when IR clears
  if (ir != DARK_LEVEL) {
    sent = false;
  }

  delay(50);
}


// void loop() {
//   int ir = digitalRead(IR_PIN);

//   if (ir == DARK_LEVEL && !sent) {
//     WiFiClient client;

//     if (client.connect(laptopIP, laptopPort)) {
//       Serial.println("Connected to Python");

//       // Sending trigger
//       client.println("TAKE_PIC");
//       Serial.println("TAKE_PIC sent");

//       // Waiting for a reply
//       unsigned long startTime = millis();
//       while (!client.available()) {
//         if (millis() - startTime > 8000) {
//           Serial.println("Timeout waiting for reply");
//           client.stop();
//           return;
//         }
//       }

//       // Reading the reply
//       int reply = client.parseInt();
//       Serial.print("Received reply: ");
//       Serial.println(reply);
      
//       //Moving motors if the coin is 50 Piasters
//       if (reply == 0) {
//         Serial.println("Opening interior gate");
//         gateServo1.write(OPEN_POS);
//         delay(2000);
//         gateServo1.write(CLOSED_POS);
//         delay(3000);
//         Serial.println("Moving exteerior Ramp");
//         rampServo.write(FIFTYQ);
//         Serial.println("Opening exterior gate");
//         gateServo2.write(OPEN2_POS);
//         delay(2000);
//         gateServo2.write(CLOSED2_POS);
//       }

//       //Moving the motors if the coin is 25 Piasters
//       if (reply == 1) {
//         Serial.println("Opening interior gate");
//         gateServo1.write(OPEN_POS);
//         delay(2000);
//         gateServo1.write(CLOSED_POS);
//         delay(3000);
//         Serial.println("Moving exteerior Ramp");
//         rampServo.write(TWENTYFIVEQ);
//         Serial.println("Opening exterior gate");
//         gateServo2.write(OPEN2_POS);
//         delay(2000);
//         gateServo2.write(CLOSED2_POS);
//       }

//       //Moving the motors if the coin is 10 piasters
//       if (reply == 2) {
//         Serial.println("Opening interior gate");
//         gateServo1.write(OPEN_POS);
//         delay(2000);
//         gateServo1.write(CLOSED_POS);
//         delay(3000);
//         Serial.println("Moving exteerior Ramp");
//         rampServo.write(TENQ);
//         Serial.println("Opening exterior gate");
//         gateServo2.write(OPEN2_POS);
//         delay(2000);
//         gateServo2.write(CLOSED2_POS);
//       }

//       //Moving the motors if the coin is 5 piasters
//       if (reply == 3) {
//         Serial.println("Opening interior gate");
//         gateServo1.write(OPEN_POS);
//         delay(2000);
//         gateServo1.write(CLOSED_POS);
//         delay(3000);
//         Serial.println("Moving exteerior Ramp");
//         rampServo.write(TENQ);
//         Serial.println("Opening exterior gate");
//         gateServo2.write(OPEN2_POS);
//         delay(2000);
//         gateServo2.write(CLOSED2_POS);
//       }

//       // Closing the socket after the end of communication
//       client.stop();
//       sent = true;
//     }
//   }

//   // Reset when IR is no longer triggered
//   if (ir != DARK_LEVEL) {
//     sent = false;
//   }

//   delay(100);
// }
