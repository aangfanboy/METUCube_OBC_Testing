from owon_psu import OwonPSU
import time
import pandas as pd

frequency = 2  # Frequency in Hz
csv_file = "power_data.csv"

# Initialize the CSV file with headers
with open(csv_file, "w") as file:
    file.write("Voltage,Current,TimePassed,Time\n")

psu = OwonPSU("/dev/ttyUSB0")
psu.open()

timeBefore = pd.Timestamp.now()

try:
    while True:
        currentTime = pd.Timestamp.now()
        voltage = psu.measure_voltage()
        current = psu.measure_current()
        timePassed = (currentTime - timeBefore).total_seconds()

        # Append the new data directly to the CSV file
        with open(csv_file, "a") as file:
            file.write(f"{voltage},{current},{timePassed},{currentTime}\n")

        print(f"Voltage: {voltage} V, Current: {current} A, Time: {currentTime}")

        timeBefore = currentTime

        time.sleep((1 / frequency) - (pd.Timestamp.now() - currentTime).total_seconds())

except KeyboardInterrupt:
    print("Data collection stopped by user.")
finally:
    psu.close()
    print(f"Data saved to {csv_file}")