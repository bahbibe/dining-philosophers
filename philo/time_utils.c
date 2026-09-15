/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 00:00:00 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/*
** usleep() alone drifts (it may oversleep, and the kernel gives no
** precision guarantee), so sleep for 90% of the target via usleep and
** spin-poll the last stretch in small steps to land within ~100us of it.
*/
void	precise_sleep(int ms)
{
	unsigned long	start;

	start = get_time();
	usleep(ms * 1000 * 0.9);
	while (get_time() - start < (unsigned long)ms)
		usleep(100);
}
