/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:02 by hghoutan          #+#    #+#             */
/*   Updated: 2025/06/28 18:58:26 by macbook          ###   ########.fr       */
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
        return -1;
    
    if (argc == 6 && !validate_input(argv[5]))
        return -1;

    conf->philo_count = ft_atoi(argv[1]);
    conf->die_time_ms = ft_atoi(argv[2]);
    conf->eat_time_ms = ft_atoi(argv[3]);
    conf->sleep_time_ms = ft_atoi(argv[4]);
    if (argv[5]){
       conf->meals_required = ft_atoi(argv[5]);
       conf->is_meals_required_set = 1;
    }
    else
        conf->is_meals_required_set = 0;
    if (conf->philo_count <= 0 || conf->die_time_ms <= 0 ||
      conf->eat_time_ms <= 0 || conf->sleep_time_ms <= 0 || (argv[5] && conf->meals_required <= 0))
        return -1;
    return 0;
  }

void *philosopher_routine(void *arg) {
  t_philo       *philo;
  unsigned int  i;

  philo = (t_philo *)arg;
  i = 0;

  while (i < philo->conf->meals_required) {
    // Thinking

    pthread_mutex_lock(&philo->conf->print_mutex);
    printf("%09lu %d is thinking\n", get_adjusted_time_ms(philo->conf),philo->id);
    pthread_mutex_unlock(&philo->conf->print_mutex);

    
    if (philo->id % 2 == 0) {
      pthread_mutex_lock(philo->left_fork);
      pthread_mutex_lock(&philo->conf->print_mutex);
      printf("%09lu %d has taken a fork\n", get_adjusted_time_ms(philo->conf), philo->id);
      pthread_mutex_unlock(&philo->conf->print_mutex);

      pthread_mutex_lock(philo->right_fork);
      pthread_mutex_lock(&philo->conf->print_mutex);
      printf("%09lu %d has taken a fork\n", get_adjusted_time_ms(philo->conf), philo->id);
      pthread_mutex_unlock(&philo->conf->print_mutex);
    } else {
      pthread_mutex_lock(philo->right_fork);
      pthread_mutex_lock(&philo->conf->print_mutex);
      printf("%09lu %d has taken a fork\n", get_adjusted_time_ms(philo->conf), philo->id);
      pthread_mutex_unlock(&philo->conf->print_mutex);
      
      pthread_mutex_lock(philo->left_fork);
      pthread_mutex_lock(&philo->conf->print_mutex);
      printf("%09lu %d has taken a fork\n", get_adjusted_time_ms(philo->conf), philo->id);
      pthread_mutex_unlock(&philo->conf->print_mutex);
    }

    // Eat 
    pthread_mutex_lock(&philo->conf->print_mutex);
    printf("%09lu %d is eating\n", get_adjusted_time_ms(philo->conf), philo->id);
    pthread_mutex_unlock(&philo->conf->print_mutex);
    usleep(philo->conf->eat_time_ms * 1000);
    
    pthread_mutex_lock(&philo->conf->meal_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->conf->meal_mutex);
    
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    
    pthread_mutex_lock(&philo->conf->print_mutex);
    printf("%09lu %d is sleeping\n", get_adjusted_time_ms(philo->conf), philo->id);
    pthread_mutex_unlock(&philo->conf->print_mutex);
    usleep(philo->conf->sleep_time_ms * 1000);

    
    i++;
  }
  return NULL;
}

void init_philo(t_philo *philos, t_philo_conf *conf) {
  unsigned int i;

  i = 0;
  while (i < conf->philo_count) {
    philos[i].id = i;
    philos[i].conf = conf;
    philos[i].left_fork = &conf->forks[i];
    philos[i].right_fork = &conf->forks[(i + 1) % conf->philo_count];
    philos[i].meals_eaten = 0;
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



int main(int argc, char **argv) {
    t_philo_conf conf; 
    pthread_t *threads;
    t_philo *philos;
    unsigned int i;
    
    if (argc < 5 || argc > 6) {
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return 1;
    }
    if (parse_data(&conf, argv, argc) == -1)
        return 1;
    if (init_simulation(&conf, &philos, &threads))
      return 1;

    i = 0;
    while (i < conf.philo_count) {
      pthread_create(&threads[i], NULL, &philosopher_routine, &philos[i]);
      i++;
    }

    i = 0;
    while (i < conf.philo_count) {
      pthread_join(threads[i], NULL);
      i++;
    }
    clean_mutexes(conf);
    free(threads);
    free(philos);
    free(conf.forks);
    return 0;
}