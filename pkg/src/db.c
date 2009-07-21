#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "RBerkeley.h"
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
/* {{{ rberkeley_db_strerror */
SEXP rberkeley_db_strerror (SEXP _error)
{
  return mkString(db_strerror(INTEGER(_error)[0]));
}
/* }}} */
/* {{{ rberkeley_db_create */
SEXP rberkeley_db_create (SEXP _dbenv)
{
  DB *dbp;
  DB_ENV *dbenv;
  int ret;

  dbp = (DB *)Calloc(1, DB);

  if(isNull(_dbenv)) {
    ret = db_create(&dbp, NULL, 0);
  } else {
    dbenv = R_ExternalPtrAddr(_dbenv);
  if(R_ExternalPtrTag(_dbenv) != RBerkeley_DB_ENV || dbenv == NULL)
    error("invalid 'dbenv' handle");
    ret = db_create(&dbp, dbenv, 0);
  }
  if(ret==0) {
    SEXP ptr = R_MakeExternalPtr(dbp, RBerkeley_DB, ScalarLogical(TRUE));
    return ptr;
  }  
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
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  ret = dbp->close(dbp, 0);
  warning("'db' handle may not be accessed again");
  R_ClearExternalPtr(_dbp);

  return(ScalarInteger(ret));
}
/* }}} */
/* {{{ rberkeley_db_compact */
SEXP rberkeley_db_compact (SEXP _dbp, SEXP _txnid, SEXP _start,
                           SEXP _stop, SEXP _c_data, SEXP _flags)
{
  DB *dbp;
  DB_TXN *txnid;
  DBT start, stop, end;
  DB_COMPACT c_data;
  u_int32_t flags;
  int ret;

  if(isNull(_txnid)) {
    txnid = R_ExternalPtrAddr(_txnid);
  } else {
    txnid = NULL;
  }
  if(!isNull(_start)) {
    memset(&start, 0, sizeof(DBT));
    start.data = (unsigned char *)RAW(_start);
    start.size = length(_start);
  }
  if(!isNull(_stop)) {
    memset(&stop, 0, sizeof(DBT));
    stop.data = (unsigned char *)RAW(_stop);
    stop.size = length(_stop);
  }
  flags = (u_int32_t)INTEGER(_flags)[0];
  /*memset(&end, 0, sizeof(end));*/

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->compact(dbp, txnid, &start, &stop, NULL, flags, &end); 

  return ScalarInteger(ret);
}
/* }}} */
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
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

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

  memset(&key, 0, sizeof(DBT));

  if(TYPEOF(_flags) == INTSXP)
    flags = (u_int32_t)INTEGER(_flags)[0];
  else flags=0;

  flags = 0;

  switch(TYPEOF(_key)) {
    case STRSXP:
      key.data = (char *)CHAR(STRING_ELT(_key, 0));
      key.size = strlen(key.data)+1;
      break;
    case RAWSXP:
      key.data = (unsigned char *)RAW(_key);
      key.size = length(_key);
      break;
    default:
    error("unsupported key type");
  } 

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  if(!isNull(_txnid)) {
    txnid = R_ExternalPtrAddr(_txnid);
    ret = dbp->exists(dbp, txnid, &key, flags);
  } else {
    ret = dbp->exists(dbp, NULL,  &key, 0);
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
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
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
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

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
/* {{{ rberkeley_db_get_byteswapped */
SEXP rberkeley_db_get_byteswapped (SEXP _dbp)
{
  DB *dbp;
  int ret, isswapped;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->get_byteswapped(dbp, &isswapped);

  if(ret != 0)
    return ScalarInteger(ret);

  return ScalarInteger(isswapped); 
}
/* }}} */
/* {{{ rberkeley_db_get_dbname */
SEXP rberkeley_db_get_dbname (SEXP _dbp)
{
  DB *dbp;
  const char *filenamep, *dbnamep;
  int ret;
  SEXP names, getnames;
  PROTECT(getnames = allocVector(VECSXP, 2));

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  ret = dbp->get_dbname(dbp, &filenamep, &dbnamep);

  if(ret==0) {
    if(filenamep) {
      SET_VECTOR_ELT(getnames, 0, mkString(filenamep));
    } else {
      SET_VECTOR_ELT(getnames, 0, R_NilValue);
    }
    if(dbnamep) {
      SET_VECTOR_ELT(getnames, 1, mkString(dbnamep));
    } else {
      SET_VECTOR_ELT(getnames, 1, R_NilValue);
    }
    PROTECT(names = allocVector(STRSXP, 2));
    SET_STRING_ELT(names, 0, mkChar("filename")); 
    SET_STRING_ELT(names, 1, mkChar("dbname")); 
    setAttrib(getnames, R_NamesSymbol, names);
  } else {
    return R_NilValue;
  }
  UNPROTECT(2);
  return getnames;
}
/* }}} */
/* {{{ rberkeley_db_get_multiple */
SEXP rberkeley_db_get_multiple (SEXP _dbp) {
  DB *dbp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->get_multiple(dbp);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_get_open_flags */
SEXP rberkeley_db_get_open_flags (SEXP _dbp)
{
  DB *dbp;
  u_int32_t flagsp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->get_open_flags(dbp, &flagsp);

  return ScalarInteger(flagsp);
}
/* }}} */
/* rberkeley_db_get_transactional */
/* {{{ rberkeley_db_get_type */
SEXP rberkeley_db_get_type (SEXP _dbp)
{
  DB *dbp;
  DBTYPE type;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
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
/* {{{ rberkeley_db_key_range */
SEXP rberkeley_db_key_range (SEXP _dbp, SEXP _txnid, SEXP _key, SEXP _flags)
{
  DB *dbp;
  DB_TXN *txnid;
  DBT key;
  DB_KEY_RANGE key_range;
  u_int32_t flags = (u_int32_t)INTEGER(_flags)[0];
  int ret;

  if(!isNull(_txnid)) {
    txnid = R_ExternalPtrAddr(_txnid);
  } else txnid = NULL;
  
  memset(&key, 0, sizeof(DBT));
  if(isNull(_key))
    error("key must be set");
  key.data = (unsigned char *)RAW(_key);
  key.size = length(_key);

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
 
  ret = dbp->key_range(dbp, txnid, &key, &key_range, flags); 
  if(ret != 0)
    return ScalarInteger(ret);
   
  SEXP KeyRange, names;
  PROTECT(KeyRange = allocVector(VECSXP, 3));
  SET_VECTOR_ELT(KeyRange, 0, ScalarReal(key_range.less));
  SET_VECTOR_ELT(KeyRange, 1, ScalarReal(key_range.equal));
  SET_VECTOR_ELT(KeyRange, 2, ScalarReal(key_range.greater));
  PROTECT(names = allocVector(STRSXP, 3));
  SET_STRING_ELT(names, 0, mkChar("less")); 
  SET_STRING_ELT(names, 1, mkChar("equal")); 
  SET_STRING_ELT(names, 2, mkChar("greater")); 
  
  setAttrib(KeyRange, R_NamesSymbol, names);
  UNPROTECT(2);
  return KeyRange;
}
/* }}} */
/* {{{ rberkeley_db_open */
SEXP rberkeley_db_open (SEXP _dbp, 
                        SEXP _txnid, 
                        SEXP _file,
                        SEXP _database, 
                        /*
                        SEXP _type,
                        */
                        SEXP _flags/*,
                        SEXP _mode*/)
{
  DB *dbp;
  DB_TXN *txnid;
  int ret;
  u_int32_t flags = INTEGER(_flags)[0];
  const char * file, * database;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

    if(!isNull(_txnid)) {
      txnid = R_ExternalPtrAddr(_txnid);
    } else txnid = NULL;

    if(isNull(_file)) {
      file = NULL;
    } else {
      file = CHAR(STRING_ELT(_file,0));
    }

    if(isNull(_database)) {
      database = NULL;
    } else {
      database = CHAR(STRING_ELT(_database,0));
    }

	if ((ret = dbp->open(dbp,
	    txnid, file, database, DB_BTREE, flags, 0664)) != 0) {
		dbp->err(dbp, ret, "%s", file);
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
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

/*
Rprintf("data.size: %i\n", data.size);
Rprintf("key.size: %i\n", key.size);
*/
	/* Store a key/data pair. */
	if ((ret = dbp->put(dbp, NULL, &key, &data, 0)) == 0) {
		/*Rprintf("db: %s: key stored.\n", (char *)key.data);*/
        return ScalarInteger(ret);
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
  const char * database; 
  if(isNull(_database)) {
    database = NULL;
  } else {
    database = (const char *)CHAR(STRING_ELT(_database,0));
  }

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  ret = dbp->remove(dbp,
                    (const char *)CHAR(STRING_ELT(_file,0)),
                    database,
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
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  ret = dbp->rename(dbp, 
                    (const char *)CHAR(STRING_ELT(_file,0)),
                    (const char *)CHAR(STRING_ELT(_database,0)),
                    (const char *)CHAR(STRING_ELT(_newname,0)),
                    flags);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_set_priority */
SEXP rberkeley_db_set_priority (SEXP _dbp, SEXP _priority)
{
  DB *dbp;
  DB_CACHE_PRIORITY priority;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  priority = (DB_CACHE_PRIORITY)INTEGER(_priority)[0];

  //ret = dbp->set_priority(dbp, priority);
  ret = dbp->set_priority(dbp, DB_PRIORITY_LOW);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_get_priority */
SEXP rberkeley_db_get_priority (SEXP _dbp)
{
  error("DB->get_priority is unavailable from R");
  DBC *dbp;
  DB_CACHE_PRIORITY priority;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  /* something is wrong with this call... */
  ret = dbp->get_priority(dbp, &priority);

  if(ret != 0) 
    return ScalarInteger(ret);

  switch(priority) {
    case DB_PRIORITY_VERY_LOW:
      return mkString("DB_PRIORITY_VERY_LOW");
      break;
    case DB_PRIORITY_LOW:
      return mkString("DB_PRIORITY_LOW");
      break;
    case DB_PRIORITY_DEFAULT:
      return mkString("DB_PRIORITY_DEFAULT");
      break;
    case DB_PRIORITY_HIGH:
      return mkString("DB_PRIORITY_HIGH");
      break;
    case DB_PRIORITY_VERY_HIGH:
      return mkString("DB_PRIORITY_VERY_HIGH");
      break;
    default:
      return R_NilValue;
  }
}
/* }}} */
/* rberkeley_db_stat */
/* {{{ rberkeley_db_stat_print */
SEXP rberkeley_db_stat_print (SEXP _dbp, SEXP _flags)
{
  DB *dbp;
  u_int32_t flags;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  flags = (u_int32_t)INTEGER(_flags)[0];

  ret = dbp->stat_print(dbp, flags);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_sync */
SEXP rberkeley_db_sync(SEXP _dbp)
{
  DB *dbp;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
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
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  if(!isNull(_txnid)) {
    txnid = R_ExternalPtrAddr(_txnid);
    ret = dbp->truncate(dbp, txnid, &countp, flags);
  } else {
    ret = dbp->truncate(dbp, NULL, &countp, flags);
  }

  return ScalarInteger(countp); 
}
/* }}} */
/* {{{ rberkeley_db_upgrade */
SEXP  rberkeley_db_upgrade (SEXP _dbp, SEXP _file, SEXP _flags)
{
  DB *dbp;
  u_int32_t flags;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  flags = (u_int32_t)INTEGER(_flags)[0];

  ret = dbp->upgrade(dbp, (const char *)CHAR(STRING_ELT(_file,0)), flags);

  return ScalarInteger(ret);
}
/* }}} */
/* rberkeley_db_verify */

/*** Database Configuration ***/
/* {{{ rberkeley_db_set_alloc   */
/* }}} */
/* {{{ rberkeley_db_set_cachesize */
SEXP rberkeley_db_set_cachesize (SEXP _dbp, SEXP _gbytes,
                                 SEXP _bytes, SEXP _ncache)
{
  DB *dbp;
  int ncache, ret;
  u_int32_t gbytes, bytes;

  gbytes = (u_int32_t)INTEGER(_gbytes)[0];
   bytes = (u_int32_t)INTEGER(_bytes)[0];
  ncache = (int)INTEGER(_ncache)[0];

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  ret = dbp->set_cachesize(dbp, gbytes, bytes, ncache);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_get_cachesize */
SEXP rberkeley_db_get_cachesize (SEXP _dbp)
{
  DB *dbp;
  int ncachep, ret;
  u_int32_t gbytesp, bytesp;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");
  ret = dbp->get_cachesize(dbp, &gbytesp, &bytesp, &ncachep);

  if(ret != 0)
    return ScalarInteger(ret);

  SEXP result;
  PROTECT(result = allocVector(VECSXP, 3));
  SET_VECTOR_ELT(result, 0, ScalarInteger(gbytesp));
  SET_VECTOR_ELT(result, 1, ScalarInteger(bytesp));
  SET_VECTOR_ELT(result, 2, ScalarInteger(ncachep));
  UNPROTECT(1);
  return result;
}
/* }}} */
/* rberkeley_db_set_dup_compare */
/* {{{ rberkeley_db_set_encrypt */
SEXP rberkeley_db_set_encrypt (SEXP _dbp, SEXP _passwd, SEXP _flags)
{
  DB *dbp;
  int ret;
  const char * passwd;
  u_int32_t flags;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  passwd = CHAR(STRING_ELT(_passwd, 0));
  flags  = (u_int32_t)INTEGER(_flags)[0];

  ret = dbp->set_encrypt(dbp, passwd, flags);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_get_encrypt_flags */
SEXP rberkeley_db_get_encrypt_flags (SEXP _dbp)
{
  DB *dbp;
  int ret;
  u_int32_t flagsp;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->get_encrypt_flags(dbp, &flagsp);

  if(ret != 0)
    return ScalarInteger(ret);

  return ScalarInteger((int)flagsp);
}
/* }}} */
/* rberkeley_db_set_errcall */
/* rberkeley_db_set_msgcall  */
/* {{{ rberkeley_db_set_errfile */
SEXP rberkeley_db_set_errfile (SEXP _dbp, SEXP _errfile)
{
  DB *dbp;
  FILE *errfile = NULL;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  if(!isNull(_errfile)) {
    /* highly unlikely to be portable --- FIXME */
    errfile = fopen(CHAR(STRING_ELT(_errfile,0)),"w");
    if(errfile == NULL)
      error("open failed!\n");
  } else errfile = NULL;

  dbp->set_errfile(dbp, errfile);
  if(errfile == NULL) {
    return R_NilValue;
  } else {
  SEXP ptr = R_MakeExternalPtr(errfile, install("errfile"), ScalarLogical(TRUE));
  R_RegisterCFinalizer(ptr, (R_CFinalizer_t) rberkeley_fclose);
  return ptr;
  } 
}
/* }}} */
/* {{{ rberkeley_db_set_msgfile */
SEXP rberkeley_db_set_msgfile (SEXP _dbp, SEXP _msgfile)
{
  DB *dbp;
  FILE *msgfile = NULL;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  if(!isNull(_msgfile)) {
    msgfile = fopen(CHAR(STRING_ELT(_msgfile,0)),"w");
    if(msgfile == NULL)
      error("open failed!\n");
  } else msgfile = NULL;

  dbp->set_msgfile(dbp, msgfile);
  if(msgfile == NULL) {
    return R_NilValue;
  } else {
  SEXP ptr = R_MakeExternalPtr(msgfile, install("msgfile"), ScalarLogical(TRUE));
  R_RegisterCFinalizer(ptr, (R_CFinalizer_t) rberkeley_fclose);
  return ptr;
  } 
}
/* }}} */
/* {{{ rberkeley_db_set_errpfx */
SEXP rberkeley_db_set_errpfx (SEXP _dbp, SEXP _errpfx)
{
  DB *dbp;
  const char *errpfx = CHAR(STRING_ELT(_errpfx,0));

  //memcpy(&errpfx, ep, strlen(CHAR(STRING_ELT(_errpfx,0))));
  
  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  dbp->set_errpfx(dbp, errpfx);
  return R_MakeExternalPtr(&errpfx, install("errpfx"), ScalarLogical(TRUE));
}
/* }}} */
/* {{{ rberkeley_db_get_errpfx */
SEXP rberkeley_db_get_errpfx (SEXP _dbp)
{
  DB *dbp;
  const char *errpfx;
  
  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  dbp->get_errpfx(dbp, &errpfx);
  return R_MakeExternalPtr(&errpfx, install("errpfx"), ScalarLogical(TRUE));
}
/* }}} */
/* rberkeley_db_set_feedback */
/* {{{ rberkeley_db_set_flags */
SEXP rberkeley_db_set_flags (SEXP _dbp, SEXP _flags)
{
  DB *dbp;
  u_int32_t flags;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  flags = (u_int32_t)INTEGER(_flags)[0];

  ret = dbp->set_flags(dbp, flags);

  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_get_flags */
SEXP rberkeley_db_get_flags (SEXP _dbp)
{
  DB *dbp;
  u_int32_t flags;
  int ret;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->get_flags(dbp, &flags);

  if(ret != 0)
    return ScalarInteger(ret);

  return ScalarInteger((int)flags);
}
/* }}} */
/* {{{ rberkeley_db_set_lorder */
SEXP rberkeley_db_set_lorder (SEXP _dbp, SEXP _lorder)
{
  DB *dbp;
  int ret;
  int lorder;
  lorder = INTEGER(_lorder)[0];

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->set_lorder(dbp, lorder);
  return ScalarInteger(ret);
}
/* }}} */
/* {{{ rberkeley_db_get_lorder */
SEXP rberkeley_db_get_lorder (SEXP _dbp)
{
  DB *dbp;
  int ret;
  int lorder;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->get_lorder(dbp, &lorder);
  if(ret != 0) 
    return ScalarInteger(ret);
  return ScalarInteger(lorder);
}
/* }}} */
/* {{{ rberkeley_db_set_pagesize */
SEXP rberkeley_db_set_pagesize (SEXP _dbp, SEXP _pagesize)
{
  DB *dbp;
  int ret;
  u_int32_t pagesize;

  pagesize = INTEGER(_pagesize)[0];

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->set_pagesize(dbp, pagesize);
  
  return ScalarInteger(ret); 
}
/* }}} */
/* {{{ rberkeley_db_get_pagesize */
SEXP rberkeley_db_get_pagesize (SEXP _dbp)
{
  DB *dbp;
  int ret;
  u_int32_t pagesize;

  dbp = R_ExternalPtrAddr(_dbp);
  if(R_ExternalPtrTag(_dbp) != RBerkeley_DB || dbp == NULL)
    error("invalid 'db' handle");

  ret = dbp->get_pagesize(dbp, &pagesize);
  if(ret != 0)
    return ScalarInteger(ret);

  return ScalarInteger(pagesize);
}
/* }}} */

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
