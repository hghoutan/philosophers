/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:37:22 by macbook           #+#    #+#             */
/*   Updated: 2025/06/30 18:05:59 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void print_status(t_philo *philo, const char *status) {
    pthread_mutex_lock(&philo->conf->print_mutex);
    printf("%09lu %d %s\n", get_adjusted_time_ms(philo->conf), philo->id, status);
    pthread_mutex_unlock(&philo->conf->print_mutex);
}

unsigned long get_time_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

unsigned long get_adjusted_time_ms(t_philo_conf *conf) {
  return  get_time_ms() - conf->start_time_ms;
}

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