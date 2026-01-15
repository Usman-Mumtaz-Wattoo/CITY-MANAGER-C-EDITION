# CITY MANAGER: C++ EDITION

#### 1. Website :  [CLICK HERE](https://city-manager-c-edition.netlify.app/)
*(Note: This link features a web-based user interface with visual menus and buttons, offering a more user-friendly experience compared to the console version.)*

#### 2. GitHub Repository : [CLICK HERE](https://github.com/Usman-Mumtaz-Wattoo/CITY-MANAGER-C-EDITION)

#### 3. Video Demo : [CLICK HERE](https://youtu.be/Q0nICQ9s5dg)

---

### Description

**City Manager** is a complex, text-based **strategy and simulation game** built entirely in C++. In this project, the user takes on the high-pressure role of a **City Mayor** who is responsible for managing a sprawling metropolis divided into five unique districts. The primary goal of the game is not just to survive, but to thrive: you must grow the population, ensure the citizens are happy, maintain a healthy financial budget, and prevent the city from collapsing under the weight of crime or pollution.

I created this project because I wanted to challenge myself to build a software system where **multiple variables interact with each other**. For example, raising taxes might increase the treasury, but it will immediately lower happiness and slow down population growth. Additionally, I wanted to move away from simple "turn-based" input and create a **real-time experience** where the game world progresses automatically if the player acts too slowly.

### Key Gameplay Features

The game runs on a strict **Real-Time Loop**. Every "month" in the game lasts exactly **120 seconds**. The player must make all their strategic decisions—building, budgeting, and policy-making—within this time limit. Once the time is up, the month automatically ends, taxes are collected, and expenses are deducted.

* **District Management System:** The city is split into 5 distinct areas (e.g., **Downtown**, **Industrial Zone**, **Suburban Hills**). Each district tracks its own independent stats for **Crime**, **Health**, **Education**, and **Traffic**.
* **Infrastructure & Utilities:** This is the most critical part of the simulation. The player must manage **Power (MW)** and **Water (GL)** supplies. If the demand from the population exceeds the supply, a crisis occurs, stopping growth and causing health issues. You must build **Wind Turbines**, **Coal Plants**, or **Water Pumps** to keep the lights on.
* **Dynamic Policy System:** The player can enact city-wide laws such as a **"Strict Curfew"** (reduces crime but lowers happiness) or **"Green Energy Subsidies"** (reduces pollution but costs money). This adds a layer of political strategy.
* **Emergency Response:** The game includes a "Command Center" where you manage **Fire Trucks**, **Riot Squads**, and **Ambulances**. If a disaster strikes (like a Massive Fire), you must deploy these units instantly.
* **Banking & Economy:** If the city goes bankrupt, the game ends. Players can take out **Bank Loans**, but they must pay monthly interest, affecting their long-term budget.

### Project Structure & Files

This project is organized into three specific files to ensure data persistence and tracking.

**1. main.cpp**
This is the core source code file containing over 800 lines of C++ logic. It houses the `main` function, the `gameloop`, and all management functions.
* It handles the **State Machine** (switching between the Start Screen, Game Loop, and Stats Screen).
* It contains the logic for **random event generation**, ensuring no two playthroughs are the same.

**2. savegame.txt**
This text file is generated when the user selects **"Save Game"** from the menu.
* It acts as a database that stores every single variable: current money, date, population per district, building counts, and active policies.
* This allows the user to close the program and resume exactly where they left off by choosing **"Load Game"**.

**3. city_log.txt**
This is an automated **history log**.
* At the end of every in-game month, the software writes a summary line to this file (Date, Money, Avg Happiness).
* This allows the player to open the text file later and analyze how their city performed over the years.

### Design Choices & Technical Implementation

I made several specific engineering choices to meet the project requirements and demonstrate advanced C++ concepts:

**1. Dynamic Memory Allocation:**
Instead of using only standard arrays, I utilized **Pointers** and **Dynamic Memory** (e.g., `float *education = new float[NUM_DISTRICTS];`). This was a deliberate design choice to demonstrate manual memory management. I also ensured to include `delete []` at the end of the program to free this memory and prevent **memory leaks**.

**2. Chrono Library for Real-Time Logic:**
To create the tension of a real-time simulation, I used the `<chrono>` and `<thread>` libraries. Inside the `gameloop`, the code calculates `elapsedSeconds` continuously.
* *Why this choice?* Standard `cin` pauses the program. By using timestamps, I force the game to update the world state (collect taxes, trigger events) every 2 minutes regardless of user input, making the simulation feel "alive."

**3. STL Vectors and Algorithms:**
In the `rankDistricts()` function, I used the **Standard Template Library (STL)** `vector` and `sort`.
* The program pairs the district names with their scores and sorts them automatically. This is much more efficient than writing a manual sorting algorithm and makes the code cleaner and more readable.

**4. Input Validation & Error Handling:**
A major issue in console apps is the program crashing if a user types a letter instead of a number. I implemented `cin.fail()` checks and `cin.ignore()` inside `do-while` loops for every menu. This ensures the program is **robust** and will not crash due to invalid user input.

**5. Object-Oriented Functional Design:**
I broke the code down into **modular functions** (e.g., `manageTransport`, `manageFinance`, `updatePopulation`). Each function handles a specific aspect of the simulation. This modularity makes the code easier to debug and allows for features (like the graphical web version linked above) to be expanded in the future.