#!/bin/sh

BLUE="\033[34m"
END="\033[0m"

echo $BLUE "Compiling rpi_7seg..." $END
make
echo $BLUE "Check if the rpi_7seg module is existing..." $END
lsmod | grep rpi_7seg
if [ "$?" = "0" ]
then
    echo $BLUE "Found the rpi_7seg, remove the old one..." $END
    sudo rmmod rpi_7seg
fi
echo $BLUE "Installing the rpi_7seg module..." $END
sudo insmod rpi_7seg.ko

echo $BLUE "The install info of dmesg (/var/log/syslog)" $END
dmesg --color=always | tail -n 7

echo $BLUE "The rpi_7seg is successfully installed at \"/sys/module/rpi_7seg\"!" $END
echo $BLUE "And you should be able to manipulate the 7-segment display using the pseudo-file \"/sys/rpi_7seg/display\"!" $END
