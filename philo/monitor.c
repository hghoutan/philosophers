/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:07:56 by macbook           #+#    #+#             */
/*   Updated: 2025/07/01 15:55:46 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_death(t_philo_conf *conf, unsigned int i)
{
	unsigned long	now;

	pthread_mutex_lock(&conf->meal_mutex);
	now = get_time_ms();
	if (now - conf->philos[i].last_meal_time > conf->die_time_ms)
	{
		pthread_mutex_unlock(&conf->meal_mutex);
		pthread_mutex_lock(&conf->print_mutex);
		printf("%09lu %d died\n", get_adjusted_time_ms(conf),
			conf->philos[i].id);
		conf->someone_died = 1;
		pthread_mutex_unlock(&conf->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&conf->meal_mutex);
	return (0);
}

int	all_philos_finished(t_philo_conf *conf)
{
	unsigned int	i;
	int				done;

	done = 1;
	i = 0;
	if (!conf->is_meals_required_set)
		return (0);
	pthread_mutex_lock(&conf->meal_mutex);
	while (i < conf->philo_count)
	{
		if (conf->philos[i].meals_eaten < conf->meals_required)
		{
			done = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&conf->meal_mutex);
	return (done);
}

void	*monitor_routine(void *arg)
{
	t_philo_conf	*conf;
	unsigned int	i;

	conf = (t_philo_conf *)arg;
	while (1)
	{
		i = 0;
		while (i < conf->philo_count)
		{
			if (check_philo_death(conf, i))
				return (NULL);
			i++;
		}
		if (all_philos_finished(conf))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
