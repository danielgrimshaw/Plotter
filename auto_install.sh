#!/bin/bash
BUILD_DIR=build
SRC_DIR=src
BIN_DIR=bin

if [ -d "$BUILD_DIR" ]; then
	rm -rf $BUILD_DIR
fi

mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

hash cmake 2>/dev/null || { echo >&2 "Error: require cmake, but it's not installed. Aborting."; exit 1; }
hash make 2>/dev/null || { echo >&2 "Error: require make, but it's not installed. Aborting."; exit 1; }

cmake "../$SRC_DIR" || { echo >&2 "Error: cmake failed, aborting!"; exit 1; }
make || { echo >&2 "Error: make failed, aborting!"; exit 1; }

cd ..

if [ -d "$BIN_DIR" ]; then
	rm -rf $BIN_DIR
fi
mkdir $BIN_DIR

cp "$BUILD_DIR/Plotter" "$BIN_DIR"
cp $SRC_DIR/*.glsl $SRC_DIR/*.csv "$BIN_DIR"

rm -rf $BUILD_DIR

echo "Compiled Plotter to $BIN_DIR"
exit 0;

