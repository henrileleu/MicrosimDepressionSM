// ModèleDépressionAdo.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "vars.h"
#include "parameters.h"
#include "vlsRandGenerator.h"
#include "adverse_events.h"
#include "individual.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <shlobj.h>

parameters<double> p;
vlsRandGenerator rnd;
bool lowerPA;
bool Options_ReverseCausation;
bool noSN;
bool noAddictiveSN;
bool USVersion;
bool Limit1h;
bool intervention;
bool individual_intervention;
std::string userFolder;

using namespace std;

std::string GetUserFolder() {
    wchar_t* widePath;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &widePath);
    if (SUCCEEDED(hr)) {
        std::wstring userFolder(widePath);
        CoTaskMemFree(widePath);
        return std::string(userFolder.begin(), userFolder.end());
    }
    else {
        std::cerr << "Error retrieving user folder path!" << std::endl;
        return "";
    }
}
void run_simulation(std::string userFolder, int i)
{
    std::array< std::ofstream, 4> myfile;
    char fileName[100];

    sprintf_s(fileName, (userFolder + "\\Downloads\\demographics%i.csv").c_str(), i);
    myfile[0].open(fileName);
    sprintf_s(fileName, (userFolder + "\\Downloads\\adverseEvents % i.csv").c_str(), i);
    myfile[1].open(fileName);
    sprintf_s(fileName, (userFolder + "\\Downloads\\socialmedia % i.csv").c_str(), i);
    myfile[2].open(fileName);
    sprintf_s(fileName, (userFolder + "\\Downloads\\psydisorder % i.csv").c_str(), i);
    myfile[3].open(fileName);

    unsigned sample = 1000;
    for (double year = 2000.0 - 18.0; year < 2022.0 - 10.0; year++)
    {
        for (unsigned i = 0; i < sample; i++)
        {

            individual ind;
            ind = individual(year, rnd() < 0.5);
            ind.history();

            // Get ouputs
            //std::stringstream stream;
            //stream << std::hex << static_cast<long>(year) * 100000 + static_cast<long>(i);
            //std::string id(stream.str());
            std::array<char[outputBufferSize], 4> buffer;
            long id(static_cast<long>(year) * 100000 + static_cast<long>(i));
            ind.outputAll(id, buffer);

            // Write output
            for (int f = 0; f < 4; f++) {
                std::string s = buffer[f];
                myfile[f] << s;
            }

        }
    }

    for (int f = 0; f < 4; f++) myfile[f].close();
}

