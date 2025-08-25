/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hghoutan <hghoutan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:04:29 by macbook           #+#    #+#             */
/*   Updated: 2025/08/25 11:31:26 by hghoutan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->conf->philo_count == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		while (!should_stop_simulation(philo->conf))
			usleep(1000);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		if (!should_stop_simulation(philo->conf))
			print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		if (!should_stop_simulation(philo->conf))
			print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		if (!should_stop_simulation(philo->conf))
			print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		if (!should_stop_simulation(philo->conf))
			print_status(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	if (should_stop_simulation(philo->conf))
		return ;
	print_status(philo, "is eating");
	precise_usleep(philo->conf->eat_time_ms);
	pthread_mutex_lock(&philo->conf->meal_mutex);
	philo->meals_eaten++;
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->conf->meal_mutex);
}

void	sleep_and_think(t_philo *philo)
{
	if (should_stop_simulation(philo->conf))
		return ;
	print_status(philo, "is sleeping");
	precise_usleep(philo->conf->sleep_time_ms);
	if (should_stop_simulation(philo->conf))
		return ;

	print_status(philo, "is thinking");
	if (philo->conf->philo_count % 2 == 0)
		return ;
	if (philo->conf->philo_count && philo->conf->eat_time_ms >= philo->conf->sleep_time_ms)
		precise_usleep(philo->conf->eat_time_ms + 1);
}

void	release_forks(t_philo *philo)
{
	// Only unlock forks for multi-philosopher scenarios
	// Single philosopher case is handled entirely in take_forks()
	if (philo->conf->philo_count > 1)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!should_stop_simulation(philo->conf))
	{
		take_forks(philo);
		if (should_stop_simulation(philo->conf))
		{
			release_forks(philo);
			break ;
		}
		eat(philo);
		release_forks(philo);
		if (should_stop_simulation(philo->conf))
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}