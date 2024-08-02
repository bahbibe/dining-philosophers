/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:38:27 by bahbibe           #+#    #+#             */
/*   Updated: 2024/08/02 02:12:25 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_data	*data;

	if (ac != 5 && ac != 6)
		return (printf(USAGE));
	if (check_args(av))
		return (1);
	data = (t_data *)malloc(sizeof(t_data));
	philo = (t_philo *)malloc(sizeof(t_philo) * ft_atoi(av[1]));
	init_data(philo, av, data);
	data->end_simulation = 1;
	pthread_mutex_lock(&data->mutex);
	data->print = 1;
	pthread_mutex_unlock(&data->mutex);
	if (create_philo(philo))
		return (free(data->fork), free(data), free(philo), 1);
	while (1)
	{
		if (death(philo))
			break ;
	}
	join_threads(philo);
	return (free(data->fork), free(data), free(philo), 0);
}
