library(RBerkeley)
library(xts)

.Call("rberkeley_db_version")

# remove current db
system("rm access.db")

dbh <- db_create()
unclass(dbh)
# open new db
db_get_flags(dbh)
db_set_flags(dbh,mkFlags(DB_DUP))
db_open(dbh,flags=mkFlags(DB_CREATE))
db_get_flags(dbh)

db_get_dbname(dbh)

#dbenv <- .Call("rberkeley_db_env_create")

# look for non-existant key
db_get(dbh, key="mykey")

# add a key=>value
db_put(dbh, key=charToRaw("myKey"), data="myValue")
x <- .xts(1:10, 1:10)
db_put(dbh, key="myKey2", data=x)
db_put(dbh, key="myKey3", data="newValue")
db_put(dbh, key="myKey4", data="myValue")
db_key_range(dbh, key="myKey2")

# add a few hundred keys/values
system.time(
for(i in 1:1000)
{
  db_put(dbh, key=i, data=x) 
}
)
db_del(dbh, key="myKey4")
db_strerror(.Last.value)

# get value by key
db_get(dbh, key="myKey2")
db_set_priority(dbh, mkFlags(DB_PRIORITY_VERY_LOW))
#db_get_priority(dbh)
xr <- db_get(dbh, key="myKey2")
#db_compact(dbh)
xr
unserialize(xr)

dbc <- db_cursor(dbh)
dbc2 <- dbcursor_dup(dbc)
dbcursor_get(dbc, key="myKey3", data="newValue", flags=mkFlags(DB_SET))
dbcursor_get(dbc2, key="myKey2", data=NULL, flags=mkFlags(DB_SET), n=1L)

dbc2
dbcursor_get_priority(dbc)
dbcursor_set_priority(dbc, mkFlags(DB_PRIORITY_LOW))
dbcursor_get_priority(dbc)
dbcursor_get(dbc, key=NULL, data=NULL, flags=mkFlags("DB_NEXT"), n=1L)
mkFlags(DB_SET);
dbcursor_put(dbc2, key="mynewKey", data="MyNewValue", flags=mkFlags(DB_KEYFIRST))
dbcursor_get(dbc2, key="myKey2", data=NULL, flags=mkFlags(DB_PREV), n=1L)
dbcursor_get(dbc2, key="myKey2", data=NULL, mkFlags(DB_FIRST), 1L)
dbcursor_del(dbc2, 0L)
dbcursor_get(dbc, key=NULL, data=NULL, n=2L)
dbcursor_close(dbc)
dbcursor_close(dbc2)

dbc <- db_cursor(dbh)
g <- dbcursor_get(dbc, n=-1L)
length(g)
g <- dbcursor_get(dbc, n=990L)
length(g)
#sapply(g, function(x) tryCatch(unserialize(x$key),error=function(e) NULL))

db_stat_print(dbh)
unlist(db_stat(dbh))
unlist(db_stat(dbh, flags=0L))
db_stat_print(dbh, mkFlags(DB_FAST_STAT))
dbcursor_close(dbc)

db_exists(dbh, NULL, "myKey2", 0L)
db_get(dbh,key="myKey")
db_strerror(db_get(dbh,key="myKey"))

# flush to disk
db_sync(dbh)

db_get_type(dbh)

# close db
db_close(dbh)

# remove (need new handle)
dbh <- db_create()
db_remove(dbh, "access.db", NULL)
