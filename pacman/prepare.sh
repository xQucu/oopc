#!/bin/sh
# Create a proper .pro file manually
cat > pacman.pro << 'EOF'
QT += widgets
TARGET = pacman
HEADERS = GameObject.h Player.h Enemy.h Blinky.h GameWidget.h
SOURCES = GameObject.cpp Player.cpp Enemy.cpp Blinky.cpp GameWidget.cpp main.cpp
EOF

qmake


