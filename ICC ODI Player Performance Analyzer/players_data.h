#ifndef PLAYERS_DATA_H
#define PLAYERS_DATA_H

#define TEAM_COUNT 10
#define INITIAL_PLAYERS 200
#define MAX_NAME_LENGTH 50

#define BATSMAN 1
#define BOWLER 2
#define ALL_ROUNDER 3

typedef struct InitialPlayer {
    int playerId;
    char name[MAX_NAME_LENGTH];
    int teamId;
    int role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
} InitialPlayer;

static const char *teamNames[TEAM_COUNT] = {
    "India",
    "Australia",
    "England",
    "Pakistan",
    "New Zealand",
    "South Africa",
    "Bangladesh",
    "Sri Lanka",
    "Afghanistan",
    "West Indies"
};

static const InitialPlayer initialPlayers[INITIAL_PLAYERS] = {

    /* ======================== INDIA (TEAM 1) ======================== */

    {  1, "Rohit Sharma",          1, BATSMAN,      9800, 48.30, 94.50,   0, 0.00},
    {  2, "Virat Kohli",          1, BATSMAN,     13000, 57.30, 93.20,   0, 0.00},
    {  3, "Shubman Gill",         1, BATSMAN,      2500, 58.20, 99.10,   0, 0.00},
    {  4, "KL Rahul",             1, BATSMAN,      4500, 47.50, 88.60,   0, 0.00},
    {  5, "Shreyas Iyer",         1, BATSMAN,      3200, 45.20, 98.40,   0, 0.00},
    {  6, "Suryakumar Yadav",     1, BATSMAN,      1800, 32.10, 106.50,  0, 0.00},
    {  7, "Rishabh Pant",         1, BATSMAN,      1500, 34.80, 105.20,  0, 0.00},
    {  8, "Sanju Samson",         1, BATSMAN,      1000, 42.50, 98.20,   0, 0.00},
    {  9, "Jasprit Bumrah",       1, BOWLER,        250,  8.50, 65.00,  145, 4.60},
    { 10, "Mohammed Shami",       1, BOWLER,        400, 10.20, 70.00,  195, 5.20},
    { 11, "Mohammed Siraj",       1, BOWLER,        150,  7.50, 62.00,   85, 5.10},
    { 12, "Kuldeep Yadav",        1, BOWLER,        200,  8.00, 66.00,  105, 4.80},
    { 13, "Yuzvendra Chahal",     1, BOWLER,        300,  9.50, 72.00,  125, 5.00},
    { 14, "Arshdeep Singh",       1, BOWLER,        100,  6.50, 60.00,   45, 5.40},
    { 15, "Hardik Pandya",        1, ALL_ROUNDER,   2300, 36.50, 121.00, 80, 5.20},
    { 16, "Ravindra Jadeja",      1, ALL_ROUNDER,   2600, 32.80, 85.20,  220, 4.90},
    { 17, "Axar Patel",           1, ALL_ROUNDER,   1000, 24.50, 91.00,   60, 4.80},
    { 18, "Washington Sundar",    1, ALL_ROUNDER,    500, 20.00, 82.00,   30, 4.70},
    { 19, "Shivam Dube",          1, ALL_ROUNDER,    800, 28.00, 105.00,  15, 5.80},
    { 20, "Nitish Kumar Reddy",   1, ALL_ROUNDER,    300, 22.00, 95.00,   10, 5.60},

    /* ====================== AUSTRALIA (TEAM 2) ====================== */

    { 21, "David Warner",          2, BATSMAN,     7000, 45.00, 96.00,    0, 0.00},
    { 22, "Steve Smith",           2, BATSMAN,     5500, 43.50, 87.00,    0, 0.00},
    { 23, "Travis Head",           2, BATSMAN,     3500, 42.80, 102.00,   0, 0.00},
    { 24, "Marnus Labuschagne",    2, BATSMAN,     2000, 39.00, 85.00,    0, 0.00},
    { 25, "Mitchell Marsh",        2, BATSMAN,     3000, 40.00, 95.00,    0, 0.00},
    { 26, "Alex Carey",            2, BATSMAN,     2500, 36.50, 91.00,    0, 0.00},
    { 27, "Josh Inglis",           2, BATSMAN,     1000, 34.00, 98.00,    0, 0.00},
    { 28, "Cameron Green",         2, BATSMAN,     1500, 38.00, 92.00,    0, 0.00},
    { 29, "Mitchell Starc",        2, BOWLER,       500, 13.00, 80.00,  240, 4.70},
    { 30, "Pat Cummins",            2, BOWLER,       400, 12.00, 78.00,  215, 4.90},
    { 31, "Josh Hazlewood",         2, BOWLER,       250,  9.00, 65.00,  130, 4.50},
    { 32, "Adam Zampa",             2, BOWLER,       200,  8.00, 70.00,  150, 5.00},
    { 33, "Nathan Lyon",            2, BOWLER,       150,  7.00, 60.00,   40, 5.10},
    { 34, "Sean Abbott",            2, BOWLER,       300, 10.00, 75.00,   55, 5.40},
    { 35, "Glenn Maxwell",           2, ALL_ROUNDER,  4000, 35.00, 125.00, 70, 5.30},
    { 36, "Marcus Stoinis",         2, ALL_ROUNDER,  2500, 32.00, 105.00, 45, 5.60},
    { 37, "Ashton Agar",            2, ALL_ROUNDER,   500, 22.00, 85.00,  45, 4.80},
    { 38, "Aaron Hardie",           2, ALL_ROUNDER,   600, 27.00, 95.00,  25, 5.30},
    { 39, "Beau Webster",            2, ALL_ROUNDER,   350, 25.00, 90.00,  20, 5.50},
    { 40, "Daniel Sams",             2, ALL_ROUNDER,   200, 18.00, 100.00, 15, 6.00},

    /* ====================== ENGLAND (TEAM 3) ======================= */

    { 41, "Jos Buttler",             3, BATSMAN,     5000, 40.00, 117.00, 0, 0.00},
    { 42, "Joe Root",                3, BATSMAN,     7000, 48.00, 87.00,  0, 0.00},
    { 43, "Harry Brook",              3, BATSMAN,     1800, 38.00, 99.00,  0, 0.00},
    { 44, "Ben Duckett",             3, BATSMAN,     1500, 40.00, 105.00, 0, 0.00},
    { 45, "Phil Salt",               3, BATSMAN,     1200, 34.00, 108.00, 0, 0.00},
    { 46, "Dawid Malan",             3, BATSMAN,     4000, 45.00, 95.00,  0, 0.00},
    { 47, "Jason Roy",               3, BATSMAN,     3000, 38.00, 105.00, 0, 0.00},
    { 48, "Jonny Bairstow",          3, BATSMAN,     4000, 42.00, 103.00, 0, 0.00},
    { 49, "Jofra Archer",             3, BOWLER,       200, 10.00, 75.00,  90, 4.90},
    { 50, "Mark Wood",               3, BOWLER,       150,  8.00, 65.00,  75, 5.20},
    { 51, "Adil Rashid",             3, BOWLER,       250, 10.00, 70.00,  180, 5.00},
    { 52, "Reece Topley",             3, BOWLER,       100,  6.00, 60.00,  60, 5.30},
    { 53, "Chris Woakes",             3, BOWLER,       700, 15.00, 90.00,  150, 5.20},
    { 54, "Liam Livingstone",         3, BOWLER,       500, 12.00, 85.00,   25, 5.70},
    { 55, "Ben Stokes",              3, ALL_ROUNDER,  3000, 40.00, 95.00,   70, 5.50},
    { 56, "Sam Curran",              3, ALL_ROUNDER,  1200, 28.00, 92.00,   55, 5.40},
    { 57, "Moeen Ali",               3, ALL_ROUNDER,  2200, 25.00, 100.00,  95, 5.20},
    { 58, "Rehan Ahmed",              3, ALL_ROUNDER,   300, 20.00, 85.00,   20, 5.60},
    { 59, "Jacob Bethell",            3, ALL_ROUNDER,   500, 25.00, 95.00,   15, 5.50},
    { 60, "Will Jacks",               3, ALL_ROUNDER,   700, 27.00, 105.00,  20, 5.80},

    /* ====================== PAKISTAN (TEAM 4) ====================== */

    { 61, "Babar Azam",              4, BATSMAN,     5500, 56.00, 88.00,  0, 0.00},
    { 62, "Fakhar Zaman",            4, BATSMAN,     3500, 45.00, 95.00,  0, 0.00},
    { 63, "Imam-ul-Haq",             4, BATSMAN,     3000, 48.00, 85.00,  0, 0.00},
    { 64, "Mohammad Rizwan",         4, BATSMAN,     2500, 45.00, 88.00,  0, 0.00},
    { 65, "Abdullah Shafique",       4, BATSMAN,     1200, 40.00, 85.00,  0, 0.00},
    { 66, "Saim Ayub",              4, BATSMAN,     1000, 35.00, 100.00, 0, 0.00},
    { 67, "Haider Ali",              4, BATSMAN,      800, 30.00, 95.00,  0, 0.00},
    { 68, "Agha Salman",             4, BATSMAN,     1000, 35.00, 90.00,  0, 0.00},
    { 69, "Shaheen Afridi",          4, BOWLER,       200,  8.00, 65.00,  190, 4.80},
    { 70, "Haris Rauf",              4, BOWLER,       150,  7.00, 60.00,  100, 5.50},
    { 71, "Naseem Shah",             4, BOWLER,       100,  6.00, 60.00,   70, 5.20},
    { 72, "Shadab Khan",             4, BOWLER,       700, 20.00, 85.00,  80, 5.10},
    { 73, "Mohammad Wasim Jr",       4, BOWLER,       100,  5.00, 55.00,   40, 5.60},
    { 74, "Usama Mir",               4, BOWLER,        50,  5.00, 55.00,   35, 5.50},
    { 75, "Mohammad Nawaz",          4, ALL_ROUNDER,  1000, 25.00, 85.00,   60, 5.00},
    { 76, "Iftikhar Ahmed",          4, ALL_ROUNDER,  1200, 30.00, 95.00,   35, 5.60},
    { 77, "Faheem Ashraf",           4, ALL_ROUNDER,   500, 20.00, 90.00,   35, 5.50},
    { 78, "Imad Wasim",              4, ALL_ROUNDER,  1000, 30.00, 85.00,   45, 4.80},
    { 79, "Khushdil Shah",           4, ALL_ROUNDER,   500, 25.00, 100.00,  15, 5.70},
    { 80, "Amir Jamal",              4, ALL_ROUNDER,   250, 18.00, 85.00,   15, 5.80},

    /* =================== NEW ZEALAND (TEAM 5) ====================== */

    { 81, "Kane Williamson",          5, BATSMAN,     7000, 48.00, 82.00,  0, 0.00},
    { 82, "Devon Conway",             5, BATSMAN,     2500, 45.00, 88.00,  0, 0.00},
    { 83, "Daryl Mitchell",           5, BATSMAN,     2500, 45.00, 95.00,  0, 0.00},
    { 84, "Tom Latham",               5, BATSMAN,     4000, 40.00, 85.00,  0, 0.00},
    { 85, "Finn Allen",               5, BATSMAN,     1000, 30.00, 105.00, 0, 0.00},
    { 86, "Will Young",               5, BATSMAN,     1500, 40.00, 85.00,  0, 0.00},
    { 87, "Mark Chapman",              5, BATSMAN,     1200, 35.00, 90.00,  0, 0.00},
    { 88, "Henry Nicholls",            5, BATSMAN,     2000, 40.00, 82.00,  0, 0.00},
    { 89, "Trent Boult",               5, BOWLER,       300, 10.00, 70.00,  200, 5.00},
    { 90, "Tim Southee",               5, BOWLER,       250,  9.00, 65.00,  150, 5.20},
    { 91, "Matt Henry",                5, BOWLER,       200,  8.00, 65.00,  100, 4.80},
    { 92, "Lockie Ferguson",            5, BOWLER,       100,  6.00, 60.00,   75, 5.50},
    { 93, "Ish Sodhi",                 5, BOWLER,       150,  7.00, 65.00,   60, 5.30},
    { 94, "Kyle Jamieson",              5, BOWLER,       100,  6.00, 60.00,   40, 5.00},
    { 95, "Mitchell Santner",           5, ALL_ROUNDER,  1000, 30.00, 85.00,   90, 4.80},
    { 96, "Glenn Phillips",              5, ALL_ROUNDER,  1500, 35.00, 100.00,  25, 5.70},
    { 97, "James Neesham",              5, ALL_ROUNDER,  1200, 30.00, 95.00,   35, 5.60},
    { 98, "Rachin Ravindra",             5, ALL_ROUNDER,  1000, 35.00, 95.00,   30, 5.20},
    { 99, "Michael Bracewell",            5, ALL_ROUNDER,   800, 28.00, 90.00,   40, 5.10},
    {100, "Cole McConchie",              5, ALL_ROUNDER,   300, 22.00, 85.00,   15, 5.50},

    /* ================= SOUTH AFRICA (TEAM 6) ====================== */

    {101, "Quinton de Kock",           6, BATSMAN,     7000, 45.00, 96.00,  0, 0.00},
    {102, "Temba Bavuma",              6, BATSMAN,     2500, 45.00, 85.00,  0, 0.00},
    {103, "Aiden Markram",             6, BATSMAN,     2500, 40.00, 95.00,  0, 0.00},
    {104, "Rassie van der Dussen",      6, BATSMAN,     2000, 50.00, 90.00,  0, 0.00},
    {105, "Heinrich Klaasen",           6, BATSMAN,     2000, 45.00, 110.00, 0, 0.00},
    {106, "David Miller",               6, BATSMAN,     4000, 40.00, 100.00, 0, 0.00},
    {107, "Reeza Hendricks",            6, BATSMAN,     1500, 35.00, 90.00,  0, 0.00},
    {108, "Tony de Zorzi",              6, BATSMAN,     1000, 35.00, 90.00,  0, 0.00},
    {109, "Kagiso Rabada",              6, BOWLER,       200,  8.00, 65.00,  185, 5.20},
    {110, "Anrich Nortje",              6, BOWLER,       100,  6.00, 60.00,  100, 5.50},
    {111, "Lungi Ngidi",                6, BOWLER,       150,  7.00, 65.00,   90, 5.30},
    {112, "Tabraiz Shamsi",             6, BOWLER,        50,  5.00, 55.00,   70, 5.00},
    {113, "Marco Jansen",               6, BOWLER,       500, 15.00, 80.00,   75, 5.10},
    {114, "Gerald Coetzee",             6, BOWLER,       100,  6.00, 60.00,   45, 5.60},
    {115, "Wayne Parnell",              6, ALL_ROUNDER,  1000, 25.00, 90.00,   60, 5.50},
    {116, "Andile Phehlukwayo",          6, ALL_ROUNDER,   800, 25.00, 95.00,   40, 5.40},
    {117, "Wiaan Mulder",                6, ALL_ROUNDER,   500, 25.00, 85.00,   25, 5.00},
    {118, "Keshav Maharaj",              6, ALL_ROUNDER,   400, 15.00, 70.00,   60, 4.80},
    {119, "Dwaine Pretorius",            6, ALL_ROUNDER,   500, 22.00, 90.00,   35, 5.60},
    {120, "Sikandar Raza",               6, ALL_ROUNDER,   900, 30.00, 95.00,   45, 5.20},

    /* ================= BANGLADESH (TEAM 7) ======================= */

    {121, "Shakib Al Hasan",             7, BATSMAN,     7000, 38.00, 82.00,  0, 0.00},
    {122, "Tamim Iqbal",                 7, BATSMAN,     8000, 36.00, 78.00,  0, 0.00},
    {123, "Litton Das",                  7, BATSMAN,     2500, 35.00, 85.00,  0, 0.00},
    {124, "Mushfiqur Rahim",             7, BATSMAN,     7000, 36.00, 80.00,  0, 0.00},
    {125, "Najmul Hossain Shanto",       7, BATSMAN,     2000, 35.00, 85.00,  0, 0.00},
    {126, "Mahmudullah",                 7, BATSMAN,     5000, 35.00, 85.00,  0, 0.00},
    {127, "Soumya Sarkar",               7, BATSMAN,     2000, 30.00, 90.00,  0, 0.00},
    {128, "Towhid Hridoy",               7, BATSMAN,     1500, 35.00, 90.00,  0, 0.00},
    {129, "Mustafizur Rahman",           7, BOWLER,       100,  6.00, 60.00,  100, 5.20},
    {130, "Taskin Ahmed",                7, BOWLER,       150,  7.00, 65.00,   80, 5.50},
    {131, "Mehidy Hasan Miraz",          7, BOWLER,       800, 20.00, 80.00,   80, 5.00},
    {132, "Shoriful Islam",              7, BOWLER,       100,  6.00, 60.00,   50, 5.40},
    {133, "Hasan Mahmud",                7, BOWLER,        50,  5.00, 55.00,   35, 5.50},
    {134, "Nasum Ahmed",                 7, BOWLER,        50,  5.00, 55.00,   25, 5.10},
    {135, "Afif Hossain",               7, ALL_ROUNDER,   800, 25.00, 90.00,   20, 5.50},
    {136, "Mahmudul Hasan",              7, ALL_ROUNDER,   500, 22.00, 85.00,   15, 5.60},
    {137, "Mosaddek Hossain",            7, ALL_ROUNDER,   600, 25.00, 90.00,   25, 5.40},
    {138, "Rishad Hossain",              7, ALL_ROUNDER,   200, 15.00, 80.00,   30, 5.20},
    {139, "Mahedi Hasan",                7, ALL_ROUNDER,   300, 20.00, 85.00,   35, 5.00},
    {140, "Zakir Hasan",                 7, ALL_ROUNDER,   250, 18.00, 85.00,   10, 5.70},

    /* ================= SRI LANKA (TEAM 8) ======================== */

    {141, "Kusal Mendis",                8, BATSMAN,     3500, 35.00, 95.00,  0, 0.00},
    {142, "Pathum Nissanka",             8, BATSMAN,     2500, 40.00, 90.00,  0, 0.00},
    {143, "Charith Asalanka",            8, BATSMAN,     2000, 40.00, 95.00,  0, 0.00},
    {144, "Dimuth Karunaratne",          8, BATSMAN,     3000, 35.00, 80.00,  0, 0.00},
    {145, "Dhananjaya de Silva",         8, BATSMAN,     1500, 35.00, 85.00,  0, 0.00},
    {146, "Avishka Fernando",            8, BATSMAN,     2000, 35.00, 95.00,  0, 0.00},
    {147, "Angelo Mathews",              8, BATSMAN,     5000, 40.00, 85.00,  0, 0.00},
    {148, "Sadeera Samarawickrama",      8, BATSMAN,     1000, 35.00, 90.00,  0, 0.00},
    {149, "Wanindu Hasaranga",           8, BOWLER,       800, 25.00, 95.00,  100, 5.00},
    {150, "Maheesh Theekshana",          8, BOWLER,       150,  7.00, 65.00,   70, 4.80},
    {151, "Dushmantha Chameera",        8, BOWLER,       100,  6.00, 60.00,   75, 5.20},
    {152, "Lahiru Kumara",              8, BOWLER,        50,  5.00, 55.00,   60, 5.50},
    {153, "Kasun Rajitha",              8, BOWLER,       100,  6.00, 60.00,   45, 5.40},
    {154, "Pramod Madushan",            8, BOWLER,        50,  5.00, 55.00,   35, 5.50},
    {155, "Dasun Shanaka",              8, ALL_ROUNDER,  1500, 30.00, 100.00, 40, 5.70},
    {156, "Chamika Karunaratne",        8, ALL_ROUNDER,   700, 25.00, 90.00,  30, 5.60},
    {157, "Dunith Wellalage",           8, ALL_ROUNDER,   500, 25.00, 85.00,  35, 5.00},
    {158, "Ashen Bandara",              8, ALL_ROUNDER,   400, 22.00, 85.00,  15, 5.50},
    {159, "Ramesh Mendis",              8, ALL_ROUNDER,   500, 20.00, 80.00,  40, 5.00},
    {160, "Jeffrey Vandersay",          8, ALL_ROUNDER,   100, 10.00, 65.00,  30, 5.30},

    /* ================= AFGHANISTAN (TEAM 9) ======================= */

    {161, "Rahmanullah Gurbaz",          9, BATSMAN,     2500, 35.00, 95.00,  0, 0.00},
    {162, "Ibrahim Zadran",              9, BATSMAN,     2000, 40.00, 85.00,  0, 0.00},
    {163, "Hashmatullah Shahidi",        9, BATSMAN,     3000, 40.00, 80.00,  0, 0.00},
    {164, "Mohammad Nabi",               9, BATSMAN,     3000, 30.00, 85.00,  0, 0.00},
    {165, "Najibullah Zadran",           9, BATSMAN,     2000, 30.00, 95.00,  0, 0.00},
    {166, "Riaz Hassan",                9, BATSMAN,     1000, 30.00, 85.00,  0, 0.00},
    {167, "Ikram Alikhil",              9, BATSMAN,      800, 30.00, 85.00,  0, 0.00},
    {168, "Sediqullah Atal",            9, BATSMAN,      600, 35.00, 90.00,  0, 0.00},
    {169, "Rashid Khan",                9, BOWLER,       1500, 25.00, 100.00, 170, 4.20},
    {170, "Mujeeb Ur Rahman",           9, BOWLER,        100,  6.00, 60.00,  162, 4.90},
    {171, "Fazalhaq Farooqi",           9, BOWLER,         50,  5.00, 55.00,   65, 5.00},
    {172, "Naveen-ul-Haq",              9, BOWLER,        100,  6.00, 60.00,   55, 5.20},
    {173, "Mohammad Saleem",             9, BOWLER,         50,  5.00, 55.00,   30, 5.50},
    {174, "Qais Ahmad",                 9, BOWLER,         50,  5.00, 55.00,   25, 5.40},
    {175, "Azmatullah Omarzai",         9, ALL_ROUNDER,   1000, 35.00, 100.00, 40, 5.00},
    {176, "Gulbadin Naib",              9, ALL_ROUNDER,   1000, 25.00, 90.00,  35, 5.50},
    {177, "Karim Janat",               9, ALL_ROUNDER,    500, 25.00, 95.00,  20, 5.60},
    {178, "Sharafuddin Ashraf",         9, ALL_ROUNDER,    300, 20.00, 85.00,  25, 5.20},
    {179, "Ziaur Rahman",               9, ALL_ROUNDER,    400, 22.00, 90.00,  15, 5.70},
    {180, "Farid Malik",                9, ALL_ROUNDER,    200, 18.00, 85.00,  10, 5.80},

    /* ================= WEST INDIES (TEAM 10) ====================== */

    {181, "Shai Hope",                  10, BATSMAN,     4500, 50.00, 85.00,  0, 0.00},
    {182, "Nicholas Pooran",            10, BATSMAN,     2500, 35.00, 100.00, 0, 0.00},
    {183, "Brandon King",               10, BATSMAN,     2000, 35.00, 90.00,  0, 0.00},
    {184, "Shimron Hetmyer",            10, BATSMAN,     1500, 35.00, 95.00,  0, 0.00},
    {185, "Evin Lewis",                 10, BATSMAN,     3000, 35.00, 100.00, 0, 0.00},
    {186, "Kyle Mayers",                10, BATSMAN,     1500, 35.00, 95.00,  0, 0.00},
    {187, "Johnson Charles",             10, BATSMAN,     1000, 30.00, 100.00, 0, 0.00},
    {188, "Keacy Carty",                10, BATSMAN,     1000, 35.00, 90.00,  0, 0.00},
    {189, "Alzarri Joseph",             10, BOWLER,        200,  8.00, 65.00,  180, 5.00},
    {190, "Shamar Joseph",              10, BOWLER,        100,  6.00, 60.00,   45, 5.50},
    {191, "Jayden Seales",              10, BOWLER,        100,  6.00, 60.00,   50, 5.20},
    {192, "Gudakesh Motie",             10, BOWLER,        100,  6.00, 60.00,   40, 5.00},
    {193, "Romario Shepherd",            10, BOWLER,        500, 15.00, 90.00,   40, 5.60},
    {194, "Oshane Thomas",              10, BOWLER,        100,  6.00, 60.00,   35, 5.80},
    {195, "Andre Russell",               10, ALL_ROUNDER,  2000, 30.00, 120.00,  45, 5.80},
    {196, "Jason Holder",                10, ALL_ROUNDER,  2500, 30.00, 90.00,   80, 5.00},
    {197, "Roston Chase",                10, ALL_ROUNDER,  1500, 30.00, 85.00,   40, 5.20},
    {198, "Fabian Allen",                10, ALL_ROUNDER,   500, 25.00, 90.00,   25, 5.50},
    {199, "Akeal Hosein",                10, ALL_ROUNDER,   400, 20.00, 80.00,   50, 4.80},
    {200, "Raymon Reifer",               10, ALL_ROUNDER,   300, 20.00, 85.00,   20, 5.60}

};

#endif