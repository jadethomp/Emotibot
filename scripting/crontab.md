# Crontab Settings

A description of the settings for BonBon's crontab

Edit crontab settings on linux with

    crontab -e

This edits the current user's crontab. You can also use sudo to edit sudo's crontab.

## Sudo Crontab

After accessing sudo crontab with

    sudo crontab -e

Old Crontab:

    @reboot XDG_RUNTIME_DIR=/run/user/$1000 bash /home/pi/bin/initBT.sh >/home/pi/logs/cronlog 2>&1

New Crontab:

    Blank :p

This is going to be moved into the pi user's crontab, to see if the bluetooth script acts less weird with paplay.

## pi Crontab

After accessing crontab with

    crontab -e

Old Crontab:

    @reboot sleep 20 && cd /home/pi/bon/ && python3 bonbonHut.py >/home/pi/logs/picronlog

The new crontab is going to be

    @reboot bash /home/pi/bin/userBTinit.sh >/home/pi/logs/cronlogBT
    @reboot sleep 20 && cd /home/pi/bon/ && python3 bonbonHut.py >/home/pi/logs/picronlog
