#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<fstream>
#include<limits>
#include<chrono>
#include<thread>
#include<ctime>
#include<cmath>
#include<vector>
#include<algorithm>
#include<string>
#include<random>

using namespace std;

void gameover();
void gameloop();
int startscreen();
int levelselect();
void gameguide();
void credit();
void stats();
void overview(int choice);
void Difficultylevel(int choice);
void collectTax();
void buildingexpenses();
void manageDistricts();
void updateDistricts();
void manageInfrastructure();
void managePolicies();
void manageTransport();
void consultAdvisors();
void calculateHappiness();
void calculateResources();
void updatePopulation();
void rankDistricts();
void saveLog();
void saveGame();
void loadGame();
void cheatMenu();
void randomEvent();
void manageFinance();
void emergencyCommandCenter();
void triggerDisaster(string type, int districtIdx);

const int NUM_DISTRICTS = 5;
string districtName[NUM_DISTRICTS];
long population[NUM_DISTRICTS];
float crimeRate[NUM_DISTRICTS];
float health[NUM_DISTRICTS];
int roadQuality[NUM_DISTRICTS];
float *education=new float[NUM_DISTRICTS];
float *tax=new float[NUM_DISTRICTS];
float *pollution=new float[NUM_DISTRICTS];
float *happiness=new float[NUM_DISTRICTS];
float *traffic=new float[NUM_DISTRICTS];

int powerDemand = 0;
int waterDemand = 0;
int powerSupply = 0;
int waterSupply = 0;

float avgCrime=0;
float avgHealth=0;
float avgEducation=0;
float avgHappiness=0;
float avgPollution=0;
float avgTraffic=0;

string difficulty[3]={"EASY MODE","NORMAL MODE","HARD MODE"};
int policeStations[NUM_DISTRICTS];
int hospitals[NUM_DISTRICTS];
int schools[NUM_DISTRICTS];
int powerPlants = 0;
int waterPumps = 0;
int parks[NUM_DISTRICTS];
int busStops[NUM_DISTRICTS];
bool policyCurfew = false;
bool policyFreeLunch = false;
bool policyGreenEnergy = false;
bool policyPoliceState = false;
bool policyTourism = false;
bool policyCarFree = false;

float Money = 0;
int currentMonth = 1;
int currentYear = 2026;
int difficultychoice=0;
bool isGameOver = false;
bool returnToMenu = false;
float taxMultiplier = 1.0;
float expenseMultiplier = 1.0;
float crimeMultiplier = 1.0;
float serviceMultiplier = 1.0;
double loanAmount = 0;
double interestRate = 0.05; 
int creditScore = 100;
int fireTrucks = 2;
int riotPolice = 1;
int ambulances = 2;
int availableFireTrucks, availableRiotPolice, availableAmbulances;

void calculateResources(){
    powerDemand = 0;
    waterDemand = 0;
    for(int i=0; i<NUM_DISTRICTS; i++){
        powerDemand += (population[i] / 200); 
        waterDemand += (population[i] / 150);
        if(policeStations[i] > 0)
         powerDemand += (policeStations[i] * 5);
        if(hospitals[i] > 0)
         {
             powerDemand += (hospitals[i] * 10);
            waterDemand += (hospitals[i] * 10); 
        }
        if(schools[i] > 0)
         {
             powerDemand += (schools[i] * 3);
              waterDemand += (schools[i] * 2); 
            }
    }
}

void updatePopulation(){
    calculateResources();
    bool powerShortage = (powerSupply < powerDemand);
    bool waterShortage = (waterSupply < waterDemand);
    for(int i=0;i<NUM_DISTRICTS;i++){
        float growth = (health[i] + education[i] + happiness[i]) / 350;
        if(powerShortage)
         growth -= 0.08;
        if(waterShortage)
         growth -= 0.10;
        if(pollution[i] > 70)
         growth -= 0.05;
        if(crimeRate[i] > 40)
         growth -= 0.05;
        if(traffic[i] > 80)
         growth -= 0.02;
        population[i] += round(population[i] * growth);
        if(population[i] < 0)
        population[i] = 0;
    }
}

void rankDistricts(){
    vector<pair<float,string>> ranking;
    for(int i=0;i<NUM_DISTRICTS;i++){
        float score = health[i] + education[i] + happiness[i] - crimeRate[i] - pollution[i] - traffic[i];
        ranking.push_back({score,districtName[i]});
    }
    sort(ranking.begin(), ranking.end(), greater<>());
    cout << "\n--- DISTRICT RANKINGS ---\n";
    for(auto &r : ranking)
    cout << r.second << " Score: " << r.first << endl;
}

void saveLog(){
    ofstream file("city_log.txt", ios::app);
    if(file.is_open()){
        file << currentMonth << "/" << currentYear
        << " Money:" << Money
        << " Crime:" << avgCrime
        << " Health:" << avgHealth
        << " Edu:" << avgEducation 
        << " Hap:" << avgHappiness 
        << " Pow:" << powerSupply << "/" << powerDemand
        << " Wat:" << waterSupply << "/" << waterDemand << endl;
        file.close();
    }
}

void triggerDisaster(string type, int districtIdx) {
    availableFireTrucks = fireTrucks;
    availableRiotPolice = riotPolice;
    
    cout << "\n\n**************************************************\n";
    cout << "!!! ALERT: " << type << " reported in " << districtName[districtIdx] << " !!!\n";
    cout << "**************************************************\n";
    
    int deploy;
    if (type == "Massive Fire") {
        cout << "Command: How many Fire Trucks to deploy? (Available: " << availableFireTrucks << "): ";
        cin >> deploy;
        if (deploy >= 2 && deploy <= availableFireTrucks) {
            cout << ">> SUCCESS: The fire was extinguished. Minimal damage ($2,000 cost).\n";
            Money -= 2000;
        } else 
        {
            cout << ">> FAILURE: Not enough units! The fire spread.\n";
            cout << ">> DAMAGES: -$30,000 | Happiness -20 | Pop -500\n";
            Money -= 30000;
            happiness[districtIdx] -= 20;
            population[districtIdx] -= 500;
        }
    } else if (type == "Civil Riot") {
        cout << "Command: How many Riot Squads to deploy? (Available: " << availableRiotPolice << "): ";
        cin >> deploy;
        if (deploy >= 1 && deploy <= availableRiotPolice) {
            cout << ">> SUCCESS: The crowd was dispersed peacefully.\n";
        } else 
        {
            cout << ">> FAILURE: The mob overwhelmed local police.\n";
            cout << ">> DAMAGES: Crime +25% | Shops Looted (-$15,000)\n";
            crimeRate[districtIdx] += 25;
            Money -= 15000;
        }
    }
    cout << "(Press Enter to continue)";
    cin.ignore(); 
    cin.get();
}

