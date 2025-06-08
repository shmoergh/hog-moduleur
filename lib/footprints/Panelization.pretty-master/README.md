
Panelization.pretty
===================

LAYOUT FILES: KiCad footprints useful for PCB panelization (mouse-bites...).

## How to add the footprints:

1. Download/clone this repo.
2. open Footprint editor in KiCad, there "File / Import footprint from KiCad File..."
3. Select one of the mouse-bite kicad_mod files
4. Save it to your project or make a local library

Or place the full Panelization.pretty folder in your KiCad library folder.

## How to use the footprints:

1. Place the footprint on your pcb edge
2. Connect the Edge.Cuts layer to the footprint
3. draw the half circles on the Edge.Cuts layer
4. enjoy

## Examples:

the 5mm wide mouse bites: 

![grafik](https://user-images.githubusercontent.com/1591573/142768275-5d10dfa5-b7f1-4350-a840-ed46f035b56c.png)

2mm wide connecting different layouts: 

![grafik](https://user-images.githubusercontent.com/1591573/142768418-415e3ee7-1b69-4f26-ba47-e184d0816b06.png)

## panelization:

when your PCB is ready, mark everything, then right click and "Create Array". Choose a grid that the spacing matches the choosen mouse bite width.

![grafik](https://user-images.githubusercontent.com/1591573/142768813-ece31682-33f7-49fe-8298-cfc120e7bace.png)

then place mouse bite footprint and modify the Edge.Cuts layer on the first pcb.  Then just "Create Array" on this again.

A Video showing different ways to make panels in KiCad:
[![Watch the video](https://user-images.githubusercontent.com/1591573/142769347-8eac12c2-11e9-4f84-a7fc-1b08710edaf1.png)](https://youtu.be/_Syjz8m-2fw)


