
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#include "Strings.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
void (*state)() = NULL;

const int LDR_PIN = A0;
const int TMP36_PIN = A1;
const int ATES_SENSOR_PIN = 8;
const int TITRESIM_SENSOR_PIN = 9;

boolean isikVarmi = 0;
boolean sicaklikDurum = 0;
boolean atesDurum = 0;
boolean titresimDurum = 0;
boolean olumDurumu = 0;
boolean cikisOdasiDurum = 0;
boolean dumanDurum = 0;
float sicaklik;
float olculendeger;


//Buton Atamaları
const int buton1 = 5;
const int buton2 = 6;

boolean butonDurumu1 = 1;
boolean butonDurumu2 = 1;
boolean sonButonDurumu1 = 1;
boolean sonButonDurumu2 = 1;
long lastDebounce1 = 0;
long lastDebounce2 = 0;


char buffer[32];


void setup() {
  Serial.begin(9600);
  pinMode(buton1 , INPUT_PULLUP);
  pinMode(buton2 , INPUT_PULLUP);
  pinMode(ATES_SENSOR_PIN, INPUT);
  lcd.begin(16, 2);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);

  int basilanButon = ButonSecim();
  state = Baslangic;

}

void loop() {
  state();
  delay(10);
}

void Baslangic() {

  for (int i = 0; i < 4; i++) {

    strcpy_P(buffer, (char*)pgm_read_word(&(baslangicText[i])));
    Serial.print(buffer);
    Serial.print(i);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    delay(3000);
    lcd.clear();

  }

  int basilanButon = ButonSecim();
  lcd.clear(); lcd.setCursor(0, 0);
  boolean basilanButonSecimi = 0;

  switch (basilanButon) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();


      basilanButonSecimi = 1;
      state = IsikOdasiDevam;
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();
      
      basilanButonSecimi = 1;
      state = SicaklikOdasi;
      break;

  }
  if (basilanButonSecimi == 1) {
    int basilanButon = ButonSecim();
  }
}


