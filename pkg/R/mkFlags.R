mkFlags <- function(...) {
  flags <- as.character(match.call(call=sys.call(sys.parent()))[-1])
  .Call("mkFlags", flags)
}
