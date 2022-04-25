/* Goksel Verep, S020835 */
typedef struct ingredient {
    char name[15]; // Name of the ingredient
    int time_to_process; // Time to prepare the ingredient
} ingredient;

typedef struct meal {
    char name[15];
    struct ingredient ingredients[3];
    int time_to_prepare; // Time to prepare the meal
} meal;