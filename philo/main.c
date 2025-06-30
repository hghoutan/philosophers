/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:02 by hghoutan          #+#    #+#             */
/*   Updated: 2025/06/30 18:16:33 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


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