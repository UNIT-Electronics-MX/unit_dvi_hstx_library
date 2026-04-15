# 🔧 Solución de Problemas - DVI/HDMI no funciona

## Configuración de Pines RP2350

Tu configuración de pines es:

```
GPIO    HDMI Signal         Descripción
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
GPIO14  TMDS Clock+         ✓ Usado
GPIO15  TMDS Clock−         (par diferencial)
GPIO12  TMDS Data2+  (Red)  ✓ Usado
GPIO13  TMDS Data2−         (par diferencial)
GPIO16  TMDS Data1+ (Green) ✓ Usado
GPIO17  TMDS Data1−         (par diferencial)
GPIO18  TMDS Data0+ (Blue)  ✓ Usado
GPIO19  TMDS Data0−         (par diferencial)
```

### Código correcto:

```cpp
DVHSTXPinout pinConfig = {14, 18, 16, 12};
// {Clock+, Data0+, Data1+, Data2+}
// {GPIO14, GPIO18, GPIO16, GPIO12}
```

O usando la macro:

```cpp
DVHSTXPinout pinConfig = RP2350_STANDARD_DVI_CFG;
```

## ✅ Checklist de Diagnóstico

Sigue estos pasos en orden:

### 1. ⚙️ Configuración de Arduino IDE

```
✓ Board: Raspberry Pi Pico 2 (o Arduino Nano RP2350 Connect)
✓ CPU Speed: 150 MHz  ← MUY IMPORTANTE
✓ Optimize: -O3 (Optimize Even More)
✓ USB Stack: Pico SDK
✓ Flash Size: 4MB (no FS)
```

**SI CPU SPEED NO ESTÁ EN 150MHz, NO FUNCIONARÁ**

### 2. 🔌 Hardware

```
✓ Cable HDMI conectado y funcional
✓ Monitor/TV encendido y en el canal correcto
✓ Conexiones de pines correctas (GPIO14, 18, 16, 12)
✓ RP2350 alimentado correctamente (USB o externa)
✓ Cable HDMI de buena calidad (cables muy largos pueden fallar)
```

### 3. 📝 Software

```
✓ Biblioteca Adafruit GFX instalada
✓ Core RP2350 actualizado (versión 4.0.0+)
✓ F_CPU se verifica en el código
```

### 4. 🧪 Test

1. **Abre el ejemplo de diagnóstico**:
   ```
   File → Examples → Adafruit-DVI-HSTX → diagnostico_pines
   ```

2. **Compila y sube**

3. **Abre Serial Monitor (115200 baud)**

4. **Observa**:
   - **LED parpadea RÁPIDO** = Error, no inicializó
   - **LED parpadea LENTO** = Funcionando OK

## 🔍 Diagnóstico por Síntomas

### Síntoma: LED parpadea rápido, Serial dice "ERROR"

**Causa probable**: CPU Speed incorrecto

**Solución**:
1. Ve a `Tools → CPU Speed → 150 MHz`
2. Recompila y sube

---

### Síntoma: Compila pero pantalla negra, LED parpadea lento

**Causa probable**: Conexiones de hardware

**Solución**:
1. Verifica con multímetro continuidad en las conexiones
2. Prueba otro cable HDMI
3. Prueba otro monitor
4. Verifica que los pines sean exactamente GPIO14, 18, 16, 12

---

### Síntoma: Error de compilación "Adafruit_GFX.h not found"

**Solución**:
```
Tools → Manage Libraries → Buscar "Adafruit GFX" → Install
```

---

### Síntoma: Pantalla con interferencia o colores raros

**Causa probable**: Cable HDMI de mala calidad o muy largo

**Solución**:
1. Usa cable HDMI de menos de 2 metros
2. Prueba cable certificado HDMI High Speed
3. Verifica que no haya interferencia electromagnética cerca

---

### Síntoma: Funciona en algunos monitores pero no en otros

**Causa probable**: Monitor no soporta la resolución/frecuencia

**Solución**:
```cpp
// Prueba diferentes resoluciones
DVHSTX_RESOLUTION_320x240  // Más compatible
DVHSTX_RESOLUTION_640x480  // Standard VGA
```

## 🧪 Test Manual de Pines

Si nada funciona, verifica los pines manualmente:

```cpp
void setup() {
  Serial.begin(115200);
  
  // Configurar pines como salida
  pinMode(14, OUTPUT);  // Clock+
  pinMode(18, OUTPUT);  // Data0+
  pinMode(16, OUTPUT);  // Data1+
  pinMode(12, OUTPUT);  // Data2+
  
  Serial.println("Pines configurados");
  Serial.println("Deberías poder medirlos con multímetro");
}

void loop() {
  // Toggle todos los pines
  digitalWrite(14, !digitalRead(14));
  digitalWrite(18, !digitalRead(18));
  digitalWrite(16, !digitalRead(16));
  digitalWrite(12, !digitalRead(12));
  delay(500);
}
```

Con un LED + resistencia o multímetro, verifica que los pines GPIO14, 18, 16, 12 estén parpadeando.

## 📊 Código de Test Mínimo

Si quieres el código más simple posible:

```cpp
#include <udvi_hstx.h>

DVHSTXPinout pinConfig = {14, 18, 16, 12};
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_320x240);

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("Iniciando...");
  
  if (!display.begin()) {
    Serial.println("ERROR!");
    while(1);
  }
  
  Serial.println("OK!");
  display.fillScreen(0xF800); // Rojo
}

void loop() {
  // Nada
}
```

**Si esto no funciona**:
1. Verifica CPU Speed = 150 MHz
2. Verifica cable HDMI
3. Verifica monitor encendido
4. Verifica que sea realmente un RP2350 (no RP2040)

## 🆘 Último Recurso

Si nada de esto funciona:

1. **Verifica que sea RP2350**: El RP2040 NO tiene HSTX, no funcionará.

2. **Actualiza el Core**:
   ```
   Tools → Board → Boards Manager
   Busca "RP2040/RP2350" → Update a última versión
   ```

3. **Prueba con Pico 2 oficial**: Si tienes una Raspberry Pi Pico 2, prueba ahí primero para descartar problemas de hardware custom.

4. **Revisa el Serial Monitor**: Siempre tiene pistas importantes.

## 📞 Información para Soporte

Si pides ayuda, incluye:

```
1. Modelo exacto de placa (ej: Raspberry Pi Pico 2)
2. Salida completa del Serial Monitor
3. Settings de Arduino IDE (Board, CPU Speed, etc)
4. Modelo de monitor/TV
5. Longitud de cable HDMI
6. ¿LED parpadea rápido o lento?
```

## ✨ Una vez que funcione

Si ya funciona, prueba los demos:
- `graphics_demo` - Efectos visuales
- `tron_mountains` - Paisaje 3D
- `cube_3d_demo` - Cubo 3D rotatorio
- `snake_game` - Juego de Snake

---

**Recuerda**: El 90% de los problemas son por CPU Speed incorrecto (debe ser 150 MHz).
