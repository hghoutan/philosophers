#!/bin/bash

# Build the Docker image using docker-compose
docker-compose up --build -d

# Hardcoded test values
PHILO_COUNT=5
TIME_TO_DIE=800
TIME_TO_EAT=200
TIME_TO_SLEEP=200
MEALS_REQUIRED=3

# Run the program with Valgrind inside the Docker container
docker-compose run --rm -it philosophers valgrind --leak-check=full --track-origins=yes ./philo \
  $PHILO_COUNT $TIME_TO_DIE $TIME_TO_EAT $TIME_TO_SLEEP $MEALS_REQUIRED

docker-compose run --rm philosophers make fclean

docker-compose down