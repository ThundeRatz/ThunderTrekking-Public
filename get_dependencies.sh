#!/bin/bash
set -e

echo "Script para ajudar no download de dependências que geram"
echo "bibliotecas estáticas."
echo
echo "Não é garantido que funcione (depende so sistema de build"
echo "de outros projetos), avise se der problema."
echo

set -x
cd "$( dirname "${BASH_SOURCE[0]}" )"/externals
git submodule update --init --recursive
svn checkout http://googletest.googlecode.com/svn/trunk/ gtest
mkdir -p lib include

cd KFilter/
make
cp -rf kalman/ ../include/
cp -f libkalman.a ../lib/
cd -

cd gtest/
cmake .
make
cp -f libgtest*.a ../lib/
cp -rf include/* ../include/
cd -
