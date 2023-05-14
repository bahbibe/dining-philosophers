/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 03:50:27 by bahbibe           #+#    #+#             */
/*   Updated: 2023/05/12 08:24:19 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_arg(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
		if (!ft_isdigit(arg[i]))
			return (-1);
	return (ft_atoi(arg));
}

int	check_args(char **av)
{
	int	i;

	i = 0;
	while (*(++av))
		if (get_arg(*av) <= 0)
			return (printf("./philo: positive numeric argument required\n"));
	return (0);
}

void	error(void)
{
	printf("Usage: "RED"./philo "YELLOW"[number_of_philosophers] \
[time_to_die] [time_to_eat] [time_to_sleep] \
"CYAN"[number_of_times_each_philosopher_must_eat \
(optional)]\n"RESET);
}

void	init_data(t_philo *philo, char **av, t_data *data)
{
	int	i;

	i = 0;
	data->n_ph = ft_atoi(av[1]);
	data->die_time = (unsigned int)ft_atoi(av[2]);
	data->eat_time = (unsigned int)ft_atoi(av[3]);
	data->sleep_time = (unsigned int)ft_atoi(av[4]);
	data->fork = (pthread_mutex_t *)malloc(data->n_ph
			* sizeof(pthread_mutex_t));
	pthread_mutex_init(&data->mutex, NULL);
	pthread_mutex_init(&data->eat, NULL);
	pthread_mutex_init(&data->meals, NULL);
	while (i < data->n_ph)
	{
		philo[i].id = i + 1;
		philo[i].last_eat = get_time();
		pthread_mutex_init(&data->fork[i], NULL);
		philo[i].data = data;
		philo[i].nb_meals = -1;
		if (av[5])
			philo[i].nb_meals = ft_atoi(av[5]);
		philo[i].finish = 0;
		i++;
	}
	data->t0 = get_time();
}
