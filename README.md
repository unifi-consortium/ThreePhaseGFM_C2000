# three-phase-inv-c2000
Files needed for manufacturing of the 5 kVA three phase 2 level inverter

## Key details:
+ Three phase 2 level, Vdc =400 V, Vac = 120V line-neutral rms LCL filter 
+ Switched with SiC devices and designed for 100 kHz switchin
+ Thermal validation : 2.5 kW open loop with max temp of inductors and switches not exceeding 70 deg C.
+ Validated till 1.5kW closed loop- droop with inner voltage and current control for grid connected operation.
+ Gerber, nc drill, pick and place files required for fabrication uploaded to repo
+ The BOM is from a vendor for a quote of 5 boards with 3 boards populated- to give you an idea of the cost. You should get the whole board ready except the three inverter side inductors. You need to make them. Please check the BOM for steps on how to build them. You need to buy shim stocks and magnet wire for these inductors.
