#include <vector>
#include "analyzer_functions.h"
#include "generator_functions.h"
#include "functions.h"
#include "test.h"

using namespace std;

int main()
{
    vector <bool> frame;

    //Generate 10 random frames and analyze them
    for(unsigned int i=0;i<10;i++)
    {
        analyze_frame(generate_frame());
        cout<<"------------------------------------------------"<<endl;
    }

    //Test the generator function
    //test_generator(1000);

    return 0;
}

