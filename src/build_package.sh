#!/bin/bash
# *************************************
# Pandora Storage Server
# Source
# 'build_package.sh'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

# Software version
VERSION="0.0.1"

# Create Package directory
mkdir pandora-storage-server-pkg && mkdir pandora-storage-server-pkg/DEBIAN

# Copy Pandora executable into the package
mkdir -p pandora-storage-server-pkg/usr/local/bin 
cp build/pandora pandora-storage-server-pkg/usr/local/bin/

# Create package control file
touch pandora-storage-server-pkg/DEBIAN/control

# Append data to control file
echo "Package: PandoraStorageServer" >> pandora-storage-server-pkg/DEBIAN/control
echo "Version: ${VERSION}" >> pandora-storage-server-pkg/DEBIAN/control
echo "Maintainer: Juan Carlos Juarez" >> pandora-storage-server-pkg/DEBIAN/control
echo "Architecture: all" >> pandora-storage-server-pkg/DEBIAN/control
echo "Description: Pandora Storage Server Debian Package" >> pandora-storage-server-pkg/DEBIAN/control

# Create postinst file
touch pandora-storage-server-pkg/DEBIAN/postinst
chmod 0755 pandora-storage-server-pkg/DEBIAN/postinst

# Append message to postinst file
echo "#!/bin/bash" >> pandora-storage-server-pkg/DEBIAN/postinst
echo "echo 'Pandora Storage Server v${VERSION} has been installed succesfully.'" >> pandora-storage-server-pkg/DEBIAN/postinst

# Create Debian package
dpkg-deb --build pandora-storage-server-pkg
