#include "error_handlers.h"

int get_error_cpp() {
   // Catch a handful of different errors here and turn them into the
   // equivalent Python errors.
   try {
      throw;
   } catch (exception& e) {
     PyErr_SetString(PyExc_RuntimeError, "my error");
   } catch (...) {
     PyErr_SetString(PyExc_RuntimeError, "error but I have not clue");
   }
   return 0;
}

const char * get_exception_what() {
   try {
   throw;
   } catch (exception& e) {
     return e.what();
   } catch (...) {
     return("error but I have not clue");
   }
}