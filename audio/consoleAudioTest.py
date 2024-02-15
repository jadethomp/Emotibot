## L being short:
### L when userPresent: anxious
### L when not userPresent: happy
## R being tall
### R when userPresent: happy
### R when not userPresent: sad

from pasimple import play_wav
from random import randrange
import pathlib

soundsPath = str(pathlib.Path(__file__).parent.resolve()) + "/sounds/"

def play(emotion, direction):
    chosenSound = soundsPath
    if direction == "L":
        if emotion == 0:
            chosenSound += "Lanxious"
        elif emotion == 1:
            chosenSound += "Lsad"
        elif emotion == 2:
            chosenSound += "Lhappy"

    if direction == "R":
        if emotion == 0:
            chosenSound += "Ranxious"
        elif emotion == 1:
            chosenSound += "Rsad"
        elif emotion == 2:
            chosenSound += "Rhappy"

    chosenSound += str(randrange(3) + 1) + ".wav"

    play_wav(chosenSound)

while True:
    emotion = input("Please enter 0, 1, or 2: ")
    direction = input("Please enter L or R")
    play(emotion, direction)

