#!/bin/bash

# Script de compilación para ejemplos DVI-HSTX con pico-sdk
# Arduino Nano RP2350

set -e  # Salir si hay algún error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  DVI-HSTX Build Script${NC}"
echo -e "${GREEN}  Arduino Nano RP2350${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""

# Verificar que PICO_SDK_PATH esté configurado
if [ -z "$PICO_SDK_PATH" ]; then
    echo -e "${RED}ERROR: PICO_SDK_PATH no está configurado${NC}"
    echo -e "${YELLOW}Por favor configura la variable de entorno:${NC}"
    echo "  export PICO_SDK_PATH=/path/to/pico-sdk"
    exit 1
fi

echo -e "${GREEN}✓${NC} PICO_SDK_PATH: $PICO_SDK_PATH"
echo ""

# Directorio base del proyecto
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

# Limpiar build anterior si se pasa el argumento "clean"
if [ "$1" == "clean" ]; then
    echo -e "${YELLOW}Limpiando build anterior...${NC}"
    rm -rf "$BUILD_DIR"
    echo -e "${GREEN}✓${NC} Build limpiado"
    echo ""
fi

# Crear directorio de build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configurar con CMake
echo -e "${YELLOW}Configurando proyecto...${NC}"
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DPICO_PLATFORM=rp2350 \
    -DPICO_BOARD=pico2

if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: Configuración con CMake falló${NC}"
    exit 1
fi

echo -e "${GREEN}✓${NC} Configuración completada"
echo ""

# Compilar
echo -e "${YELLOW}Compilando...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: Compilación falló${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  ✓ Compilación exitosa!${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""

# Mostrar archivos generados
echo -e "${YELLOW}Archivos generados:${NC}"
echo ""

for uf2_file in $(find "$BUILD_DIR/examples" -name "*.uf2"); do
    example_name=$(basename $(dirname "$uf2_file"))
    file_size=$(ls -lh "$uf2_file" | awk '{print $5}')
    echo -e "${GREEN}✓${NC} $example_name:"
    echo -e "   $uf2_file"
    echo -e "   Tamaño: $file_size"
    echo ""
done

echo -e "${YELLOW}Para flashear:${NC}"
echo "1. Conecta tu Arduino Nano RP2350 en modo BOOTSEL"
echo "2. Copia el archivo .uf2 al dispositivo USB que aparece"
echo ""
echo -e "${YELLOW}Para debug serial:${NC}"
echo "  screen /dev/ttyACM0 115200"
echo "  # o"
echo "  minicom -D /dev/ttyACM0 -b 115200"
echo ""
