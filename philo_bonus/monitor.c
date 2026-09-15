/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 00:00:00 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Runs as a second thread inside every philosopher process, watching that
** process's own last_meal. Calling exit() straight from here (instead of
** setting a flag for the main thread to notice) also cleanly kills the
** main thread even if it's blocked inside sem_wait() waiting on a fork
** that will now never come.
*/
void	*monitor_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->stop)
	{
		if (get_time() - philo->last_meal >= philo->data->die_time)
		{
			printing(philo, "died");
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}
