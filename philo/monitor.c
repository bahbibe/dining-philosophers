/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 00:00:00 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Silently drops the line once the simulation has ended, so no state
** message can ever be printed after (or overlapping) the death line. */
void	printing(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (philo->data->running)
		printf("%lu %d %s\n", get_time() - philo->data->t0, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	report_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (philo->data->running)
	{
		printf("%lu %d died\n", get_time() - philo->data->t0, philo->id);
		philo->data->running = 0;
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

static int	starved(t_philo *philo)
{
	unsigned long	since_last_meal;

	pthread_mutex_lock(&philo->data->meal_lock);
	since_last_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->data->meal_lock);
	return (since_last_meal >= (unsigned long)philo->data->die_time);
}

/*
** Polled from main(): true once a philosopher has starved (and its death
** has just been reported) or every philosopher has hit its meal quota.
** A philosopher that already finished is skipped: its last_meal stops
** updating on purpose once it stops eating, so it must not be judged
** against die_time any more.
*/
int	is_dead(t_philo *philo)
{
	int	i;
	int	done_count;
	int	finished;

	done_count = 0;
	for (i = 0; i < philo->data->n_ph; i++)
	{
		pthread_mutex_lock(&philo->data->progress_lock);
		finished = philo[i].finished;
		pthread_mutex_unlock(&philo->data->progress_lock);
		done_count += finished;
		if (!finished && starved(&philo[i]))
		{
			report_death(&philo[i]);
			return (1);
		}
	}
	return (done_count == philo->data->n_ph);
}
