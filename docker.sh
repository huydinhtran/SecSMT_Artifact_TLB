#!/bin/bash

# sudo docker build -t sec-smt .
sudo docker run -it --mount type=bind,source="$(pwd)",target=/home/dockeruser/workdir secsmt-gem5