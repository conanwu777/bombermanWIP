#!/bin/sh

# Dependency for Ubuntu
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev mesa-common-dev libglu1-mesa-dev

# Create lib directory if it doesn't exist.
LIB_DIR="./lib";
if [ ! -d "$LIB_DIR" ]; then
    echo "Creating lib directory";
	mkdir lib;
fi
cd ./lib &&\

# Getting GLFW
git clone https://github.com/glfw/glfw.git GLFW &&\
echo "GLFW install done."

# Getting GLM
git clone https://github.com/g-truc/glm.git tmp_GLM &&\
mkdir GLM &&\
cp -r ./tmp_GLM/glm/* ./GLM/ &&\
rm -rf tmp_GLM &&\
echo "GLM install done."

# Getting GLI
git clone https://github.com/g-truc/gli.git tmp_GLI &&\
mkdir GLI &&\
cp -r ./tmp_GLI/gli/* ./GLI/ &&\
rm -rf tmp_GLI &&\
ln -s ./GLM/ ./glm &&\
echo "GLI install done."