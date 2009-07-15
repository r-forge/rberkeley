#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

SEXP rberkeley_check_pointer (SEXP _dbh)
{
  int *ptr;

  if(TYPEOF(_dbh) != EXTPTRSXP) {
    warning("invalid database handle");
    return ScalarInteger(0);
  }
  ptr = R_ExternalPtrAddr(_dbh);
  if(ptr == NULL) {
    return ScalarInteger(0);
  } else return ScalarInteger(1);
}
