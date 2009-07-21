db_cursor <- function(dbh, txnid=NULL, flags=0L)
{
  .Call("rberkeley_db_cursor", dbh, txnid, flags)
}

dbcursor_close <- function(dbc)
{
  .Call("rberkeley_dbcursor_close", dbc)
}

dbcursor_count <- function(dbc, flags=0L) 
{
  .Call("rberkeley_dbcursor_count", dbc, flags)
}

dbcursor_del <- function(dbc, flags=0L) 
{
  .Call("rberkeley_dbcursor_del", dbc, flags)
}

dbcursor_dup <- function(dbc, flags=0L)
{
  .Call("rberkeley_dbcursor_dup", dbc, as.integer(flags))
}

dbcursor_get <- function(dbc, key, data, flags, n)
{
  if(!is.null(key) && !is.raw(key))
    key <- serialize(key, NULL)
  if(!is.null(data) && !is.raw(data))
    data <- serialize(data, NULL)

  .Call("rberkeley_dbcursor_get", dbc, key, data, flags, as.integer(n))
}

dbcursor_put <- function(dbc, key, data, flags)
{
  if(!is.null(key) && !is.raw(key))
    key <- serialize(key, NULL)
  if(!is.null(data) && !is.raw(data))
    data <- serialize(data, NULL)
  
  .Call("rberkeley_dbcursor_put", dbc, key, data, flags)
}

dbcursor_set_priority <- function(dbc, priority)
{
  if(missing(priority))
    priority <- mkFlags("DB_PRIORITY_DEFAULT")

  .Call("rberkeley_dbcursor_set_priority", dbc, priority)
}

dbcursor_get_priority <- function(dbc)
{
  .Call("rberkeley_dbcursor_get_priority", dbc)
}
