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

echo "Listando paquetes compilados..."
find tmp/work -mindepth 2 -maxdepth 2 -type d | awk -F'/' '{print $4}' | sort -u > all_built_recipes.txt


echo "Obteniendo paquetes instalados en la imagen..."
sort "$INSTALLED_PKG_FILE" > used-pkgs.txt

echo "🔍 Detectando paquetes compilados pero no usados..."
comm -23 all_built_recipes.txt used-pkgs.txt > unused-pkgs.txt

echo "🧹 Limpiando cache (sstate y workdir) de paquetes no usados..."
while read -r pkg; do
    echo " → Limpiando $pkg"
    bitbake -c cleansstate "$pkg"
done < unused-pkgs.txt

echo "✅ Limpieza completa."
