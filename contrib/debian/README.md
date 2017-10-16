
Debian
====================
This directory contains files used to package civilbitd/civilbit-qt
for Debian-based Linux systems. If you compile civilbitd/civilbit-qt yourself, there are some useful files here.

## civilbit: URI support ##


civilbit-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install civilbit-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your civilbit-qt binary to `/usr/bin`
and the `../../share/pixmaps/civilbit128.png` to `/usr/share/pixmaps`

civilbit-qt.protocol (KDE)

