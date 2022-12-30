#!/bin/sh

file=$1
softdevice=$2
adapter=stlink

[ -z $file ] && echo "usage $0 <app> [softdevice (optional)]" && exit 1

openocd -f "interface/$adapter.cfg" -f "target/nrf52.cfg" -c \
	"program $file; reset; exit"

[ ! -z $softdevice ] && \
	openocd -f "interface/$adapter.cfg" -f "target/nrf52.cfg" -c \
	"program $softdevice; reset; exit"

