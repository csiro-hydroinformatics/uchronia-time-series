// This is a SWIG IDL file to test the feasibility for uchronia
/* File : manual_test.i */
%module uchronia
%{
/* Put headers and other declarations here */
#include <stdbool.h> 

%include "../uchronia/data/structs_cdef.h"
%include "../uchronia/data/funcs_cdef.h"

%}
// -*- c++ -*-

%include typemaps.i

%apply int *OUTPUT { int *size };

// To cater (TBC) for things such as:
// extern char** GetRunoffModelIdentifiers(int* size);
%typemap(in) char** (int* size) {
  int i;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $1 = PyList_Size($input);
  $2 = (char **) malloc(($1+1)*sizeof(char *));
  for (i = 0; i < $1; i++) {
    PyObject *s = PyList_GetItem($input,i);
    if (!PyString_Check(s)) {
        free($2);
        PyErr_SetString(PyExc_ValueError, "List items must be strings");
        return NULL;
    }
    $2[i] = PyString_AsString(s);
  }
  $2[i] = 0;
}

%typemap(freearg) (int argc, char *argv[]) {
   if ($2) free($2);
}

%typemap(out) char** {
  int len;
  int i;
  len = *arg1;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result, i, PyString_FromString($1[i]));
  }
}


//////////////////////////
%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init %{
import_array();
%}

%apply (double* IN_ARRAY1, int DIM1) {(double* seq, int n)};

%include "../uchronia/data/structs_cdef.h"
%include "../uchronia/data/funcs_cdef.h"
