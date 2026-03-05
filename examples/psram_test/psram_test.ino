// PSRAM External Memory Test - APS6404L 8MB
// Arduino RP2350 with external QSPI PSRAM
// 
// Hardware connections:
// PSRAM_CS  -> GPIO0
// QSPI-0    -> GPIO47 (Data 0)
// QSPI-1    -> GPIO48 (Data 1) 
// QSPI-2    -> GPIO49 (Data 2)
// QSPI-3    -> GPIO50 (Data 3)
// QSPI-CLK  -> GPIO52 (Clock)
// FLASH_CS  -> (Flash CS on same bus)

#include <Arduino.h>
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"

// PSRAM Configuration
#define PSRAM_CS_PIN    0
#define QSPI_D0_PIN     47
#define QSPI_D1_PIN     48
#define QSPI_D2_PIN     49
#define QSPI_D3_PIN     50
#define QSPI_CLK_PIN    52
#define PSRAM_SIZE_MB   8
#define PSRAM_SIZE      (PSRAM_SIZE_MB * 1024 * 1024)

// QSPI PSRAM Commands (APS6404L)
#define PSRAM_CMD_READ          0x03
#define PSRAM_CMD_FAST_READ     0x0B
#define PSRAM_CMD_QUAD_READ     0xEB
#define PSRAM_CMD_WRITE         0x02
#define PSRAM_CMD_QUAD_WRITE    0x38
#define PSRAM_CMD_RESET_ENABLE  0x66
#define PSRAM_CMD_RESET         0x99
#define PSRAM_CMD_READ_ID       0x9F
#define PSRAM_CMD_ENTER_QUAD    0x35
#define PSRAM_CMD_EXIT_QUAD     0xF5

// Simple SPI wrapper for PSRAM control
class PSRAMChip {
private:
  uint8_t csPin;
  bool initialized;
  
  void select() {
    digitalWrite(csPin, LOW);
    delayMicroseconds(1);
  }
  
  void deselect() {
    delayMicroseconds(1);
    digitalWrite(csPin, HIGH);
  }
  
  uint8_t transfer(uint8_t data) {
    uint8_t result = 0;
    for (int i = 7; i >= 0; i--) {
      // Simple bit-bang for now - could use hardware SPI
      // This is a basic implementation
      result = (result << 1) | digitalRead(QSPI_D0_PIN);
    }
    return result;
  }
  
public:
  PSRAMChip(uint8_t cs) : csPin(cs), initialized(false) {}
  
  bool begin() {
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    
    delay(10);
    
    // Reset PSRAM
    select();
    transfer(PSRAM_CMD_RESET_ENABLE);
    deselect();
    delay(1);
    
    select();
    transfer(PSRAM_CMD_RESET);
    deselect();
    delay(10);
    
    // Read ID
    uint32_t id = readID();
    
    Serial.print("PSRAM ID: 0x");
    Serial.println(id, HEX);
    
    if (id == 0x0D5D52 || id == 0x5D0D52) {  // APS6404L ID variants
      initialized = true;
      Serial.println("PSRAM APS6404L detected!");
      return true;
    }
    
    Serial.println("PSRAM not detected");
    initialized = false;
    return false;
  }
  
  uint32_t readID() {
    select();
    transfer(PSRAM_CMD_READ_ID);
    transfer(0x00);  // Address byte 1
    transfer(0x00);  // Address byte 2
    transfer(0x00);  // Address byte 3
    
    uint32_t id = 0;
    id |= ((uint32_t)transfer(0xFF) << 16);
    id |= ((uint32_t)transfer(0xFF) << 8);
    id |= transfer(0xFF);
    
    deselect();
    return id;
  }
  
  void writeByte(uint32_t address, uint8_t data) {
    if (!initialized || address >= PSRAM_SIZE) return;
    
    select();
    transfer(PSRAM_CMD_WRITE);
    transfer((address >> 16) & 0xFF);
    transfer((address >> 8) & 0xFF);
    transfer(address & 0xFF);
    transfer(data);
    deselect();
  }
  
