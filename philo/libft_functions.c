/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:20:23 by hghoutan          #+#    #+#             */
/*   Updated: 2025/06/30 19:04:29 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	ft_atoi(const char *str)
{
	unsigned int	num;
	int				i;

	i = 0;
	num = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i] - 48);
		if (num > UINT_MAX)
			return (UINT_MAX);
		i++;
	}
	return (num);
}
