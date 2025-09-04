# Production instructions

This folder contains all the production files and a complete parts list necessary for manufacturing this board. The board contains only through-hole components. The PCB files are optimized for ordering from JLCPCB, while the through-hole components are configured for ordering from Mouser.

## PCB

Upload the file `./jlcpcb/midi2cv.zip` on [jlcpcb.com](https://www.jlcpcb.com) to order the PCB.

## Through hole assembly

To make ordering through hole components easy we've prepared a CSV file to generate a BOM on [mouser.com](https://www.mouser.com). Here's what you need to do:

1. Go to [https://www.mouser.com](https://www.mouser.com) and log in with your account
2. Open [BOMs](https://hu.mouser.com/OrderHistory/Boms) from the account menu
3. Click on Create New BOM
4. Upload the `./mouser/midi2cv-mouser-bom.csv` file from this directory
5. Make sure you map the `Quantity 1` and `Mouser Product Number` columns correctly
6. Create the BOM

You can order all necessary parts at once. If a part is not available you need to find replacement components, or if you want another vendor (like Digikey) then you'll need to hunt all parts yourself.

## Disclaimer

We are not responsible for any errors in the PCB design or the parts list. All responsibility for ordering PCBs, components, and assembly services rests solely with you.

## More important disclaimer

Have fun!