  uint8_t readByte(uint32_t address) {
    if (!initialized || address >= PSRAM_SIZE) return 0;
    
    select();
    transfer(PSRAM_CMD_READ);
    transfer((address >> 16) & 0xFF);
    transfer((address >> 8) & 0xFF);
    transfer(address & 0xFF);
    uint8_t data = transfer(0xFF);
    deselect();
    
    return data;
  }
  
  void writeBlock(uint32_t address, uint8_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
      writeByte(address + i, data[i]);
    }
  }
  
  void readBlock(uint32_t address, uint8_t* buffer, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
      buffer[i] = readByte(address + i);
    }
  }
  
  bool testMemory() {
    Serial.println("\nTesting PSRAM...");
    
    // Test 1: Write and read single bytes
    Serial.print("Test 1: Single byte R/W... ");
    writeByte(0x0000, 0xAA);
    writeByte(0x0001, 0x55);
    
    if (readByte(0x0000) == 0xAA && readByte(0x0001) == 0x55) {
      Serial.println("PASS");
    } else {
      Serial.println("FAIL");
      return false;
    }
    
    // Test 2: Pattern test
    Serial.print("Test 2: Pattern test... ");
    uint8_t pattern[] = {0x00, 0xFF, 0xAA, 0x55, 0x0F, 0xF0, 0x33, 0xCC};
    writeBlock(0x1000, pattern, 8);
    
    uint8_t readback[8];
    readBlock(0x1000, readback, 8);
    
    bool patternOk = true;
    for (int i = 0; i < 8; i++) {
      if (readback[i] != pattern[i]) {
        patternOk = false;
        break;
      }
    }
    
    if (patternOk) {
      Serial.println("PASS");
    } else {
      Serial.println("FAIL");
      return false;
    }
    
    // Test 3: Address test (scattered addresses)
    Serial.print("Test 3: Address test... ");
    writeByte(0x0000, 0x11);
    writeByte(0x1000, 0x22);
    writeByte(0x10000, 0x33);
    writeByte(0x100000, 0x44);
    
    if (readByte(0x0000) == 0x11 && 
        readByte(0x1000) == 0x22 && 
        readByte(0x10000) == 0x33 && 
        readByte(0x100000) == 0x44) {
      Serial.println("PASS");
    } else {
      Serial.println("FAIL");
      return false;
    }
    
    // Test 4: Large block test
    Serial.print("Test 4: Large block (1KB)... ");
    uint8_t largeBuffer[1024];
    for (int i = 0; i < 1024; i++) {
      largeBuffer[i] = i & 0xFF;
    }
    
    writeBlock(0x2000, largeBuffer, 1024);
    
    uint8_t verifyBuffer[1024];
    readBlock(0x2000, verifyBuffer, 1024);
    
    bool blockOk = true;
    for (int i = 0; i < 1024; i++) {
      if (verifyBuffer[i] != (i & 0xFF)) {
        blockOk = false;
        break;
      }
    }
    
    if (blockOk) {
      Serial.println("PASS");
    } else {
      Serial.println("FAIL");
      return false;
    }
    
    Serial.println("\nAll PSRAM tests PASSED!");
    return true;
  }
  
  void benchmark() {
    Serial.println("\n--- PSRAM Benchmark ---");
    
    const uint32_t testSize = 4096;  // 4KB test
    uint8_t testData[256];
    
    // Fill test data
    for (int i = 0; i < 256; i++) {
      testData[i] = i;
    }
    
    // Write speed test
    unsigned long startTime = micros();
    for (uint32_t addr = 0; addr < testSize; addr += 256) {
      writeBlock(addr, testData, 256);
    }
    unsigned long writeTime = micros() - startTime;
    
    float writeSpeed = (testSize / 1024.0) / (writeTime / 1000000.0);
    Serial.print("Write speed: ");
    Serial.print(writeSpeed);
    Serial.println(" KB/s");
    
    // Read speed test
    startTime = micros();
    for (uint32_t addr = 0; addr < testSize; addr += 256) {
      readBlock(addr, testData, 256);
    }
    unsigned long readTime = micros() - startTime;
    
    float readSpeed = (testSize / 1024.0) / (readTime / 1000000.0);
    Serial.print("Read speed: ");
    Serial.print(readSpeed);
    Serial.println(" KB/s");
  }
  
  void dumpMemory(uint32_t address, uint32_t length) {
    Serial.print("\nMemory dump at 0x");
    Serial.print(address, HEX);
    Serial.print(" (");
    Serial.print(length);
    Serial.println(" bytes):");
    
    for (uint32_t i = 0; i < length; i++) {
      if (i % 16 == 0) {
        Serial.println();
        Serial.print("0x");
        if (address + i < 0x10000) Serial.print("0");
        if (address + i < 0x1000) Serial.print("0");
        if (address + i < 0x100) Serial.print("0");
        if (address + i < 0x10) Serial.print("0");
        Serial.print(address + i, HEX);
        Serial.print(": ");
      }
      
      uint8_t data = readByte(address + i);
      if (data < 0x10) Serial.print("0");
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
};

PSRAMChip psramChip(PSRAM_CS_PIN);

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.println("\n=== PSRAM APS6404L Test ===");
  Serial.println("8MB External QSPI PSRAM");
  Serial.println("==========================\n");
  
  Serial.print("Initializing PSRAM on CS pin ");
  Serial.print(PSRAM_CS_PIN);
  Serial.println("...");
  
  if (psramChip.begin()) {
    digitalWrite(LED_BUILTIN, HIGH);
    
    // Run tests
    psramChip.testMemory();
    
    // Run benchmark
    psramChip.benchmark();
    
    // Write some test data
    Serial.println("\nWriting test pattern...");
    const char* testString = "Hello PSRAM! APS6404L 8MB Memory Test";
    psramChip.writeBlock(0x0000, (uint8_t*)testString, strlen(testString));
    
    // Dump memory
    psramChip.dumpMemory(0x0000, 64);
    
    Serial.println("\n\nPSRAM ready for use!");
    Serial.println("Available memory: 8 MB (8,388,608 bytes)");
    Serial.println("Address range: 0x000000 - 0x7FFFFF");
    
  } else {
    Serial.println("ERROR: PSRAM initialization failed!");
    
    // Blink LED to indicate error
    while (true) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(250);
    }
  }
}

