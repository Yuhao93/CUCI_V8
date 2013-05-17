#ifndef V8_HANDLER_H
#define V8_HANDLER_H

#include <v8.h>
#include <util.h>
#include <string>

using namespace v8;

CodeStatus execute_code(Handle<String> source);
CodeStatus run_unit_test(const char* arg);
string report_exception(TryCatch* try_catch);
Handle<String> read_file(const char * name);


#endif;