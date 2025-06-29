/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:02 by hghoutan          #+#    #+#             */
/*   Updated: 2025/06/29 19:51:04 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int validate_input(char *str) {
    int i;
    
    if (!str || str[0] == '\0')
        return 0;
    i = 0;
    while (str[i]) {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}
int parse_data(t_philo_conf *conf, char **argv, int argc) {
    if (!(validate_input(argv[1]) && validate_input(argv[2]) 
        && validate_input(argv[3]) && validate_input(argv[4])))
        return 1;
    
    if (argc == 6 && !validate_input(argv[5]))
        return 1;
    
    conf->philo_count = ft_atoi(argv[1]);
    conf->die_time_ms = ft_atoi(argv[2]);
    conf->eat_time_ms = ft_atoi(argv[3]);
    conf->sleep_time_ms = ft_atoi(argv[4]);
    conf->someone_died = 0;
    if (argv[5]){
       conf->meals_required = ft_atoi(argv[5]);
       conf->is_meals_required_set = 1;
    }
    else
        conf->is_meals_required_set = 0;
    if (conf->philo_count <= 0 || conf->die_time_ms <= 0 ||
      conf->eat_time_ms <= 0 || conf->sleep_time_ms <= 0 || (argv[5] && conf->meals_required <= 0))
        return 1;
    return 0;
  }

void print_status(t_philo *philo, const char *status) {
    pthread_mutex_lock(&philo->conf->print_mutex);
    printf("%09lu %d %s\n", get_adjusted_time_ms(philo->conf), philo->id, status);
    pthread_mutex_unlock(&philo->conf->print_mutex);
}

void take_forks(t_philo *philo) {
    if (philo->id % 2 == 0) {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
    } else {
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
    }
}

void eat(t_philo *philo) {
    print_status(philo, "is eating");
    usleep(philo->conf->eat_time_ms * 1000);    
    pthread_mutex_lock(&philo->conf->meal_mutex);
    philo->meals_eaten++;
    philo->last_meal_time = get_time_ms();
    pthread_mutex_unlock(&philo->conf->meal_mutex);
}

void sleep_and_think(t_philo *philo) {
    print_status(philo, "is sleeping");
    usleep(philo->conf->sleep_time_ms * 1000);
    print_status(philo, "is thinking");
}

void release_forks(t_philo *philo) {
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void *monitor_routine(void *arg) {
    t_philo_conf *conf;
    unsigned int i;
    unsigned long now;

    conf = (t_philo_conf *)arg;
    while (1) {
        i = 0;
        while (i < conf->philo_count) {
            pthread_mutex_lock(&conf->meal_mutex);
            now = get_time_ms();
            if (now - conf->philos[i].last_meal_time > conf->die_time_ms) {
                pthread_mutex_unlock(&conf->meal_mutex);
                pthread_mutex_lock(&conf->print_mutex);
                printf("%09lu %d died\n", get_adjusted_time_ms(conf), conf->philos[i].id);
                conf->someone_died = 1;
                pthread_mutex_unlock(&conf->print_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&conf->meal_mutex);
            i++;
        }
        usleep(1000);
    }
    return NULL;
}


void *philosopher_routine(void *arg) {
    t_philo *philo = (t_philo *)arg;
    unsigned int i = 0;

    while (!philo->conf->someone_died && 
      (!philo->conf->is_meals_required_set || i < philo->conf->meals_required)) {
        take_forks(philo);
        eat(philo);
        release_forks(philo);
        sleep_and_think(philo);
        i++;
    }
    return NULL;
}


void init_philo(t_philo *philos, t_philo_conf *conf) {
  unsigned int i;

  i = 0;
  while (i < conf->philo_count) {
    philos[i].id = i + 1;;
    philos[i].conf = conf;
    philos[i].left_fork = &conf->forks[i];
    philos[i].right_fork = &conf->forks[(i + 1) % conf->philo_count];
    philos[i].meals_eaten = 0;
    philos[i].last_meal_time = conf->start_time_ms;
    
    i++;
  }
}


int handle_allocation_error() {
  printf("Error: Failed to allocate memory.\n");
  return 1;
}

int init_forks(t_philo_conf *conf) {
  unsigned int i;

  i = 0;
  while (i < conf->philo_count) {
    if (pthread_mutex_init(&conf->forks[i], NULL) != 0)
      return 1;
    i++;
  }
  return 0;
}

void clean_mutexes(t_philo_conf conf) {
  unsigned int i;


  pthread_mutex_destroy(&conf.print_mutex);
  pthread_mutex_destroy(&conf.meal_mutex);
  i = 0;
  while(i < conf.philo_count) {
    pthread_mutex_destroy(&conf.forks[i]);
    i++;
  }
}

void free_all(t_philo *philos, pthread_t *threads, pthread_mutex_t *forks) {
    free(philos);
    free(threads);
    free(forks);
}

void free_all_resources(t_philo_conf *conf, pthread_t *threads, t_philo *philos) {
    if (conf != NULL) {
        clean_mutexes(*conf);  
        free(conf->forks);
    }
    free(threads);
    free(philos);
}



int init_simulation(t_philo_conf *conf, t_philo **philos, pthread_t **threads) {
    conf->start_time_ms = get_time_ms();
    pthread_mutex_init(&conf->print_mutex, NULL);
    pthread_mutex_init(&conf->meal_mutex, NULL);
    *threads = malloc(conf->philo_count * sizeof(pthread_t));
    if (!*threads)
        return handle_allocation_error();

    *philos = malloc(conf->philo_count * sizeof(t_philo));
    if (!*philos) {
        free(*threads);
        return handle_allocation_error();
    }

    conf->forks = malloc(conf->philo_count * sizeof(pthread_mutex_t));
    if (!conf->forks) {
        free(*threads);
        free(*philos);
        return handle_allocation_error();
    }

    if (init_forks(conf)) {
        free_all(*philos, *threads, conf->forks);
        return handle_allocation_error();
    }

    init_philo(*philos, conf);
    return 0;
}

int error_exit(const char *msg, t_philo_conf *conf, pthread_t *threads, t_philo *philos) {

  printf("Error: %s\n", msg);
  free_all_resources(conf, threads, philos);
  return 1;
}

int main(int argc, char **argv) {
    t_philo_conf conf; 
    t_philo *philos;
    pthread_t *threads;
    pthread_t monitor_thread;
    unsigned int i;
    
    if (argc < 5 || argc > 6) {
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return 1;
    }
    if (parse_data(&conf, argv, argc))
        return 1;
    if (init_simulation(&conf, &philos, &threads))
      return 1;

    conf.philos = philos;
    i = 0;
    while (i < conf.philo_count) {
      if (pthread_create(&threads[i], NULL, &philosopher_routine, &philos[i]) != 0) 
        return error_exit("Cannot create thread", &conf, threads, philos);
      i++;
    }
    if (pthread_create(&monitor_thread, NULL, &monitor_routine, &conf) != 0)
      return error_exit("Cannot create monitor thread ", &conf, threads, philos);
    i = 0;
    while (i < conf.philo_count) {
      if (pthread_join(threads[i], NULL) != 0)
        return error_exit("Cannot join thread", &conf, threads, philos);
      i++;
    }
    pthread_join(monitor_thread, NULL);
    free_all_resources(&conf, threads, philos);
    return 0;
}