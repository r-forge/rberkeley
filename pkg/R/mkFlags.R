mkFlags <- function(...) {
  flags <- as.character(match.call(call=sys.call())[-1])
  .Call("mkFlags", flags)
}
