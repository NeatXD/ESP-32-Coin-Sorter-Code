#include "esp_camera.h"
#include <WiFi.h>
//Done By Yazan Ayash
// WiFi Setup
const char* ssid = "GJU_STUDENT";
const char* password = "GJUstudent";

// Ip Addresses
IPAddress camIP(10, 1, 138, 81);
IPAddress gateway(10, 12, 0, 1);
IPAddress subnet(255, 255, 0, 0);

IPAddress laptopIP(10, 12, 14, 43);
#define CMD_PORT 7000
#define IMG_PORT 8000

WiFiServer cmdServer(CMD_PORT);
WiFiClient imgClient;

// Standard universal Code for ESP-CAM
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
#define FLASH_PIN 4

bool initCamera() {
  camera_config_t c;
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer = LEDC_TIMER_0;
  c.pin_d0 = Y2_GPIO_NUM;
  c.pin_d1 = Y3_GPIO_NUM;
  c.pin_d2 = Y4_GPIO_NUM;
  c.pin_d3 = Y5_GPIO_NUM;
  c.pin_d4 = Y6_GPIO_NUM;
  c.pin_d5 = Y7_GPIO_NUM;
  c.pin_d6 = Y8_GPIO_NUM;
  c.pin_d7 = Y9_GPIO_NUM;
  c.pin_xclk = XCLK_GPIO_NUM;
  c.pin_pclk = PCLK_GPIO_NUM;
  c.pin_vsync = VSYNC_GPIO_NUM;
  c.pin_href = HREF_GPIO_NUM;
  c.pin_sscb_sda = SIOD_GPIO_NUM;
  c.pin_sscb_scl = SIOC_GPIO_NUM;
  c.pin_pwdn = PWDN_GPIO_NUM;
  c.pin_reset = RESET_GPIO_NUM;
  c.xclk_freq_hz = 20000000;
  c.pixel_format = PIXFORMAT_JPEG;
  pinMode(FLASH_PIN, OUTPUT);

  //Checking if the camera im using has a psram or not (Affects the transfer for images)
  if (psramFound()) {
    c.frame_size = FRAMESIZE_VGA;
    c.fb_count = 2;
  } else {
    c.frame_size = FRAMESIZE_QVGA;
    c.fb_count = 1;
  }
  c.jpeg_quality = 12;

  if (esp_camera_init(&c) != ESP_OK) {
    Serial.println("Camera init failed!");
    return false;
  }
  Serial.println("Camera initialized successfully");
  return true;
}

void setup() {

  for (int i = 0; i < 50; i++) {
  Serial.println();
}
  Serial.begin(115200);
  delay(1000);

  // Wifi configuration
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".\n");
  }
  Serial.println();
  Serial.print("Wi-Fi connected. IP: ");
  Serial.println(WiFi.localIP());

  // Initializing camera
  if (!initCamera()) {
    while (true) delay(1000); // stop if camera fails
  }

  // Starting command server
  cmdServer.begin();
  Serial.print("Command server started on port ");
  Serial.println(CMD_PORT);
}

void loop() {
  WiFiClient cmd = cmdServer.available();
  if (!cmd) {
    Serial.println("No client");
    delay(500);
    return;
  };

  if (cmd.available()) {
    uint8_t c = cmd.read();
    Serial.println("Read c");

    if (c != 0x01) 
    {
      Serial.println("Not 1");
      return;
      }

    // Capture image
    digitalWrite(FLASH_PIN, HIGH);
    camera_fb_t *fb = esp_camera_fb_get();
    delay(10);
    digitalWrite(FLASH_PIN, LOW);
    if (!fb) {
      Serial.println("Failed to capture image");
      return;
    }

    // Send image size
    uint32_t size = fb->len;
    cmd.write((uint8_t*)&size, sizeof(size));

    // Send image data
    cmd.write(fb->buf, fb->len);
    cmd.flush();
    cmd.stop();

    esp_camera_fb_return(fb);
    Serial.print("Image sent, size: ");
    Serial.println(size);
  }
}
