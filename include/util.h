#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

struct CodeStatus{
  string status;
  string message;
};

struct IOPair{
  string type;
  string value;
};


class Test{
  public:
    Test(char* tid, int p_count, char* argv[], IOPair expected_out) ;
    string generate_input_string() ;
  
  
    string tid;
    string status;
    
    IOPair gen_output;
    IOPair expected_output;
    vector<IOPair> inputs;
};

#endif