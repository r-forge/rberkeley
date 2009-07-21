\name{RBerkeley-package}
\Rdversion{1.1}
\alias{RBerkeley-package}
\docType{package}
\title{
R Interface to Embedded Oracle Berkeley DB
}
\description{
Package provides a low and high level interface
to the C API of the Oracle Berkeley DB embedded
database product.
}
\details{
Initial implementation will be to provide direct
access to the low-level API calls in their
entirety.

The project will eventually include a higher-level
interface to make using the DB from R easier.
}
\author{
Jeffrey A. Ryan
}
\references{
Oracle Berkey DB 4.7.25 \url{http://www.oracle.com/technology/documentation/berkeley-db/db/index.html}
}
\keyword{ package }
\seealso{
~~ Optional links to other man pages, e.g. ~~
~~ \code{\link[<pkg>:<pkg>-package]{<pkg>}} ~~
}
\examples{
\dontrun{
db_version()

dbh <- db_create()

# set some flags and open new db
db_get_flags(dbh)
db_set_flags(dbh,mkFlags(DB_DUP))
db_open(dbh,flags=mkFlags(DB_CREATE))
db_get_flags(dbh)

db_get_dbname(dbh)

# look for non-existant key
db_get(dbh, "mykey")

# add a key=>value
db_put(dbh, charToRaw("myKey"), "myValue")
x <- matrix(1:10)
db_put(dbh, "myKey2", x)
db_put(dbh, "myKey3", "newValue")
db_put(dbh, "myKey4", "myValue")
db_key_range(dbh, key="myKey2")

# add a few hundred keys/values
system.time(
for(i in 1:1000)
{
  db_put(dbh, i, x) 
}
)

# get value by key
db_get(dbh, "myKey2")
db_set_priority(dbh, mkFlags(DB_PRIORITY_VERY_LOW))

xr <- db_get(dbh, "myKey2")

xr
unserialize(xr)

# cursors
dbc <- db_cursor(attr(dbh,"conn_id"))
dbc2 <- dbcursor_dup(dbc)
dbc2

dbcursor_get_priority(dbc)
dbcursor_set_priority(dbc, mkFlags(DB_PRIORITY_LOW))
dbcursor_get_priority(dbc)
dbcursor_get(dbc, NULL, NULL, 0L, 1L)

# using mkFlags for bitwise OR operations on DB constants
mkFlags(DB_SET);

dbcursor_get(dbc2, "myKey2", NULL, mkFlags(DB_SET), 1L)
dbcursor_put(dbc2, "mynewKey", "MyNewValue", mkFlags(DB_KEYFIRST))
dbcursor_get(dbc2, "myKey2", NULL, mkFlags(DB_PREV), 1L)
dbcursor_get(dbc2, "myKey2", NULL, mkFlags(DB_FIRST), 1L)
dbcursor_get(dbc, NULL, NULL, 0L, 2L)
dbcursor_close(dbc)
dbcursor_close(dbc2)

dbc <- db_cursor(attr(dbh,"conn_id"))
dbcursor_get(dbc, NULL, NULL, 0L, -1L)
db_stat_print(dbh)
db_stat_print(dbh, mkFlags(DB_FAST_STAT))
dbcursor_close(dbc)

db_exists(dbh, NULL, "myKey2", 0L)
db_get(dbh,"myKey")

# flush to disk
db_sync(dbh)

db_get_type(dbh)

# close db
db_close(dbh)

# remove (need new handle)
dbh <- db_create()
db_remove(dbh, "access.db", NULL)
}
}