void randomEvent() {
    static default_random_engine engine(time(0));
    uniform_int_distribution<int> dist(1, 25);
    int event = dist(engine);

    cout << "\n--------------------------------------------------\n";

    if (event == 1) {
        Money += 50000;
        cout << "EVENT: Foreign Investment Boost! +$50,000\n";
    } 
    else if (event == 2) 
    {
        triggerDisaster("Massive Fire", rand() % NUM_DISTRICTS);
    } 
    else if (event == 3) {
        triggerDisaster("Civil Riot", rand() % NUM_DISTRICTS);
    } 
    else if (event == 4) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            health[i] += 5;
        cout << "EVENT: New Vaccine Rollout! Health improved.\n";
    } 
    else if (event == 5) {
        waterSupply += 100;
        cout << "EVENT: Heavy Rains fill the reservoirs. Water Supply +100 GL.\n";
    } 
    else if (event == 6) {
        powerSupply -= 50;
        cout << "EVENT: Power Grid Malfunction! Supply dropped by 50 MW.\n";
    } 
    else if (event == 7) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            education[i] += 4;
        cout << "EVENT: Tech Boom! Education effectiveness up.\n";
    } 
    else if (event == 8) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            happiness[i] -= 10;
        cout << "EVENT: Political Scandal! Public Happiness drops.\n";
    } 
    else if (event == 9) {
        Money -= 15000;
        cout << "EVENT: Infrastructure Rust. Maintenance costs $15,000.\n";
    } 
    else if (event == 10) {
        happiness[0] += 15;
        cout << "EVENT: A famous singer held a concert in " << districtName[0] << ". Happiness up!\n";
    } 
    else if (event == 11) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            pollution[i] += 10;
        cout << "EVENT: Factory Smog! Pollution levels rising dangerously.\n";
    } 
    else if (event == 12) {
        Money -= 30000;
        cout << "EVENT: Embezzlement discovered! $30,000 stolen from treasury.\n";
    } 
    else if (event == 13) {
        education[2] += 8;
        cout << "EVENT: University opens in " << districtName[2] << ". Education boost.\n";
    } 
    else if (event == 14) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            health[i] -= 5;
        cout << "EVENT: Pandemic Warning! Health drops across the city.\n";
    } 
    else if (event == 15) {
        Money += 20000;
        cout << "EVENT: Federal Grant received for city planning. +$20,000\n";
    } 
    else if (event == 16) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            traffic[i] += 10;
        cout << "EVENT: Major road accident! Traffic jams everywhere.\n";
    } 
    else if (event == 17) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            traffic[i] -= 10;
        cout << "EVENT: Holiday weekend! Roads are clear.\n";
    } 
    else if (event == 18) {
        powerSupply += 50;
        cout << "EVENT: Sunny weather boosts solar panels. Power +50 MW.\n";
    } 
    else if (event == 19) {
        schools[1] += 1;
        cout << "EVENT: Philanthropist builds a school in " << districtName[1] << ".\n";
    } 
    else if (event == 20) 
    {
        for (int i = 0; i < NUM_DISTRICTS; i++) 
            roadQuality[i] -= 10;
        cout << "EVENT: Severe winter damaged roads. Road Quality down.\n";
    } 
    else if (event == 21) {
        population[0] += 1000;
        cout << "EVENT: Migration wave! 1000 people moved to " << districtName[0] << ".\n";
    } 
    else if (event == 22) {
        population[3] -= 500;
        cout << "EVENT: Brain drain. 500 people left " << districtName[3] << ".\n";
    } 
    else if (event == 23) {
        taxMultiplier += 0.1;
        cout << "EVENT: Economic Boom! Tax revenue increased slightly.\n";
    } 
    else if (event == 24) {
        taxMultiplier -= 0.1;
        cout << "EVENT: Recession! Tax revenue decreased.\n";
    } 
    else {
        cout << "EVENT: A quiet month. No major news.\n";
    }

    cout << "--------------------------------------------------\n";
}

void gameover(){
    if(Money<= -50000 && loanAmount > 50000)
    {
        overview(difficultychoice);
        stats();
        cout<<"Bankruptcy! You are drowning in debt... Game Over.";
        isGameOver=true;
    }
    else if(avgCrime > 60)
    {
        overview(difficultychoice);
        stats();
        cout<<"Anarchy! Crime Rate Too High... Game Over.";
        isGameOver=true;
    }
    else if(avgHealth<20)
    {
        overview(difficultychoice);
        stats();
        cout<<"Plague! Average Health Too Low... Game Over.";
        isGameOver=true;
    }
    else if(avgHappiness < 15)
    {
        overview(difficultychoice);
        stats();
        cout<<"Revolution! The people have overthrown you... Game Over.";
        isGameOver=true;
    }
}

