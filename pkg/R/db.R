db_create <- function() {
  .Call("rberkeley_db_create")
}

DB_open <- function(dbh) {
  .Call("rberkeley_db_open", dbh)
}

DB_put <- function(db, key, data)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)
  if(!is.raw(data))
    data <- serialize(data, NULL)

  .Call("rberkeley_db_put", db, key, data)
}

DB_get <- function(db, key)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)

  .Call("rberkeley_db_get", db, key)
}


