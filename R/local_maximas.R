#' Find locally maximal points in a canopy height map
#'
#' \code{local_maxima_search} performs a fixed-radius nearest neighbour search
#' around each canopy height map (CHM) cell and identifies those cells that are
#' locally maximal with respect to their height above ground.
#'
#' The search can be performed on either \code{.laz} (or \code{.las}) files,
#' containing the LiDAR point-cloud, or on a \code{.tif} file containing the CHM
#' raster. Note that when a point-cloud is provided, the CHM must be generated
#' from the files imported and that this will take additional time.
#'
#' @param files a character vector of file paths.
#' @param search.radius the radius around each CHM point that will be searched.
#' @param chm.res the length of the side of each CHM cell (in metres). Only used
#'   for \code{.laz} files
#'
#' @return a \code{\link[data.table]{data.table}} containing the 3D coordinates
#'   of the local maxima in the CHM.
#' @export
local_maxima_search <- function(files, search.radius){
    
    ## Input checks
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
    
    
    ## Different function for .tif or .laz files
    maxima <- lapply(files, local_maximas_tif, search.radius)
    names(maxima) <- files
    
    return(maxima)
}




#' Local maxima search of CHM .tif file
#'
#' @param file path to a single .tif file
#' @param search.radius the radius around each CHM point that will be searched.
#'
#' @return a \code{\link[data.table]{data.table}} containing the 3D coordinates
#'   of the local maxima in the CHM.
#' @import data.table
#' @import rgdal
local_maximas_tif <- function(file, search.radius){
    
    ## Get file handles
    file.handle <- GDAL.open(file)
    
    
    ## Check that all files are a valid (band == 1, square cells and no obliqueness)
    silent <- check_chm_file(file)
    
    
    ## Find index of local maxima
    FindLocalMaxima(file.handle, search.radius)
}
