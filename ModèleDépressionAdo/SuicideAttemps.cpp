#include "SuicideAttemps.h"

SuicideAttemps::SuicideAttemps()
{
}

void SuicideAttemps::generateEpisode(double* param, std::vector<psyEpisode>& PsyDisorder)
{

    double baseline = (param[0] == 0.0 ? p[pSuicideAttemps_Baseline_MaleOR] : p[pSuicideAttemps_Baseline_FemaleOR]);
    double suicide_ors[] = { p[pSuicideAttemps_Depression_OR], p[pSuicideAttemps_GAD_OR], p[pSuicideAttemps_DepressionGAD_OR] };

    // Reparse Mood et Anxiety
    bool mood[numberOfAgeGroups] = {};
    bool anxiety[numberOfAgeGroups] = {};
    reparseEpisodes(mood, PsyDisorder, psychatric_disorder::depression);
    reparseEpisodes(anxiety, PsyDisorder, psychatric_disorder::anxiety);

    // Parse through each year
    for (int i = 0; i < numberOfAgeGroups; i++)
    {
        double risk(baseline);
        if (mood[i] && anxiety[i]) risk += suicide_ors[2];
        else
        {
            risk += (mood[i] ? suicide_ors[0] : 0);
            risk += (anxiety[i] ? suicide_ors[1] : 0);
        }

        // Estimate risk
        risk = 1 / (1 + exp(risk));

          if (rnd() < risk) {
            double start(static_cast<double>(i));
            double end(start + 1);

            PsyDisorder.push_back({ psychatric_disorder::suicide, start,end });

        }
    }

}

void SuicideAttemps::reparseEpisodes(bool* episodes, std::vector<psyEpisode> &source, psychatric_disorder type)
{

    for (size_t i = 0; i < source.size(); i++)
    {
        int a(static_cast<int>(source[i].start + 0.5));
        int b(static_cast<int>(source[i].end + 0.5));

        if (a >= numberOfAgeGroups || b >= numberOfAgeGroups) break; // Episodes need to be kept in increasing order

        if (source[i].type == type)
        {
            episodes[a] = true;
            episodes[b] = true;
        }
    }
}
