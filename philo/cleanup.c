/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:09:31 by macbook           #+#    #+#             */
/*   Updated: 2025/06/30 18:09:43 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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