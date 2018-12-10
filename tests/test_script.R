
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
coords <- localmaximas:::localmaximas_get_coordinates(file)


# test_get_heights
heights <- localmaximas:::get_heights(file)


