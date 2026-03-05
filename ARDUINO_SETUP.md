# Compilar con Arduino IDE - Arduino Nano RP2350

Este documento explica cómo compilar los ejemplos DVI-HSTX usando Arduino IDE para el Arduino Nano RP2350.

## Configuración de Pines

| Arduino Pin | GPIO | HDMI Signal  | Función       |
|-------------|------|--------------|---------------|
| D1          | 18   | Clock+       | TMDS Clock    |
| D7          | 12   | Blue+ (D0)   | TMDS Data0    |
| D5          | 14   | Green+ (D1)  | TMDS Data1    |
| D3          | 16   | Red+ (D2)    | TMDS Data2    |

## Instalación

### 1. Instalar Arduino IDE

Si no lo tienes instalado:
```bash
# En Linux (Ubuntu/Debian)
sudo snap install arduino

# O descarga desde: https://www.arduino.cc/en/software
```

### 2. Instalar soporte para Arduino Nano RP2350

1. Abre Arduino IDE
2. Ve a **Archivo → Preferencias**
3. En "Gestor de URLs Adicionales de Tarjetas", agrega:
   ```
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```
4. Ve a **Herramientas → Placa → Gestor de tarjetas**
5. Busca "**Raspberry Pi Pico/RP2040/RP2350**" por Earle F. Philhower
6. Instala la última versión

### 3. Instalar la biblioteca Adafruit GFX

1. Ve a **Herramientas → Administrar bibliotecas**
2. Busca "**Adafruit GFX Library**"
3. Instala la última versión

### 4. Instalar esta biblioteca

Copia toda la carpeta `Adafruit-DVI-HSTX` a tu carpeta de bibliotecas de Arduino:

```bash
# Linux
cp -r /media/mr/firmware/personal/pico/Adafruit-DVI-HSTX ~/Arduino/libraries/

# O desde Arduino IDE:
# Sketch → Include Library → Add .ZIP Library
# Y selecciona la carpeta del proyecto
```

## Configuración de la Placa

En Arduino IDE, configura:

1. **Placa**: `Tools → Board → Raspberry Pi RP2040/RP2350 Boards → Arduino Nano RP2350 Connect`
   - Si no aparece "Arduino Nano RP2350 Connect", usa "Raspberry Pi Pico 2"

2. **CPU Speed**: `Tools → CPU Speed → 150 MHz`
   - **IMPORTANTE**: Aunque el código sobrescribe esto a 264MHz internamente, el menú debe estar en 150MHz

3. **Optimize**: `Tools → Optimize → Optimize Even More (-O3)`

4. **USB Stack**: `Tools → USB Stack → Pico SDK`

5. **Puerto**: `Tools → Port → /dev/ttyACM0` (o el puerto que corresponda)

## Compilar y Subir

### Ejemplo recomendado: nano_rp2350_test

Este ejemplo ya tiene los pines configurados correctamente:

1. Abre `examples/nano_rp2350_test/nano_rp2350_test.ino`

2. Verifica la configuración de pines en el código:
   ```cpp
   DVHSTXPinout pinConfig = {18, 12, 14, 16};
   // {Clock+, Blue+, Green+, Red+}
   // {GPIO18, GPIO12, GPIO14, GPIO16}
   ```

3. Haz clic en **Verificar** (✓) para compilar

4. Conecta tu Arduino Nano RP2350 por USB

5. Haz clic en **Subir** (→) para flashear

### Usar otros ejemplos

Para usar `00simpletest`, `01palettetest`, o `02texttest`:

Estos ejemplos intentan detectar automáticamente la placa. Si usas Arduino Nano RP2350 Connect, deberían funcionar automáticamente.

Si no funciona, puedes forzar la configuración comentando las líneas de detección y usando directamente:
```cpp
DVHSTXPinout pinConfig = {18, 12, 14, 16};
```

## Monitor Serial

Para ver la salida de debug:

1. Sube el código
2. Abre `Tools → Serial Monitor`
3. Configura a **115200 baud**
4. Verás mensajes como:
   ```
   ========================================
     DVI-HSTX Arduino Nano RP2350
   ========================================
   
   Configuración de pines:
     Clock+ : GPIO18 (D1)
     Blue+  : GPIO12 (D7) - Data0
     Green+ : GPIO14 (D5) - Data1
     Red+   : GPIO16 (D3) - Data2
   
   ✓ Display inicializado correctamente!
   Resolución: 320x240
   ```

## Solución de Problemas

### Error: "DVHSTX_PINOUT_DEFAULT was not declared"

Usa la configuración explícita de pines:
```cpp
DVHSTXPinout pinConfig = {18, 12, 14, 16};
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_320x240);
```

### Error: "Adafruit_GFX.h: No such file or directory"

Instala la biblioteca Adafruit GFX desde el gestor de bibliotecas.

### LED parpadeando después de subir

El display no se inicializó. Verifica:
- Conexiones de hardware (pines D1, D3, D5, D7)
- Que tengas un cable/adaptador HDMI conectado
- Monitor serial para ver el mensaje de error específico

### Pantalla negra

- Verifica que el cable HDMI esté bien conectado
- Prueba con otro monitor/TV
- Verifica que los pines estén correctamente conectados
- Asegúrate de que el monitor soporte la resolución 640x480@60Hz (señal base)

### Error de compilación sobre F_CPU

Asegúrate de tener seleccionado `CPU Speed: 150 MHz` en las herramientas.

## Cambiar Resolución

Puedes cambiar la resolución editando el código:

```cpp
// Opciones disponibles:
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_320x180);  // 16:9
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_640x360);  // 16:9 (usa más RAM)
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_320x240);  // 4:3
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_640x480);  // 4:3 (usa más RAM)
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_400x300);  // 4:3
```

**Nota**: Resoluciones más altas requieren más RAM. El RP2350 tiene aproximadamente 520KB de RAM disponible.

## Uso de memoria

- **320x240 RGB565**: ~150 KB
- **640x360 RGB565**: ~450 KB
- **640x480 RGB565**: ~600 KB (puede no caber en RAM)

Si ves el LED parpadeando, es posible que no haya suficiente RAM para la resolución seleccionada.

## Siguiente paso

Una vez que funcione el ejemplo básico, puedes usar todas las funciones de Adafruit GFX:

```cpp
display.fillScreen(0x0000);                    // Negro
display.drawRect(10, 10, 100, 100, 0xFFFF);    // Rectángulo blanco
display.fillCircle(50, 50, 20, 0xF800);        // Círculo rojo
display.setCursor(10, 10);
display.setTextColor(0xFFFF);
display.print("Hello HDMI!");
```

Ver la documentación de Adafruit GFX para más funciones: https://learn.adafruit.com/adafruit-gfx-graphics-library
