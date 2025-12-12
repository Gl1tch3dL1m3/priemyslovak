import time

for i in range(100):
    time.sleep(1)
    print("Loading... " + str(i) + "%\r")
