\name{BerkeleyDB Database Operations}
\Rdversion{1.1}
\alias{Databases}
\alias{DB}
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
BerkeleyDB Database Operations and Methods
}
\description{
Low-level calls to the BerkeleyDB API. These
functions simply wrap most of the DB functionality
and expose it at the \R level.

Documentation on usage, valid arguments, and
flags from the official Oracle C API should
be all that is required to correctly use this API.

\tabular{ll}{
  \emph{Database Operations} \tab \emph{Description}\cr
  \tab \cr
  db_create     \tab Create a database handle \cr
  db_close      \tab Close a database \cr
  db_compact    \tab Compact a database \cr
  db_cursor     \tab Create a cursor \cr
  db_del        \tab Delete items from a database \cr
  db_exists     \tab Return if an item appears in a database \cr
  db_get        \tab Get items from a database\cr
  db_get_byteswapped         \tab Return if a database is in host order \cr
  db_get_type         \tab Return a database type \cr
  db_key_range         \tab Return estimate of key location \cr
  db_open         \tab Open a database \cr
  db_put         \tab Store items in a database \cr
  db_remove         \tab Remove a database \cr
  db_rename         \tab Rename a database \cr
  db_set_priority         \tab Set cache page priority \cr
  db_get_priority         \tab Get cache page priority \cr
  db_stat_print         \tab Database statistics \cr
  db_sync         \tab Flush a databse to stable storage \cr
  db_truncate         \tab Empty a database \cr
  db_upgrade         \tab Upgrade a database \cr
}

\tabular{ll}{
  \emph{Database Configuration} \tab \emph{Description}\cr
  \tab \cr
  db_set_cachesize     \tab Set the database cache size\cr
  db_get_cachesize     \tab Get the database cache size\cr
  db_set_encrypt      \tab Set the database cryptographic key \cr
  db_get_encrypt_flags      \tab Get the database cryptographic key flags\cr
  db_set_errfile    \tab Set error and informational message FILE \cr
  db_set_msgfile    \tab Set error and informational message FILE \cr
  db_set_errpfx     \tab Set error message prefix \cr
  db_get_errpfx     \tab Get error message prefix \cr
  db_set_flags        \tab General database configuration \cr
  db_get_flags        \tab General database configuration \cr
  db_set_lorder        \tab Set the database byte order \cr
  db_get_lorder        \tab Get the database byte order \cr
  db_set_pagesize        \tab Set the database page size \cr
  db_get_pagesize        \tab Get the database page size \cr
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
Oracle Berkeley DB\cr
\url{http://www.oracle.com/technology/documentation/berkeley-db/db/index.html}
Oracle Berkeley DB C API\cr
\url{http://www.oracle.com/technology/documentation/berkeley-db/db/api_c/frame.html}
Oracle Berkeley DB Reference Guide\cr
\url{http://www.oracle.com/technology/documentation/berkeley-db/db/ref/toc.html}
}
\author{
Jeffrey A. Ryan for the R API
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ database }
