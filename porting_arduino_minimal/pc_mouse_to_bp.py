import time
import threading
import serial
from pynput import mouse
import tkinter as tk

PORT = "COM33"
BAUD = 115200

DISPLAY_W = 320
DISPLAY_H = 240

ser = serial.Serial(PORT, BAUD, timeout=0.05)
time.sleep(2)

root = tk.Tk()
root.withdraw()
SCREEN_W = root.winfo_screenwidth()
SCREEN_H = root.winfo_screenheight()
root.destroy()

state = {
    "x": DISPLAY_W // 2,
    "y": DISPLAY_H // 2,
    "pressed": 0,
    "dirty": True,
}

lock = threading.Lock()


def clamp(v, lo, hi):
    return max(lo, min(hi, v))


def screen_to_display(x, y):
    dx = int(x * (DISPLAY_W - 1) / max(1, SCREEN_W - 1))
    dy = int(y * (DISPLAY_H - 1) / max(1, SCREEN_H - 1))
    return clamp(dx, 0, DISPLAY_W - 1), clamp(dy, 0, DISPLAY_H - 1)


def send_packet():
    with lock:
        msg = f"M {state['x']} {state['y']} {state['pressed']}\n"
        state["dirty"] = False
    ser.write(msg.encode("ascii"))


def on_move(x, y):
    dx, dy = screen_to_display(x, y)
    with lock:
        if state["x"] != dx or state["y"] != dy:
            state["x"] = dx
            state["y"] = dy
            state["dirty"] = True


def on_click(x, y, button, pressed):
    if button != mouse.Button.left:
        return
    dx, dy = screen_to_display(x, y)
    with lock:
        state["x"] = dx
        state["y"] = dy
        state["pressed"] = 1 if pressed else 0
        state["dirty"] = True


def reader():
    while True:
        line = ser.readline()
        if line:
            print("BP:", line.decode(errors="ignore").rstrip())


threading.Thread(target=reader, daemon=True).start()

listener = mouse.Listener(on_move=on_move, on_click=on_click)
listener.start()

print(f"Connected to {PORT} at {BAUD}")
print(f"PC screen mapped to {DISPLAY_W}x{DISPLAY_H}")
print("Move mouse, left click to press. Ctrl+C to exit.")

try:
    while True:
        with lock:
            dirty = state["dirty"]
        if dirty:
            send_packet()
        time.sleep(0.01)
except KeyboardInterrupt:
    pass
finally:
    listener.stop()
    ser.close()
