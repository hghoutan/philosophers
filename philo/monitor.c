/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:07:56 by macbook           #+#    #+#             */
/*   Updated: 2025/06/30 18:08:31 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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