#include <util.h>
#include <string>
#include <algorithm>

using namespace std;

string add_slashes(string str){
  string out = "";
  
  const char* c_str = str.c_str();
  for(size_t i = 0; i < str.size(); i ++){
    if(c_str[i] == '\\'){
      out.append("\\\\");
    }else if(c_str[i] == '\"'){
      out.append("\\\"");
    }else{
      out.push_back(c_str[i]);
    }
  }
  
  return out;
}

string format_string(IOPair io_pair){
  string type = io_pair.type;
  string value = io_pair.value;
  string str = "";
  if(type.compare("String") == 0 || type.compare("char") == 0){
    str.append("\"");
    str.append(add_slashes(value));
    str.append("\"");
  }else{
    str.append(value);
  }
  return str;
}

Test::Test(char* tid, int p_count, char* argv[], IOPair expected_out) 
    : tid(tid), status(""), expected_output(expected_out){
  for (int i = 0; i < p_count * 2; i += 2){
    IOPair input;
    
    input.type = argv[i];
    input.value = argv[i + 1];
    
    inputs.push_back(input);
  }
}

string Test::generate_input_string(){
  string str;
  
  for_each(inputs.begin(), inputs.end(), [&str](IOPair io_pair) -> void {
    str.append(format_string(io_pair));
    str.append(",");
  });
  
  if(str.size() > 0){
    str = str.substr(0, str.size() - 1);
  }
  
  return str;
}