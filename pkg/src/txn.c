#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "RBerkeley.h"
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

/* Berkeley DB: Transaction Subsystem and Related Methods */

/* Transaction Subsystem and Related Methods */

/* Transaction Subsystem Configuration */

/* Transaction Operations */
/* rberkeley_dbenv_txn_begin */
SEXP rberkeley_dbenv_txn_begin (SEXP _dbenv, SEXP _parent, SEXP _flags) {
  DB_ENV *dbenv;
  DB_TXN *parent, *tid;
  u_int32_t flags;
  int ret;

  flags = (u_int32_t)INTEGER(_flags)[0]; /* currently must be 0L */

  dbenv = R_ExternalPtrAddr(_dbenv);
  if(R_ExternalPtrTag(_dbenv) != RBerkeley_DB_ENV || dbenv == NULL)
    error("invalid 'dbenv' handle");

  if(!isNull(_parent)) {
    parent = R_ExternalPtrAddr(_parent); /* check for txnid */
  } else parent = NULL;

  ret = dbenv->txn_begin(dbenv, parent, &tid, flags);

  if(ret != 0)
    return ScalarInteger(ret);

  return R_MakeExternalPtr(tid, install("DB_TXN"), ScalarLogical(TRUE));   
}
/* }}} */
