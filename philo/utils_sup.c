/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_sup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:45:49 by macbook           #+#    #+#             */
/*   Updated: 2025/07/01 17:03:02 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_stop_simulation(t_philo_conf *conf)
{
	int	stop;

	pthread_mutex_lock(&conf->death_mutex);
	stop = conf->someone_died || conf->simulation_stop;
	pthread_mutex_unlock(&conf->death_mutex);
	return (stop);
}

void	precise_usleep(unsigned long time_ms)
{
	unsigned long	start;
	unsigned long	current;

	start = get_time_ms();
	while (1)
	{
		current = get_time_ms();
		if (current - start >= time_ms)
			break ;
		usleep(100);
	}
}
