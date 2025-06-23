#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philosophers {
    int nphilos;
    int nforks;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int ntimes_to_eat;
} t_philosophers;


int	ft_atoi(const char *str);


#endif