int main()
{

    // User folder
    std::string userFolder = GetUserFolder();
    if (!userFolder.empty()) {
        std::cout << L"User folder: " << userFolder << std::endl;
    }

    // Option menu

    // Options
    lowerPA = false;
    Options_ReverseCausation = false;
    noSN = false;
    noAddictiveSN = false;
    USVersion = false;
    Limit1h = false;
    intervention = false;
    individual_intervention = false;
    bool dsa = false;

    int choice(-1);

    while (choice == -1) {
        cout << "Choose an option:" << endl;
        cout << "0. Basecase" << endl;
        cout << "1. Reverse Causation" << endl;
        cout << "2. No Social Networks" << endl;
        cout << "3. No Addictive Social Networks" << endl;
        cout << "4. Limit social network to one hour a day" << endl;
        cout << "5. Intervention" << endl;
        cout << "6. Individual Intervention" << endl;
        cout << "7. US" << endl;
        cout << "8. DSA" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 0:
            break;
        case 1:
            Options_ReverseCausation = true;
            break;
        case 2:
            noSN = true;
            break;
        case 3:
            noAddictiveSN = true;
            break;
        case 4:
            Limit1h = true;
            break;
        case 5:
            intervention = true;
            break;
        case 6:
            individual_intervention = true;
            break;
        case 7:
            USVersion = true;
            break;
        case 8:
            dsa = true;
            break;
        case 9:
            cout << "Exiting program..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
            break;
        }
    }

    // Parameters are here for now
    std::array<param<double>, NumberOfParameters> default_p({});
    default_p[pParental_psychopathology_Pr] = { 0.437,0.3496,0.5244,2 };
    default_p[pPhysical_abuse_Pr] = { 0.172,0.1376,0.2064,2 };
    default_p[pEmotional_abuse_Pr] = { 0.347,0.2776,0.4164,2 };
    default_p[pSexual_abuse_Pr] = { 0.044,0.0352,0.0528,2 };
    default_p[pNeglect_Pr] = { 0.16,0.128,0.192,2 };
    default_p[pCorrSNUseMentalState] = { 0.25,0.2,0.3,1 };
    default_p[pOptions_ReverseCausation] = { 0,0,0,0 };
    default_p[pCorrSNUsePhysicalActivity] = { 0,0,0,0 };
    default_p[pDurrationDepression_a] = { 0.400576830636116,0.400576830636116,0.400576830636116,0 };
    default_p[pDurrationDepression_b] = { 20.96976,20.96976,20.96976,0 };
    default_p[pDepressionRelapseNoRisk] = { 0.4,0.37362127787151,0.42637872212849,2 };
    default_p[pTimeToDepressionRelapse] = { 0.046494873025874,0.0341874066366721,0.060122680636906,3 };
    default_p[pBackgroundMortalityGompertz_a] = { 0.1227,0.1227,0.1227,0 };
    default_p[pBackgroundMortalityGompertz_b] = { 0.000001993,0.000001993,0.000001993,0 };
    default_p[pBackgroundMortalityGompertz_year] = { -0.01479,-0.01479,-0.01479,0 };
    default_p[pBackgroundMortalityGompertz_sex] = { -0.065,-0.065,-0.065,0 };
    default_p[pReverseCausationDepIncreaseRate] = { -0.045,-0.045,-0.045,0 };
    default_p[pBaselineDepressionRisk] = { 1.8,1.8*0.8,1.8*1.2,0 };
    default_p[pCorrSN] = { 0.1,0.1,0.1,0 };
    default_p[pReverseCausationSNIncrease] = { 1,1,1,0 };
    default_p[pBullyingMale] = { 0.07925,0.07925,0.07925,2 };
    default_p[pBullyingFemale] = { 0.0645,0.0645,0.0645,2 };
    default_p[pParental_psychopathology_OR] = { -0.0861776962410524,0.0512932943875506,-0.215111379616945,1 };
    default_p[pPhysical_abuse_OR] = { 0.127833371509885,0.261364764134408,-0.00995033085316809,1 };
    default_p[pEmotional_abuse_OR] = { -0.285178942233662,-0.139761942375159,-0.431782416425538,1 };
    default_p[pSexual_abuse_OR] = { -0.0392207131532813,0.150822889734584,-0.231111720963387,1 };
    default_p[pNeglect_OR] = { -0.0861776962410524, -0.0861776962410524, -0.0861776962410524,1 };
    default_p[pBullying_OR] = { -0.438254930931155,-0.300104592450338,-0.570979546585738,1 };
    default_p[pAdversities2_OR] = { 0,0.174353387144778,-0.165514438477573,1 };
    default_p[pAdversities3_OR] = { 0,0.0741079721537218,-0.254642218373581,1 };
    default_p[pAdversities4p_OR] = { 0.342490308946776,0.464888528578965,-0.00995033085316809,1 };
    default_p[pSNNumber2_OR] = { -0.451075619360217,-0.0769610411361284,-0.815364813284194,1 };
    default_p[pSNNumber3_OR] = { -0.770108221696074,-0.336472236621213,-1.20297230399235,1 };
    default_p[pSNNumber5_OR] = { -1.12492959698548,-0.647103242058538,-1.60743590976343,1 };
    default_p[pSNDuration30_OR] = { 0.0512932943875506,0.462035459596559,-0.350656871613169,1 };
    default_p[pSNDuration60_OR] = { -0.27763173659828,0.127833371509885,-0.688134638736401,1 };
    default_p[pSNDuration120_OR] = { -0.182321556793955,0.210721031315653,-0.576613364303994,1 };
    default_p[pPA_OR] = { 0.478035800943,0.82098055206983,0.127833371509885,1 };
    default_p[pMale_OR] = { 0.23572233352107,0.49429632181478,-0.0392207131532814,1 };
    default_p[pPA_Freq10Male] = { 0.337,0.26,0.424,2 };
    default_p[pPA_Freq10Female] = { 0.202,0.143,0.277,2 };
    default_p[pPA_Freq14Male] = { 0.401,0.28,0.536,2 };
    default_p[pPA_Freq14Female] = { 0.157,0.097,0.244,2 };
    default_p[pPA_Decrease] = { 0.971272349667177,0.87414511470046,0.961559626170506,2 };
    default_p[pPA_COVIDReduction] = { 0.587,0.587,0.587,2 };
    default_p[pParental_psychopathology_GAD_OR] = { -0.0676586484738149,0.0725706928348354,-0.198850858745165,1 };
    default_p[pPhysical_abuse_GAD_OR] = { 0.116533816255952,0.2484613592985,-0.0198026272961797,1 };
    default_p[pEmotional_abuse_GAD_OR] = { -0.113328685307003,0.0304592074847086,-0.262364264467491,1 };
    default_p[pSexual_abuse_GAD_OR] = { 0.105360515657826,0.287682072451781,-0.0769610411361284,1 };
    default_p[pNeglect_GAD_OR] = { -0.0198026272961797,0.116533816255952,-0.148420005118273,1 };
    default_p[pBullying_GAD_OR] = { -0.46373401623214,-0.322083499169113,-0.598836501088704,1 };
    default_p[pAdversities2_GAD_OR] = { -0.048790164169432,0.116533816255952,-0.215111379616945,1 };
    default_p[pAdversities3_GAD_OR] = { 0.0618754037180875,0.3147107448397,-0.182321556793955,1 };
    default_p[pAdversities4p_GAD_OR] = { 0.27443684570176,0.63487827243597,-0.0953101798043249,1 };
    default_p[pSNNumber2_GAD_OR] = { -0.357674444271816,0.0100503358535015,-0.727548607277278,1 };
    default_p[pSNNumber3_GAD_OR] = { -0.636576829071551,-0.231111720963387,-1.03673688495002,1 };
    default_p[pSNNumber5_GAD_OR] = { -1.18478998490916,-0.698134722070984,-1.66581824587021,1 };
    default_p[pSNDuration30_GAD_OR] = { 0.0725706928348354,0.49429632181478,-0.343589704390077,1 };
    default_p[pSNDuration60_GAD_OR] = { -0.131028262406404,0.261364764134408,-0.524728528934982,1 };
    default_p[pSNDuration120_GAD_OR] = { -0.307484699747961,0.083381608939051,-0.698134722070984,1 };
    default_p[pPA_GAD_OR] = { 0.301105092783922,0.478035800943,0.127833371509885,1 };
    default_p[pMale_GAD_OR] = { 0.430782916092454,0.693147180559945,0.150822889734584,1 };
    default_p[pSuicideAttemps_Depression_OR] = { -1.28923264827676,-1.28923264827676,-1.28923264827676,1 };
    default_p[pSuicideAttemps_GAD_OR] = { -2.03077636969855,-2.03077636969855,-2.03077636969855,1 };
    default_p[pSuicideAttemps_DepressionGAD_OR] = { -2.83085763036376,-2.83085763036376,-2.83085763036376,1 };
    default_p[pSuicideAttemps_Baseline_MaleOR] = { 8.2,8.2,8.2,0 };
    default_p[pSuicideAttemps_Baseline_FemaleOR] = { 6.6,6.6,6.6,0 };
    default_p[pCOVID_GAD_OR] = { -1.48160454092422,-1.28093384546206,-1.68639895357023,0 };
    default_p[pTimeSpentFacebook] = { 0.110617283950617, 0.0884938271604936, 0.13274074074074, 3 };
    default_p[pTimeSpentTwitter] = { 0.0382905982905983, 0.0306324786324786, 0.045948717948718, 3 };
    default_p[pTimeSpentSnapshat] = { 0.343063063063063, 0.27445045045045, 0.411675675675676, 3 };
    default_p[pTimeSpentWhatsapp] = { 0.0311111111111111, 0.0248888888888889, 0.0373333333333333, 3 };
    default_p[pTimeSpentInstagram] = { 0.109268292682927, 0.0874146341463416, 0.131121951219512, 3 };
    default_p[pTimeSpentTikTok] = { 0.40280701754386, 0.322245614035088, 0.483368421052632, 3 };
    default_p[pBaselineGAD_female] = { 2.10,2.65,2.65,0 };
    default_p[pBaselineGAD_male] = { 2.80, 2.65,2.65,0 };
    default_p[pCOVID_Dep_OR] = { -1.48160454092422,-1.28093384546206,-1.68639895357023,0 };
    default_p[pPA_Freq10Male_alt] = { 0.741589172663785, 0.593271338131028, 0.889907007196542, 2 };
    default_p[pPA_Freq10Female_alt] = { 0.666269296938929, 0.533015437551143, 0.799523156326715, 2 };
    default_p[pPA_Freq14Male_alt] = { 0.771831319725874, 0.617465055780699, 0.926197583671049, 2 };
    default_p[pPA_Freq14Female_alt] = { 0.633823948037943, 0.507059158430354, 0.760588737645531, 2 };
    default_p[pObesityMale_a] = { 0.0184249712637903, 0.0184249712637903, 0.0184249712637903, 0 };
    default_p[pObesityMale_b] = { -40.2424952736282, -40.2424952736282, -40.2424952736282, 0 };
    default_p[pObesityFemale_a] = { 0.0289139701217571, 0.0289139701217571, 0.0289139701217571, 0 };
    default_p[pObesityFemale_b] = { -61.2782776766231, -61.2782776766231, -61.2782776766231, 0 };
    default_p[pChronicConditionMale_a] = { 0.00121643806743669, 0.00121643806743669, 0.00121643806743669, 0 };
    default_p[pChronicConditionMale_b] = { -2.41389140719746, -2.41389140719746, -2.41389140719746, 0 };
    default_p[pChronicConditionFemale_a] = { 0.000495036512599704, 0.000495036512599704, 0.000495036512599704, 0 };
    default_p[pChronicConditionFemale_b] = { -0.969375080829743, -0.969375080829743, -0.969375080829743, 0 };
    default_p[pOverweightMale_a] = { -0.0015182932855653, -0.0015182932855653, -0.0015182932855653, 0 };
    default_p[pOverweightMale_b] = { 0.983390568470981, 0.983390568470981, 0.983390568470981, 0 };
    default_p[pOverweightFemale_a] = { 0.00992647409541568, 0.00992647409541568, 0.00992647409541568, 0 };
    default_p[pOverweightFemale_b] = { -21.9693540706075, -21.9693540706075, -21.9693540706075, 0 };
    default_p[pAlcoholMale_a] = { -0.0162922012578616, -0.0162922012578616, -0.0162922012578616, 0 };
    default_p[pAlcoholMale_b] = { 33.2566894339623, 33.2566894339623, 33.2566894339623, 0 };
    default_p[pAlcoholFemale_a] = { -0.0136525798525799, -0.0136525798525799, -0.0136525798525799, 0 };
    default_p[pAlcoholFemale_b] = { 27.6360324324324, 27.6360324324324, 27.6360324324324, 0 };
    default_p[pTabacoMale_a] = { -0.00813298701298701, -0.00813298701298701, -0.00813298701298701, 0 };
    default_p[pTabacoMale_b] = { 16.4897828571429, 16.4897828571429, 16.4897828571429, 0 };
    default_p[pTabacoFemale_a] = { -0.0157877987421384, -0.0157877987421384, -0.0157877987421384, 0 };
    default_p[pTabacoFemale_b] = { 32.2270705660377, 32.2270705660377, 32.2270705660377, 0 };
    default_p[pCannabisMale_a] = { -0.00879832923832924, -0.00879832923832924, -0.00879832923832924, 0 };
    default_p[pCannabisMale_b] = { 17.8098875675676, 17.8098875675676, 17.8098875675676, 0 };
    default_p[pCannabisFemale_a] = { -0.00609974025974026, -0.00609974025974026, -0.00609974025974026, 0 };
    default_p[pCannabisFemale_b] = { 12.3673371428571, 12.3673371428571, 12.3673371428571, 0 };
    default_p[pCannabis_Depression_OR] = { 0.285178942233662, 0.173953307123438, 0.398776119957368, 1 };
    default_p[pTabaco_Depression_OR] = { 0.593326845277734, 0.207014169384326, 1.04731899428056, 1 };
    default_p[pAlcohol_Depression_OR] = { 0.431782416425538, 0.173953307123438, 0.693147180559945, 1 };
    default_p[pOverweight_Depression_OR] = { 0.0392207131532813, -0.0512932943875506, 0.131028262406404, 1 };
    default_p[pObesity_DepressionMale_OR] = { 0.0769610411361284, -0.162518929497775, 0.314810739840034, 1 };
    default_p[pObesity_DepressionFemale_OR] = { 0.364643113587909, 0.182321556793955, 0.542324290825362, 1 };
    default_p[pChronicCondition_Depression_OR] = { 0.385186214231569, 0.162042662917359, 0.567507771025523, 1 };

    // Initiate random number generator
    VSLStreamStatePtr stream;
    vslNewStream(&stream, VSL_BRNG_MT2203, 200882);
    rnd.init(stream);

    // Initiate parameters
    p = parameters<double>(default_p);
    
    if (!dsa)
    {
        // Initiate Rnd for PSA
        VSLStreamStatePtr stream_psa;
        vslNewStream(&stream_psa, VSL_BRNG_MT2203, 1082016);
        vlsRandGenerator rnd_psa;
        rnd_psa.init(stream_psa);

        for (int i = 0; i < 500; i++)
        {
            vslNewStream(&stream, VSL_BRNG_MT2203, 200882);
            rnd.init(stream);
            p.psa(rnd_psa);

            // Change some parameters for US
            if (USVersion)
            {
                p.set(pBaselineDepressionRisk, 1.3);
                p.set(pCOVID_Dep_OR, 0);
            }

            run_simulation(userFolder, i);

            std::cout << "Completed: " << i << "\n";

        }
    }

    if (dsa)
    {
        size_t p_size = p.getN();
        for (int i = 0; i < p_size; i++)
        {
            p.dsa(i, true);
            run_simulation(userFolder, i);
            p.dsa(i, false);
            run_simulation(userFolder, i + p_size);

            std::cout << "Completed: " << i << "out of " << p_size << "\n";
        }

   }

    return 0;
    
    std::array< std::ofstream, 4> myfile;

    myfile[0].open((userFolder + "\\Downloads\\demographics.csv").c_str());
    myfile[1].open((userFolder + "\\Downloads\\adverseEvents.csv").c_str());
    myfile[2].open((userFolder + "\\Downloads\\socialmedia.csv").c_str());
    myfile[3].open((userFolder + "\\Downloads\\psydisorder.csv").c_str());

    unsigned sample = 1000;
    for (double year = 2000.0 - 18.0; year < 2022.0 - 10.0; year++)
    {
        for (unsigned i = 0; i < sample; i++)
        {
       
            individual ind;
            ind = individual(year, rnd() < 0.5);
             ind.history();

            // Get ouputs
            //std::stringstream stream;
            //stream << std::hex << static_cast<long>(year) * 100000 + static_cast<long>(i);
            //std::string id(stream.str());
            std::array<char[outputBufferSize], 4> buffer;
            long id(static_cast<long>(year) * 100000 + static_cast<long>(i));
            ind.outputAll(id,buffer);

            // Write output
            for (int f = 0; f < 4; f++) {
                std::string s = buffer[f];
                myfile[f] << s;
            }

        }
    }
    
    for (int f = 0; f < 4; f++) myfile[f].close();

    std::cout << "Completed!\n";
}
