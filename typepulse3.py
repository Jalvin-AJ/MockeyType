import serial
import time
import keyboard
from playsound3 import playsound
import threading

ser = serial.Serial('COM3', 9600)  # Adjust if needed
typed_chars = 0
start_time = time.time()
last_wpm_range = None

def on_key_press(e):
    global typed_chars
    typed_chars += 1

def play_mp3_for_wpm(wpm):
    global last_wpm_range
    if wpm < 80:
        if last_wpm_range != "slow":
            last_wpm_range = "slow"
            playsound("slow.mp3", block=False)
    elif wpm < 140:
        if last_wpm_range != "medium":
            last_wpm_range = "medium"
            playsound("medium.mp3", block=False)
    elif wpm < 200:
        if last_wpm_range != "fast":
            last_wpm_range = "fast"
            playsound("fast.mp3", block=False)
    else:
        if last_wpm_range != "superfast":
            last_wpm_range = "superfast"
            playsound("superfast.mp3", block=False)

keyboard.on_press(on_key_press)

try:
    while True:
        elapsed = time.time() - start_time
        if elapsed >=1:
            wpm = int((typed_chars / 5) / (elapsed / 60))
            ser.write(f"{wpm}\n".encode())

            # WPM display with indicator
            if wpm < 80:
                speed_label = "SLOW"
            elif wpm < 140:
                speed_label = "MEDIUM"
            elif wpm < 200:
                speed_label = "FAST"
            else:
                speed_label = "SUPERFAST"

            print(f"WPM: {wpm} ({speed_label})")

            threading.Thread(target=play_mp3_for_wpm, args=(wpm,)).start()
            typed_chars = 0
            start_time = time.time()

        time.sleep(0.05)

except KeyboardInterrupt:
    print("Stopped.")
