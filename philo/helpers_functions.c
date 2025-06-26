/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:37:22 by macbook           #+#    #+#             */
/*   Updated: 2025/06/26 20:40:01 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long get_time_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
