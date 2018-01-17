
Debian
====================
This directory contains files used to package stonecoind/stonecoin-qt
for Debian-based Linux systems. If you compile stonecoind/stonecoin-qt yourself, there are some useful files here.

## stonecoin: URI support ##


stonecoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install stonecoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your stonecoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/stonecoin128.png` to `/usr/share/pixmaps`

stonecoin-qt.protocol (KDE)

