

local_maximas <- function(files, search.radius){
    
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
    
    not.tif <- !grepl("\\.tif$", files)
    if(any(not.tif)){
        stop("input must be a vector of files with a .tif extension")
    }
    
    if(search.radius < 0){
        stop("search.radius must be non-negative")
    }
    
    
    ## Get file handles
    file.handles <- setNames(vector("list", length(files)), files)
    for(f in files){
        file.handles[[f]] <- GDAL.open(f)
    }
    
    
    ## Check that all files are a valid (band == 1, square cells and no obliqueness)
    for(handle in file.handles){
        silent <- check_chm_file(handle)
    }
    
    
    ## Pass file handles to C++ layer (what's returned?)
    Rcpp_local_maximas(file.handles, search.radius)
}