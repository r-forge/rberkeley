#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#define	DATABASE "access.db"

/* {{{ do_db_version */
SEXP do_db_version ()
{
  int major, minor, patch;
  major = minor = patch = 1;
  char *version;

  version = db_version(&major, &minor, &patch);

  SEXP Version, names;
  PROTECT(Version = allocVector(VECSXP, 4));
  SET_VECTOR_ELT(Version, 0, mkString(version));
  SET_VECTOR_ELT(Version, 1, ScalarInteger(major));
  SET_VECTOR_ELT(Version, 2, ScalarInteger(minor));
  SET_VECTOR_ELT(Version, 3, ScalarInteger(patch));

  PROTECT(names = allocVector(STRSXP, 4));
  SET_STRING_ELT(names, 0, mkChar("Version")); 
  SET_STRING_ELT(names, 1, mkChar("major")); 
  SET_STRING_ELT(names, 2, mkChar("minor")); 
  SET_STRING_ELT(names, 3, mkChar("patch")); 
  
  setAttrib(Version, R_NamesSymbol, names);
  UNPROTECT(2);
  return Version;
}
/* }}} */
/* {{{ do_db_create */
SEXP do_db_create (SEXP _dbenvp)
{
  DB *dbp;
  int ret;

  /* could use env instead of NULL... */
  ret = db_create(&dbp, NULL, 0);
  if(ret==0)
    return R_MakeExternalPtr(dbp, R_NilValue, R_NilValue);
  return ScalarInteger(ret);
}
/* }}} */
/* do_db_associate */
/* {{{ do_db_close */
SEXP do_db_close(SEXP _dbp)
{
  DB *dbp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->close(dbp, 0);
  return(ScalarInteger(ret));
}
/* }}} */
/* do_db_compact */
/* {{{ do_db_del */
SEXP do_db_del(SEXP _dbp, SEXP _key)
{
  DB *dbp;
  DBT key, data;
  int ret;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	key.data = (char *)CHAR(STRING_ELT(_key,0));
	key.size = strlen(key.data)+1;

  dbp = R_ExternalPtrAddr(_dbp);

	if ((ret = dbp->del(dbp, NULL, &key, 0)) == 0)
		Rprintf("db: %s: key was deleted.\n", (char *)key.data);
	else {
		dbp->err(dbp, ret, "DB->get");
	}
    return R_NilValue;
}
/* }}} */
/* do_db_err */
/* do_db_errx */
/* do_db_exists */
/* {{{ do_db_fd */
SEXP do_db_fd (SEXP _dbp)
{
  DB *dbp;
  int fdp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->fd(dbp, &fdp);

  return ScalarInteger(fdp);
}
/* }}} */
/* {{{ do_db_get */
SEXP do_db_get(SEXP _dbp, SEXP _key)
{
  DB *dbp;
  DBT key, data;
  int ret;

  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));
  key.data = (char *)CHAR(STRING_ELT(_key,0));
  key.size = strlen(key.data)+1; //LENGTH(STRING_ELT(_key,0));

  dbp = R_ExternalPtrAddr(_dbp);

  if ((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0) {
    return mkString(data.data);
  } else {
    dbp->err(dbp, ret, "DB->get");
    return R_NilValue;
  }
}
/* }}} */
/* do_db_get_byteswapped */
/* {{{ do_db_get_type */
SEXP do_db_get_type (SEXP _dbp)
{
  DB *dbp;
  DBTYPE type;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->get_type(dbp, &type);

  switch(type) {
    case DB_BTREE:
      return mkString("DB_BTREE");
      break;
    case DB_HASH:
      return mkString("DB_HASH");
      break;
    case DB_RECNO:
      return mkString("DB_RECNO");
      break;
    case DB_QUEUE:
      return mkString("DB_QUEUE");
      break;
  }
}
/* }}} */

