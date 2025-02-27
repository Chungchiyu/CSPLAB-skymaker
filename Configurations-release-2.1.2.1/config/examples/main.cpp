#include <Arduino.h>

#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))
int data_num;
int ID[][4] = {
    {0X6C, 0X9F, 0X22, 0x23}, // set card
    {0xF3, 0x44, 0x6F, 0x03}, //
    {0x35, 0xF5, 0xD7, 0x47}, // 仲其宇
    {0xD5, 0x2C, 0xDC, 0x67}, // 林柏漢
    {0xB3, 0xFF, 0xBC, 0xE9}, // 顏澹寧
    {0xB5, 0xC1, 0x06, 0x48}, // 王偉呈
    {0x85, 0x03, 0xF7, 0x67}, // 李易鴻
    {0xD2, 0x08, 0xD8, 0x0C}, // 賴冠中
    {0x92, 0xBE, 0xE5, 0x0C}, // 薛子承
    {0xF5, 0xF4, 0xC6, 0x47}, // 賴楷均
    {0xF3, 0x6C, 0xBC, 0xE9}, // 李德慶
    {0x35, 0x75, 0xCF, 0x67}, // 蔡閔佑
    {0x35, 0xF4, 0xFF, 0x47}, // 陳靖棠
    {0x53, 0xC0, 0xBC, 0xE9}, // 黃弘翔
    {0x35, 0xA1, 0x96, 0x48}, // 張庭瑋
    {0x03, 0x7E, 0xF1, 0xDB}, // 何書菲
    {0xD5, 0x0A, 0x05, 0x68}, // 史國政
    {0x15, 0xB9, 0xA3, 0x48}, // 羅湲晨
    {0xD5, 0xB9, 0xCE, 0x67}, // 張子凡
    {0xC2, 0xCC, 0xD4, 0x16}, // 潘沛則
    {0xF5, 0xFC, 0xD9, 0x47}, // 林煒成
    {0x95, 0x9C, 0x1E, 0x48}, // 林星辰
    {0x75, 0x52, 0xD7, 0x47}, // 李睿芹
    {0xB5, 0x0E, 0x9A, 0x48}, // 李璟欣
    {0x83, 0xB0, 0x73, 0xE3}, // 林韋辰
    {0x53, 0xA5, 0x73, 0xE3}, // 曾浩恩
    {0xF3, 0x81, 0x73, 0xE3}, // 賈鈞霖
    {0x35, 0xD7, 0xFC, 0x47}, // 賴冠霖
    {0x55, 0x80, 0xBF, 0x47}, // 何昌倫
    {0xA2, 0x5E, 0x87, 0x1B}, // 游鈞閎
    {0x43, 0xBD, 0x73, 0xE3}, // 王仁牧
    {0x22, 0x4D, 0x89, 0x1B}, // 黃子昕
    {0x82, 0xB7, 0x84, 0x1B}, // 伍峻寬
    {0x72, 0xD9, 0xBB, 0x1B}, // 游承嘉
    {0x62, 0xC9, 0x67, 0x1B}, // 呂佳諺
    {0xF2, 0x55, 0xB6, 0x1B}, // 吳秉樺
    {0x22, 0xFC, 0xB1, 0x1B}, // 許懷仁
    {0xC5, 0x3B, 0x07, 0x48}, // 陳凱宥
    {0x08, 0xDE, 0xFB, 0x00}, // 張庭瑋手機
    {0x15, 0xE6, 0xC5, 0x47}, // 林偉呈
};
/**
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from more than one PICC to
 * serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples
 * see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from more than one PICC (that
 * is: a RFID Tag or Card) using a
 * MFRC522 based RFID Reader on the Arduino SPI interface.
 *
 * Warning: This may not work! Multiple devices at one SPI are difficult and
 * cause many trouble!! Engineering skill
 *          and knowledge are required!
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro
 * Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5 RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required
 * **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required
 * **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4 16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1 14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3 15
 *
 * More pin layouts for other boards can be found here:
 * https://github.com/miguelbalboa/rfid#pin-layout
 *
 */

#include <EEPROM.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

#define RST_PIN 9 // 讀卡機的重置腳位
#define SS_PIN 10 // 晶片選擇腳位

MFRC522 mfrc522(SS_PIN, RST_PIN); // 建立MFRC522物件
Servo door;

bool state = true;
bool servo_state = true;
int open_angle = 180, close_angle = 10;
bool read = false;
int mode =
    0; // mode:0 -> normal state, mode:1 -> add card, mode:2 -> delete all card, mode:3 -> read all eeprom card

