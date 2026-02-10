# Electrical Load Monitoring (C++)

This is a console C++ program that:

- Lets you register appliances (name, watts, hours/day)
- Calculates energy used in kWh
- Calculates electricity cost using a tariff you enter
- Saves/loads data using text files

## Files

- main.cpp = the program
- appliances.txt = saved appliances (auto loaded on startup)
- billing_summary.txt = saved billing reports (optional)

## Formula used

kWh/day = (Watts ÷ 1000) × Hours/day

Example:
TV 100W used for 4 hours:
(100/1000) × 4 = 0.4 kWh/day

## How to run (Windows / Mac / Linux)

1. Open terminal in the project folder.
2. Compile:
   g++ -std=c++17 -o app main.cpp
3. Run:
   - Windows: app.exe
   - Mac/Linux: ./app

## Appliances file format

Each line in appliances.txt looks like:
Name|Watts|Hours

Example:
Fridge|150|8
TV|100|4
Fan|60|10
