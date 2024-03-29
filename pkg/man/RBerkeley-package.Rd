\name{RBerkeley-package}
\alias{RBerkeley}
\alias{rberkeley}
\alias{db_stat}
\alias{dbenv_txn_begin}
\alias{dbenv_txn_stat_print}
\alias{dbtxn_abort}
\alias{dbtxn_id}
\alias{dbtxn_commit}
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

A large part of the API had been coded, though
a much smaller subset is currently documented
with respect to differences of design or necessity
between \pkg{RBerkleley} and the C-API.

Primary difference with respect to design include the
automatic serialization of \R objects via \code{serialize}.
This can be bypassed by providing a RAW type vector to the \code{get}
and \code{put} operations.

From a API perspective, some functions related to threads are
not yet implemented, as they are not an absolute priority.  Additionally
functions that require C level callback function pointers have also
been intentionally skipped until a later time.

All functions that appear in the \pkg{RBerkeley} documentation
with an asterisk next to them are currently not available in the \R API.
}
\author{
Jeffrey A. Ryan
}
\references{
Oracle Berkey DB 11gR2\cr
\url{http://www.oracle.com/technology/documentation/berkeley-db/db/index.html}
}
\keyword{ package }
\seealso{
\code{\link{Databases}},
\code{\link{Environments}},
\code{\link{Cursors}},
\code{\link{Mutexes}}
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
db_get(dbh, key="mykey")

# add a key=>value
db_put(dbh, key=charToRaw("myKey"), data="myValue")
x <- matrix(1:10)
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

# get value by key
db_get(dbh, key="myKey2")
db_set_priority(dbh, priority=mkFlags(DB_PRIORITY_VERY_LOW))

xr <- db_get(dbh, key="myKey2")

xr
unserialize(xr)

# cursors
dbc <- db_cursor(dbh)
dbc2 <- dbcursor_dup(dbc)
dbc2

dbcursor_get_priority(dbc)
dbcursor_set_priority(dbc, flags=mkFlags(DB_PRIORITY_LOW))
dbcursor_get_priority(dbc)
dbcursor_get(dbc, key=NULL, data=NULL, flags=0L, n=1L)

# using mkFlags for bitwise OR operations on DB constants
mkFlags(DB_SET);

dbcursor_get(dbc2, key="myKey2", data=NULL, flags=mkFlags(DB_SET), n=1L)
dbcursor_put(dbc2, key="mynewKey", data="MyNewValue", flags=mkFlags(DB_KEYFIRST))
dbcursor_get(dbc2, key="myKey2", data=NULL, flags=mkFlags(DB_PREV), n=1L)
dbcursor_get(dbc2, key="myKey2", data=NULL, flags=mkFlags(DB_FIRST), n=1L)
dbcursor_get(dbc, key=NULL, data=NULL, flags=0L, n=2L) # get 2 records
dbcursor_close(dbc)
dbcursor_close(dbc2)

dbc <- db_cursor(dbh)
dbcursor_get(dbc, key=NULL, data=NULL, flags=0L, n=-1L)
db_stat_print(dbh)
db_stat_print(dbh, flags=mkFlags(DB_FAST_STAT))
dbcursor_close(dbc)

db_exists(dbh, NULL, "myKey2", 0L)
db_get(dbh,key="myKey")

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
