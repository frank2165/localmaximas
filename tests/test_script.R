
wd <- getwd()
tif <- "HQP_2017_C2_479000_7018000_chm_25cm.tif"
if(basename(wd) == "localmaximas"){
    tif <- paste("tests", tif, sep = "/")
}


library(rgdal)
library(magrittr)
library(localmaximas)


file <- GDAL.open(tif)

# test_get_coordinates
coords <- localmaximas:::test_get_coordinates(file)


# test_get_heights
heights <- localmaximas:::test_get_heights(file)


# test_index_missing
idx <- localmaximas:::test_index_missing(file, heights)


