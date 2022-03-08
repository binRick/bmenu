#!/bin/bash
#make && ./bin/bmenu -t "wow 123" -c $(pwd)/c1
clear; make && env CONFIG_FILE=$(pwd)/c1 MENU_TITLE="wow $(date +%s)" passh -L .o sh -c "make && ./bin/bmenu -t xxxxxxxxx"
