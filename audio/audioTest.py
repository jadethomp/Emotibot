from pygame import mixer
from random import randrange
import pathlib, time

soundsPath = str(pathlib.Path(__file__).parent.resolve()) + "/sounds/"

mixer.init()

emotion = 0

chosenSound = soundsPath

if emotion == 0:
    chosenSound += "anxious"
elif emotion == 1:
    chosenSound += "sad"
elif emotion == 2:
    chosenSound += "happy"

chosenSound += str(randrange(3) + 1) + ".wav"

print(chosenSound)

sound = mixer.Sound(chosenSound)

sound.play()

time.sleep(5)