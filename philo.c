/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 23:56:07 by bahbibe           #+#    #+#             */
/*   Updated: 2023/05/12 06:27:56 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philo *philo)
{
	if (philo->id == philo->data->n_ph)
	{
		pthread_mutex_lock(&philo->data->fork[philo->id % philo->data->n_ph]);
		printing(philo, "has taken a fork", get_time() - philo->data->t0);
		pthread_mutex_lock(&philo->data->fork[philo->id - 1]);
		printing(philo, "has taken a fork", get_time() - philo->data->t0);
	}
	else
	{
		pthread_mutex_lock(&philo->data->fork[philo->id - 1]);
		printing(philo, "has taken a fork", get_time() - philo->data->t0);
		pthread_mutex_lock(&philo->data->fork[philo->id % philo->data->n_ph]);
		printing(philo, "has taken a fork", get_time() - philo->data->t0);
	}
}

void	print_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex);
	printf("%ld ms %d %s \n", \
			(get_time() - philo->data->t0), philo->id, "died");
	pthread_mutex_unlock(&philo->data->mutex);
	pthread_mutex_lock(&philo->data->simulation);
	philo->data->end_simulation = 0;
	pthread_mutex_lock(&philo->data->mutex);
	philo->data->print = 0;
	pthread_mutex_unlock(&philo->data->mutex);
	pthread_mutex_unlock(&philo->data->simulation);
}

int	death(t_philo *philo)
{
	int				i;
	int				count;
	unsigned long	t;

	i = 0;
	count = 0;
	while (i < philo[0].data->n_ph)
	{
		pthread_mutex_lock(&philo->data->meals);
		if (philo[i].finish == 1)
			count++;
		pthread_mutex_unlock(&philo->data->meals);
		pthread_mutex_lock(&philo->data->eat);
		t = philo[i].last_eat;
		if ((unsigned long)philo[i].data->die_time <= get_time() - t)
		{
			print_dead(&philo[i]);
			pthread_mutex_unlock(&philo->data->eat);
			return (1);
		}
		pthread_mutex_unlock(&philo->data->eat);
		i++;
	}
	return (count == philo->data->n_ph);
}

void	spaghetti(t_philo *philo)
{
	assign_forks(philo);
	printing(philo, "is eating", get_time() - philo->data->t0);
	timer(philo->data->eat_time);
	pthread_mutex_lock(&philo->data->eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->data->eat);
	pthread_mutex_lock(&philo->data->meals);
	philo->nb_meals--;
	pthread_mutex_unlock(&philo->data->meals);
	pthread_mutex_unlock(&philo->data->fork[philo->id % philo->data->n_ph]);
	pthread_mutex_unlock(&philo->data->fork[philo->id - 1]);
	printing(philo, "is sleeping", get_time() - philo->data->t0);
	timer(philo->data->sleep_time);
	printing(philo, "is thinking", get_time() - philo->data->t0);
}

void	*simulate(void *arg)
{
	t_philo	*philo;
	int		end;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->simulation);
	end = philo->data->end_simulation;
	pthread_mutex_unlock(&philo->data->simulation);
	while (end)
	{
		pthread_mutex_lock(&philo->data->meals);
		if (philo->nb_meals == 0)
		{
			philo->finish = 1;
			pthread_mutex_unlock(&philo->data->meals);
			break ;
		}
		pthread_mutex_unlock(&philo->data->meals);
		spaghetti(philo);
		pthread_mutex_lock(&philo->data->simulation);
		end = philo->data->end_simulation;
		pthread_mutex_unlock(&philo->data->simulation);
	}
	return (NULL);
}
