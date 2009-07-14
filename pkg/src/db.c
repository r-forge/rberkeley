#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#define	DATABASE "access.db"

/* {{{ rberkeley_db_version */
SEXP rberkeley_db_version ()
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
/* {{{ rberkeley_db_create */
SEXP rberkeley_db_create (SEXP _dbenv)
{
  DB *dbp;
  DB_ENV *dbenv;
  int ret;

  /* could use env instead of NULL... */
  if(isNull(_dbenv)) {
    ret = db_create(&dbp, NULL, 0);
  } else {
    dbenv = R_ExternalPtrAddr(_dbenv);
    ret = db_create(&dbp, dbenv, 0);
  }
  if(ret==0)
    return R_MakeExternalPtr(dbp, R_NilValue, R_NilValue);
  return ScalarInteger(ret);
}
/* }}} */
/* rberkeley_db_associate */
/* {{{ rberkeley_db_close */
SEXP rberkeley_db_close(SEXP _dbp)
{
  DB *dbp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->close(dbp, 0);
  return(ScalarInteger(ret));
}
/* }}} */
/* rberkeley_db_compact */
/* {{{ rberkeley_db_del */
SEXP rberkeley_db_del(SEXP _dbp, SEXP _key)
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
/* rberkeley_db_err */
/* rberkeley_db_errx */
/* {{{ rberkeley_db_exists */
SEXP rberkeley_db_exists (SEXP _dbp, SEXP _txnid, SEXP _key, SEXP _flags)
{
  DB *dbp;
  DB_TXN *txnid;
  DBT key;
  u_int32_t flags;
  int ret;

  if(TYPEOF(_flags) == INTSXP)
    flags = (u_int32_t)INTEGER(_flags)[0];
  else flags=0;

  flags = 0;

  switch(TYPEOF(_key)) {
    case STRSXP:
      key.data = (char *)CHAR(STRING_ELT(_key, 0));
      key.size = strlen(key.data)+1;
      break;
    default:
    error("unsupported key type");
  } 

  dbp = R_ExternalPtrAddr(_dbp);

  if(!isNull(_txnid)) {
    txnid = R_ExternalPtrAddr(_txnid);
    ret = dbp->exists(dbp, txnid, &key, flags);
  } else {
    ret = dbp->exists(dbp, NULL,  &key, flags);
  }
  
  if(ret == DB_NOTFOUND)
    return mkString("DB_NOTFOUND");
  if(ret == DB_KEYEMPTY)
    return mkString("DB_KEYEMPTY");

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_fd */
SEXP rberkeley_db_fd (SEXP _dbp)
{
  DB *dbp;
  int fdp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->fd(dbp, &fdp);

  return ScalarInteger(fdp);
}
/* }}} */
/* {{{ rberkeley_db_get */
SEXP rberkeley_db_get(SEXP _dbp, SEXP _key)
{
  DB *dbp;
  DBT key, data;
  int ret;

  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));

  switch(TYPEOF(_key)) {
    case STRSXP:
      key.data = (char *)CHAR(STRING_ELT(_key,0));
      key.size = strlen(key.data)+1;
      break;
    case RAWSXP:
      key.data = (unsigned char *)RAW(_key);
      key.size = length(_key);
      break;
  }

  dbp = R_ExternalPtrAddr(_dbp);

  if ((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0) {
    SEXP retdata;
    PROTECT(retdata = allocVector(RAWSXP, data.size));
    memcpy(RAW(retdata), data.data, data.size);
    UNPROTECT(1);
    return retdata;
  } else {
    dbp->err(dbp, ret, "DB->get");
    return R_NilValue;
  }
}
/* }}} */
/* rberkeley_db_get_byteswapped */
/* {{{ rberkeley_db_get_type */
SEXP rberkeley_db_get_type (SEXP _dbp)
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

