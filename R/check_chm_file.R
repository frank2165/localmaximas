#' Check attributes of .tif file
#'
#' @param handle object of class \linkS4class{GDALReadOnlyDataset}, created by
#'   call to \code{\link[rgdal]{GDAL.open}}.
#'
#' @return NULL
#' @import rgdal
check_chm_file <- function(handle){
    info <- GDALinfo(handle, silent = TRUE, returnStats = FALSE)
    
    bands <- info[["bands"]]
    res.x <- info[["res.x"]]
    res.y <- info[["res.y"]]
    oblique.x <- info[["oblique.x"]]
    oblique.y <- info[["oblique.y"]]
    
    if(res.x != res.y){
        msg <- paste0("(", res.x, ", ", res.y, ")")
        stop("raster cells are not square (res.x, res.y) = ", msg)
    }
    
    if((oblique.x > 0) || (oblique.y > 0)){
        stop("Coordinates cannot be oblique")
    }
    
    if(bands > 1){
        stop("single band raster only")
    }
    
    return(NULL)
}
