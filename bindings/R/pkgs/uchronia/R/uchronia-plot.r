
xtsQuantiles <- function(tSeries, probs) {
  uchronia::xtsApply(tSeries, function(x){quantile(x, probs = probs)})
}

#' Create a ggplot object of quantiles of a multivariate xts time series
#'
#' Create a ggplot object of quantiles of a multivariate xts time series
#'
#' @param tSeries a time series of class 'xts'
#' @param probs data frame with colnames 'low', 'high'. You should order from wider to narrower confidence intervals, e.g. data.frame(low=c(.05, .25), high=c(0.95, 0.75))
#' @param title optional, title for the graph
#' @param xlabel x-label
#' @param ylabel optional, label for y axis 
#' @examples
#' \dontrun{
#' n <- 15
#' myrand <- function(i) { i + 0.5*sqrt(i) * rnorm(100)}
#' set.seed(0)
#' d <- t(sapply(seq_len(n), myrand))
#' ensTs <- xts::xts(d, order.by=lubridate::origin + 0:(n-1) * 3600)
#' g <- plotXtsQuantiles(ensTs, probs = data.frame(low=c(.05, .1, .25), high=c(0.95, .9, 0.75)))
#' g
#' # To customize your color scheme you can use scale_fill_brewer
#' g + ggplot2::scale_fill_brewer(palette="Reds")
#' }
#' @importFrom ggplot2 ggplot geom_ribbon aes geom_line xlab ylab
#' @importFrom dplyr bind_rows
#' @export
plotXtsQuantiles <- function(tSeries, probs, title, xlabel='Time', ylabel) {
  if(missing(probs)) {
    probs <- data.frame(low=c(.05, .25), high=c(0.95, 0.75), colour=c('red','blue'))
  } else {
    stopifnot(is.data.frame(probs))
    stopifnot(all(c("high","low") %in% names(probs)))
  }
  nPercentiles = nrow(probs)
  probsVec <- as.vector(as.matrix(probs[,c('low','high')]))
  # [1] 0.05 0.10 0.95 0.90

  probsVec <- c(probsVec, .5)
  d <- xtsQuantiles(tSeries, probs = probsVec)

  medianSeries <- as.data.frame(d[,length(probsVec)])
  names(medianSeries) <- 'median'
  medianSeries$datetime = zoo::index(tSeries)

  # http://r-statistics.co/Top50-Ggplot2-Visualizations-MasterList-R-Code.html#Density%20Plot
  # https://plot.ly/ggplot2/geom_ribbon/

  columnsSlice <- function(d, colInd, colNames, percentileTag) {
    x <- as.data.frame(d[,colInd]) 
    names(x) <- colNames
    x$percentile = percentileTag
    return(x)
  }

  mkLongDf <- function(i) {
      tag <- paste0( probs[i,'low'], "-", probs[i,'high'])
      columnsSlice(d, colInd=c(i,i+nPercentiles), colNames=c('low','high'), percentileTag=tag)
  }

  long_df <- lapply(1:nPercentiles, FUN=mkLongDf)

  names(long_df) <- as.character(1:nPercentiles)
  d <- dplyr::bind_rows(long_df)
  d$percentile <- factor(d$percentile)

  # mkName <- function(i, prefix) {paste0(prefix, i)}
  # mins <- sapply(1:nPercentiles, FUN=mkName, 'min')
  # maxs <- sapply(1:nPercentiles, FUN=mkName, 'max')
  # names(d) <- c(mins, maxs, 'y')

  d$datetime = medianSeries$datetime

  g <- ggplot2::ggplot(d, ggplot2::aes(x=datetime))
  g <- g + 
    ggplot2::geom_ribbon(ggplot2::aes(ymin=low, ymax=high, fill=percentile)) +
    ggplot2::geom_line(data=medianSeries,aes(x=datetime,y=median))

  g <- g + ggplot2::xlab(xlabel)
  if(missing(ylabel)) {ylabel <- "value" } 
  g <- g + ggplot2::ylab(ylabel)
  if(!missing(title)) { g <- g + ggplot2::ggtitle(title) }
  return(g)
}


