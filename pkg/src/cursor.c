#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

/* {{{ rberkeley_db_cursor */
SEXP rberkeley_db_cursor (SEXP _dbp, SEXP _txnid, SEXP _flags)
{
	DB *dbp;
    DBC *cursor;
    DB_TXN *txnid;
	int ret;
    u_int32_t flags = INTEGER(_flags)[0];

    dbp = R_ExternalPtrAddr(_dbp);

    if(!isNull(_txnid)) {
      txnid = R_ExternalPtrAddr(_txnid);
    } else txnid = NULL;

    ret = dbp->cursor(dbp, txnid, &cursor, flags);

    if(ret != 0)
      return ScalarInteger(ret);

    return R_MakeExternalPtr(cursor, R_NilValue, R_NilValue);
}
/* }}} */
/* {{{ rberkeley_dbcursor_close */
SEXP rberkeley_dbcursor_close (SEXP _dbc)
{
  DBC *dbc;
  int ret;

  dbc = R_ExternalPtrAddr(_dbc);

  ret = dbc->close(dbc);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_dbcursor_count */
SEXP rberkeley_dbcursor_count (SEXP _dbc)
{
  DBC *dbc;
  db_recno_t countp;
  u_int32_t flags;
  int ret;

  flags = 0; /* unsed by API */

  dbc = R_ExternalPtrAddr(_dbc);

  ret = dbc->count(dbc, &countp, flags);

  if(ret != 0)
    return ScalarInteger(ret);

  return ScalarInteger((u_int32_t)countp);
}
/* }}} */
