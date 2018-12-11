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
#' @import future
#' @importFrom stats setNames
local_maxima_search <- function(files, search.radius, chm.res = 0.25){
    
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
    is.laz <- grepl("\\.la(s|z)", files)
    bad.ext <- !(is.tif | is.laz)
    if(any(bad.ext)){
        stop("input must be a vector of files with either a .tif or a .laz extension")
    }
    
    if(any(is.tif) && any(is.laz)){
        stop("inputs must all have the same extension (either .tif or .laz)")
    }
    
    
    if(search.radius < 0){
        stop("search.radius must be non-negative")
    }
    
    
    ## Different function for .tif or .laz files
    maxima.f <- setNames(vector("list", length = length(files)), files)
    use.tif <- all(is.tif)
    if(use.tif){
        for(f in files){
            maxima[[f]] <- future({local_maximas_tif(f, search.radius)}, substitute = TRUE)
        }
    } else {
        for(f in files){
            maxima[[f]] <- future({local_maximas_laz(f, search.radius, chm.res)}, substitute = TRUE)
        }
    }
    
    maxima <- setNames(lapply(maxima.f, value), files)
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
    silent <- check_chm_file(file.handle)
    
    
    ## Get coordinates and heights
    coords  <- get_coordinates(file.handle)
    heights <- get_heights(file.handle)
    
    
    ## Remove points with missing heights
    idx.missing <- which(is.na(heights))
    if(length(idx.missing) > 0){
        coords  <- coords[-idx.missing, ]
        heights <- heights[-idx.missing]
    }
    
    
    ## Find index of local maxima
    idx.maxima <- frNN_search(coords, heights, search.radius)
    maxima <- if(length(idx.maxima) > 0){
        coords  <- as.data.table(coords[idx.maxima, ])
        heights <- heights[idx.maxima]
        
        names(coords) <- c("X", "Y")
        maxima <- data.table(coords, Z = heights)
    } else {
        data.table(X = NA_real_, Y = NA_real_, Z = NA_real_)
    }
    
    return(maxima)
}



#' Local maxima search of CHM generated from a .laz file
#'
#' @param file path to a single .laz file
#' @param search.radius the radius around each CHM point that will be searched.
#' @param chm.res the length of the side of each CHM cell (in metres).
#'
#' @return a \code{\link[data.table]{data.table}} containing the 3D coordinates
#'   of the local maxima in the CHM.
#' @import data.table
#' @import lidR
local_maximas_laz <- function(file, search.radius, chm.res = 0.25){
    
    ## Import pointcloud and create CHM grid.
    candidates <- readLAS(file, select = "xyz", filter = "-keep_classification 5") %>% 
        grid_metrics(list(Z = max(Z)), res = chm.res) %>% 
        as.data.table
    
    
    ## Get coordinates and heights
    points  <- data.matrix(candidates[, c("X", "Y")])
    heights <- candidates$Z
    
    
    ## Find index of local maxima
    idx.maxima <- frNN_search(points, heights, min.radius)
    maxima <- if(length(idx.maxima) > 0){
        candidates[idx.maxima, ]
    } else {
        data.table(X = NA_real_, Y = NA_real_, Z = NA_real_)
    }
    
    return(maxima)
}
