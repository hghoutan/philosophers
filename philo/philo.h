/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hghoutan <hghoutan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:16:50 by macbook           #+#    #+#             */
/*   Updated: 2025/08/25 11:38:09 by hghoutan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

struct					s_philo;
typedef struct s_philo	t_philo;

typedef struct s_philo_conf
{
	unsigned int		philo_count;
	unsigned int		die_time_ms;
	unsigned int		eat_time_ms;
	unsigned int		sleep_time_ms;
	unsigned int		meals_required;
	unsigned int		is_meals_required_set;
	unsigned long		start_time_ms;
	unsigned int		someone_died;
	int					simulation_stop;

	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		death_mutex;
}						t_philo_conf;

typedef struct s_philo
{
	unsigned int		id;
	unsigned int		meals_eaten;
	unsigned long		last_meal_time;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_philo_conf		*conf;
}						t_philo;

unsigned int			ft_atoi(const char *str);
unsigned long			get_time_ms(void);
unsigned long			get_adjusted_time_ms(t_philo_conf *conf);
int						validate_input(char *str);
int						parse_data(t_philo_conf *conf, char **argv, int argc);
int						init_simulation(t_philo_conf *conf, t_philo **philos,
							pthread_t **threads);
void					*philosopher_routine(void *arg);
void					print_status(t_philo *philo, const char *status);
void					*monitor_routine(void *arg);

void					free_all(t_philo *philos, pthread_t *threads,
							pthread_mutex_t *forks);
void					free_all_resources(t_philo_conf *conf,
							pthread_t *threads, t_philo *philos);
int						should_stop_simulation(t_philo_conf *conf);
void					precise_usleep(unsigned long time_ms, t_philo_conf *conf);

#endif