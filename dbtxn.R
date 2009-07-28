library(RBerkeley)
system("rm myEnv/*")
dbenv <- db_env_create()
dbenv_open(dbenv, "myEnv", flags=mkFlags(DB_CREATE,
                                         DB_INIT_TXN,
                                         DB_INIT_LOCK,
                                         DB_INIT_LOG,
                                         DB_INIT_MPOOL), 0L)
dbh <- db_create(dbenv)
db_open(dbh,NULL,file="mydb.db",NULL,
        "BTREE",flags=mkFlags(DB_CREATE,DB_AUTO_COMMIT))
tid <- dbenv_txn_begin(dbenv)
ret <- db_put(dbh, tid, "myKey", "myValue", 0L)
db_strerror(ret)

# note that the transaction handle can no longer be used
dbtxn_abort(tid)

tid <- dbenv_txn_begin(dbenv)
ret <- db_put(dbh, tid, "myKey3", "myValue", 0L)
db_strerror(ret)
dbtxn_commit(tid, 0L)

# using the fact that the db_open is using DB_AUTO_COMMIT
ret <- db_put(dbh, NULL, "myKey2", "myValue", 0L)
db_strerror(ret)

dbc <- db_cursor(dbh)
ret <- dbcursor_get(dbc, n=-1)
lapply(ret, function(x) lapply(x, unserialize))
dbcursor_close(dbc)
# close the db
db_close(dbh)
