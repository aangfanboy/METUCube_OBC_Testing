from owon_psu import OwonPSU

with OwonPSU("/dev/ttyUSB0") as opsu:
  print("Identity:", opsu.read_identity())
  print("Measured Voltage:", opsu.measure_voltage())
  print("Measured Current:", opsu.measure_current())
