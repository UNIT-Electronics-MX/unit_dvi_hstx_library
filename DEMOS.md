# 🎮 Demos Mejorados para Arduino Nano RP2350 + DVI

Colección de ejemplos avanzados que demuestran las capacidades gráficas del RP2350 con salida DVI/HDMI.

## 📋 Lista de Demos

### 1. **nano_rp2350_test** - Test Básico
- **Archivo**: `nano_rp2350_test/nano_rp2350_test.ino`
- **Descripción**: Ejemplo simple de verificación, dibuja líneas aleatorias
- **Nivel**: Principiante
- **Características**:
  - Configuración de pines correcta para Nano RP2350
  - Salida de debug por Serial
  - Indicador LED en caso de error

### 2. **graphics_demo** - 🌈 Demo Gráfico Multi-Efecto
- **Archivo**: `graphics_demo/graphics_demo.ino`
- **Descripción**: 6 efectos visuales que cambian cada 10 segundos
- **Nivel**: Intermedio
- **Efectos incluidos**:
  1. **Bouncing Balls** - Bolas rebotando con trail effect
  2. **Plasma Effect** - Efecto de plasma animado con senos
  3. **Rainbow Bars** - Barras de colores arcoíris en movimiento
  4. **Starfield** - Campo de estrellas 3D
  5. **Circles** - Círculos concéntricos animados
  6. **Matrix Rain** - Estilo "Matrix" lluvia de código
- **FPS**: 30-60 dependiendo del efecto
- **Memoria**: ~150KB para framebuffer 320x240

### 3. **text_graphics_demo** - 📊 UI y Visualización de Datos
- **Archivo**: `text_graphics_demo/text_graphics_demo.ino`
- **Descripción**: Interfaz de usuario con gráficos y texto
- **Nivel**: Intermedio
- **Características**:
  - Barra de título y estado
  - Información del sistema en tiempo real
  - Espectro de audio simulado (barras animadas)
  - Barra de progreso
  - Contador de FPS
  - Uptime del sistema
- **Ideal para**: Proyectos de monitoreo, dashboards

### 4. **cube_3d_demo** - 🎲 Cubo 3D Rotatorio
- **Archivo**: `cube_3d_demo/cube_3d_demo.ino`
- **Descripción**: Renderizado 3D en tiempo real de un cubo
- **Nivel**: Avanzado
- **Características**:
  - Rotación en 3 ejes (X, Y, Z)
  - Proyección ortográfica
  - Colores por cara
  - Muestra ángulos de rotación en pantalla
- **Matemáticas**: Matrices de rotación 3D
- **FPS**: ~30

### 5. **snake_game** - 🐍 Juego de Snake
- **Archivo**: `snake_game/snake_game.ino`
- **Descripción**: Juego clásico de Snake funcional
- **Nivel**: Intermedio/Avanzado
- **Características**:
  - Control por Serial Monitor (w/a/s/d)
  - Sistema de puntuación
  - Velocidad incrementa con el score
  - Detección de colisiones
  - Game Over y reinicio automático
- **Grid**: 40x30 celdas
- **Controles**:
  - `w` = arriba
  - `s` = abajo
  - `a` = izquierda
  - `d` = derecha

## 🔌 Configuración de Pines (Todos los ejemplos)

Todos los ejemplos usan la misma configuración:

```cpp
DVHSTXPinout pinConfig = {18, 12, 14, 16};
```

| Arduino Pin | GPIO | HDMI Signal |
|-------------|------|-------------|
| D1          | 18   | Clock+      |
| D7          | 12   | Blue+       |
| D5          | 14   | Green+      |
| D3          | 16   | Red+        |

## 🚀 Cómo Usar

### Instalación Rápida

1. **Instala las bibliotecas necesarias**:
   - Adafruit GFX Library (desde el gestor de bibliotecas)
   - Esta biblioteca (copia a ~/Arduino/libraries/)

2. **Configura Arduino IDE**:
   - Board: Arduino Nano RP2350 Connect (o Raspberry Pi Pico 2)
   - CPU Speed: **150 MHz** (importante)
   - USB Stack: Pico SDK
   - Optimize: -O3

3. **Abre cualquier ejemplo**:
   - File → Examples → Adafruit-DVI-HSTX → [nombre del ejemplo]

4. **Compila y sube** (Ctrl+U)

### Visualizar Debug Info

Todos los ejemplos tienen salida serial:

1. Sube el sketch
2. Abre Tools → Serial Monitor
3. Configura a 115200 baud
4. Verás información de inicialización y estado

## 📊 Comparación de Ejemplos

