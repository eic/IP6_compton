library(tidyverse)
library(stringr)

data <- read.csv("magnet_KEKB.csv")

# renaming and adding columns 
colnames(data)[1] <- "name"
colnames(data)[2] <- "center_z"
colnames(data)[3] <- "center_x"
colnames(data)[4] <- "center_y"
colnames(data)[6] <- "length"
colnames(data)[8] <- "angle"
data$"rin(z-in)" <- 0.1
data$"rin(z-out)" <- 0.1
data$"dout" <- 0.6
data$"B" <- 0
data$"gradient" <- 12

# rearranging columns
data <- data[c("name", "center_x", "center_y", "center_z", "rin(z-in)", 
               "rin(z-out)", "dout", "length", "angle", "B", "gradient")]

# filtering unneeded components, mutating some columns 
data <- data %>% 
  filter(!is.na(center_x), !str_detect(name, "M\\w+"), 
         !str_detect(name, "Z\\w+"), !str_detect(name, "S\\w+")) %>%
  mutate(angle = angle*pi/180)
  