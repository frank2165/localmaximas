

# User defined variables
radius <- 1.5
tifs <- dir("tests", pattern = "\\.tif$", full.names = TRUE, recursive = TRUE)

# Attach Packages
library(localmaximas)

system.time(local_maxima_search(tifs, radius, 4))
system.time(local_maxima_search(tifs, radius, 1))

gc()
