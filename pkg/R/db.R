db_create <- function(dbenv=NULL, flags=0L) {
  .Call("rberkeley_db_create", dbenv, flags)
}

db_strerror <- function(error)
{
  .Call("rberkeley_db_strerror", as.integer(error))
} 

db_open <- function(dbh, txnid=NULL, file="access.db",
                    database=NULL, flags=0L) {
  .Call("rberkeley_db_open", dbh, txnid, file, database, flags)
}

db_close <- function(dbh) {
  .Call("rberkeley_db_close", dbh)
}

db_upgrade <- function(dbh, file, flags)
{
  .Call("rberkeley_db_upgrade", dbh, file, as.integer(flags))
}

db_compact <- function(dbh, txnid=NULL, start=NULL, stop=NULL,
                       c_data=NULL, flags=0L)
{
  .Call("rberkeley_db_compact", dbh, txnid, start, stop, c_data, as.integer(flags))
}

db_set_priority <- function(dbh, priority)
{
  if(is.character(priority))
    priority <- mkFlags(priority)
  .Call("rberkeley_db_set_priority", dbh, as.integer(priority))
}

db_get_priority <- function(dbh)
{
  .Call("rberkeley_db_get_priority", dbh)
}

db_set_flags <- function(dbh, flags=0L)
{
  .Call("rberkeley_db_set_flags", dbh, as.integer(flags))
}

db_get_flags <- function(dbh)
{
  .Call("rberkeley_db_get_flags", dbh)
}

db_put <- function(dbh, key, data)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)
  if(!is.raw(data))
    data <- serialize(data, NULL)

  .Call("rberkeley_db_put", dbh, key, data)
}

db_get <- function(dbh, key)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)

  .Call("rberkeley_db_get", dbh, key)
}

db_key_range <- function(dbh, txnid=NULL, key, flags=0L)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)
  .Call("rberkeley_db_key_range", dbh, txnid, key, as.integer(flags))
}

db_exists <- function(dbh, txnid=NULL, key, flags=0L)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)
  .Call("rberkeley_db_exists", dbh, txnid, key, flags)
}

db_truncate <- function(dhh)
{

}
db_get_byteswapped <- function(dbh)
{
  .Call("rberkeley_db_get_byteswapped", dbh)
}

db_set_cachesize <- function(dbh, gbytes, bytes, ncache)
{
  .Call("rberkeley_db_set_cachesize", 
        dbh, 
        as.integer(gbytes),
        as.integer(bytes),
        as.integer(ncache))
}

db_get_cachesize <- function(dbh)
{
  cachesize <- .Call("rberkeley_db_get_cachesize", dbh)
  names(cachesize) <- c("gbytes","bytes","ncache")
  cachesize
}

db_set_pagesize <- function(dbh, pagesize)
{
  .Call("rberkeley_db_set_pagesize", dbh, as.integer(pagesize))
}

db_get_pagesize <- function(dbh)
{
  pagesize <- .Call("rberkeley_db_get_pagesize", dbh)
  pagesize
}

db_set_encrypt <- function(dbh, passwd, flags)
{
  if(missing(flags))
    flags = mkFlags(DB_ENCRYPT_AES)
  
  .Call("rberkeley_db_set_encrypt", dbh, passwd, flags)
}

db_get_encrypt_flags <- function(dbh)
{
  .Call("rberkeley_db_get_encrypt_flags", dbh)
}
 
db_set_lorder <- function(dbh, lorder)
{
  lorder <- as.integer(lorder)
  if(lorder != 1234L || lorder != 4321L)
    stop("incorrect 'lorder' value")

  .Call("rberkeley_db_set_lorder", dbh, lorder)
}

db_get_lorder <- function(dbh)
{
  lorder <- .Call("rberkeley_db_get_lorder", dbh)
  if(lorder == 1234L)
   return("little.endian")
  if(lorder == 4321L)
   return("big.endian")
}

db_stat_print <- function(dbh, flags=0L)
{
  .Call("rberkeley_db_stat_print", dbh, as.integer(flags)) 
}

db_remove <- function(dbh, file, database)
{
  .Call("rberkeley_db_remove", dbh, file, database)
}

db_rename <- function(dbh, file, database, newname)
{
  .Call("rberkeley_db_rename", dbh, file, database, newname)
}

db_version <- function() 
{
  .Call("rberkeley_db_version")
}
