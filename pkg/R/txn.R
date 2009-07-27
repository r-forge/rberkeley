dbenv_txn_begin <- function(dbenv, parent=NULL, flags=0L)
{
  .Call("rberkeley_dbenv_txn_begin", dbenv, parent, as.integer(flags))
}