void manageDistricts() {
    int distChoice = 0;
    int actionChoice = 0;

    cout << "\n\n-- SELECT A DISTRICT TO MANAGE --\n";
    cout << "---------------------------------\n";

    for(int i=0; i<NUM_DISTRICTS; i++) {
        cout << " [" << i+1 << "] " << districtName[i] << " (Pop: " << population[i] << ")\n";
    }

    cout << " [6] CANCEL / BACK\n";
    cout << "---------------------------------\n";
    do{
        cout << "Choice: ";
        cin >> distChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(distChoice<1||distChoice>6){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(distChoice<1||distChoice>6);
    if(distChoice < 1 || distChoice > 5)
        return;

    int idx = distChoice - 1;

    cout << "\n\n-- MANAGING: " << districtName[idx] << " --\n";
    cout << "Treasury: $" << Money << "\n";
    cout << "Stats: Crime " << crimeRate[idx] << "% | Health " << health[idx] << "% | Edu " << education[idx] << "%\n";
    cout << "Env: Pollution " << pollution[idx] << "% | Happiness " << happiness[idx] << "% | Traffic " << traffic[idx] << "%\n";
    cout << "Buildings: Police: " << policeStations[idx] << " | Hospitals: " << hospitals[idx] << " | Schools: " << schools[idx] << " | Parks: " << parks[idx] << "\n";
    cout << "------------------------------------------\n";
    cout << " [1] Build Police Station ($15,000)\n";
    cout << " [2] Build Hospital ($25,000)\n";
    cout << " [3] Build School ($12,000)\n";
    cout << " [4] Build Public Park ($8,000)\n";
    cout << " [5] Change Tax Rate (Current: " << tax[idx] << "%)\n";
    cout << " [6] Back to Menu\n";

    do{
        cout << "Select Action: ";
        cin >> actionChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(actionChoice<1||actionChoice>6){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(actionChoice<1||actionChoice>6);
    switch(actionChoice) {
        case 1:
            if(Money >= 15000) {
                Money -= 15000;
                policeStations[idx]++;
                cout << ">> SUCCESS: Police Station Built!\n";
            }
            else {
                cout << ">> FAILED: Insufficient Funds!\n";
            }
            break;

        case 2:
            if(Money >= 25000) {
                Money -= 25000;
                hospitals[idx]++;
                cout << ">> SUCCESS: Hospital Built!\n";
            }
            else {
                cout << ">> FAILED: Insufficient Funds!\n";
            }
            break;

        case 3:
            if(Money >= 12000) {
                Money -= 12000;
                schools[idx]++;
                cout << ">> SUCCESS: School Built!\n";
            }
            else {
                cout << ">> FAILED: Insufficient Funds!\n";
            }
            break;

        case 4:
            if(Money >= 8000) {
                Money -= 8000;
                parks[idx]++;
                happiness[idx] += 5;
                pollution[idx] -= 2;
                cout << ">> SUCCESS: Public Park Built!\n";
            }
            else {
                cout << ">> FAILED: Insufficient Funds!\n";
            }
            break;

        case 5:
            cout << "Enter new tax rate (1-20%): ";
            float newTax;
            do{
               cout << "Select Action: ";
               cin >> newTax;
               if(cin.fail()){
                   cin.clear();
                   cin.ignore(numeric_limits<streamsize>::max(), '\n');
                   cout<<"Please enter a valid option."<<endl;
                   continue;
               }
               if(newTax<1){
                   cout<<"Please enter a valid option."<<endl;
               }
               }while(newTax<1);

            if(newTax >= 1 && newTax <= 20) {
                tax[idx] = newTax;
                cout << ">> SUCCESS: Tax rate updated.\n";
            }
            else {
                cout << ">> FAILED: Invalid tax rate (Must be 1-20).\n";
            }
            break;
    }
}


void manageInfrastructure() {
    calculateResources();

    int infraChoice = 0;

    cout << "\n\n-- INFRASTRUCTURE MANAGEMENT --\n";
    cout << "POWER: Generated: " << powerSupply << " MW | Required: " << powerDemand << " MW\n";

    if(powerSupply < powerDemand)
        cout << "WARNING: POWER DEFICIT! Build power plants immediately!\n";
    else
        cout << "STATUS: Power Grid Stable.\n";

    cout << "WATER: Generated: " << waterSupply << " GL | Required: " << waterDemand << " GL\n";

    if(waterSupply < waterDemand)
        cout << "WARNING: WATER DEFICIT! Build pumps immediately!\n";
    else
        cout << "STATUS: Water Supply Stable.\n";

    cout << "---------------------------------\n";
    cout << " [1] Build Coal Power Plant ($40,000) [+150 Power, +High Pollution]\n";
    cout << " [2] Build Wind Turbine ($60,000) [+50 Power, No Pollution]\n";
    cout << " [3] Build Water Pumping Station ($30,000) [+100 Water]\n";
    cout << " [4] Nuclear Power Plant ($150,000) [+500 Power, Low Pollution]\n";
    cout << " [5] Desalination Plant ($80,000) [+300 Water, High Upkeep]\n";
    cout << " [6] Back\n";
    do{
        cout << "Choice: ";
        cin >> infraChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(infraChoice<1||infraChoice>6){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(infraChoice<1||infraChoice>6);

    switch(infraChoice) {
        case 1:
            if(Money >= 40000) {
                Money -= 40000;
                powerSupply += 150;
                powerPlants++;

                for(int i = 0; i < NUM_DISTRICTS; i++)
                    pollution[i] += 8;

                cout << ">> Constructed Coal Plant.\n";
            }
            else {
                cout << ">> Not enough money.\n";
            }
            break;

        case 2:
            if(Money >= 60000) {
                Money -= 60000;
                powerSupply += 50;
                powerPlants++;
                cout << ">> Constructed Wind Turbine.\n";
            }
            else {
                cout << ">> Not enough money.\n";
            }
            break;

        case 3:
            if(Money >= 30000) {
                Money -= 30000;
                waterSupply += 100;
                waterPumps++;
                cout << ">> Constructed Water Pump.\n";
            }
            else {
                cout << ">> Not enough money.\n";
            }
            break;

        case 4:
            if(Money >= 150000) {
                Money -= 150000;
                powerSupply += 500;
                powerPlants++;
                cout << ">> Constructed Nuclear Plant.\n";
            }
            else {
                cout << ">> Not enough money.\n";
            }
            break;

        case 5:
            if(Money >= 80000) {
                Money -= 80000;
                waterSupply += 300;
                waterPumps++;
                cout << ">> Constructed Desalination Plant.\n";
            }
            else {
                cout << ">> Not enough money.\n";
            }
            break;
    }
}


void manageTransport() {
    int transChoice = 0;
    int dist = 0;
    cout << "\n\n-- TRANSPORT DEPARTMENT --\n";
    cout << "Average Traffic Congestion: " << avgTraffic << "%\n";
    cout << "Manage Roads and Bus Lines to reduce traffic.\n";
    cout << "---------------------------------\n";
    cout << " [1] Repair Roads in a District ($5,000)\n";
    cout << " [2] Build Bus Stop in a District ($8,000)\n";
    cout << " [3] Back\n";
    do{
        cout << "Choice: ";
        cin >> transChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(transChoice<1||transChoice>3){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(transChoice<1||transChoice>3);

    if(transChoice == 1) {
        do{
        cout << "Select District (1-5): ";
        cin >> dist;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(dist<1||dist>5){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(dist<1||dist>5);
        if(dist >= 1 && dist <= 5) {
            if(Money >= 5000) {
                Money -= 5000;
                roadQuality[dist-1] = 100;
                traffic[dist-1] -= 10;
                cout << ">> Roads repaired!\n";
            }
            else {
                cout << ">> Insufficient Funds.\n";
            }
        }
    }
    else if(transChoice == 2) {
        do{
        cout << "Select District (1-5): ";
        cin >> dist;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(dist<1||dist>5){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(dist<1||dist>5);

        if(dist >= 1 && dist <= 5) {
            if(Money >= 8000) {
                Money -= 8000;
                busStops[dist-1]++;
                traffic[dist-1] -= 5;
                cout << ">> Bus Stop built!\n";
            }
            else {
                cout << ">> Insufficient Funds.\n";
            }
        }
    }
}


void managePolicies() {
    int polChoice = 0;
    cout << "\n\n-- CITY ORDINANCES & POLICIES --\n";
    cout << "Enacting policies costs monthly upkeep but provides bonuses.\n";
    cout << "---------------------------------\n";
    cout << " [1] Strict Curfew (Reduces Crime, Reduces Happiness) [Active: ";
    if(policyCurfew)
        cout << "YES";
    else
        cout << "NO";
    cout << "]\n";
    
    cout << " [2] Free School Lunch (Increases Health & Edu, Costs Money) [Active: ";
    if(policyFreeLunch)
        cout << "YES";
    else
        cout << "NO";
    cout << "]\n";
    
    cout << " [3] Green Energy Subsidy (Reduces Pollution, Costs Money) [Active: ";
    if(policyGreenEnergy)
        cout << "YES";
    else
        cout << "NO";
    cout << "]\n";
    
    cout << " [4] Police State (Greatly Reduces Crime, Greatly Reduces Happiness) [Active: ";
    if(policyPoliceState)
        cout << "YES";
    else
        cout << "NO";
    cout << "]\n";
    
    cout << " [5] Tourism Board (Increases Money, Increases Crime) [Active: ";
    if(policyTourism)
        cout << "YES";
    else
        cout << "NO";
    cout << "]\n";
    
    cout << " [6] Car-Free Sundays (Reduces Traffic, Reduces Happiness) [Active: ";
    if(policyCarFree)
        cout << "YES";
    else
        cout << "NO";
    cout << "]\n";
    
    cout << " [7] Back\n";

    do{
        cout << "Toggle Policy: ";
        cin >> polChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(polChoice<1||polChoice>7){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(polChoice<1||polChoice>7);

    switch(polChoice) {
        case 1:
            policyCurfew = !policyCurfew;
            break;

        case 2:
            policyFreeLunch = !policyFreeLunch;
            break;

        case 3:
            policyGreenEnergy = !policyGreenEnergy;
            break;

        case 4:
            policyPoliceState = !policyPoliceState;
            break;

        case 5:
            policyTourism = !policyTourism;
            break;

        case 6:
            policyCarFree = !policyCarFree;
            break;
    }

    cout << "Policy updated.\n";
}


void consultAdvisors() {
    int advChoice = 0;
    cout << "\n\n-- CABINET OF ADVISORS --\n";
    cout << " [1] Financial Advisor\n";
    cout << " [2] Public Safety Commissioner\n";
    cout << " [3] Environmental Expert\n";
    cout << " [4] City Planner\n";
    cout << " [5] Back\n";
    do{
        cout << "Who do you want to speak to? ";
        cin >> advChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(advChoice<1||advChoice>5){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(advChoice<1||advChoice>5);
    switch(advChoice) {
        case 1:
            cout << "\n[Finance]: 'Boss, we have $" << Money << ". Debt is at $" << loanAmount << ".'\n";
            cout << "[Finance]: 'Remember, we pay interest on loans every month!'\n";
            break;
        case 2:
            cout << "\n[Police]: 'Crime is at " << avgCrime << "%.'\n";
            cout << "[Police]: 'Ensure our Riot Squads are ready in case of civil unrest.'\n";
            break;
        case 3:
            cout << "\n[Eco]: 'Pollution is " << avgPollution << "%.'\n";
            cout << "[Eco]: 'Parks and Wind Turbines are your best friends here.'\n";
            break;
        case 4:
            calculateResources();
            cout << "\n[Planner]: 'Power Demand: " << powerDemand << ". Water Demand: " << waterDemand << ".'\n";
            cout << "[Planner]: 'If you don't meet demand, growth stops and people get sick.'\n";
            break;
    }
    cout << "\nPress Enter to return.";
    cin.ignore();
    cin.get();
    cin.ignore(1000,'\n');
}

void calculateHappiness() {
    avgHappiness = 0;
    avgPollution = 0;
    avgTraffic = 0;

    for(int i=0; i<NUM_DISTRICTS; i++) {
        float base = 100;

        base -= (tax[i] * 2.5);
        base -= (crimeRate[i] * 1.2);
        base -= (pollution[i] * 0.8);
        base -= (traffic[i] * 0.5);

        if(loanAmount > 100000)
            base -= 10;

        if(health[i] > 80)
            base += 5;

        if(education[i] > 80)
            base += 5;

        if(policyCurfew)
            base -= 10;

        if(policyPoliceState)
            base -= 25;
        if(policyFreeLunch)
            base += 5;
        if(policyCarFree)
            base -= 5;
        if(parks[i] > 0)
            base += (parks[i] * 4);
        if(roadQuality[i] < 50)
            base -= 5;
        if(base > 100)
            base = 100;
        if(base < 0)
            base = 0;
        happiness[i] = base;
        avgHappiness += happiness[i];
        avgPollution += pollution[i];
        avgTraffic += traffic[i];
    }

    avgHappiness /= NUM_DISTRICTS;
    avgPollution /= NUM_DISTRICTS;
    avgTraffic /= NUM_DISTRICTS;
}

void manageFinance() {
    int finChoice;

    cout << "\n\n--- CITY CENTRAL BANK ---\n";
    cout << "Treasury: $" << fixed << setprecision(0) << Money << endl;
    cout << "Debt: $" << loanAmount << " (Interest Rate: " << (interestRate * 100) << "%)\n";
    cout << "Credit Score: " << creditScore << "\n";
    cout << " [1] Take Loan (Max $50k)\n";
    cout << " [2] Repay Debt\n";
    cout << " [3] Back\n";

    do{
        cout << "Choice : ";
        cin >> finChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(finChoice<1||finChoice>3){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(finChoice<1||finChoice>3);
    if(finChoice == 1) {
        if(loanAmount > 50000) {
            cout << ">> Debt too high for new loan.\n";
        }
        else {
            double amt;
            do{
                cout << "Amount : ";
                cin >> amt;
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout<<"Please enter a valid option."<<endl;
                    continue;
                }
                if(amt<1){
                    cout<<"Amount Cannot be negative."<<endl;
                }
                }while(amt<1);

            if(amt > 0 && amt <= 50000) {
                loanAmount += amt;
                Money += amt;
                creditScore -= 10;
                cout << ">> Funds Deposited.\n";
            }
        }
    }
    else if(finChoice == 2) {
        double amt;
        do{
                cout << "Amount : ";
                cin >> amt;
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout<<"Please enter a valid option."<<endl;
                    continue;
                }
                if(amt<1){
                    cout<<"Amount Cannot be negative."<<endl;
                }
                }while(amt<1);
        if(loanAmount>=amt){
            if(amt > 0 && Money >= amt) {
            Money -= amt;
            loanAmount -= amt;
            creditScore += 5;
            cout << ">> Debt Repaid.\n";
        }
        else {
            cout << ">> Insufficient Funds.\n";
        }
        }
        else {
            cout<<"Amount cannot be greater then debt.";
        }
        
    }
}

void emergencyCommandCenter() {
    cout << "\n\n--- EMERGENCY HQ ---\n";
    cout << "Units: Fire Trucks: " << fireTrucks << " | Riot Squads: " << riotPolice << " | Ambulances: " << ambulances << endl;
    cout << " [1] Buy Fire Truck ($10k)\n";
    cout << " [2] Buy Riot Squad ($12k)\n";
    cout << " [3] Buy Ambulance ($8k)\n";
    cout << " [4] Back\n";

    int choice;
    do{
        cout << "Choice : ";
        cin >> choice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(choice<1||choice>4){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(choice<1||choice>4);

    switch(choice) {
        case 1:
            if(Money >= 10000) {
                Money -= 10000;
                fireTrucks++;
                cout << ">> Purchased.\n";
            }
            else {
                cout << ">> No Money.\n";
            }
            break;

        case 2:
            if(Money >= 12000) {
                Money -= 12000;
                riotPolice++;
                cout << ">> Purchased.\n";
            }
            else {
                cout << ">> No Money.\n";
            }
            break;

        case 3:
            if(Money >= 8000) {
                Money -= 8000;
                ambulances++;
                cout << ">> Purchased.\n";
            }
            else {
                cout << ">> No Money.\n";
            }
            break;
    }
}


void gameloop() {
    auto monthStartTime = chrono::steady_clock::now();
    int menuChoice = 0;
    avgCrime = 0;
    avgEducation = 0;
    avgHealth = 0;
    returnToMenu = false;

    while(isGameOver == false && returnToMenu == false) {
        auto currentTime = chrono::steady_clock::now();
        auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - monthStartTime).count();

        if(elapsedSeconds >= 120) {
            cout << "\n--- MONTH AUTOMATICALLY ENDED ---\n";
            collectTax();
            buildingexpenses();
            updateDistricts();
            updatePopulation();
            saveLog();
            randomEvent();
            currentMonth++;

            if(currentMonth > 12) {
                currentMonth = 1;
                currentYear++;
            }

            avgCrime = avgHealth = avgEducation = 0;

            for(int i=0; i<NUM_DISTRICTS; i++)
                avgCrime += crimeRate[i];

            for(int i=0; i<NUM_DISTRICTS; i++)
                avgHealth += health[i];

            for(int i=0; i<NUM_DISTRICTS; i++)
                avgEducation += education[i];

            avgCrime /= NUM_DISTRICTS;
            avgHealth /= NUM_DISTRICTS;
            avgEducation /= NUM_DISTRICTS;

            gameover();
            monthStartTime = chrono::steady_clock::now();
        }

        cout << "\n\nTime left in month: " << (120 - elapsedSeconds) << " seconds\n";

        overview(difficultychoice);

        cout << "--- MONTHLY ACTIONS ---\n";
        cout << " [1] Manage Districts\n";
        cout << " [2] Manage Infrastructure\n";
        cout << " [3] Manage Transportation\n";
        cout << " [4] City Policies\n";
        cout << " [5] View Detailed Stats\n";
        cout << " [6] Finance & Banking\n";
        cout << " [7] Emergency Command\n";
        cout << " [8] Consult Advisors\n";
        cout << " [9] Save Game\n";
        cout << " [10] Save & Exit\n";
        cout << " [11] Cheat (Debug)\n";
        cout << "-----------------------\n";
        do{
        cout << "Select Action: ";
        cin >> menuChoice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(menuChoice<1||menuChoice>11){
            cout<<"Please enter a valid option."<<endl;
        }
        }while(menuChoice<1||menuChoice>11);
        switch(menuChoice) {
            case 1:
                manageDistricts();
                break;
            case 2:
                manageInfrastructure();
                break;
            case 3:
                manageTransport();
                break;
            case 4:
                managePolicies();
                break;
            case 5:
                stats();
                cout << "\n(Press Enter to return...)";
                cin.ignore();
                cin.get();
                cin.ignore(1000,'\n');
                rankDistricts();
                break;
            case 6:
                manageFinance();
                break;
            case 7:
                emergencyCommandCenter();
                break;
            case 8:
                consultAdvisors();
                break;
            case 9:
                saveGame();
                break;
            case 10:
                saveGame();
                returnToMenu = true;
                break;
            case 11:
                cheatMenu();
                break;
        }

        calculateHappiness();
    }
}


void cheatMenu() {
    int code;
    while(true) {
    cout << "ENTER CHEAT CODE: ";
    cin >> code;

    if(cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid option." << endl;
        continue;
    }
    break; 
    }


    if(code == 786) {
        Money += 1000000;
        cout << "Cheat Activated: Massive Funding!\n";
    }
    else if(code == 1122) {
        for(int i=0; i<NUM_DISTRICTS; i++)
            crimeRate[i] = 0;
        cout << "Cheat Activated: Crime wiped out!\n";
    }
    else if(code == 911) {
        powerSupply += 1000;
        waterSupply += 1000;
        cout << "Cheat Activated: Infinite Resources!\n";
    }
    else {
        cout << "Invalid Code.\n";
    }
}


void saveGame() {
    ofstream save("savegame.txt");
    if(save.is_open()) {
        save << Money << endl << currentMonth << endl << currentYear << endl << difficultychoice << endl;
        for(int i=0; i<NUM_DISTRICTS; i++) {
            save << districtName[i] << endl;
            save << population[i] << endl << crimeRate[i] << endl << health[i] << endl << education[i] << endl
            << tax[i] << endl << pollution[i] << endl << happiness[i] << endl << traffic[i] << endl 
            << roadQuality[i] << endl << policeStations[i] << endl << hospitals[i] << endl << schools[i] << endl 
            << parks[i] << endl << busStops[i] << endl;
        }
        save << powerSupply << endl << waterSupply << endl << powerPlants << endl << waterPumps << endl;
        save << policyCurfew << endl << policyFreeLunch << endl << policyGreenEnergy << endl 
        << policyPoliceState << endl << policyTourism << endl << policyCarFree << endl;

        save << loanAmount << endl << interestRate << endl << creditScore << endl;
        save << fireTrucks << endl << riotPolice << endl << ambulances << endl;
        
        cout << "Game Saved Successfully to 'savegame.txt'!\n";
        save.close();
    } else {
        cout << "ERROR: Could not open file for saving.\n";
    }
}

void loadGame() {
    ifstream load("savegame.txt");

    if(load.is_open()) {
        load >> Money >> currentMonth >> currentYear >> difficultychoice;
        for(int i=0; i<NUM_DISTRICTS; i++) {
            load.ignore();
            getline(load, districtName[i]);
            load >> population[i] >> crimeRate[i] >> health[i] >> education[i] 
            >> tax[i] >> pollution[i] >> happiness[i] >> traffic[i] >> roadQuality[i]
            >> policeStations[i] >> hospitals[i] >> schools[i] >> parks[i] >> busStops[i];
        }

        load >> powerSupply >> waterSupply >> powerPlants >> waterPumps;
        load >> policyCurfew >> policyFreeLunch >> policyGreenEnergy 
        >> policyPoliceState >> policyTourism >> policyCarFree;

        load >> loanAmount >> interestRate >> creditScore;
        load >> fireTrucks >> riotPolice >> ambulances;

        cout << "Game Loaded! Resuming...\n";

        load.close();

        overview(difficultychoice);
        stats();
        gameloop();
    }
    else {
        cout << "No save file found or file error.\n";
    }
}



int startscreen(){
    cout << "==================================================\n";
    cout << "|                                                |\n";
    cout << "|                 CITY MANAGER                   |\n";
    cout << "|                                                |\n";
    cout << "|   [1] START NEW CITY                           |\n";
    cout << "|   [2] LOAD GAME                                |\n";
    cout << "|   [3] GAME GUIDE                               |\n";
    cout << "|   [4] CREDITS                                  |\n";
    cout << "|   [5] EXIT                                     |\n";
    cout << "|                                                |\n";
    cout << "==================================================\n";
    int choice=0;
    do{
        cout << "Enter your choice: ";
        cin>>choice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(choice<1||choice>5){
            cout<<"Please enter a valid option."<<endl;
        }
    }while(choice<1||choice>5);
    return choice;
}

int levelselect(){
    int choice=0;
    cout << "==================================================\n";
    cout << "|                                                |\n";
    cout << "|                START NEW CITY                  |\n";
    cout << "|                                                |\n";
    cout << "|   [1] EASY MODE (High Budget)                  |\n";
    cout << "|   [2] NORMAL MODE (Balanced)                   |\n";
    cout << "|   [3] HARD MODE (Low Budget, High Crime)       |\n";
    cout << "|   [4] BACK TO MAIN MENU                        |\n";
    cout << "|                                                |\n";
    cout << "==================================================\n";
    do{
        cout << "Enter your choice: ";
        cin>>choice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Please enter a valid option."<<endl;
            continue;
        }
        if(choice<1||choice>5){
            cout<<"Please enter a valid option."<<endl;
        }
    }while(choice<1||choice>5);
    return choice;
}

void gameguide(){
    int choice;
    cout << "==================================================\n";
    cout << "|                  HOW TO PLAY                   |\n";
    cout << "| - Start New City to play.                      |\n";
    cout << "| - Select Easy, Normal, or Hard difficulty.     |\n";
    cout << "| - Manage city wisely to grow population & $$   |\n";
    cout << "| - INFRASTRUCTURE IS KEY: You must generate     |\n";
    cout << "|   enough Power and Water for your population!  |\n";
    cout << "| - If Power/Water Demand > Supply, city dies.   |\n";
    cout << "| - Build Parks to reduce pollution.             |\n";
    cout << "| - Manage Traffic with roads and bus stops.     |\n";
    cout << "|                                                |\n";
    cout << "| Press ENTER to return to the main menu.        |\n";
    cout << "==================================================\n";
    choice=cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void credit(){
    int choice;
    cout << "==================================================\n";
    cout << "|                     CREDITS                    |\n";
    cout << "|                                                |\n";
    cout << "|   Developed by: M Usman Mumtaz                 |\n";
    cout << "|   Course: BS Software Engineering              |\n";
    cout << "|   University: The University of Lahore         |\n";
    cout << "|   Extended Edition 2026                        |\n";
    cout << "|                                                |\n";
    cout << "| Press ENTER to return to the main menu.        |\n";
    cout << "==================================================\n";
    choice=cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void collectTax(){
    float income = 0;
    for(int i = 0; i < NUM_DISTRICTS; i++){
        income += population[i] * tax[i] * taxMultiplier;
    }
    if(policyTourism) income += 50000;
    Money += income;
}

void buildingexpenses() {
    float cost = 0;
    float districtCost = 0;
    float policeCost = 0;
    float hospitalCost = 0;
    float schoolCost = 0;
    float parkCost = 0;
    float busStopCost = 0;
    for(int i = 0; i < NUM_DISTRICTS; i++) {
        policeCost = policeStations[i] * 2600 * expenseMultiplier;
        hospitalCost = hospitals[i] * 2800 * expenseMultiplier;
        schoolCost = schools[i] * 2200 * expenseMultiplier;
        parkCost = parks[i] * 500 * expenseMultiplier;
        busStopCost = busStops[i] * 200 * expenseMultiplier;

        districtCost = policeCost + hospitalCost + schoolCost + parkCost + busStopCost;
        cost += districtCost;
    }

    float vehicleCost = 0;
    float fireTruckCost = fireTrucks * 500;
    float riotPoliceCost = riotPolice * 800;
    float ambulanceCost = ambulances * 400;
    vehicleCost = fireTruckCost + riotPoliceCost + ambulanceCost;
    cost += vehicleCost;

    float loanInterest = 0;
    if (loanAmount > 0) {
        loanInterest = loanAmount * interestRate;
        cost += loanInterest;
        cout << "\n>> FINANCE ALERT: Paid $" << loanInterest << " in loan interest.\n";
    }

    float policyCost = 0;
    float freeLunchCost = 0;
    float greenEnergyCost = 0;
    float carFreeCost = 0;

    if(policyFreeLunch) {
        freeLunchCost = 10000;
        policyCost += freeLunchCost;
    }

    if(policyGreenEnergy) {
        greenEnergyCost = 15000;
        policyCost += greenEnergyCost;
    }

    if(policyCarFree) {
        carFreeCost = 5000;
        policyCost += carFreeCost;
    }

    cost += policyCost;

    float utilityCost = 0;
    float powerPlantCost = powerPlants * 3000;
    float waterPumpCost = waterPumps * 2500;
    utilityCost = powerPlantCost + waterPumpCost;
    cost += utilityCost;
    float totalExpenses = cost;
    Money -= totalExpenses;
}


void updateDistricts(){
    calculateResources();
    bool powerShortage = (powerSupply < powerDemand);
    bool waterShortage = (waterSupply < waterDemand);
    for(int i = 0; i < NUM_DISTRICTS; i++){
        float crimeChange = crimeMultiplier - (policeStations[i] * 0.8 * serviceMultiplier);
        if(policyCurfew){
            crimeChange -= 2.0;
        }
        if(policyPoliceState){
            crimeChange -= 5.0;
        }
        if(policyTourism){
            crimeChange += 1.5;
        }
        crimeRate[i] += crimeChange;
        float healthChange = (hospitals[i] * 1.2 * serviceMultiplier) - 1.5;
        if(pollution[i] > 50){
            healthChange -= 2.0;
        }
        if(policyFreeLunch){
            healthChange += 1.0;
        }
        if(waterShortage){
            healthChange -= 5.0;
        }
        health[i] += healthChange;
        float eduChange = (schools[i] * 1.0 * serviceMultiplier) - 1.0;
        if(policyFreeLunch){
            eduChange += 0.5;
        }
        if(powerShortage){
            eduChange -= 3.0;
        }
        education[i] += eduChange;
        traffic[i] += (population[i] / 5000);
        traffic[i] -= (busStops[i] * 2);
        if(roadQuality[i] < 50){
            traffic[i] += 2;
        }
        roadQuality[i] -= (traffic[i] / 20);
        if(policyCarFree){
            traffic[i] -= 10;
        }
        float pollutionGrowth = (traffic[i] / 10.0) + (population[i] / 8000.0);
        pollutionGrowth -= (parks[i] * 1.5);
        pollution[i] += pollutionGrowth;

        if(traffic[i] < 0){
            traffic[i] = 0;
        }
        if(traffic[i] > 100){
            traffic[i] = 100;
        }
        if(roadQuality[i] < 0){
            roadQuality[i] = 0;
        }
        if(policyGreenEnergy && pollution[i] > 0){
            pollution[i] -= 2;
        }
        if(crimeRate[i] < 0){
            crimeRate[i] = 0;
        }
        if(crimeRate[i] > 100){
            crimeRate[i] = 100;
        }
        if(health[i] < 0){
            health[i] = 0;
        }
        if(health[i] > 100){
            health[i] = 100;
        }
        if(education[i] < 0){
            education[i] = 0;
        }
        if(education[i] > 100){
            education[i] = 100;
        }
        if(pollution[i] < 0){
            pollution[i] = 0;
        }
        if(pollution[i] > 100){
            pollution[i] = 100;
        }
    }
    calculateHappiness();
}


void Difficultylevel(int choice){
    
    if (choice == 1) {
        Money = 1000000;

        districtName[0] = "Downtown";
        population[0] = 12000;
        crimeRate[0] = 5.0;
        health[0] = 94.0;
        education[0] = 88.0;
        tax[0] = 6.0;
        policeStations[0] = 4;
        hospitals[0] = 2;
        schools[0] = 3;

        districtName[1] = "Old Town";
        population[1] = 8000;
        crimeRate[1] = 4.0;
        health[1] = 91.0;
        education[1] = 76.0;
        tax[1] = 5.0;
        policeStations[1] = 2;
        hospitals[1] = 2;
        schools[1] = 2;

        districtName[2] = "Industrial Zone";
        population[2] = 17000;
        crimeRate[2] = 7.0;
        health[2] = 82.0;
        education[2] = 79.0;
        tax[2] = 8.0;
        policeStations[2] = 3;
        hospitals[2] = 3;
        schools[2] = 2;

        districtName[3] = "Suburban Hills";
        population[3] = 1200;
        crimeRate[3] = 5.0;
        health[3] = 92.0;
        education[3] = 85.0;
        tax[3] = 8.0;
        policeStations[3] = 4;
        hospitals[3] = 2;
        schools[3] = 3;

        districtName[4] = "East Colony";
        population[4] = 15000;
        crimeRate[4] = 7.0;
        health[4] = 92.0;
        education[4] = 85.0;
        tax[4] = 8.0;
        policeStations[4] = 4;
        hospitals[4] = 2;
        schools[4] = 3;

        taxMultiplier = 1.2;
        expenseMultiplier = 0.8;
        crimeMultiplier = 0.7;
        serviceMultiplier = 1.3;

        for(int i=0;i<NUM_DISTRICTS;i++){
        parks[i] = 2; 
        busStops[i] = 2;
        pollution[i] = 0; 
        happiness[i] = 70; 
        traffic[i] = 15; 
        roadQuality[i] = 95;
    }
    powerSupply = 600; 
    waterSupply = 600;
    } 
    else if (choice == 2) {
        Money = 600000;

        districtName[0] = "Downtown";
        population[0] = 15000;
        crimeRate[0] = 7.0;
        health[0] = 90.0;
        education[0] = 82.0;
        tax[0] = 7.0;
        policeStations[0] = 3;
        hospitals[0] = 2;
        schools[0] = 2;

        districtName[1] = "Old Town";
        population[1] = 10000;
        crimeRate[1] = 6.0;
        health[1] = 86.0;
        education[1] = 70.0;
        tax[1] = 6.0;
        policeStations[1] = 2;
        hospitals[1] = 1;
        schools[1] = 2;

        districtName[2] = "Industrial Zone";
        population[2] = 20000;
        crimeRate[2] = 9.0;
        health[2] = 78.0;
        education[2] = 74.0;
        tax[2] = 8.0;
        policeStations[2] = 2;
        hospitals[2] = 2;
        schools[2] = 2;

        districtName[3] = "Suburban Hills";
        population[3] = 3000;
        crimeRate[3] = 6.0;
        health[3] = 88.0;
        education[3] = 80.0;
        tax[3] = 7.0;
        policeStations[3] = 2;
        hospitals[3] = 1;
        schools[3] = 2;

        districtName[4] = "East Colony";
        population[4] = 12000;
        crimeRate[4] = 7.0;
        health[4] = 88.0;
        education[4] = 78.0;
        tax[4] = 7.0;
        policeStations[4] = 3;
        hospitals[4] = 1;
        schools[4] = 2;

        taxMultiplier = 1.0;
        expenseMultiplier = 1.0;
        crimeMultiplier = 1.0;
        serviceMultiplier = 1.0;

        for(int i=0;i<NUM_DISTRICTS;i++){
        parks[i] = 1; 
        busStops[i] = 1;
        pollution[i] = 5; 
        happiness[i] = 60; 
        traffic[i] = 20; 
        roadQuality[i] = 90;
    }
    powerSupply = 500; 
    waterSupply = 500;
    } 
    else if (choice == 3) {
        Money = 350000;

        districtName[0] = "Downtown";
        population[0] = 18000;
        crimeRate[0] = 10.0;
        health[0] = 85.0;
        education[0] = 75.0;
        tax[0] = 9.0;
        policeStations[0] = 2;
        hospitals[0] = 1;
        schools[0] = 1;

        districtName[1] = "Old Town";
        population[1] = 12000;
        crimeRate[1] = 9.0;
        health[1] = 80.0;
        education[1] = 65.0;
        tax[1] = 8.0;
        policeStations[1] = 1;
        hospitals[1] = 1;
        schools[1] = 1;

        districtName[2] = "Industrial Zone";
        population[2] = 22000;
        crimeRate[2] = 12.0;
        health[2] = 75.0;
        education[2] = 70.0;
        tax[2] = 10.0;
        policeStations[2] = 2;
        hospitals[2] = 1;
        schools[2] = 1;

        districtName[3] = "Suburban Hills";
        population[3] = 4000;
        crimeRate[3] = 8.0;
        health[3] = 82.0;
        education[3] = 75.0;
        tax[3] = 9.0;
        policeStations[3] = 2;
        hospitals[3] = 1;
        schools[3] = 1;

        districtName[4] = "East Colony";
        population[4] = 15000;
        crimeRate[4] = 10.0;
        health[4] = 80.0;
        education[4] = 72.0;
        tax[4] = 9.0;
        policeStations[4] = 2;
        hospitals[4] = 1;
        schools[4] = 1;

        taxMultiplier = 0.8;
        expenseMultiplier = 1.3;
        crimeMultiplier = 1.4;
        serviceMultiplier = 0.7;

        for(int i=0;i<NUM_DISTRICTS;i++){
        parks[i] = 0; 
        busStops[i] = 0;
        pollution[i] = 10; 
        happiness[i] = 50; 
        traffic[i] = 20; 
        roadQuality[i] = 85;
    }
    powerSupply = 400; 
    waterSupply = 400;
    }
    calculateHappiness();

for (int i = 0; i < 5; i++) {
    avgCrime += crimeRate[i];
}
avgCrime /= NUM_DISTRICTS;

for (int i = 0; i < 5; i++) {
    avgEducation += education[i];
}
avgEducation /= NUM_DISTRICTS;

for (int i = 0; i < 5; i++) {
    avgHealth += health[i];
}
avgHealth /= NUM_DISTRICTS;

}

void stats() {
    calculateResources();
    cout << "------------------------------------------------------------------------------------------------------------------------\n";
    cout << "DISTRICT         | POP   | CRM | HLT | EDU | HAP | POLUTION | TRF | ROAD | POLICE | HOS | SCH | PRK | BUS\n";
    cout << "------------------------------------------------------------------------------------------------------------------------\n";
    for(int i = 0; i < 5; i++) {
        cout << left << setw(18) << districtName[i]<< setw(8) << population[i]<< setw(6) << crimeRate[i]
             << setw(6) << health[i]<< setw(6) << education[i] << setw(6) << happiness[i] << setw(10) << pollution[i] 
             << setw(7) << traffic[i] << setw(7) << roadQuality[i] << setw(9) << policeStations[i] << setw(6) << hospitals[i] 
             << setw(6) << schools[i] << setw(6) << parks[i] << setw(5) << busStops[i] << endl;
    }
    cout << "------------------------------------------------------------------------------------------------------------------------\n";
    cout << "POWER SUPPLY: " << powerSupply << " MW / DEMAND: " << powerDemand << " MW  ||  ";
    cout << "WATER SUPPLY: " << waterSupply << " GL / DEMAND: " << waterDemand << " GL\n";
    cout << "EMERGENCY UNITS: [Fire: " << fireTrucks << "] [Riot: " << riotPolice << "] [Amb: " << ambulances << "]\n";
}

void overview(int choice){
    calculateResources();
    
    cout<<"\n\n";
    cout << "==================================================\n";
    cout << "|                 CITY OVERVIEW                  |\n";
    cout << "|------------------------------------------------|\n";
    cout << "| Month: " << setw(2) << currentMonth << "  Year: " << setw(4) << currentYear 
         << "                          |\n";

    string diffName = "UNKNOWN";
    if(choice >= 1 && choice <= 3) diffName = difficulty[choice-1];

    cout << "| Difficulty: " << setw(33) << left << diffName << "  |\n";
    cout << "| Total Money: $" << setw(29) << fixed << setprecision(0) << Money << "    |\n";
    cout << "| Avg Happiness: " << setw(3) << avgHappiness << "%                            |\n";
    cout << "| Power Status:  " << setw(28) << ((powerSupply>=powerDemand)?"STABLE":"CRITICAL DEFICIT") << "    |\n";
    cout << "| Water Status:  " << setw(28) << ((waterSupply>=waterDemand)?"STABLE":"CRITICAL DEFICIT") << "    |\n";
    cout << "==================================================\n";
    cout<<"\n\n";
}

int main(){
    int startscreenchoice=0;
    start:
    startscreenchoice=startscreen();
    switch (startscreenchoice)
    {
    case 1:
        difficultychoice=levelselect();
        if(difficultychoice>0&&difficultychoice<4){
            Difficultylevel(difficultychoice);
            overview(difficultychoice);
            stats();
            gameloop();
            goto start;
        }
        else{ goto start; }
        break;
    case 2:
        loadGame();
        goto start;
        break;
    case 3:
        gameguide();
        goto start;
        break;
    case 4:
        credit();
        goto start;
        break;
    case 5: 
        goto end;
        break;
    default:
        break;
    }
    end:
    delete [] education;
    delete [] tax;
    delete [] pollution;
    delete [] happiness;
    delete [] traffic;
    return 0;
}