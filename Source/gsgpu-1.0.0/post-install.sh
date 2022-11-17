#!/bin/bash

echo "do post install..."

cp -f ./config/loongson.conf /etc/modprobe.d/loongson.conf
cp -f ./config/gsgpu.conf /etc/modprobe.d/gsgpu.conf


