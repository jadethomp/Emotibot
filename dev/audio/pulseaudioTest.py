from pasimple import play_wav
from random import randrange
import pathlib

soundsPath = str(pathlib.Path(__file__).parent.resolve()) + "/sounds/"

emotion = 1

chosenSound = soundsPath

if emotion == 0:
    chosenSound += "anxious"
elif emotion == 1:
    chosenSound += "sad"
elif emotion == 2:
    chosenSound += "happy"

chosenSound += str(randrange(3) + 1) + ".wav"

play_wav(chosenSound)