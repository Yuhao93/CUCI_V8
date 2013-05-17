#include <util.h>
#include <v8_handler.h>
#include <pthread.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace v8;

Handle<String> file_content;
vector<Test> tests;

/**
 * Runs a given class against the unit tests
 */
void* run_engine(void* data){
  // Initalize V8 context stuff
  
  Persistent<Context> context = Context::New();
  Context::Scope context_scope(context);
  
  // Catches Exceptions, don't ask me how it works...
  TryCatch try_catch;
  
  // If an exception has occurred while testing
  bool test_exception = false;
  
  // Run the the user and setup code
  CodeStatus result = execute_code(file_content);
  if(result.status.compare("exception") == 0){
    printf("%s", result.message.c_str());
    return NULL;
  }

  // Run each unit test
  for_each(tests.begin(), tests.end(), [&test_exception](Test test) -> void {
    CodeStatus status = run_unit_test(test.generate_input_string().c_str());
    if(status.status.compare("exception") == 0){
      // If unit test execution resulted in an exception
      test_exception = true;
      
      return;
    }
    
    // If unit test output matches expected
    if(status.message.compare(test.expected_output.value) == 0){
      
      
    // If unit test output doesn't match expected
    }else{
      
    }
  });
  
  // Get the console
  CodeStatus consoleOut = execute_code(String::New("_cons_;"));
  if(consoleOut.status.compare("complete") == 0){
    printf("CONSOLE : %s", consoleOut.message.c_str()); 
  }

  context.Dispose();
  return NULL;
}  


/**
 *
 * Input is Classname, follwed by k, number of unit tests
 * Then followed by k unit tests
 *   each unit test contains tid and then l, number of inputs,
 *     followed by l inputs, each is a type followed by value
 *     then by output type and value 
 */
int main(int argc, char * argv[]){
  if(argc <= 2){
    return 0;
  }
  HandleScope handle_scope;
  
  // Load the file
  file_content = read_file(argv[1]);
  
  // Run the unit tests
  int test_case_count = atoi(argv[2]);

  // Loop through
  int ind = 3;
  for(int i = 0; i < test_case_count; i ++){
  
    // Test Id
    char* tid = argv[ind++];
    
    // How many parameters each test has
    int param_count = atoi(argv[ind++]);
    
    // Array of Input type/values
    char ** args = new char*[param_count * 2];
    for(int j = 0; j < param_count * 2; j ++){
      args[j] = argv[ind++];
    }
    
    // Expected Output
    IOPair expected_out;
    expected_out.type = argv[ind++];
    expected_out.value = argv[ind++];
    
    // Create the Test object and add to list
    Test test(tid, param_count, args, expected_out);
    tests.push_back(test);
    
    // Delete the dynamically allocated memory
    delete[] args;
  }

  pthread_t exec_thread;
  pthread_create(&exec_thread, NULL, &run_engine, NULL);
  int count = 0;
  int MAX_COUNT = 60;
  bool success = false;
  
  for(int i = 0; i < MAX_COUNT; i ++){
    if(pthread_kill(exec_thread, 0) != 0){
      success = true;
      break;
    }
    Sleep(1);
  }
  
  // Kill Code Excecution
  if(!success){
    V8::TerminateExecution();
  }
  
  return 0;
}