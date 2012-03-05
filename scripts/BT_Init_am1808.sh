#!/bin/sh

/usr/bin/BT_Exit.sh

rm   /var/run/messagebus.pid &> /dev/null
rm   /var/run/dbus/pid &> /dev/null
sleep 1
dbus-daemon --system &> /dev/null &
sleep 2
#bluetoothd -n &> /dev/null &
insmod `find /lib/modules/ -name "gpio_en.ko"`
sleep 2
hciattach /dev/ttyS1 texas 2000000 &
sleep 10
hciconfig hci0 piscan &> /dev/null
agent --path /org/bluez/agent 0000 &> /dev/null &
