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
