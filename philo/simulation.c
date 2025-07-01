/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:01:12 by macbook           #+#    #+#             */
/*   Updated: 2025/07/01 16:36:26 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_allocation_error(void)
{
	printf("Error: Failed to allocate memory.\n");
	return (1);
}

int	init_forks(t_philo_conf *conf)
{
	unsigned int	i;

	i = 0;
	while (i < conf->philo_count)
	{
		if (pthread_mutex_init(&conf->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	init_philo(t_philo *philos, t_philo_conf *conf)
{
	unsigned int	i;

	i = 0;
	while (i < conf->philo_count)
	{
		philos[i].id = i + 1;
		philos[i].conf = conf;
		philos[i].left_fork = &conf->forks[i];
		philos[i].right_fork = &conf->forks[(i + 1) % conf->philo_count];
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = conf->start_time_ms;
		i++;
	}
}

int	allocate_resources(t_philo_conf *conf, t_philo **philos,
		pthread_t **threads)
{
	*threads = malloc(conf->philo_count * sizeof(pthread_t));
	if (!*threads)
		return (handle_allocation_error());
	*philos = malloc(conf->philo_count * sizeof(t_philo));
	if (!*philos)
	{
		free(*threads);
		return (handle_allocation_error());
	}
	conf->forks = malloc(conf->philo_count * sizeof(pthread_mutex_t));
	if (!conf->forks)
	{
		free(*threads);
		free(*philos);
		return (handle_allocation_error());
	}
	return (0);
}

int	init_simulation(t_philo_conf *conf, t_philo **philos, pthread_t **threads)
{
	conf->start_time_ms = get_time_ms();
	conf->simulation_stop = 0;
	pthread_mutex_init(&conf->print_mutex, NULL);
	pthread_mutex_init(&conf->meal_mutex, NULL);
	pthread_mutex_init(&conf->death_mutex, NULL);
	if (allocate_resources(conf, philos, threads))
		return (1);
	if (init_forks(conf))
	{
		free_all(*philos, *threads, conf->forks);
		return (handle_allocation_error());
	}
	init_philo(*philos, conf);
	return (0);
}
