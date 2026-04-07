# SCHEPHERD System — Printing Parts Overview

## Acrylics

Laser cutting files (.dxf) for 3mm thick PMMA (acrylic/plexiglass):

- **Agar_brdg_3mm** — for casting salt bridges for all inserts except transwell stimulation
- **long_agar_bridge_transwell** and **short_agar_bridge_transwell** — for casting in transwell stimulation inserts; use one each
- **chamber_top_lid_PCB** — transparent lid for incubator enclosure with electrode connectors; can use thinner PMMA sheet
- **Chamber_top_lid_flaps** — transparent lid for incubator enclosure with soldered wire connection

---

## Incubator_STL

Parts for incubator enclosure and peripheral tools, optimized for Nikon Ti2E stage holders:

- **Electrode_8x23** — guide for cutting 8x23mm silver foil; mark with a razor and cut with scissors
- **PCB_hold_plate** — guide plate for PCB enclosures
- **PIES_Chamber_top_lid_Nikon** — enclosure for soldered wire connection (without transparent acrylic)
- **PIES_Chamber_top_lid_Nikon_PCB** — enclosure for electrode connector
- **Tissue_stamp** — guide for laying down PDMS stencil (optional)

---

## Insert_STL

Individual stimulation insert files:

- **glsBtn** (prefix) — for use with glass bottom 24-well plates: Cellvis P24-1.5H-N
- **plasticBtn** (prefix) — for use with Cole-Parmer 01927-74
- **Transmitted_light** — for use with 12-well plate Cole-Parmer 01927-72
- **Transwell insert** — for use with 12-well transwell plate Corning 3513 and transwell Corning 353180
- **Dummy_insert** — no function; used to test material compatibility

---

## PCB_files

Gerber and pick-and-place files for the current source and integrated electrode lid:

- **Motherboard + 4x subunit** — complete stimulation current source
- **Electrode_lid** — electrode connection using connector (as shown in main manuscript)
- **Breakout_board** — for soldered wire connection

---

## Assembly Configurations

### PCB-connected electrode setup

> Uses this HDMI cable for pin compatibility: https://www.amazon.com/Thsucords-Flexible-Support-Blu-ray-Projector/dp/B0CBBWBT2J

| Component | Part |
|-----------|------|
| Acrylic | chamber_top_lid_PCB |
| 3D print | PCB_hold_plate, PIES_Chamber_top_lid_Nikon_PCB |
| PCB fabrication | Motherboard, 4x subunit, Electrode_lid |

### Soldered wire connections

| Component | Part |
|-----------|------|
| Acrylic | Chamber_top_lid_flaps |
| 3D print | PIES_Chamber_top_lid_Nikon |
| PCB fabrication | Motherboard, 4x subunit, Breakout_board (or use existing power supply) |