| Ejemplo              | Complejidad | FPS | Uso RAM | Interactivo | Mejor para...           |
|----------------------|-------------|-----|---------|-------------|-------------------------|
| nano_rp2350_test     | ⭐          | 60  | Bajo    | No          | Verificar hardware      |
| graphics_demo        | ⭐⭐⭐       | 30+ | Medio   | No          | Demos visuales          |
| text_graphics_demo   | ⭐⭐        | 60  | Bajo    | No          | UIs, dashboards         |
| cube_3d_demo         | ⭐⭐⭐⭐     | 30  | Bajo    | No          | Aprender 3D             |
| snake_game           | ⭐⭐⭐       | Var | Bajo    | Sí          | Juegos, aprendizaje     |

## 🎨 Personalización

### Cambiar Colores

Los ejemplos usan formato RGB565:
```cpp
// Formato: 0xRRRRRGGGGGGBBBBB (5-6-5 bits)
uint16_t red    = 0xF800;
uint16_t green  = 0x07E0;
uint16_t blue   = 0x001F;
uint16_t white  = 0xFFFF;
uint16_t black  = 0x0000;

// O usa color565():
display.color565(255, 0, 0);  // Rojo
```

### Cambiar Resolución

Edita la línea de inicialización:
```cpp
// Opciones disponibles:
DVHSTX_RESOLUTION_320x240  // 4:3 - Recomendado
DVHSTX_RESOLUTION_320x180  // 16:9
DVHSTX_RESOLUTION_640x360  // 16:9 - Más RAM
DVHSTX_RESOLUTION_640x480  // 4:3 - Más RAM
```

### Agregar Controles

Para el juego Snake o agregar controles a otros demos:

**Opción 1: Serial Monitor**
```cpp
if (Serial.available() > 0) {
  char cmd = Serial.read();
  // Procesar comando
}
```

**Opción 2: Botones físicos**
```cpp
const int BTN_UP = 2;
pinMode(BTN_UP, INPUT_PULLUP);

if (digitalRead(BTN_UP) == LOW) {
  // Botón presionado
}
```

## 🔧 Optimización

### Mejorar FPS

1. **Reduce operaciones por frame**:
   ```cpp
   // Mal: Limpiar toda la pantalla
   display.fillScreen(0x0000);
   
   // Mejor: Limpiar solo lo necesario
   display.fillRect(x, y, w, h, 0x0000);
   ```

2. **Usa operaciones rápidas**:
   ```cpp
   display.drawFastHLine(x, y, w, color);  // Más rápido
   display.drawFastVLine(x, y, h, color);  // Más rápido
   ```

3. **Pre-calcula valores**:
   ```cpp
   // Pre-calcular fuera del loop
   int centerX = display.width() / 2;
   int centerY = display.height() / 2;
   ```

### Reducir Uso de Memoria

1. Usa resolución más baja (320x240 en vez de 640x480)
2. Evita arrays grandes
3. Usa `const` para datos constantes
4. Considera usar PROGMEM para datos grandes

## 🐛 Solución de Problemas

### Ejemplo no compila

- Verifica que Adafruit GFX esté instalada
- Verifica CPU Speed = 150 MHz
- Actualiza el core RP2350

### Pantalla negra

- Verifica conexiones de hardware
- Prueba con otro ejemplo simple primero
- Revisa el Serial Monitor para errores

### FPS bajo

- Reduce complejidad del efecto
- Aumenta delay entre frames
- Reduce resolución

### Snake no responde

- Asegúrate de enviar comandos desde Serial Monitor
- Verifica que "No line ending" esté desactivado
- Caracteres válidos: w, a, s, d (minúsculas)

## 📝 Crear Tu Propio Demo

Plantilla básica:

```cpp
#include <Adafruit_dvhstx.h>

DVHSTXPinout pinConfig = {18, 12, 14, 16};
DVHSTX16 display(pinConfig, DVHSTX_RESOLUTION_320x240);

void setup() {
  Serial.begin(115200);
  
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    while (true) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  display.fillScreen(0x0000);
  // Tu inicialización aquí
}

void loop() {
  // Tu código de animación aquí
  
  delay(16); // ~60 FPS
}
```

## 🌟 Ideas para Más Demos

- **Pong Game**: Juego de pong con dos jugadores
- **Image Viewer**: Mostrar imágenes desde SD
- **Audio Visualizer**: Con entrada de audio real
- **Mandelbrot Set**: Fractal renderer
- **Oscilloscope**: Osciloscopio digital
- **Retro Console**: Emulador de consola retro
- **Weather Station**: Dashboard con datos meteorológicos
- **System Monitor**: Monitoreo de CPU/RAM en tiempo real

## 📚 Recursos Adicionales

- [Adafruit GFX Tutorial](https://learn.adafruit.com/adafruit-gfx-graphics-library)
- [RP2350 Datasheet](https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf)
- [HDMI/DVI Specification](https://www.hdmi.org/spec/index)

## 🤝 Contribuir

¿Creaste un demo interesante? ¡Compártelo!

1. Documenta tu código
2. Incluye instrucciones de uso
3. Especifica requisitos de hardware adicional
4. Agrega capturas o videos si es posible

---

**¡Disfruta creando con DVI-HSTX en tu Arduino Nano RP2350!** 🎉
