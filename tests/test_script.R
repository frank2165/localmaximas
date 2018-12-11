
wd <- getwd()
tif <- "HQP_2017_C2_479000_7018000_chm_25cm.tif"
if(basename(wd) == "localmaximas"){
    tif <- paste("tests", tif, sep = "/")
}


library(rgdal)
library(localmaximas)


file <- GDAL.open(tif)

system.time({
    # test_get_coordinates
    coords <- localmaximas:::get_coordinates(file)
    
    
    # test_get_heights
    heights <- localmaximas:::get_heights(file)
    
    
    # Remove points with missing values
    idx.missing <- which(is.na(heights))
    heights <- heights[-idx.missing]
    coords  <- coords[-idx.missing, ]
    
    
    # frNN_search
    nn <- localmaximas:::frNN_search(coords, heights, 1.5)
})

gc()
