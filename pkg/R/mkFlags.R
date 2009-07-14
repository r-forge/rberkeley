# mkFlags.R

mkFlags <- function(...) {
  flags <- as.character(match.call(call=sys.call(sys.parent()))[-1])
  .Call("mkFlags", flags)
}

#mkFlags(DB_CREATE,DB_EXCL)
#mkFlags(DB_CREATE,DB_EXCL,DB_EXTENT)
##.Call("mkFlags", c("DB_CREATE","DB_EXLC"))
#flags <- mkFlags(DB_CREATE,DB_EXCL)
#flags
#
#.Call("mkFlags", c("DB_CREATE","DB_EXCL"))
#.Call("mkFlags", mkFlags(DB_CREATE,DB_EXLC))
#.Call("mkFlags", flags)
#.Call("mkFlags", c("DB_CREATE","DB_EXTENT"))

