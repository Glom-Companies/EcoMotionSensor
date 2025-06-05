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

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// ------------ Variables globales pour la calibration --------------
// Offsets calculés au démarrage (biais à 0 ) pour le gyroscope
float gyroOffsetX = 0;
float gyroOffsetY = 0;
float gyroOffsetZ = 0;

// Offsets pour l’accéléromètre (optionnel, on se base surtout sur la norme vect.)
// On peut aussi ajuster l’échelle si on veut un 1g très exact
float accelOffsetX = 0;
float accelOffsetY = 0;
float accelOffsetZ = 0;

// Sensibilités selon la plage choisie
const float ACCEL_SENSITIVITY = 16384.0; // ±2 g → 16384 LSB/g
const float GYRO_SENSITIVITY = 131.0;    // ±250 °/s → 131 LSB/(°/s)


//--------------- Fonction de calibration du Gyroscope ---------------
// Mesure le biais du gyro pendant "samples" échantillons
void calibrateGyro(int samples = 500) {
  long sumX = 0, sumY = 0, sumZ = 0;
  int16_t gx, gy, gz, ax, ay, az;

   for (int i; i < samples; i++){
      // Lire les 6 valeurs brutes (ici, seul le gyro nous intéresse) 
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      sumX += gx;
      sumY += gy;
      sumZ += gz;
      delay(5); // petite pause pour ne pas saturer l'I2C (≈100 Hz)
   }
   // On fait la moyenne : on suppose que le capteur est parfaitement immobile
   gyroOffsetX = float(sumX)/samples;
   gyroOffsetY = float(sumY)/samples;
   gyroOffsetZ = float(sumZ)/samples;

}

// ---------- Initialisation (setup) ----------
void setup() {
  // 1. Initialisation du port série pour debug (9600 bauds)
  Serial.begin(9600); 
  while (!Serial) {/* attendre l'ouverture du port série si besoin */}

  // 2. Initialisation I2C (Wire) et du capteur
  Wire.begin();     // Configure l'Arduino en tant que maître I2C
  mpu.initialize(); //Initialise le MPU-6050 (commence la communication)

  // 3. Vérifier la connexion avec le MPU-6050
  if (!mpu.testConnection()) {
    // Si la communication avec le MPU-6050 est impossible, on bloque
    Serial.println("Erreur : impossible de connecter au MPU-6050");
    lcd.begin(16, 2);
    lcd.print("MPU6050 ERROR");
    while(true) {
      // Boucle infinie : on arrête tout
    }
  }
  Serial.println(F("MPU-6050 détecté avec suucès"));

  // 4. Configuration des plages de mesure pour plus de précision
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);  // ±2 g → résolution 16384 LSB/g
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250); // ±250 °/s → résolution 131 LSB/(°/s)

  // 5. Configuration du filtre passe-bas (DLPF) pour réduire le bruit
  // Options possibles (voir datasheet) : 
  //    0 → 260 Hz (gyro), 256 Hz (accel)  
  //    1 → 188 Hz, 188 Hz  
  //    2 → 98 Hz, 98 Hz  
  //    3 → 42 Hz, 42 Hz  
  //    4 → 20 Hz, 20 Hz  
  // Ici on choisit 42 Hz pour un bon compromis bruit/réactivité
  mpu.setDLPFMode(MPU6050_DLPF_BW_42);

  // 6. Calibrer le gyroscope (mesure et soustraction du bias)
  Serial.println(F("Debut calibration gyroscope..."));
  calibrateGyro(500); // 500 échantillons à ≈100 Hz = 5 s
  Serial.print(F("Gyro offsets: X= "));
  Serial.print(gyroOffsetX);
  Serial.print(F("  Y="));
  Serial.print(gyroOffsetY);
  Serial.print(F("  Z="));
  Serial.println(gyroOffsetZ);

  // 7. Initialisation de l'écran LCD
  lcd.begin(16, 2);
  lcd.print("Initialisation");
  delay(1000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly: 

}
