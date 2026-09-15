/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:38:27 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	spawn_philos(t_philo *philo)
{
	int	i;

	for (i = 0; i < philo[0].data->n_ph; i++)
	{
		if (pthread_create(&philo[i].thread, NULL, &live, &philo[i]))
		{
			printf("./philo: error creating philosopher threads\n");
			return (1);
		}
		usleep(10);
	}
	return (0);
}

void	join_philos(t_philo *philo)
{
	int	i;

	for (i = 0; i < philo[0].data->n_ph; i++)
		pthread_join(philo[i].thread, NULL);
}

static int	setup(int ac, char **av, t_philo **philo, t_data **data)
{
	if (ac != 5 && ac != 6)
		return (printf(USAGE), 1);
	if (check_args(av))
		return (1);
	*data = (t_data *)malloc(sizeof(t_data));
	*philo = (t_philo *)malloc(sizeof(t_philo) * ft_atoi(av[1]));
	if (!*data || !*philo)
		return (free(*data), free(*philo), 1);
	if (init_data(*philo, av, *data))
		return (free(*data), free(*philo), 1);
	return (0);
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_data	*data;

	if (setup(ac, av, &philo, &data))
		return (1);
	if (spawn_philos(philo))
	{
		destroy_data(data);
		return (free(data), free(philo), 1);
	}
	while (!is_dead(philo))
		usleep(500);
	join_philos(philo);
	destroy_data(data);
	return (free(data), free(philo), 0);
}
