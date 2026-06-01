================================================================================
               RENEWABLE ENERGY MICROGRID BATTERY SCHEDULER
================================================================================
Course: Fundamentals of Programming (FoCP) - Fall 2025 Final Project
Author: Muhammad Mohsin (Reg. ID: 540446)
Language: C++ (Standard ISO C++)
Repository Description & Technical Documentation
================================================================================

--------------------------------------------------------------------------------
1. PROJECT OVERVIEW
--------------------------------------------------------------------------------
The Renewable Energy Microgrid Battery Scheduler is a high-fidelity 24-hour 
simulation system designed to optimize energy distribution and storage in a 
solar-powered residential or commercial microgrid. The program models solar 
generation, house/appliance electricity demand, and battery state-of-charge 
(SoC) dynamics to prevent system failures (blackouts) and maximize renewable 
energy utilization.

By analyzing hourly power input and load demand, the software intelligently 
schedules battery operations in 15-minute (quarter-hourly) increments. It helps 
users and engineers visualize the performance of a clean energy system, track 
efficiency metrics, and analyze the viability of a 2220 Wh battery setup.

--------------------------------------------------------------------------------
2. SYSTEM SPECIFICATIONS
--------------------------------------------------------------------------------
* Battery Capacity: 2220 Wh (representing a heavy-duty 180 Ah deep-cycle battery 
  operating at a standard 12V system voltage).
* Simulation Duration: 24 Hours, evaluated in 4 quarter-hour blocks per hour 
  (total of 96 distinct simulation steps).
* Appliance/Load Count: Models the consumption profiles of 5 distinct domestic or 
  industrial devices simultaneously.
* Battery Reserve Boundary (DoD Limit): 30.0% State of Charge (SoC). The scheduler 
  restricts discharging once the battery falls to or below this threshold to protect 
  battery longevity and avoid deep-discharge degradation.

--------------------------------------------------------------------------------
3. STATE DECISION MATRIX
--------------------------------------------------------------------------------
At each 15-minute quarter-hour block, the system calculates the net difference 
between solar power generation and total device demand:
    Net Power (W) = Solar Power (W) - Total Appliance Demand (W)

Based on this net power and the current State of Charge (SoC), one of four states 
is triggered:

  1. CHARGE:
     - Condition: Net Power > 0 (excess solar energy is being produced) AND SoC < 100.0%.
     - Action: The surplus energy is directed to the battery, updating its charge level 
       proportionately (capped at 100% capacity).

  2. DISCHARGE:
     - Condition: Net Power < 0 (generation is insufficient for the load) AND SoC > 30.0%.
     - Action: The deficit is covered by the battery, draining its energy storage.

  3. BLACKOUT:
     - Condition: Net Power < 0 (insufficient solar) AND SoC <= 30.0% (battery depleted).
     - Action: The microgrid cannot sustain the load. A blackout is flagged for the 
       quarter-hour, and the uptime metrics are penalized.

  4. IDLE:
     - Condition: Net Power is perfectly balanced (0) OR the battery is completely full 
       (100.0%) during surplus generation, meaning no energy is exchanged.

--------------------------------------------------------------------------------
4. CODE & DATA ARCHITECTURE
--------------------------------------------------------------------------------
The project utilizes clean, modular procedural programming paradigms in C++:
* 1D Array (`solar[HOURS]`): Stores 24 hourly solar generation levels (Watts) 
  parsed from the input configuration.
* 2D Array (`demand[HOURS][DEVICES]`): Tracks the power demand (Watts) of all 5 
  individual appliances across each of the 24 hours.
* File I/O (`ifstream` & `ofstream`): Handles input/output operations. Reads solar 
  irradiance data and device profiles from an external configuration file and outputs 
  a professional, structured log report.
* Enums (`enum State`): Elegantly represents the operating state of the battery 
  (CHARGE, DISCHARGE, IDLE, BLACKOUT) to ensure state safety.
* Math Optimization (`cmath`): Uses `fmin()` and `fmax()` to model physical charging 
  limits and storage boundaries.

--------------------------------------------------------------------------------
5. INPUT & OUTPUT SCHEMAS
--------------------------------------------------------------------------------
### Input File (`solar_input.txt`):
The simulation consumes a text configuration file containing two major datasets:
  - Line 1: 24 space-separated floating-point values representing solar power output (Watts) 
    from 00:00 (midnight) to 23:00 (11 PM).
  - Line 2: 5 space-separated integer values representing the constant base load (Watts) 
    demanded by the 5 simulated appliances.

### Output Report (`simulation_report.txt`):
Upon running the executable, the program generates a clean, tab-formatted report listing:
  - The precise time (HH:MM format) in 15-minute intervals.
  - Generational power supplied by solar arrays (W) during that interval.
  - Consumed power demanded by local devices (W).
  - The Net Power balance (W).
  - Action taken by the battery controller (CHARGE, DISCHARGE, IDLE, BLACKOUT).
  - The resulting Battery State of Charge (SoC%).
  - Uptime statistics, including total blackout hours and a success flag for achieving 
    100% grid stability.

--------------------------------------------------------------------------------
6. HOW TO COMPILE AND RUN
--------------------------------------------------------------------------------
You can compile and run this project on Windows, macOS, or Linux using any modern 
C++ compiler (e.g., GCC/G++).

1. Clone or download the repository files to your local machine.
2. Open a terminal/command prompt and navigate to the directory containing the files.
3. Compile the C++ source file using g++:
   $ g++ -O3 microgrid_optimiser.cpp -o microgrid_optimiser

4. Ensure that `solar_input.txt` is present in the same directory.
5. Execute the compiled binary:
   - On Windows:
     $ .\microgrid_optimiser.exe
   - On Linux/macOS:
     $ ./microgrid_optimiser

6. Check the console output or open the newly created `simulation_report.txt` file 
   to review the detailed simulation metrics.

================================================================================
               Developed for the Fall 2025 FoCP Semester Project.
================================================================================
