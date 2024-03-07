# BonBon To Fix

List of Things to Fix for Bonbon

- Autostart python script
  - Main issue is paplay.
  - Currently, crontab is set up as follows:
    - sudo's crontab tries to connect to the speaker and ESP's
      - This outputs to /home/pi/logs/cronlog
    - pi's crontab sleeps for 20 seconds to let this happen, then starts a python script
      - This outputs to /home/pi/logs/picronlog
  - It seems that it could work but pi's crontab is freezing when it gets to a paplay command, rather than completely crashing: maybe a longer delay?

## March 5 Updates

- Autostart is still an issue
- paplay is still the issue
- Crontab is set up so that:
  - sudo's crontab opens serial ports for the ESP's
    - This outputs to /home/pi/logs/cronlog
  - pi's crontab runs 2 concurrent processes:
    - One sleeps for 20 seconds, then runs the python bonbonHut script
      - This outputs to /home/pi/logs/picronlog
    - The other immediately tries to connect to the bluetooth speaker, but for some reason this fails sometimes
      - This outputs to /home/pi/logs/cronlogBT

Two different observations:

- before pi's crontab contained the speaker connection script, the python script would hang when it tried to play audio
- now that pi's crontab contains the speaker connection script, the python script seems to not run at all

- If the speaker connection script fails, the main remedy is to manually run

```bash
bluetoothctl
connect 50:91:99:EE:17:3F
```

- which will somehow magically fix bt audio

## March 6 Updates (Jade)

- Autostart is seemingly working! (after lots of trial and error with crontab ~~and rc.local~~)
  - I still plan on testing the system more extensively, especially watching for issues with BT not connecting
  - the BT init scripts could be made more robust if they continue to cause issues sometimes
- SOLUTION: added `export XDG_RUNTIME_DIR="/run/user/1000` to driver.sh (which runs the python script)
  - the added sleeps in crontab may have also contributed to this solution!
- ssh is also now safe to do while the system is running from autostart 
  (previously, the process would die for some reason if anyone logged in)

- Crontab is now set up so that:
  - sudo's crontab sleeps 20, then opens serial ports for the ESP's (sudoBTinit.sh)
    - This outputs to /home/pi/logs/sudoBTinit.log (and includes stderr)
  - pi's crontab runs two commands:
    - sleep 20, then connect to bluetooth speaker (userBTinit.sh)
      - This outputs to /home/pi/logs/userBTinit.log
    - sleep 30, then run script that sets up pulse audio/environment and runs python script (driver.sh)
      - This outputs to driverOutput.log

## March 7 Updates (Jade)

- Autostart bug fixed by configuring pi to automatically log in on boot! (solution found at https://forums.raspberrypi.com/viewtopic.php?t=346924)
- Deduced that switching between SD cards requires a "bluetoothctl -- remove" and then re-pairing/trusting/connecting to the speaker unit for the audio to work again (speaker gets confused somehow)