/* rberkeley_db_join */
/* rberkeley_db_key_range */
/* {{{ rberkeley_db_open */
SEXP rberkeley_db_open (SEXP _dbp, SEXP _txnid, SEXP _file /*,
                       SEXP _database, SEXP _type, SEXP _flags,
                       SEXP _mode*/)
{
	DB *dbp;
    DB_TXN *txnid;
	int ret;

    dbp = R_ExternalPtrAddr(_dbp);

    if(!isNull(_txnid)) {
      txnid = R_ExternalPtrAddr(_txnid);
    } else txnid = NULL;

    const char * file = CHAR(STRING_ELT(_file,0));

	if ((ret = dbp->open(dbp,
	    NULL, file, NULL, DB_BTREE, DB_CREATE, 0664)) != 0) {
		dbp->err(dbp, ret, "%s", DATABASE);
	}
    return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_put */
SEXP rberkeley_db_put(SEXP _dbp, SEXP _key, SEXP _data)
{
  DB *dbp;
  DBT key, data;
  int ret;

  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));

  switch(TYPEOF(_key)) {
    case STRSXP:
      key.data = (char *)CHAR(STRING_ELT(_key,0));
      key.size = strlen(key.data)+1;
      break;
    case RAWSXP:
      key.data = (unsigned char *)RAW(_key);
      key.size = length(_key);
      break;
  }
  switch(TYPEOF(_data)) {
/* need to memcpy for this to work I assume... */
    case STRSXP:
      data.data = (char *)CHAR(STRING_ELT(_data,0));
      data.size = strlen(data.data)+1;
      break;
    case RAWSXP:
      data.data = (unsigned char *)RAW(_data);
      data.size = length(_data);
  }

  dbp = R_ExternalPtrAddr(_dbp);

Rprintf("data.size: %i\n", data.size);
Rprintf("key.size: %i\n", key.size);
	/* Store a key/data pair. */
	if ((ret = dbp->put(dbp, NULL, &key, &data, 0)) == 0) {
		/*Rprintf("db: %s: key stored.\n", (char *)key.data);*/
	} else {
		dbp->err(dbp, ret, "DB->put");
	}
    return R_NilValue;

}
/* }}} */
/* {{{ rberkeley_db_remove */
SEXP rberkeley_db_remove (SEXP _dbp, SEXP _file, SEXP _database)
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
/* {{{ rberkeley_db_rename */
SEXP rberkeley_db_rename (SEXP _dbp, SEXP _file, SEXP _database,
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
/* rberkeley_db_set_priority */
/* rberkeley_db_stat */
/* rberkeley_db_stat_print */
/* {{{ rberkeley_db_sync */
SEXP rberkeley_db_sync(SEXP _dbp)
{
  DB *dbp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  ret = dbp->sync(dbp, 0);
  return(ScalarInteger(ret));
}
/* }}} */
/* {{{ rberkeley_db_truncate */
SEXP rberkeley_db_truncate (SEXP _dbp, SEXP _txnid)
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
/* rberkeley_db_upgrade */
/* rberkeley_db_verify */

/*** Database Configuration ***/
/* rberkeley_db_set_alloc   */
/* rberkeley_db_set_cachesize */
/* rberkeley_db_set_dup_compare */
/* rberkeley_db_set_encrypt */
/* rberkeley_db_set_errcall */
/* rberkeley_db_set_msgcall  */
/* rberkeley_db_set_errfile */
/* rberkeley_db_set_msgfile */
/* rberkeley_db_set_errpfx */
/* rberkeley_db_set_feedback */
/* rberkeley_db_set_flags */
/* rberkeley_db_set_lorder */
/* rberkeley_db_set_pagesize */

/*** BTree/Recno Configuration ***/    
/* rberkeley_db_set_append_recno */
/* rberkeley_db_set_bt_compare */
/* rberkeley_db_set_bt_minkey */
/* rberkeley_db_set_bt_prefix */
/* rberkeley_db_set_re_delim */
/* rberkeley_db_set_re_len */
/* rberkeley_db_set_re_pad */
/* rberkeley_db_set_re_source */

/*** Hash Configuration ***/    
/* rberkeley_db_set_h_compare */
/* rberkeley_db_set_h_ffactor */
/* rberkeley_db_set_h_hash */
/* rberkeley_db_set_h_nelem  */

/*** Queue Configuration ***/
/* rberkeley_db_set_q_extentsize */
