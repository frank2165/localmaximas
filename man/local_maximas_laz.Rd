% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/local_maximas.R
\name{local_maximas_laz}
\alias{local_maximas_laz}
\title{Local maxima search of CHM generated from a .laz file}
\usage{
local_maximas_laz(file, search.radius, chm.res = 0.25)
}
\arguments{
\item{file}{path to a single .laz file}

\item{search.radius}{the radius around each CHM point that will be searched.}

\item{chm.res}{the length of the side of each CHM cell (in metres).}
}
\value{
a \code{\link[data.table]{data.table}} containing the 3D coordinates
  of the local maxima in the CHM.
}
\description{
Local maxima search of CHM generated from a .laz file
}
