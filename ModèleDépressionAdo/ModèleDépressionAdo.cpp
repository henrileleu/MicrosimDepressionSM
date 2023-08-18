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
bool lowerPA;
bool Options_ReverseCausation;
bool noSN;

int main()
{
    // Parameters are here for now
    std::array<param<double>, NumberOfParameters> default_p({});
    default_p[pParental_psychopathology_Pr] = { 0.437,0.3496,0.5244,0 };
    default_p[pPhysical_abuse_Pr] = { 0.172,0.1376,0.2064,0 };
    default_p[pEmotional_abuse_Pr] = { 0.347,0.2776,0.4164,0 };
    default_p[pSexual_abuse_Pr] = { 0.044,0.0352,0.0528,0 };
    default_p[pNeglect_Pr] = { 0.16,0.128,0.192,0 };
    default_p[pCorrSNUseMentalState] = { 0.25,0.2,0.3,0 };
    default_p[pOptions_ReverseCausation] = { 0,0,0,0 };
    default_p[pCorrSNUsePhysicalActivity] = { 0.0,0.616,0.924,0 };
    default_p[pDurrationDepression_a] = { 0.400576830636116,0.400576830636116,0.400576830636116,0 };
    default_p[pDurrationDepression_b] = { 20.96976,20.96976,20.96976,0 };
    default_p[pDepressionRelapseNoRisk] = { 0.4,0.32,0.48,0 };
    default_p[pTimeToDepressionRelapse]= {0.05,0.04,0.06,0};
    default_p[pBackgroundMortalityGompertz_a] = { 0.1227,0.1227,0.1227,0 };
    default_p[pBackgroundMortalityGompertz_b] = { 0.000001993,0.000001993,0.000001993,0 };
    default_p[pBackgroundMortalityGompertz_year] = { -0.01479,-0.01479,-0.01479,0 };
    default_p[pBackgroundMortalityGompertz_sex] = { -0.065,-0.065,-0.065,0 };
    default_p[pReverseCausationDepIncreaseRate] = { -0.045,0.003,0.003,0 };
    default_p[pBaselineDepressionRisk] = { 1.30,-0.2,-0.2,0 };

    std::ofstream afile;
    afile.open("C:\\Users\\HenriLeleu\\Downloads\\test.csv");
    afile.close();

    // Options
    lowerPA = true;
    Options_ReverseCausation = false;
    noSN = false;

    // Initiate random number generator
    VSLStreamStatePtr stream;
    vslNewStream(&stream, VSL_BRNG_MT2203, 200882);
    rnd.init(stream);

    // Initiate parameters
    p = parameters<double>(default_p);
   /*
    std::ofstream afile;
    afile.open("C:\\Users\\HenriLeleu\\Downloads\\test.csv");
    double _year = 1990;
    int male = 0;
    for (int i = 0; i < 1000; i++) {

        double ln_hr((_year - 2019) * p[pBackgroundMortalityGompertz_year] + male * p[pBackgroundMortalityGompertz_sex]);
        double age_of_death(0.0);
        age_of_death = rnd.gompertz(p[pBackgroundMortalityGompertz_a], p[pBackgroundMortalityGompertz_b], exp(ln_hr));
        afile << age_of_death << std::endl;
    }
    
    afile.close();

    return 0;
    */
    std::array< std::ofstream, 4> myfile;

    myfile[0].open("C:\\Users\\HenriLeleu\\Downloads\\demographics.csv");
    myfile[1].open("C:\\Users\\HenriLeleu\\Downloads\\adverseEvents.csv");
    myfile[2].open("C:\\Users\\HenriLeleu\\Downloads\\socialmedia.csv");
    myfile[3].open("C:\\Users\\HenriLeleu\\Downloads\\psydisorder.csv");

    unsigned sample = 10000;
    for (double year = 1980.0; year < 2030.0; year++)
    {
        for (unsigned i = 0; i < sample; i++)
        {
       
            individual ind;
            ind = individual(year, rnd() < 0.5);
             ind.history();

            // Get ouputs
            std::array<std::string, 4> out = ind.outputAll(static_cast<long>(year) * 100000 + static_cast<long>(i));

            // Write output
            for (int f = 0; f < 4; f++) myfile[f] << out[f];

        }
    }
    
    for (int f = 0; f < 4; f++) myfile[f].close();

    std::cout << "Completed!\n";
}
