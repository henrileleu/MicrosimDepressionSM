// ModèleDépressionAdo.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "vars.h"
#include "parameters.h"
#include "vlsRandGenerator.h"
#include "adverse_events.h"
#include "individual.h"
#include <fstream>

parameters<double> p;
vlsRandGenerator rnd;

int main()
{
    std::vector<param<double>> default_p({
    { 0,0,0,0 }
        });

    // Initiate random number generator
    VSLStreamStatePtr stream;
    vslNewStream(&stream, VSL_BRNG_MT2203, 200882);
    rnd.init(stream);

    // Initiate parameters
    p = parameters<double>(default_p);
    
    unsigned depressions[22] = {};
    unsigned denominator[22] = {};

    std::ofstream myfile;
    myfile.open("C:\\Users\\PHE\\Downloads\\test.csv");

    unsigned sample = 100;
    for (unsigned i = 0; i < sample; i++)
    {
        for (double year = 1990.0; year < 2012.0; year++)
        {
            individual ind;
            ind = individual(year, rnd() < 0.5);
            ind.history();
            myfile << ind.outputAdverseEvent();

            // Add depressions
            double dob = ind.getDate_of_birth();

            for (int out_years = 2000; out_years < 2022; out_years++)
            {
                if (ind.hasPsychiatricDisorder(0, out_years - dob)) depressions[out_years - 2000]++;
                if (out_years - dob >= 10 && out_years - dob <= 18) denominator[out_years - 2000]++;
            }

        }
    }
    
    myfile.close();

    // Output depressions
    myfile.open("C:\\Users\\PHE\\Downloads\\dep.csv");

    for (int out_years = 0; out_years < 22; out_years++)
    {
        myfile << std::to_string(depressions[out_years]) << "," << std::to_string(denominator[out_years]) << "\n";
    }

    myfile.close();

    std::cout << "Completed!\n";
}
