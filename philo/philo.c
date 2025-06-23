/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hghoutan <hghoutan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:02 by hghoutan          #+#    #+#             */
/*   Updated: 2025/06/21 19:52:34 by hghoutan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int validate_input(char *str) {
    int i;
    
    if (!str)
        return 1;
    i = 0;
    while (str[i]) {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}
int parse_data(t_philosophers *simulation, char **argv) {
    if (!(validate_input(argv[1]) && validate_input(argv[2]) 
        && validate_input(argv[3]) && validate_input(argv[4]) && validate_input(argv[5])))
        return -1;
    
    simulation->nphilos = atoi(argv[1]);
    simulation->nforks = atoi(argv[1]);
    simulation->time_to_die = atoi(argv[2]);
    simulation->time_to_eat = atoi(argv[3]);
    simulation->time_to_sleep = atoi(argv[4]);
    if (argv[5])
        simulation->ntimes_to_eat = atoi(argv[5]);
    return 0;
}

int main(int argc, char **argv) {
    t_philosophers simulation;
    if (argc < 5 || argc > 6) {
        printf("err");
        return 1;
    }
    if (parse_data(&simulation, argv) == -1) {
        return 1;
    }
    
    printf("Number of phios : %d\n", simulation.nphilos);
    printf("Number of forks : %d\n", simulation.nforks);
    printf("time to die : %d\n", simulation.time_to_die);
    printf("time to eat : %d\n", simulation.time_to_eat);
    printf("time to sleep : %d\n", simulation.time_to_sleep);
    if (simulation.ntimes_to_eat)
        printf("n times to eat: %d\n", simulation.ntimes_to_eat);
    return 0;
}