/* do_db_join */
/* do_db_key_range */
/* {{{ do_db_open */
SEXP do_db_open (SEXP _dbp, SEXP _dbenvp)
{
	DB *dbp;
	int ret;

	/* Create the database handle and open the underlying database. */
    dbp = R_ExternalPtrAddr(_dbp);
	if ((ret = dbp->open(dbp,
	    NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0664)) != 0) {
		dbp->err(dbp, ret, "%s", DATABASE);
	}
    return ScalarInteger(ret);
}
/* }}} */
/* {{{ do_db_put */
SEXP do_db_put(SEXP _dbp, SEXP _key, SEXP _value)
{
  DB *dbp;
  DBT key, data;
  int ret;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	key.data = (char *)CHAR(STRING_ELT(_key,0));
	key.size = strlen(key.data)+1;
	data.data = (char *)CHAR(STRING_ELT(_value,0));
	data.size = strlen(data.data)+1;

  dbp = R_ExternalPtrAddr(_dbp);

	/* Store a key/data pair. */
	if ((ret = dbp->put(dbp, NULL, &key, &data, 0)) == 0)
		Rprintf("db: %s: key stored.\n", (char *)key.data);
	else {
		dbp->err(dbp, ret, "DB->put");
	}
    return R_NilValue;

}
/* }}} */
/* {{{ do_db_remove */
SEXP do_db_remove (SEXP _dbp, SEXP _file, SEXP _database)
{
  DB *dbp;
  int ret;
  u_int32_t flags = 0;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->remove(dbp,
                    (const char *)CHAR(STRING_ELT(_file,0)),
                    (const char *)CHAR(STRING_ELT(_database,0)),
                    flags);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ do_db_rename */
SEXP do_db_rename (SEXP _dbp, SEXP _file, SEXP _database,
                   SEXP _newname)
{
  DB *dbp;
  int ret;
  u_int32_t flags = 0;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->rename(dbp, 
                    (const char *)CHAR(STRING_ELT(_file,0)),
                    (const char *)CHAR(STRING_ELT(_database,0)),
                    (const char *)CHAR(STRING_ELT(_newname,0)),
                    flags);

  return ScalarInteger(ret);
}
/* }}} */
/* do_db_set_priority */
/* do_db_stat */
/* do_db_stat_print */
/* {{{ do_db_sync */
SEXP do_db_sync(SEXP _dbp)
{
  DB *dbp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->sync(dbp, 0);
  return(ScalarInteger(ret));
}
/* }}} */
/* {{{ do_db_truncate */
SEXP do_db_truncate (SEXP _dbp, SEXP _txnid)
{
  DB *dbp;
  DB_TXN *txnid;
  u_int32_t countp, flags = 0;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);

  if(!isNull(_txnid)) {
    txnid = R_ExternalPtrAddr(_txnid);
    ret = dbp->truncate(dbp, txnid, &countp, flags);
  } else {
    ret = dbp->truncate(dbp, NULL, &countp, flags);
  }

  return ScalarInteger(countp); 
}
/* }}} */
/* do_db_upgrade */
/* do_db_verify */

/*** Database Configuration ***/
/* do_db_set_alloc   */
/* do_db_set_cachesize */
/* do_db_set_dup_compare */
/* do_db_set_encrypt */
/* do_db_set_errcall */
/* do_db_set_msgcall  */
/* do_db_set_errfile */
/* do_db_set_msgfile */
/* do_db_set_errpfx */
/* do_db_set_feedback */
/* do_db_set_flags */
/* do_db_set_lorder */
/* do_db_set_pagesize */

/*** BTree/Recno Configuration ***/    
/* do_db_set_append_recno */
/* do_db_set_bt_compare */
/* do_db_set_bt_minkey */
/* do_db_set_bt_prefix */
/* do_db_set_re_delim */
/* do_db_set_re_len */
/* do_db_set_re_pad */
/* do_db_set_re_source */

/*** Hash Configuration ***/    
/* do_db_set_h_compare */
/* do_db_set_h_ffactor */
/* do_db_set_h_hash */
/* do_db_set_h_nelem  */

/*** Queue Configuration ***/
/* do_db_set_q_extentsize */
