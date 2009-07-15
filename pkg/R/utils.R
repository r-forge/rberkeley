check_pointer <- function(dbh) {
  .Call("rberkeley_check_pointer", dbh)
}
