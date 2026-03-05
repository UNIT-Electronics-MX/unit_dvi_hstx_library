# Problemas Conocidos - Modo Texto

## DVHSTXText no funciona correctamente

### Problema
El modo texto nativo `DVHSTXText` del ejemplo `02texttest` no funciona en la configuración actual del Arduino Nano RP2350. El display se inicializa pero no muestra texto.

### Causa
Posibles causas:
- Incompatibilidad de memoria con la resolución de texto nativa
- Problema con el driver de texto en la versión actual de la librería
- Conflicto con la configuración de pines personalizada

### Solución: Usar Modo Gráfico (DVHSTX16) con Adafruit_GFX

En lugar de usar `DVHSTXText`, utiliza `DVHSTX16` con las funciones de texto de Adafruit_GFX:

```cpp
// NO FUNCIONA:
DVHSTXPinout pinConfig = {18, 12, 14, 16};
DVHSTXText display(pinConfig);  // ❌ No muestra texto

// SÍ FUNCIONA:
DVHSTXPinout pinConfig = {18, 12, 14, 16};
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_640x480);  // ✅ Funciona bien

void setup() {
  display.begin();
  display.setTextSize(2);
  display.setTextColor(0xFFFF);  // Blanco
  display.setCursor(10, 10);
  display.println("Hola Mundo!");
}
```

### Ejemplos Funcionales con Texto

#### Ejemplos que SÍ funcionan (usan DVHSTX16 + GFX):
- ✅ `03texttest_gfx` - Test de texto completo usando Adafruit_GFX
- ✅ `text_graphics_demo` - Dashboard con texto y gráficos
- ✅ `graphics_demo` - Incluye labels de texto
- ✅ `colour_bars` - Texto de diagnóstico
- ✅ Todos los demos 3D con texto overlay

#### Ejemplos que NO funcionan:
- ❌ `02texttest` - Usa DVHSTXText (no muestra nada)

### Funciones de Texto Disponibles con Adafruit_GFX

```cpp
// Configuración
display.setTextSize(size);        // 1-4 (tamaño)
display.setTextColor(color);      // RGB565
display.setTextColor(fg, bg);     // Con color de fondo
display.setTextWrap(true/false);  // Wrap automático
display.setCursor(x, y);          // Posición

// Escritura
display.print("texto");           // Sin salto de línea
display.println("texto");         // Con salto de línea
display.printf("num=%d", val);    // Formato (si soportado)
display.write('c');               // Un caracter

// Ejemplos
display.setTextSize(2);
display.setTextColor(0xF800);     // Rojo
display.setCursor(10, 10);
display.println("RP2350");

// Colores RGB565
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define WHITE   0xFFFF
```

### Tamaños de Fuente

| Tamaño | Ancho | Alto | Caracteres por línea (640px) | Líneas (480px) |
|--------|-------|------|------------------------------|----------------|
| 1      | 6px   | 8px  | ~106 caracteres              | 60 líneas      |
| 2      | 12px  | 16px | ~53 caracteres               | 30 líneas      |
| 3      | 18px  | 24px | ~35 caracteres               | 20 líneas      |
| 4      | 24px  | 32px | ~26 caracteres               | 15 líneas      |

### Ejemplo Completo de Texto Multi-Color

```cpp
#include <Adafruit_dvhstx.h>

DVHSTXPinout pinConfig = {18, 12, 14, 16};
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_640x480);

void setup() {
  display.begin();
  display.fillScreen(0x0000);  // Negro
  
  // Título grande
  display.setTextSize(3);
  display.setTextColor(0xFFE0);  // Amarillo
  display.setCursor(10, 10);
  display.println("RP2350 HDMI");
  
  // Subtítulo
  display.setTextSize(2);
  display.setTextColor(0x07FF);  // Cyan
  display.println();
  display.println("DVI Output Test");
  
  // Información detallada
  display.setTextSize(1);
  display.setTextColor(0xFFFF);  // Blanco
  display.println();
  display.println("Pin Configuration:");
  
  display.setTextColor(0xF800);  // Rojo
  display.println("  D3 (GPIO16) = Red");
  
  display.setTextColor(0x07E0);  // Verde
  display.println("  D5 (GPIO14) = Green");
  
  display.setTextColor(0x001F);  // Azul
  display.println("  D7 (GPIO12) = Blue");
  
  display.setTextColor(0xFFFF);  // Blanco
  display.println("  D1 (GPIO18) = Clock");
}

void loop() {
  // Texto dinámico
  static int counter = 0;
  
  display.fillRect(10, 200, 200, 20, 0x0000);  // Borrar área
  display.setCursor(10, 200);
  display.setTextSize(2);
  display.setTextColor(0x07E0);  // Verde
  display.print("Contador: ");
  display.println(counter++);
  
  delay(1000);
}
```

### Recomendaciones

1. **Siempre usa DVHSTX16** para texto, no DVHSTXText
2. **Usa 640x480** para mejor calidad de texto
3. **setTextSize(1)** da mejor legibilidad para texto pequeño
4. **setTextSize(2)** es ideal para títulos y menús
5. **fillRect antes de escribir** para actualizar texto dinámico sin parpadeos

### Si necesitas actualizar texto frecuentemente

```cpp
// Buena práctica: borrar solo el área necesaria
void updateCounter(int value) {
  // Borrar área anterior
  display.fillRect(100, 50, 150, 30, 0x0000);
  
  // Escribir nuevo valor
  display.setCursor(100, 50);
  display.setTextSize(2);
  display.setTextColor(0xFFFF);
  display.print("Count: ");
  display.println(value);
}
```

### Debugging de Problemas de Texto

Si el texto no se ve:
1. ✓ Verifica que `display.begin()` retorne `true`
2. ✓ Usa `fillScreen(0x0000)` antes de escribir
3. ✓ Asegúrate de usar colores visibles (no negro sobre negro)
4. ✓ Verifica que setCursor esté dentro de la pantalla
5. ✓ Prueba con `setTextSize(3)` para texto más grande
6. ✓ Usa Serial.println() para confirmar que el código se ejecuta

```cpp
void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("Inicializando display...");
  if (!display.begin()) {
    Serial.println("ERROR: begin() falló!");
    while(1);
  }
  Serial.println("Display OK");
  
  display.fillScreen(0x0000);
  Serial.println("fillScreen OK");
  
  display.setTextSize(3);
  display.setTextColor(0xFFFF);
  display.setCursor(10, 10);
  Serial.println("Escribiendo texto...");
  
  display.println("PRUEBA");
  Serial.println("Texto escrito");
}
```

## Resumen

- ❌ **No usar**: `DVHSTXText` (no funciona)
- ✅ **Usar**: `DVHSTX16` + funciones de texto de Adafruit_GFX
- ✅ **Ejemplo funcional**: `03texttest_gfx`
- ✅ **Resolución recomendada**: 640x480 para mejor texto
- ✅ **Todos los otros ejemplos funcionan**: gráficos, 3D, juegos, etc.
