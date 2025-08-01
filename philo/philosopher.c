/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:04:29 by macbook           #+#    #+#             */
/*   Updated: 2025/07/01 17:46:16 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
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
  unsigned long	think_time;

	if (should_stop_simulation(philo->conf))
		return ;
	print_status(philo, "is sleeping");
	precise_usleep(philo->conf->sleep_time_ms);
	if (should_stop_simulation(philo->conf))
		return ;

  print_status(philo, "is thinking");
  think_time = (philo->conf->die_time_ms - philo->conf->eat_time_ms - philo->conf->sleep_time_ms);
  if (think_time > 0 && think_time < philo->conf->die_time_ms / 2)
		precise_usleep(think_time / 2);
  else if (philo->conf->philo_count % 2 == 1)
		precise_usleep(1);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
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
