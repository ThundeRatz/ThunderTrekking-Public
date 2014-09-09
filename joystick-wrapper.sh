#!/bin/bash
# Só chama joystick.py com joystick_interface.c na entrada padrão
stdbuf -i0 -o0 -e0 ./joystick $* | ./joystick_run.py
