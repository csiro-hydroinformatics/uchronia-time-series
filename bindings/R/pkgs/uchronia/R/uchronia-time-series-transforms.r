#' Apply a function to each row of a time series
#'
#' Apply a function to each row of a time series
#'
#' @param tSeries a time series of class 'xts'
#' @param func a function to apply to time series values for each time stamp. 
#' @examples
#' \dontrun{
#' n <- 15
#' myrand <- function(i) { i + 0.5*sqrt(i) * rnorm(100)}
#' set.seed(0)
#' d <- laply(seq_len(n), myrand)
#' ensTs <- xts(d, order.by=lubridate::origin + 0:(n-1) * 3600)
#' xtsApply(ensTs)
#' }
#' @import xts
#' @export
xtsApply <- function(tSeries, func=function(x){ quantile(x, probs = c(0.05, 0.5, 0.95)) } )
{
  stopifnot(is.xts(tSeries))
  xts(t(apply(tSeries, func, MARGIN=1)), order.by=zoo::index(tSeries))
}
