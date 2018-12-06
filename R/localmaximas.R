

#
local_maximas <- function(file, min.radius = 1, max.radius = 2, chm.res = 0.25){
    
    candidates <- lidR::readLAS(file, select = "xyz", filter = "-keep_classification 5") %>% 
        lidR::grid_metrics(list(Z = max(Z)), res = chm.res) %>% 
        as.data.table
    
    points  <- candidates[, c("X", "Y")]
    points  <- data.matrix(points)
    heights <- candidates$Z
    
    idx.maxima <- maxima_search(points, heights, min.radius)
    candidates[idx.maxima, ]
}


#
maxima_search <- function(xy, z, eps){
    
    ## Check input data types and dimensions
    if (is.data.frame(xy)) {
        xy <- data.matrix(xy)
    } else if (!is.matrix(xy)){
        stop("`xy` must be either a 'data.frame' or a 'matrix'!")
    }
    
    if (!is.numeric(z)) {
        z <- as.numeric(z)
    }
    
    if (!is.numeric(eps) || (eps < 0) || (length(eps) > 1)) {
        stop("`eps` is a distance and must be greater than zero!")
    }
    
    if (length(z) != nrow(xy)) {
        stop("length(z) != nrow(xy)")
    }
    
    if(ncol(xy) != 2){
        stop("`xy`` must be a matrix with 2 columns!")
    }
    
    
    ## Call Cpp export function
    idx <- R_maxima_search(as.matrix(xy), as.double(z), as.double(eps[1]))
    
    return(idx)
}

