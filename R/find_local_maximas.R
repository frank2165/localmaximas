#' find_local_maximas wrapper
#'
#' \code{local_maxima_search} is a wrapper around the \code{find_local_maximas}
#' function, which implements the local maxima finding algorithm in C++.
#'
#' @param files a character vector of paths to tif files containing the canopy
#'   height map.
#' @param search.radius the distance around each raster cell that defines the
#'   search neighbourhood.
#' @param num.cores integer defining the number of cores to use.
#'
#' @return A matrix containing the (x,y,z) coordinates of each maximum.
#' @export
#' @importFrom rgdal GDAL.open
local_maxima_search <- function(files, search.radius, num.cores = 4){
    
    ## Input Checks
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
    

    ## Open files
    handles <- lapply(files, GDAL.open, read.only = TRUE, silent = TRUE)
    
    
    ## Find local maxima
    maxima <- FindLocalMaxima(handles, search.radius, num.cores)
    do.call("rbind", maxima)
}
