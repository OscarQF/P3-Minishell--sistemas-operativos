#!/bin/bash
# Script para restaurar la imagen de Minix desde una copia de seguridad

set -e

IMG_DIR="$HOME/Disco/Imagen"
BACKUP_FILE="$IMG_DIR/minix3hd.plot.backup.qcow2.gz"
ID_FILE="/home/alumno/Disco/p1-intro-entorno/id.txt"

# Comprobación de si QEMU está ejecutándose
if pgrep -f "qemu-system" >/dev/null; then
    echo "✘ Parece que Minix está en ejecución (qemu está activo)."
    echo "   Debes apagar Minix antes de hacer la copia de seguridad."
    exit 1
fi

if [ ! -f "$BACKUP_FILE" ]; then
    echo "✘ No se encontró ninguna copia de seguridad en $BACKUP_FILE"
    exit 2
fi

echo "⚠ Vas a restaurar la imagen de Minix."
echo "   Esto eliminará tu imagen actual y la sustituirá por la copia."
echo "   ¡Perderás todos los cambios hechos desde la última copia!"
echo
read -p "¿Quieres continuar con la restauración? (s/n): " RESP
if [ "$RESP" != "s" ]; then
    echo "Operación cancelada."
    exit 3
fi

# --- Obtener el ID del alumno ---
ID_ALUMNO=""

# 1. Buscar si existe una imagen actual
IMG_FILE_EXISTENTE=$(ls "$IMG_DIR"/minix3hd.plot.*.qcow2 2>/dev/null | head -n 1)
if [ -n "$IMG_FILE_EXISTENTE" ]; then
    ID_ALUMNO=$(basename "$IMG_FILE_EXISTENTE" | sed -E 's/^minix3hd\.plot\.([^.]+)\.qcow2$/\1/')
    echo "✓ Identificador obtenido de la imagen existente: $ID_ALUMNO"
fi

# 2. Si no hay imagen, mirar si existe id.txt
if [ -z "$ID_ALUMNO" ] && [ -f "$ID_FILE" ]; then
    ID_ALUMNO=$(tr -d ' \t\n\r' < $ID_FILE)
    echo "✓ Identificador obtenido de $ID_FILE: $ID_ALUMNO"
fi

# 3. Si aún no hay ID, preguntar al usuario
if [ -z "$ID_ALUMNO" ]; then
    read -p "Introduce tu número de matrícula (el mismo que metiste en la práctica 1): " ID_ALUMNO
    if [ -z "$ID_ALUMNO" ]; then
        echo "✘ No se introdujo ningún identificador. Cancelando."
        exit 4
    fi
fi

IMG_FILE="$IMG_DIR/minix3hd.plot.$ID_ALUMNO.qcow2"

# Si ya existía una imagen previa, la borramos
if [ -f "$IMG_FILE" ]; then
    echo "→ Eliminando imagen existente: $IMG_FILE"
    rm -f "$IMG_FILE"
fi

echo "→ Restaurando copia en $IMG_FILE..."
gunzip -c "$BACKUP_FILE" > "$IMG_FILE"

echo "✔ Restauración completada."

exit 0
