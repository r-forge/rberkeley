library(RBerkeley)

dbenv <- db_env_create(0L)
#dbh <- db_create(dbenv)

ret <- dbenv_set_cachesize(dbenv, 0L, as.integer(5*1024*1024), 0L)
ret <- dbenv_set_data_dir(dbenv, "dbenv.env")
ret

dbenv_get_data_dirs(dbenv)

dbenv_get_cachesize(dbenv)
dbenv_open(dbenv, db_home="dbenv.env", flags=mkFlags(DB_CREATE,DB_INIT_LOG,DB_INIT_LOCK,
                                                  DB_INIT_MPOOL,DB_INIT_TXN), 0L)
dbenv_set_verbose(dbenv, mkFlags(DB_VERB_FILEOPS_ALL))
dbenv
tid <- dbenv_txn_begin(dbenv)
dbenv_txn_stat_print(dbenv)
dbtxn_id(tid)
dbtxn_abort(tid)
dbenv_txn_stat_print(dbenv)
dbenv_get_verbose(dbenv, mkFlags(DB_VERB_FILEOPS_ALL))
#dbenv_get_verbose(dbenv, mkFlags(DB_VERB_FILEOPS))
dbh <- db_create(dbenv, 0L)
db_open(dbh, file=NULL, flags=mkFlags(DB_CREATE))
db_put(dbh, key="KEY1", data="DATA1")
db_get(dbh, key="KEY1")
#dbenv_memp_stat_print(dbenv, mkFlags(DB_STAT_ALL))

mutex1 <- dbenv_mutex_alloc(dbenv, 0L)
mutex1
dbenv_mutex_lock(dbenv, mutex1)
dbenv_mutex_unlock(dbenv, mutex1)
#dbenv_stat_print(dbenv, 0L)
#dbenv_mutex_stat_print(dbenv, 0L)
dbenv_mutex_free(dbenv, mutex1)
dbenv_set_flags(dbenv, mkFlags(DB_AUTO_COMMIT), 1L)
dbenv_get_flags(dbenv)
dbenv
#dbh

dbenv_get_home(dbenv)
dbenv_get_tmp_dir(dbenv)
dbenv_set_tmp_dir(dbenv, "/Users/jryan/Rforge/rberkeley/tmp")
dbenv_get_tmp_dir(dbenv)
dbenv_get_open_flags(dbenv)


#db_get_env(dbh)
db_close(dbh)
dbenv_close(dbenv, 0L)