void loop() {
  // Interactive test via Serial
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd == "test") {
      psramChip.testMemory();
    } else if (cmd == "bench") {
      psramChip.benchmark();
    } else if (cmd.startsWith("dump ")) {
      uint32_t addr = strtoul(cmd.substring(5).c_str(), NULL, 16);
      psramChip.dumpMemory(addr, 64);
    } else if (cmd.startsWith("write ")) {
      int spaceIdx = cmd.indexOf(' ', 6);
      if (spaceIdx > 0) {
        uint32_t addr = strtoul(cmd.substring(6, spaceIdx).c_str(), NULL, 16);
        String data = cmd.substring(spaceIdx + 1);
        psramChip.writeBlock(addr, (uint8_t*)data.c_str(), data.length());
        Serial.println("Written");
      }
    } else if (cmd == "help") {
      Serial.println("\nAvailable commands:");
      Serial.println("  test       - Run memory tests");
      Serial.println("  bench      - Run benchmark");
      Serial.println("  dump ADDR  - Dump 64 bytes from address (hex)");
      Serial.println("  write ADDR DATA - Write string to address");
      Serial.println("  help       - Show this help");
    } else {
      Serial.println("Unknown command. Type 'help' for commands.");
    }
  }
  
  // Blink LED to show activity
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 1000) {
    lastBlink = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
