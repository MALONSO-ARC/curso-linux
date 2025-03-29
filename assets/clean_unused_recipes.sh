#!/bin/bash

# Configuración
INSTALLED_PKG_FILE="installed_packages.txt"

# Verifica que exista buildhistory
if [ ! -f "$INSTALLED_PKG_FILE" ]; then
    echo "ERROR: No se encontró el archivo con los paquetes instalados:"
    echo "       $INSTALLED_PKG_FILE"
    echo "Asegúrate de haber activado buildhistory y compilado la imagen."
    exit 1
fi

echo "📦 Listando paquetes compilados..."
oe-pkgdata-util list-pkgs | sort > all-pkgs.txt

echo "📄 Obteniendo paquetes instalados en la imagen..."
sort "$INSTALLED_PKG_FILE" > used-pkgs.txt

echo "🔍 Detectando paquetes compilados pero no usados..."
comm -23 all-pkgs.txt used-pkgs.txt > unused-pkgs.txt

echo "🧹 Limpiando cache (sstate y workdir) de paquetes no usados..."
while read -r pkg; do
    echo " → Limpiando $pkg"
    bitbake -c cleansstate "$pkg"
done < unused-pkgs.txt

echo "✅ Limpieza completa."
