# .onLoad <- function(lib, pkg) {
#     library.dynam('localmaximas', pkg, lib)
# }

#' @useDynLib localmaximas
#' @importFrom Rcpp sourceCpp
NULL



.onUnload <- function(libpath) {
    library.dynam.unload('localmaximas', libpath)
}




rebuild <- function(path = NULL){
    if("localmaximas" %in% .packages()){
        detach("package:localmaximas", unload = TRUE)
    }
    if(is.null(path)){
        path <- devtools::build()
    }
    install.packages(path, repos = NULL, type = "source")
    library(localmaximas)
}
