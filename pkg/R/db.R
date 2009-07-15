db_create <- function(dbenv=NULL) {
  .Call("rberkeley_db_create", dbenv)
}

db_open <- function(dbh, file="access.db", flags=0L) {
  if(check_pointer(dbh))
    .Call("rberkeley_db_open", dbh, NULL, file, flags)
}

db_close <- function(dbh) {
  if(check_pointer(dbh))
  .Call("rberkeley_db_close", dbh)
}

db_put <- function(dbh, key, data)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)
  if(!is.raw(data))
    data <- serialize(data, NULL)

  if(check_pointer(dbh))
  .Call("rberkeley_db_put", dbh, key, data)
}

db_get <- function(dbh, key)
{
  if(!is.raw(key))
    key <- serialize(key, NULL)

  if(check_pointer(dbh))
  .Call("rberkeley_db_get", dbh, key)
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
  if(check_pointer(dbh))
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

db_env_create <- function(flags) {
  .Call("rberkeley_db_env_create", flags)
}
