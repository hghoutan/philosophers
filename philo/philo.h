#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <limits.h>

typedef struct s_philo_conf {
    unsigned int philo_count;
    unsigned int die_time_ms;
    unsigned int eat_time_ms;
    unsigned int sleep_time_ms;
    unsigned int meals_required;
    unsigned int is_meals_required_set;
    pthread_mutex_t *forks;
} t_philo_conf;


typedef struct s_philo {
    unsigned int id;
    unsigned int meals_eaten;
    unsigned long last_meal_time;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_philo_conf *conf;
} t_philo;

unsigned int	ft_atoi(const char *str);
unsigned long get_time_ms();


#endif