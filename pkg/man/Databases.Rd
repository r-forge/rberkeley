\name{BerkeleyDB Database Operations}
\Rdversion{1.1}
\alias{Databases}
\alias{DB}
\alias{db_associate}
\alias{db_close}
\alias{db_compact}
\alias{db_del}
\alias{db_err}
\alias{db_errx}
\alias{db_exists}
\alias{db_fd}
\alias{db_pget}
\alias{db_get_byteswapped}
\alias{db_get_type}
\alias{db_join}
\alias{db_key_range}
\alias{db_open}
\alias{db_remove}
\alias{db_rename}
\alias{db_set_priority}
\alias{db_stat,}
\alias{db_sync}
\alias{db_truncate}
\alias{db_upgrade}
\alias{db_verify}
\alias{db_set_alloc}
\alias{db_set_cachesize}
\alias{db_set_dup_compare}
\alias{db_set_encrypt}
\alias{db_set_errcall}
\alias{db_set_msgcall}
\alias{db_set_errfile}
\alias{db_set_msgfile}
\alias{db_set_errpfx}
\alias{db_set_feedback}
\alias{db_set_flags}
\alias{db_set_lorder}
\alias{db_set_pagesize}
% these aren't documented yet
\alias{db_set_re_source}
\alias{db_get_cachesize}
\alias{db_get_dbname}
\alias{db_get_encrypt_flags}
\alias{db_get_errpfx}
\alias{db_get_flags}
\alias{db_get_lorder}
\alias{db_get_mpf}
\alias{db_get_pagesize}
\alias{db_get_priority}
\alias{db_stat_print}
\alias{db_strerror}
\alias{db_version}
\alias{db_full_version}

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
  db_create \tab   Create a database handle\cr
  db_associate* \tab   Associate a secondary index\cr
  db_close \tab   Close a database\cr
  db_compact \tab Compact a database\cr
  db_cursor \tab  Create a cursor\cr
  db_del \tab Delete items from a database\cr
  db_err* \tab Error message\cr
  db_errx* \tab  Error message\cr
  db_exists \tab  Return if an item appears in a database\cr
  db_fd \tab  Return a file descriptor from a database\cr
  db_get \tab   Get items from a database\cr
  db_pget* \tab  Get items from a database\cr
  db_get_byteswapped \tab Return if the underlying database is in host order\cr
  db_get_type \tab    Return the database type\cr
  db_join* \tab    Perform a database join on cursors\cr
  db_key_range \tab   Return estimate of key location\cr
  db_open \tab    Open a database\cr
  db_put \tab Store items into a database\cr
  db_remove \tab  Remove a database\cr
  db_rename \tab  Rename a database\cr
  db_set_priority \tab    Set cache page priority\cr
  db_stat, \tab db_stat_print    Database statistics\cr
  db_sync \tab    Flush a database to stable storage\cr
  db_truncate \tab    Empty a database\cr
  db_upgrade \tab Upgrade a database\cr
  db_verify* \tab  Verify/salvage a database\cr
  db_strerror \tab  Error strings\cr
  db_version \tab  Return version information\cr
  \tab \cr
  \emph{Database Configuration} \tab \emph{Description}\cr
  \tab \cr
  db_set_alloc* \tab   Set local space allocation functions\cr
  db_set_cachesize \tab   Set the database cache size\cr
  db_set_dup_compare* \tab Set a duplicate comparison function\cr
  db_set_encrypt \tab Set the database cryptographic key\cr
  db_set_errcall* \tab    Set error and informational message callback\cr
  db_set_msgcall* \tab    Set error and informational message callback\cr
  db_set_errfile \tab    Set error and informational message FILE\cr
  db_set_msgfile \tab    Set error and informational message FILE\cr
  db_set_errpfx \tab  Set error message prefix\cr
  db_set_feedback* \tab    Set feedback callback\cr
  db_set_flags \tab   General database configuration\cr
  db_set_lorder \tab  Set the database byte order\cr
  db_set_pagesize \tab    Set the underlying database page size\cr
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
\url{http://www.oracle.com/technology/documentation/berkeley-db/db/index.html}\cr\cr
Oracle Berkeley DB C API\cr
\url{http://www.oracle.com/technology/documentation/berkeley-db/db/api_c/frame.html}\cr\cr
Oracle Berkeley DB Reference Guide\cr
\url{http://www.oracle.com/technology/documentation/berkeley-db/db/ref/toc.html}
}
\author{
Jeffrey A. Ryan for the R API
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ database }
