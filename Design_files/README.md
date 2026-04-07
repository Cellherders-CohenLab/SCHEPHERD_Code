Here are a brief description of printing parts in SCHEPHERD system

##Acrylics: They include the .dxf file for laser cutting 3mm thick PMMA (acryilc/plexiglass)

    Agar_brdg_3mm: for casting salt bridges for all insert except transwell stimulation

    long_agar_bridge_transwell and short_agar_bridge_transwell: for casting in transwell stimulation inserts, use one each

    chamber_top_lid_PCB: transparent lid for incubator enclosure with electrode connectors, can use thinner PMMA sheet.

    Chamber_top_lid_flaps: transparent lid for incubator enclosure with soldered wire connection. 

##Incubator_STL: for incubator enclosure parts and peripheral tools, they are optomized for Nikon Ti2E stage holders
    Electrode_8x23: a handy guide to help cutting 8x23mm silver foil, mark with a razor and then cut with a scissor

    PCB_hold_plate: a guide plate use for PCB enclosures

    PIES_Chamber_top_lid_Nikon: Enclosure for soldered wire connection (without transparent acrylic) 

    PIES_Chamber_top_lid_Nikon_PCB: Enclosure for electrode connector.

    Tissue_stamp: a guide helping lay down PDMS stencil (optional)

##Insert_STL: for making individual stimulation inserts
    Everything labelled with ''glsBtn' is for use with glass bottom 24 well plates: cellvis p24-1.5h-n

    plasticBtn is for use with Cole-Parmer 01927-74

    Transmitted_light is for use with 12 well plate Cole-Parmer	01927-72

    Transwell insert is for use with 12 well transwell plate Corning 3513 and transwell Corning 353180

    Dummy_insert does not have any function, it is used to test material compatibility

##PCB_files: they are gerber, pick and place files for the current source as well as integrated electrode lid

    Motherboard+ 4xsubunit: for a complete stimulation current source
    
    Electrode_lid: for electrode connection using connector (as shown in main manuscript)

    Breakout_board: for soldered wire connection

##For PCB connected electrode setup (using this HDMI cable for pin compatibility: https://www.amazon.com/Thsucords-Flexible-Support-Blu-ray-Projector/dp/B0CBBWBT2J), manufacture the following:
    Acrylic: chamber_top_lid_PCB

    3D print: PCB_hold_plate, PIES_Chamber_top_lid_Nikon_PCB

    PCB fabrication: Motherboard, 4x subunit, Electrode_lid


##For soldered wire connections, manufacture the following:

    Acrylic: Chamber_top_lid_flaps

    3D print: PIES_Chamber_top_lid_Nikon

    PCB fabrication: Motherboard, 4x subunit, Breakout_board (or use your existing power supply)