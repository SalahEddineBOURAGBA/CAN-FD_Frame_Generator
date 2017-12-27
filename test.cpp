#include "test.h"

using namespace std;

void test_generator(unsigned int MaxSize)
{
    unsigned int i,error(0);
    string test;
    vector <bool> frame;

    //Generate MaxSize random frames
    ofstream file("generated_frames.txt", ios::app);
    for(i=0;i<MaxSize;i++)
    {
        frame=generate_frame();

        for(unsigned int j=0;j<frame.size();j++)
            file <<frame[j];
        file <<endl;

        //free
        frame.clear();
    }
    file.close();

    //Search the generated frames  for errors
    ifstream file2("generated_frames.txt");
    for(i=0;i<MaxSize;i++)
    {
        getline(file2,test);
        frame=convert_string_to_bool_vector(test);

        //analyze_frame returns 0 if the frame is erroneous
        if(analyze_frame(frame)==0)
            error++;
    }
    cout <<"There are : "<<error <<" errors." <<endl;
    file2.close();
}

vector <bool> convert_string_to_bool_vector(string input)
{
    vector <bool> output;

    for(unsigned int i=0;i<input.size();i++)
    {
        if(input[i]=='1')
            output.push_back(1);
        else if(input[i]=='0')
            output.push_back(0);
    }

    return output;
}
