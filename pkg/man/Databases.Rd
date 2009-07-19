\name{BerkeleyDB Database Functions}
\Rdversion{1.1}
\alias{db_create}
\alias{db_open}
\alias{db_close}
\alias{db_truncate}
\alias{db_remove}
\alias{db_rename}
\alias{db_exists}
\alias{db_put}
\alias{db_get}
\alias{db_version}
\title{
BerkeleyDB Database Function
}
\description{
Low-level calls to the BerkeleyDB API. These
functions simply wrap most of the DB functionality
and expose it at the \R level.

Documentation on usage, valid arguments, and
flags from the official Oracle C API should
be all that is required to correctly use this API.
}
\usage{
db_create(dbenv = NULL, flags = 0L)

db_open(dbh, file="access.db", flags=0L)

db_close(dbh)

db_put(dbh, key, data)

db_get(dbh, key)

db_exists(dbh, txnid=NULL, key, flags=0L)

db_truncate(dbh)

db_get_byteswapped(dbh)

db_remove(dbh, file, database)

db_rename(dbh, file, database, newname)

db_version()
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{dbenv}{
a valid \code{dbenv} environment handle
}
  \item{flags}{
a flag set with \code{mkFlags}
}
  \item{dbh}{
a valid \code{dbh} database handle
}
  \item{key}{
a database key. See details.
}
  \item{data}{
a database value. See details.
}
  \item{txnid}{
a valid \code{txnid} transaction handle.
}
  \item{file}{
a valid \code{file} name
}
  \item{database}{
a valid \code{database} name
}
  \item{newname}{
a valid \code{file} name
}
}
\details{
The user should refer to the official API documentation for the
Berkeley DB database available from Oracle.
}
\value{
Varies by function call.
}
\references{
Oracle Berkeley DB \url{http://www.oracle.com/technology/documentation/berkeley-db/db/index.html}
Oracle Berkeley DB C API \url{http://www.oracle.com/technology/documentation/berkeley-db/db/api_c/frame.html}
Oracle Berkeley DB Reference Guide \url{http://www.oracle.com/technology/documentation/berkeley-db/db/ref/toc.html}
}
\author{
Jeffrey A. Ryan for the R API
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ database }
