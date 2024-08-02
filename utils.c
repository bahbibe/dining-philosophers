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
	while (*(++av))
		if (get_arg(*av) <= 0)
			return (printf("./philo: positive numeric argument required\n"));
	return (0);
}

void	printing(t_philo *philo, char *msg, unsigned long time)
{
	pthread_mutex_lock(&philo->data->mutex);
	if (philo->data->print == 1)
		printf("%ld ms %d %s \n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->mutex);
}

void	init_data(t_philo *philo, char **av, t_data *data)
{
	int	i;

	i = -1;
	data->n_ph = ft_atoi(av[1]);
	data->die_time = (unsigned int)ft_atoi(av[2]);
	data->eat_time = (unsigned int)ft_atoi(av[3]);
	data->sleep_time = (unsigned int)ft_atoi(av[4]);
	data->fork = \
		(pthread_mutex_t *)malloc(data->n_ph * sizeof(pthread_mutex_t));
	pthread_mutex_init(&data->mutex, NULL);
	pthread_mutex_init(&data->eat, NULL);
	pthread_mutex_init(&data->meals, NULL);
	pthread_mutex_init(&data->simulation, NULL);
	while (++i < data->n_ph)
	{
		philo[i].id = i + 1;
		philo[i].last_eat = get_time();
		pthread_mutex_init(&data->fork[i], NULL);
		philo[i].data = data;
		philo[i].nb_meals = -1;
		if (av[5])
			philo[i].nb_meals = ft_atoi(av[5]);
		philo[i].finish = 0;
	}
	data->t0 = get_time();
}
