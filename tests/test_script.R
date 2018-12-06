

tif <- "veg_height.tif"


library(rgdal)
library(magrittr)
library(localmaximas)


file <- GDAL.open(tif)


localmaximas.dlls <- getLoadedDLLs() %>% extract("localmaximas") %>% 
    unlist %>% extract2("localmaximas.filepath")

getDLLRegisteredRoutines(localmaximas.dll)
    


# test_get_coordinates
coords <- .Call("_localmaximas_test_get_coordinates", PACKAGE = "localmaximas", file)


# test_get_heights
heights <- .Call("_localmaximas_test_get_heights", PACKAGE = "localmaximas", file)


# test_index_missing
idx <- .Call("_localmaximas_test_index_missing", PACKAGE = "localmaximas", file, heights)


