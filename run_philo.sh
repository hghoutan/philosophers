#!/bin/bash

docker compose up --build -d

PHILO_COUNT=5
TIME_TO_DIE=1000
TIME_TO_EAT=200
TIME_TO_SLEEP=200
MEALS_REQUIRED=2


# For leaks :
# docker compose run --rm -it philosophers valgrind --leak-check=full --track-origins=yes ./philo \
#   $PHILO_COUNT $TIME_TO_DIE $TIME_TO_EAT $TIME_TO_SLEEP $MEALS_REQUIRED

# For Deadlocks
docker compose run --rm -it philosophers valgrind --tool=helgrind ./philo \
  $PHILO_COUNT $TIME_TO_DIE $TIME_TO_EAT $TIME_TO_SLEEP $MEALS_REQUIRED


docker compose run --rm philosophers make fclean

docker compose down