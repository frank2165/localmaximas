.onLoad <- function(lib, pkg) {
    library.dynam('localmaximas', pkg, lib)
}

.onUnload <- function(libpath) {
    library.dynam.unload('localmaximas', libpath)
}


rebuild <- function(path = NULL){
    detach("package:localmaximas")
    unloadNamespace("localmaximas")
    if(is.null(path)){
        path <- devtools::build()
    }
    install.packages(path, repos = NULL, type = "source")
    library(localmaximas)
}