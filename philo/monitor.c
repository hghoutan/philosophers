/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:07:56 by macbook           #+#    #+#             */
/*   Updated: 2025/07/01 17:43:08 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_death(t_philo_conf *conf, unsigned int i)
{
	unsigned long	now;
	unsigned long	last_meal;

	pthread_mutex_lock(&conf->meal_mutex);
	now = get_time_ms();
	last_meal = conf->philos[i].last_meal_time;
	pthread_mutex_unlock(&conf->meal_mutex);
	if (now - last_meal > conf->die_time_ms)
	{
		pthread_mutex_lock(&conf->death_mutex);
		if (!conf->someone_died && !conf->simulation_stop)
		{
			conf->someone_died = 1;
			pthread_mutex_lock(&conf->print_mutex);
			printf("%09lu %d died\n", get_adjusted_time_ms(conf),
				conf->philos[i].id);
			pthread_mutex_unlock(&conf->print_mutex);
		}
		pthread_mutex_unlock(&conf->death_mutex);
		return (1);
	}
	return (0);
}

int	all_philos_finished(t_philo_conf *conf)
{
	unsigned int	i;
	int				all_done;

	all_done = 1;
	i = 0;
	if (!conf->is_meals_required_set)
		return (0);
	pthread_mutex_lock(&conf->meal_mutex);
	while (i < conf->philo_count)
	{
		if (conf->philos[i].meals_eaten < conf->meals_required)
		{
			all_done = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&conf->meal_mutex);
	if (all_done)
	{
		pthread_mutex_lock(&conf->death_mutex);
		conf->simulation_stop = 1;
		pthread_mutex_unlock(&conf->death_mutex);
	}
	return (all_done);
}

void	*monitor_routine(void *arg)
{
	t_philo_conf	*conf;
	unsigned int	i;

	conf = (t_philo_conf *)arg;
	usleep(1000);
	while (!should_stop_simulation(conf))
	{
		i = 0;
		while (i < conf->philo_count && !should_stop_simulation(conf))
		{
			if (check_philo_death(conf, i))
				return (NULL);
			i++;
		}
		if (all_philos_finished(conf))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
