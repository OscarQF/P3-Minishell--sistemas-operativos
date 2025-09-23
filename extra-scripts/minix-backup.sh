#!/bin/bash
# Script para crear una copia de seguridad de la imagen de Minix

set -e

IMG_DIR="$HOME/Disco/Imagen"
BACKUP_DIR="$HOME/Disco/Imagen"
IMG_FILE=$(ls "$IMG_DIR"/minix3hd.plot.*.qcow2 2>/dev/null | head -n 1)
BACKUP_FILE="$BACKUP_DIR/minix3hd.plot.backup.qcow2.gz"

# Comprobación de si QEMU está ejecutándose
if pgrep -f "qemu-system" >/dev/null; then
    echo "✘ Parece que Minix está en ejecución (qemu está activo)."
    echo "   Debes apagar Minix antes de hacer la copia de seguridad."
    exit 1
fi

if [ ! -f "$IMG_FILE" ]; then
    echo "✘ No se encontró la imagen de Minix en $IMG_DIR"
    exit 2
fi

echo "⚠ Vas a crear una copia de seguridad de tu imagen de Minix."
echo "   Solo debes hacerlo si estás seguro de que la imagen NO está corrompida."
echo "   Si la imagen está dañada, también lo estará la copia."
echo
read -p "¿Quieres continuar? (s/n): " RESP
if [ "$RESP" != "s" ] ; then
    echo "Operación cancelada."
    exit 3
fi

echo "→ Creando copia comprimida en $BACKUP_FILE..."
gzip -c "$IMG_FILE" > "$BACKUP_FILE"

echo "✔ Copia de seguridad completada."

exit 0
