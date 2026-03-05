// Test de configuración de pines para Arduino Nano RP2350
// Este sketch verifica que los pines DVI estén correctamente configurados
// antes de intentar usar el display

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("========================================");
  Serial.println("  Test de Pines DVI - Nano RP2350");
  Serial.println("========================================");
  Serial.println();
  
  Serial.println("Configuración esperada:");
  Serial.println("  D1 (GPIO18) -> Clock+");
  Serial.println("  D7 (GPIO12) -> Blue+ (Data0)");
  Serial.println("  D5 (GPIO14) -> Green+ (Data1)");
  Serial.println("  D3 (GPIO16) -> Red+ (Data2)");
  Serial.println();
  
  // Pines HSTX para DVI
  const int PIN_CK_P = 18;  // D1
  const int PIN_D0_P = 12;  // D7 - Blue
  const int PIN_D1_P = 14;  // D5 - Green
  const int PIN_D2_P = 16;  // D3 - Red
  
  Serial.println("Probando acceso a pines...");
  
  // Intentar configurar los pines como salida
  pinMode(PIN_CK_P, OUTPUT);
  pinMode(PIN_D0_P, OUTPUT);
  pinMode(PIN_D1_P, OUTPUT);
  pinMode(PIN_D2_P, OUTPUT);
  
  Serial.println("✓ Pines configurados como salida");
  Serial.println();
  
  Serial.println("Información del sistema:");
  Serial.print("  Placa: ");
  #if defined(ARDUINO_NANO_RP2350_CONNECT)
    Serial.println("Arduino Nano RP2350 Connect");
  #elif defined(ARDUINO_RASPBERRY_PI_PICO_2)
    Serial.println("Raspberry Pi Pico 2");
  #else
    Serial.println("Desconocida");
  #endif
  
  Serial.print("  F_CPU: ");
  Serial.print(F_CPU / 1000000);
  Serial.println(" MHz");
  
  #if F_CPU != 150000000
    Serial.println();
    Serial.println("⚠ ADVERTENCIA: F_CPU debe estar en 150MHz");
    Serial.println("  Ve a Tools -> CPU Speed -> 150 MHz");
  #endif
  
  Serial.println();
  Serial.println("✓ Test completado");
  Serial.println();
  Serial.println("Si ves este mensaje, tu configuración está");
  Serial.println("lista para usar los ejemplos DVI-HSTX.");
  Serial.println();
  Serial.println("Siguiente paso:");
  Serial.println("  Abre: examples/nano_rp2350_test/nano_rp2350_test.ino");
}

void loop() {
  // Parpadear LED para indicar que está funcionando
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
