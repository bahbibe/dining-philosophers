/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 02:11:20 by bahbibe           #+#    #+#             */
/*   Updated: 2024/08/02 02:12:35 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].data->n_ph)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

int	create_philo(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].data->n_ph)
	{
		if (pthread_create(&philo[i].thread, NULL, &simulate, &philo[i]))
			return (printf("error creating philos"));
		i += 1;
		usleep(10);
	}
	return (0);
}