bool door_move() {
  door.attach(3);
  if (state) {
    door.write(close_angle);
    Serial.println("close");
    state = false;
    return true;
  } else {
    door.write(open_angle);
    Serial.println("open");
    state = true;
    return true;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("RFID reader is ready!");

  // door.attach(3);
  // door.write(close_angle);

  SPI.begin();
  mfrc522.PCD_Init(); // 初始化MFRC522讀卡機模組

  data_num = LEN(ID);
}

void loop() {

  static unsigned long t = millis();
  static unsigned long t2 = millis();

  if (millis() - t > 3000 && servo_state) {
    door.detach();
    servo_state = false;
    Serial.println("detach");
  }

  // 確認是否有新卡片
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    servo_state = true;
    t = millis();
    byte *id = mfrc522.uid.uidByte; // 取得卡片的UID
    byte idSize = mfrc522.uid.size; // 取得UID的長度

    Serial.print("PICC type: "); // 顯示卡片類型
    // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    Serial.print("UID Size: "); // 顯示卡片的UID長度值
    Serial.println(idSize);

    Serial.print("id:{");
    for (byte i = 0; i < idSize; i++) { // 逐一顯示UID碼
      Serial.print(id[i], HEX);              // 以16進位顯示UID值
      if (i != idSize - 1)
        Serial.print(", ");
    }
    Serial.println("}");

    mfrc522.PICC_HaltA(); // 讓卡片進入停止模式

    if (id[0] == ID[0][0] && id[1] == ID[0][1] && id[2] == ID[0][2] &&
        id[3] == ID[0][3]) {
      Serial.println("Add card mode");
      t2 = millis();
      mode = 1;
    }
    if (millis() - t2 > 10000 && mode == 1) {
      mode = 0;
      Serial.println("Back to read mode");
    }

    if (mode == 0) { // read mode
      for (int i = 0; i < data_num; i++) {
        if (id[0] == ID[i][0] && id[1] == ID[i][1] && id[2] == ID[i][2] &&
            id[3] == ID[i][3]) {
          if (!read)
            read = door_move();
        }
      }
      for (int i = 0; i < 256 && !read; i++) {
        if (EEPROM.read(4 * i) != 0) {
          if (id[0] == EEPROM.read(4 * i) && id[1] == EEPROM.read(4 * i + 1) &&
              id[2] == EEPROM.read(4 * i + 2) &&
              id[3] == EEPROM.read(4 * i + 3)) {
            if (!read)
              read = door_move();
          }
        }
      }
    } else if (mode == 1) { // add mode
      bool alreadyHad = false;
      for (int i = 0; i < data_num; i++) {
        if (id[0] == ID[i][0] && id[1] == ID[i][1] && id[2] == ID[i][2] &&
            id[3] == ID[i][3]) {
          Serial.println("Already had in code");
          alreadyHad = true;
          break;
        }
      }
      for (int i = 0; i < 256 && !alreadyHad; i++) {
        if (EEPROM.read(4 * i) != 0) {
          if (id[0] == EEPROM.read(4 * i) && id[1] == EEPROM.read(4 * i + 1) &&
              id[2] == EEPROM.read(4 * i + 2) &&
              id[3] == EEPROM.read(4 * i + 3)) {
            Serial.println("Already had in eeprom");
            break;
          }
        } else {
          EEPROM.write(4 * i, id[0]);
          EEPROM.write(4 * i + 1, id[1]);
          EEPROM.write(4 * i + 2, id[2]);
          EEPROM.write(4 * i + 3, id[3]);
          Serial.println("New card has been added");
          break;
        }
      }
    } else if (mode == 2) // delete all card mode
    {
      for (int i = 0; i < 256; i++) {
        EEPROM.write(4 * i, 0);
        EEPROM.write(4 * i + 1, 0);
        EEPROM.write(4 * i + 2, 0);
        EEPROM.write(4 * i + 3, 0);
        Serial.println("write0done");
      }
    } else if (mode == 3) { // read all card info mode
      for (int i = 0; i < 256; i++) {
        Serial.print(EEPROM.read(4 * i));
        Serial.print(',');
        Serial.print(EEPROM.read(4 * i + 1));
        Serial.print(',');
        Serial.print(EEPROM.read(4 * i + 2));
        Serial.print(',');
        Serial.print(EEPROM.read(4 * i + 3));
        Serial.println();
      }
    }
    read = false;

    delay(1000);
  }
}