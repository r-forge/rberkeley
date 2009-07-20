#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

/* RBerkeley Constants */

#define RBerkeley_DB        install("DB")
#define RBerkeley_DB_ENV    install("DB_ENV")
#define RBerkeley_DBC       install("DBC")

SEXP rberkeley_fclose (SEXP _file_ptr);
