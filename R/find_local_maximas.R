#' find_local_maximas wrapper
#'
#' \code{local_maxima_search} is a wrapper around the \code{find_local_maximas}
#' function, which implements the local maxima finding algorithm in C++.
#'
#' @param files a character vector of paths to tif files containing the canopy
#'   height map.
#' @param search.radius the distance around each raster cell that defines the
#'   search neighbourhood.
#'
#' @return A matrix containing the (x,y,z) coordinates of each maximum.
#' @export
local_maxima_catalog <- function(files, search.radius){
    
    if(!is.character(files)){
        stop("files must be a character vector of file paths")
    }
    
    file.not.found <- !file.exists(files)
    if(any(file.not.found)){
        msg <- files[which(file.not.found)]
        msg <- paste(files, collapse = "\n")
        stop("The following files could not be found: \n", msg)
    }
    
    is.tif <- grepl("\\.tif$", files)
    bad.ext <- !is.tif
    if(any(bad.ext)){
        stop("input must be a vector of files with a .tif extension")
    }
    
    
    if(search.radius < 0){
        stop("search.radius must be non-negative")
    }
    
    files  <- normalizePath(files, mustWork = FALSE)
    maxima <- find_local_maxima(files, search.radius)
    do.call("rbind", maxima)
}
