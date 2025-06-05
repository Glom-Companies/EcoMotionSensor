/***************************************
 *  Test 1 : Gyroscope + Accéléromètre 
 *  (MPU-6050) + Afficheur LCD 16×2
 *  
 *  Objectif : 
 *   - Lire les données brutes du MPU-6050 
 *     (accélération X/Y/Z et gyro X/Y/Z). 
 *   - Calculer l’orientation (G/D/Haut/Bas/Stable). 
 *   - Estimer l’accélération (en m/s²). 
 *   - Afficher le tout en temps réel sur le LCD.
 *  
 *  Matériel :
 *   - Arduino Uno
 *   - Module MPU-6050 (I2C)
 *   - Écran LCD 16×2 (HD44780) en mode 4 bits
 *   - Potentiomètre 10 kΩ pour le contraste du LCD
 *   - Régulateur 7805 + 4×AA pour produire du 5 V
 *   - Résistances de pull-up 4,7 kΩ pour SDA/SCL si nécessaire
 *  
 *  Auteur : Electronique, IFRI, TRC2K25 
 *  Date : juin 2025
 **************************************/

#include <Wire.h>           // Bibliothèque pour I2C (communication MCU ↔ MPU-6050)
#include <I2Cdev.h>         // Bibliothèque générique I2Cdevlib
#include <MPU6050.h>        // Bibliothèque spécifique MPU-6050 (I2Cdevlib)
#include <LiquidCrystal.h>  // Bibliothèque pour écran LCD HD44780


// ------------ Déclaration des objets ---------------
// Création de l’objet "mpu" qui représentera le capteur
MPU6050 mpu;

// Création de l'objet "lcd" pour piloter l'écran LCD 16x2
//              (RS, E, D4, D5, D6, D7)
// Borniers de l'Arduino :
//    RS → D12  
//    E  → D11  
//    D4 → D5  
//    D5 → D4  
//    D6 → D3  
//    D7 → D2
//s


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
