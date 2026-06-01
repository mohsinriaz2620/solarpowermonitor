/*FoCP Fall 2025 Final Project
Muhammad Mohsin - 540446
Renewable Energy Microgrid Battery Scheduler
Simulates 24-hour solar power management with 2220 Wh (180 Ah) battery
*/

#include<iostream>
#include<iomanip> //for file formatting
#include<fstream> //for file handling
#include<cmath>   //for mathematical functions (fmin, fmax)

using namespace std;

const int HOURS = 24; //for 24 hours of the day
const int DEVICES = 5; // I take 5 devices, for example
const int QUARTERS = 4; //four quarters of the hour
const float BATTERY_CAP = 2220.0; //180 Ah - for 12 V = 2220 Wh.

enum State {CHARGE, DISCHARGE, IDLE, BLACKOUT}; //added enum for different states of battery.

int main()
{
    float solar[HOURS]; //Arrays to input devices and hourly data.
    float demand[HOURS][DEVICES];  //2D array for device demand over 24 hours.

    ifstream infile("solar_input.txt"); //take input of data from file
    if (!infile)  // if file has errors/file does not exist:
    {
        cout << "ERROR: Cannot open solar_input.txt\n";
        cout << "Make sure the file is in the same folder.\n";
        return 1;
    }

   
    for (int i = 0; i < HOURS; i++)  // Read 24 solar values, spread over lines in the file
    {
        infile >> solar[i];
    }

   
    int base_load[DEVICES];
    for (int d = 0; d < DEVICES; d++)  // Read 5 device loads (they are going to be same for all hours)
    {
        infile >> base_load[d];
    }

   
    for (int h = 0; h < HOURS; h++)  // Copy base load to all 24 hours
    {
        for (int d = 0; d < DEVICES; d++)
        {
            demand[h][d] = base_load[d];
        }
    }
    infile.close();

    ofstream outfile("simulation_report.txt"); //create output file
    if (!outfile) // if file creation has errors
    {
        cout << "ERROR: Cannot create simulation_report.txt\n"; //file creation error
        return 1;
    }

    float charge_state = 30.0;  //at 0700 or 0800 hrs the charge state is roundabout ~30%.
    int blackout_quads = 0;   //if battery percentage is less than 30%, add 1 to blackout counter.

    cout << fixed << setprecision(1);                    //set decimal precision to 1 place.
    outfile << fixed << setprecision(1); //for output file
    cout << "\n=== MICROGRID 24-HOUR SIMULATION ===\n\n";
    outfile << "\n=== MICROGRID 24-HOUR SIMULATION ===\n\n"; //for output file
    cout << "Time       Solar(W)   Load(W)    Net(W)     Action       SOC(%)\n";
    outfile << "Time       Solar(W)   Load(W)    Net(W)     Action       SOC(%)\n"; //for output file   
    cout << "---------------------------------------------------------------\n";
    outfile << "---------------------------------------------------------------\n"; //for output file
   for (int hour = 0; hour < HOURS; hour++) 
    {
        float hour_solar = solar[hour];
        float hour_load = 0;

                                             
        for (int d = 0; d < DEVICES; d++) // Add load of all 5 devices for this particular hour
        {
            hour_load += demand[hour][d];
        }

    for(int quarter = 0; quarter < QUARTERS; quarter++)
      {
      //Core Logic here to calculate net charge, state of charge, action, etc.
      float quarter_solar = hour_solar / 4.0;
      float quarter_load  = hour_load  / 4.0;
      float net_charge = quarter_solar - quarter_load;  //remaining charge after solar power produced minus the power usage that quarter.
      State action;
    
     if (net_charge > 0 && charge_state < 100.0) 
     {
                // CHARGE: extra solar power available and battery is not full
                action = CHARGE;
                float space_in_battery = (100.0 - charge_state) / 100.0 * BATTERY_CAP;
                float available_energy = net_charge * 0.25;                 // 15 min = 0.25 hours
                float actual_charge = fmin(available_energy, space_in_battery);
                charge_state += (actual_charge / BATTERY_CAP) * 100.0;
    }
     else if (net_charge < 0 && charge_state > 30.0) 
     {
                // DISCHARGE: need power and battery has enough power
                action = DISCHARGE;
                float energy_needed = -net_charge * 0.25; //negative net charge means we need this much energy
                float available_energy = (charge_state - 30.0) / 100.0 * BATTERY_CAP;
                float actual_discharge = fmin(energy_needed, available_energy);
                charge_state -= (actual_discharge / BATTERY_CAP) * 100.0;
   }
     else if (net_charge < 0 && charge_state <= 30.0) 
      {
                // BLACKOUT: need power but battery is too low
                action = BLACKOUT;
                blackout_quads++;
       }
     else
    {
                // IDLE: balanced or no activity
                action = IDLE;
    }

           
            cout << setw(2) << hour << ":" << setw(2) << (quarter * 15) << "    "  // Display one line per quarter-hour
                 << setw(8) << quarter_solar << " "
                 << setw(8) << quarter_load << " "
                 << setw(8) << net_charge << "   ";

            outfile << setw(2) << hour << ":" << setw(2) << (quarter * 15) << "    "  //for output file
                    << setw(8) << quarter_solar << " "
                    << setw(8) << quarter_load << " "
                    << setw(8) << net_charge << "   ";

          string action_str;
            if (action == CHARGE)       
             action_str = "CHARGE   ";
            else if (action == DISCHARGE) 
             action_str = "DISCHARGE";
            else if (action == BLACKOUT)  
             action_str = "BLACKOUT ";
            else                          
             action_str = "IDLE     ";

            cout << action_str << " " << setw(6) << charge_state << "%\n"; // Print to screen
            outfile << action_str << " " << setw(6) << charge_state << "%\n";  // Print to file 
     }
    }

 int uptime_hours = HOURS - (blackout_quads / QUARTERS);

    cout << "---------------------------------------------------------------\n";
    outfile << "---------------------------------------------------------------\n"; //for output file
    cout << "Simulation complete!\n";
    outfile << "Simulation complete!\n"; //for output file
    cout << "Final battery level: " << charge_state << "%\n";
    outfile << "Final battery level: " << charge_state << "%\n"; //for output file
    cout << "Uptime: " << uptime_hours << " out of 24 hours\n";
    outfile << "Uptime: " << uptime_hours << " out of 24 hours\n"; //for output file

    if (uptime_hours == 24) 
    {
        cout << "SUCCESS: 100% uptime achieved!\n";
        outfile << "SUCCESS: 100% uptime achieved!\n"; //for output file
    }
    else 
    {
        cout << "Blackouts occurred in " << (blackout_quads / QUARTERS) << " hours.\n";
        outfile << "Blackouts occurred in " << (blackout_quads / QUARTERS) << " hours.\n"; //for output file
    }
outfile.close();  //Closing the file
    cout << "\nReport saved to 'simulation_report.txt'\n";
return 0;
}

