\name{BerkeleyDB Environments}
\Rdversion{1.1}
\alias{Environments}
\alias{db_env_create}
\alias{db_get_env}
\alias{dbenv_close}
\alias{dbenv_open}
\alias{dbenv_get_home}
\alias{dbenv_get_open_flags}
\alias{dbenv_set_cachesize}
\alias{dbenv_get_cachesize}
\alias{dbenv_set_data_dir}
\alias{dbenv_get_data_dirs}
\alias{dbenv_set_tmp_dir}
\alias{dbenv_get_tmp_dir}
\alias{dbenv_set_flags}
\alias{dbenv_set_flags}
\alias{dbenv_set_intermediate_dir_mode}
\alias{dbenv_get_intermediate_dir_mode}
\alias{dbenv_set_shm_key}
\alias{dbenv_get_shm_key}
\title{
BerkeleyDB Database Environment Operations
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
db_env_create(flags)

db_get_env(dbh)

dbenv_set_cachesize(dbenv, gbytes, bytes, ncache)
dbenv_get_cachesize(dbenv, gbytes, bytes, ncache)

dbenv_set_data_dir(dbenv, dir)
dbenv_get_data_dirs(dbenv)

dbenv_set_tmp_dir(dbenv, dir)
dbenv_get_tmp_dir(dbenv)

dbenv_set_flags(dbenv, flags, onoff)
dbenv_get_flags(dbenv)

dbenv_set_intermediate_dir_mode(dbenv, mode)
dbenv_get_intermediate_dir_mode(dbenv)

dbenv_set_shm_key(dbenv, shm_key)
dbenv_get_shm_key(dbenv)

dbenv_open(dbenv, dbhome, flags, mode)

dbenv_get_home(dbenv)

dbenv_get_open_flags(dbenv)

dbenv_close(dbenv, flags)

dbenv_stat_print(dbenv, flags)
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
  \item{gbytes}{
size of cache in gigabytes
}
  \item{bytes}{
size of cache in bytes
}
  \item{ncache}{
number of caches
}
  \item{mode}{
mode to set file permission to
}
  \item{dir}{
a valid directory name
}
  \item{dbhome}{
a valid directory name
}
  \item{shm_key}{
base segment ID
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
