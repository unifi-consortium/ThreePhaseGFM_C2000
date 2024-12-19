# three-phase-inv-c2000

## Topology
![3PhInv4png](https://github.com/unifi-consortium/three-phase-inv-c2000/assets/117390647/d84dceee-4ed1-4035-8424-cd716fe81151)


## Hardware design
+ Three phase 2 level, Vdc =400 V, Vac = 120 V line-neutral rms (208 V line-line) LCL filter 
+ Switched with SiC devices and designed for 100 kHz switching
![3_phase_board_fig](https://github.com/unifi-consortium/three-phase-inv-c2000/assets/117390647/3a36e519-630a-4050-8421-31b0a1dbe0f2)

## Thermal validation 
2.5 kW open loop with max temp of inductors and switches not exceeding 80 deg C as seen in the figure below:
![image](https://github.com/unifi-consortium/three-phase-inv-c2000/assets/117390647/139f9f25-4ba0-4dbe-bf88-efcfe297e9ad)

## Grid connected operation
Validated till 4.1 kW closed loop- droop with inner voltage and current control for grid connected operation.
![exptat4kW2](https://github.com/unifi-consortium/three-phase-inv-c2000/assets/117390647/d3389e85-848c-4928-a8ae-a9779f1ef06f)


## Note
Files needed for manufacturing of the 5 kVA three phase 2 level inverter are uploaded on this repo. The Gerber, nc drill, pick and place files are required for fabrication. To give you an idea of the cost, a BOM from a vendor for a quote of 5 boards with 3 boards populated is uploaded. You should get the whole board except the three inverter side inductors which need to be made by hand. Please check the BOM for steps on how to build them. You need to buy shim stocks and magnet wire for these inductors.
