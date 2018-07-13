
Debian
====================
This directory contains files used to package stoned/stone-qt
for Debian-based Linux systems. If you compile stoned/stone-qt yourself, there are some useful files here.

## stone: URI support ##


stone-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install stone-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your stone-qt binary to `/usr/bin`
and the `../../share/pixmaps/stone128.png` to `/usr/share/pixmaps`

stone-qt.protocol (KDE)

