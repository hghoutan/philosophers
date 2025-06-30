/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:04:29 by macbook           #+#    #+#             */
/*   Updated: 2025/06/30 19:15:17 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	usleep(philo->conf->eat_time_ms * 1000);
	pthread_mutex_lock(&philo->conf->meal_mutex);
	philo->meals_eaten++;
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->conf->meal_mutex);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->conf->sleep_time_ms * 1000);
	print_status(philo, "is thinking");
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philosopher_routine(void *arg)
{
	t_philo			*philo;
	unsigned int	i;

	philo = (t_philo *)arg;
	i = 0;
	while (!philo->conf->someone_died && (!philo->conf->is_meals_required_set
			|| i < philo->conf->meals_required))
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		sleep_and_think(philo);
		i++;
	}
	return (NULL);
}
