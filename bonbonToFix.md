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
