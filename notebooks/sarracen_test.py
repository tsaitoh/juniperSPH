import sarracen as sar

sdf = sar.read_phantom("../ot_00003")
sdf.to_csv("ot_00003.csv")