//
//  Işık Odası
//
void IsikOdasiDevam() {
  lcd.clear();
  if (isikVarmi == 0) {
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(isikOdasiBaslangicText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(isikOdasiDevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(isikOdasiSonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    int ldrSensorDegeri = analogRead(LDR_PIN);
    Serial.print(ldrSensorDegeri);
    if (ldrSensorDegeri > 100) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Isik Yandi.");
      lcd.setCursor(0, 1);;
      lcd.print("Devam Et");
      delay(1000);
      int basilanButon = ButonSecim();
      lcd.clear();
      lcd.setCursor(0, 0);
      boolean basilanButonSecimi = 0;
      switch (basilanButon) {
        case 1:
          state = TitresimOdasiSolYol;
          break;
        case 2:
          state = DumanOdasi;
          break;
      }
    }
  }
}

//
// Sıcaklık Odası Devam
//
void SicaklikOdasi() {
  lcd.clear();
  if (sicaklikDurum == 0) {
    for (int i = 0; i < 4; i++) {
        strcpy_P(buffer, (char*)pgm_read_word(&(sicaklikOdasiBaslangicText[i])));
        lcd.setCursor(0, 0);
        lcd.print(buffer);
        delay(3000);
        lcd.clear();
      }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(sicaklikOdasiDevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(sicaklikOdasiSonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    int sicaklikSensorDegeri = sicaklikSensoru();
    Serial.print(sicaklikSensorDegeri);
    if (sicaklikSensorDegeri > 20) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Oda Isindi");
      lcd.setCursor(0, 1);;
      lcd.print("Devam Et");
      delay(2000);
      int basilanButon = ButonSecim();
      lcd.clear();
      lcd.setCursor(0, 0);
      boolean basilanButonSecimi = 0;
      switch (basilanButon) {
        case 1:
          state = SarmasikOdasi;
          break;
        case 2:
          state = TitresimOdasi;
          break;
      }
    }
  }
}


void SarmasikOdasi() {
  lcd.clear();
  if (atesDurum == 0) {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(atesOdasiBaslangicText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(atesOdasiDevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(1000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(atesOdasiSonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    int atesSensoru = atesAlgilayiciSensor();
    Serial.print(atesSensoru);
    if (atesSensoru  == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);;
      lcd.print("Sarmasiklar Yandi");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cok iyisin ^-^");
      delay(2000);
      int basilanButon = ButonSecim();
      lcd.clear();
      lcd.setCursor(0, 0);
      boolean basilanButonSecimi = 0;
      switch (basilanButon) {
        case 1:
          state = IsikOdasiDevam;
          break;
        case 2:
          state = TitresimOdasi;
          break;
      }
    }
  }
}

void TitresimOdasi() {
  lcd.clear();
  if (titresimDurum == 0) {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(tiresimOdasiBaslangicText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(tiresimOdasiDevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(tiresimOdasiSonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    int titresimSensoru = titresimSensor();
    Serial.println(titresimSensoru);
    if (titresimSensoru == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);;
      lcd.print("Golemi Dusurdun");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cok iyisin ^-^");
      delay(2000);
      int basilanButon = ButonSecim();
      lcd.clear();
      lcd.setCursor(0, 0);
      boolean basilanButonSecimi = 0;
      switch (basilanButon) {
        case 1:
          state = SarmasikOdasi;
          break;
        case 2:
          state = OlumOdasi;
          break;
      }
    }
  }
}

void OlumOdasi() {

  lcd.clear();
  if (olumDurumu == 0) {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(olumOdasiBaslangicText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(olumOdasiDevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(olumOdasiSonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    int basilanButon = ButonSecim();
    lcd.clear();
    lcd.setCursor(0, 0);
    boolean basilanButonSecimi = 0;
    switch (basilanButon) {
      case 1:
        state = Baslangic;
        break;
      case 2:
        state = Baslangic;
        break;
    }

  }

}


void TitresimOdasiSolYol() {
  lcd.clear();
  if (titresimDurum == 0) {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(sallantiOdasi2BaslangicText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(sallantiOdasi2DevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(sallantiOdasi2SonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    int titresimSensoru = titresimSensor();
    Serial.println(titresimSensoru);
    if (titresimSensoru == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);;
      lcd.print("Goblinlerden");
      delay(2000);
      lcd.setCursor(0, 1);;
      lcd.print("Kurtuldun");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Devam Et ^-^");
      delay(2000);
      int basilanButon = ButonSecim();
      lcd.clear();
      lcd.setCursor(0, 0);
      boolean basilanButonSecimi = 0;
      switch (basilanButon) {
        case 1:
          state = CikisOdasi;
          break;
        case 2:
          state = CikisOdasi;
          break;
      }
    }
  }

}

void CikisOdasi() {
  lcd.clear();
  if (cikisOdasiDurum == 0) {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(cikisOdasiBaslangicText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(cikisOdasiDevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(cikisOdasiSonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Oyunu Bitirdin");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tebrikler ^-^");
    int basilanButon = ButonSecim();
    lcd.clear();
    lcd.setCursor(0, 0);
    boolean basilanButonSecimi = 0;
    switch (basilanButon) {
      case 1:
        state = Baslangic;
        break;
      case 2:
        state = Baslangic;
        break;
    }
  }
}

void DumanOdasi() {
  lcd.clear();
  if (dumanDurum == 0) {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeni Odaya");
      lcd.setCursor(0, 1);
      lcd.print("Girdin");
      delay(2000);
      lcd.clear();
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(dumanOdasiBaslangicText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(dumanOdasiDevamText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    for (int i = 0; i < 4; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(dumanOdasiSonText[i])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      delay(3000);
      lcd.clear();
    }
    delay(2000);
    int dumanDeger = atesAlgilayiciSensor();
    Serial.println(dumanDeger);
    if (dumanDeger == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);;
      lcd.print("Dag Trolunden");
      delay(2000);
      lcd.setCursor(0, 1);;
      lcd.print("Kurtuldun");
      delay(2000);
      int basilanButon = ButonSecim();
      lcd.clear();
      lcd.setCursor(0, 0);
      boolean basilanButonSecimi = 0;
      switch (basilanButon) {
        case 1:
          state = TitresimOdasiSolYol;
          break;
        case 2:
          state = OlumOdasi;
          break;
      }
    }
  }
}



int ButonSecim() {

  int secilenButon = 0;
  while (secilenButon == 0) {
    int okunanButon1 = digitalRead(buton1);
    int okunanButon2 = digitalRead(buton2);

    if (okunanButon1 != sonButonDurumu1 ) {
      lastDebounce1 = millis();
    }
    if (okunanButon2 != sonButonDurumu2 ) {
      lastDebounce2 = millis();
    }

    //Buton 1
    if ((millis() - lastDebounce1) > 50) {
      if (okunanButon1 != butonDurumu1) {
        butonDurumu1 = okunanButon1;
        if (butonDurumu1 == LOW) {
          secilenButon = 1;
        }
      }
    }

    //Buton 2
    if ((millis() - lastDebounce2) > 50) {
      if (okunanButon2 != butonDurumu2) {
        butonDurumu2 = okunanButon2;
        if (butonDurumu2 == LOW) {
          secilenButon = 2;
        }
      }
    }

    sonButonDurumu1 = okunanButon1;
    sonButonDurumu2 = okunanButon2;
  }

  return secilenButon;
}



float sicaklikSensoru() {
  float degerOku = analogRead(TMP36_PIN);
  float olculenDeger = (degerOku / 1023) * 5000;
  float derece = olculenDeger / 10.0;
  return derece;
}

int atesAlgilayiciSensor() {
  int atesSensorDegeri = digitalRead(ATES_SENSOR_PIN);
  return atesSensorDegeri;
}

int titresimSensor() {
  int titresimSensorDegeri = digitalRead(TITRESIM_SENSOR_PIN);
  return titresimSensorDegeri;
}
