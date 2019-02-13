

# User defined variables
radius <- 1.5
tifs <- dir(pattern = "\\.tif$", full.names = TRUE, recursive = TRUE)

# Attach Packages
library(localmaximas)

system.time(local_maxima_search(tifs, radius))

gc()
