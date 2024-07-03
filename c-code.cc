/*
Reference: The New WBF IMP to VP Scales
Technical Report of WBF Scoring Panel (January 2013)
*/
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    const double R = 0.2360679775; /* == Tau^3 == sqrt(5) - 2 */
    const double V0 = 10.0; /* VP score in drawn match */
    double N, /* number of boards played */
           X, /* maximum win IMP margin, 15*sqrt(N) */
           VP,
           VPraw;

    cout << "number of boards played: ";
    cin >> N;

    // CALCULATION OF RAW VP SCORES
    X = 15 * sqrt(N);
    int blitz = ceil(X); // IMP margin for 20-0 score
    double * scores; // array of VP scores
    scores = new double[blitz+1]; // including zero margin
    for (int i=0; i!=blitz+1; ++i) // i is the IMP margin
    {
        // calculate VP scores
        VPraw = V0 + V0 * ((1 - pow(R, i/X)) / (1 - R));
        VP = min((round(100 * VPraw) /100), 2*V0); // rounding
        // add score to array
        scores[i] = VP;
    }

    // CORRECTION OF CONCAVITY RULE VIOLATIONS
    int * diff; // array of differences
    diff = new int[blitz];
    for (int i=0; i!=blitz; ++i)
        diff[i] = round(100*(scores[i+1] - scores[i]));

    int * diff2; // array of second differences
    diff2 = new int[blitz-1];
    for (int i=0; i!=blitz-1; ++i)
        diff2[i] = diff[i+1] - diff[i];

    bool violation = true;
    while (violation)
    {
        violation = false;
        for (int i=0; i!=blitz-1; ++i)
        {
            if (diff2[i] > 0)
            {
                scores[i+1] += 0.01;
                --diff2[i];
                violation = true;
            }
        }
        if (violation)
        { /* recalculate arrays diff and diff2 */
            for (int i=0; i!=blitz; ++i)
                diff[i] = round(100*(scores[i+1] - scores[i]));
            for (int i=0; i!=blitz-1; ++i)
                diff2[i] = diff[i+1] - diff[i];
        }
    }

    // SEND SCALE TO SCREEN
    cout << endl << endl;
    cout << "WBF CONTINUOUS VP SCALE for " << N << " boards" << endl
         << "(trailing zeros not shown)"
         << endl << endl << endl << "IMP margin and score" << endl;
    for (int i=0; i!=blitz+1; ++i)
        cout << i << "  " << scores[i] << " - " << 20 - scores[i] << endl;
    cout << endl << endl;

    delete [] scores; delete [] diff; delete [] diff2;
    char ch; cin >> ch;

    return 0;
}
