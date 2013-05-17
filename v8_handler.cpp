#include <v8.h>
#include <v8_handler.h>
#include <util.h>
#include <string>

using namespace v8;

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

// Runs a unit test
CodeStatus run_unit_test(const char* arg){
  std::string param = "execute(";
  param.append(arg);
  param.append(");");
  
  return execute_code(String::New(param.c_str()));
}

// Reads a file into a v8 string.
Handle<String> read_file(const char * name) {
  FILE* file = fopen(name, "rb");
  if (file == NULL){
    return Handle<String>();
  }

  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  rewind(file);

  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (int i = 0; i < size;) {
    int read = fread(&chars[i], 1, size - i, file);
    i += read;
  }
  fclose(file);
  Handle<String> result = String::New(chars, size);
  delete[] chars;
  return result;
}

// Execute javascript code
CodeStatus execute_code(Handle<String> source){
  // Catches Exceptions, don't ask me how it works
  TryCatch try_catch;

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);
  
  // Code Status
  CodeStatus stat;

  if(script.IsEmpty()){
    stat.status = "exception";
    stat.message = report_exception(&try_catch);
  }else{
    // Run the script to get the result.
    Handle<Value> result = script->Run();

    if(result.IsEmpty()){
      stat.status = "exception";
      stat.message = report_exception(&try_catch);
    }else{
      // Convert the result to an ASCII string and print it.
      String::AsciiValue ascii(result);
      stat.status = "complete";
      stat.message = *ascii;
    }
  }

  return stat;
}

// Report Exception
string report_exception(TryCatch* try_catch) {
  HandleScope handle_scope;
  std::string out = "";
  String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  Handle<Message> message = try_catch->Message();
  
  // If there's no message
  if (message.IsEmpty()) {
    out.append(exception_string);
  } else {
    // Get the exception information
    String::Utf8Value filename(message->GetScriptResourceName());
    String::Utf8Value sourceline(message->GetSourceLine());
    String::Utf8Value stack_trace(try_catch->StackTrace());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber();
    
    if (stack_trace.length() > 0) {
      out.append(ToCString(stack_trace));
      out.append("\n");
    }
    
    // Print out
    out.append("\n");
    out.append(ToCString(sourceline));
    out.append("\n");

    int start = message->GetStartColumn();
    for (int i = 0; i < start; i++) {
      out.append(" ");
    }
    
    int end = message->GetEndColumn();
    for (int i = start; i < end; i++) {
      out.append("^");
    }
    
    out.append("\n");
  }
  return out;
}