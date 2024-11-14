#include "swephexp.h"
#include <stdio.h>
#include <string.h>

// Define the structure to hold the planet data
typedef struct {
    char name[20];
    char quality[20];
    char element[20];
    char sign[20];
    int sign_num;
    double pos;
    double abs_pos;
    char emoji[20];
    char house[20];
    int retrograde;
} PlanetData;

char *get_sign(double pos) {
    // Array of zodiac signs
    static char *signs[] = {"Ari", "Tau", "Gem", "Can", "Leo", "Vir", "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"};

    // Calculate the sign number
    int sign_num = (int)(pos / 30.0);

    // Return the sign based on the sign number
    return signs[sign_num];
}

char *get_house(double pos) {
    // Array of house numbers
    static char *houses[] = {"First_House", "Second_House", "Third_House",    "Fourth_House",
                             "Fifth_House", "Sixth_House",  "Seventh_House",  "Eighth_House",
                             "Ninth_House", "Tenth_House",  "Eleventh_House", "Twelfth_House"};

    int house_num = (int)(pos / 30.0);

    return houses[house_num];
}

char *get_emoji(char *sign) {
    // Array of zodiac signs and their emojis
    static char *emojis[] = {"♈️", "♉️", "♊️", "♋️", "♌️", "♍️", "♎️", "♏️", "♐️", "♑️", "♒️", "♓️"};

    // Calculate the sign number
    int sign_num = 0;
    for (int i = 0; i < 12; i++) {
        if (strcmp(sign, emojis[i]) == 0) {
            sign_num = i;
            break;
        }
    }

    // Return the emoji based on the sign number
    return emojis[sign_num];
}

char *get_quality(char *sign) {
    // Array of zodiac signs and their qualities
    static char *qualities[] = {"Cardinal", "Fixed", "Mutable"};

    // Calculate the sign number
    int sign_num = 0;
    for (int i = 0; i < 12; i++) {
        if (strcmp(sign, qualities[i]) == 0) {
            sign_num = i;
            break;
        }
    }

    // Return the quality based on the sign number
    return qualities[sign_num % 3];
}

char *get_element(char *sign) {
    // Array of zodiac signs and their elements
    static char *elements[] = {"Fire", "Earth", "Air", "Water"};

    // Calculate the sign number
    int sign_num = 0;
    for (int i = 0; i < 12; i++) {
        if (strcmp(sign, elements[i]) == 0) {
            sign_num = i;
            break;
        }
    }

    // Return the element based on the sign number
    return elements[sign_num % 4];
}

int get_sign_number(char *sign) {
    // Array of zodiac signs
    static char *signs[] = {"Ari", "Tau", "Gem", "Can", "Leo", "Vir", "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"};

    // Calculate the sign number
    int sign_num = 0;
    for (int i = 0; i < 12; i++) {
        if (strcmp(sign, signs[i]) == 0) {
            sign_num = i;
            break;
        }
    }

    return sign_num;
}

// Get Planet Name
char *get_planet_name(int planet_id) {
    // Array of planet names
    static char *planet_names[] = {"Sun",         "Moon",
                                   "Mercury",     "Venus",
                                   "Mars",        "Jupiter",
                                   "Saturn",      "Uranus",
                                   "Neptune",     "Pluto",
                                   "Mean Node",   "True Node",
                                   "Mean Apogee", "Osculating Apogee",
                                   "Earth",       "Chiron",
                                   "Pholus",      "Ceres",
                                   "Pallas",      "Juno",
                                   "Vesta",       "Interpreted Asteroids"};

    // Return the planet name based on the planet ID
    return planet_names[planet_id];
}

// Check if is retrograde
int is_retrograde(double pos, double abs_pos) {
    return pos > abs_pos;
}

// Get Planet Data Structure
PlanetData *get_planet_data(int planet_id, double tjd_ut, int iflags) {
    // Dynamically allocate memory for the planet data structure
    PlanetData *planet = (PlanetData *)malloc(sizeof(PlanetData));

    // Array for planet coordinates
    double xx[6];

    // Error buffer
    char serr[256];

    // Call to Swiss Ephemeris to calculate the planet's position
    if (swe_calc(tjd_ut, planet_id, iflags, xx, serr) == ERR) {
        printf("Error: %s\n", serr);
        free(planet);
        return NULL;
    }

    // Set the position and absolute position in the structure
    planet->pos = xx[0];
    planet->abs_pos = planet->pos + 60.0;

    // Set the planet name in the structure
    strcpy(planet->name, get_planet_name(planet_id));

    // Set the sign, sign number, emoji, quality, element, house, and retrograde in the structure
    strcpy(planet->sign, get_sign(planet->pos));
    planet->sign_num = get_sign_number(planet->sign);
    strcpy(planet->emoji, get_emoji(planet->sign));
    strcpy(planet->quality, get_quality(planet->sign));
    strcpy(planet->element, get_element(planet->sign));
    strcpy(planet->house, get_house(planet->pos));
    planet->retrograde = 0;

    // Return the planet data structure
    return planet;
}
int main() {
    // Initialize the structure for the planet
    PlanetData planet;

    double tjd_ut = 2451545.0;                // Julian Day for 2000-01-01 12:00:00 UTC (J2000)
    int planet_id = SE_SUN;                   // Identifier for the Sun
    int iflags = SEFLG_SWIEPH | SEFLG_HELCTR; // Swiss Ephemeris + heliocentric coordinates
    double xx[6];                             // Array for planet coordinates
    char serr[256];                           // Error buffer

    // Call to Swiss Ephemeris to calculate the Sun's position
    if (swe_calc_ut(tjd_ut, planet_id, iflags, xx, serr) == ERR) {
        printf("Error: %s\n", serr);
        return 1;
    }

    planet = *get_planet_data(planet_id, tjd_ut, iflags);

    printf("Planet Data:\n");

    // Output the planet data in human-readable format
    printf("Name: %s\n", planet.name);
    printf("Quality: %s\n", planet.quality);
    printf("Element: %s\n", planet.element);
    printf("Sign: %s\n", planet.sign);
    printf("Sign Number: %d\n", planet.sign_num);
    printf("Position: %.15f\n", planet.pos);
    printf("Absolute Position: %.15f\n", planet.abs_pos);
    printf("Emoji: %s\n", planet.emoji);
    printf("House: %s\n", planet.house);
    printf("Retrograde: %s\n", planet.retrograde ? "True" : "False");

    // Close Swiss Ephemeris
    swe_close();

    return 0;
}