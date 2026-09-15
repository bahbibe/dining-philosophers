/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 00:00:00 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	put_nbr(char *buf, int pos, long n)
{
	char	digits[20];
	int		n_digits;

	n_digits = 0;
	if (n == 0)
		digits[n_digits++] = '0';
	while (n > 0)
	{
		digits[n_digits++] = '0' + (n % 10);
		n /= 10;
	}
	while (n_digits > 0)
		buf[pos++] = digits[--n_digits];
	return (pos);
}

/*
** write() straight to fd 1 instead of printf(): with one process per
** philosopher, buffered stdio means each process keeps its own private
** output buffer. If that process is later SIGKILLed (as happens to every
** survivor once one philosopher dies), whatever was sitting unflushed in
** its buffer is lost for good. write() has no such buffer to lose.
*/
void	printing(t_philo *philo, char *msg)
{
	char	buf[128];
	int		pos;
	int		i;

	pos = put_nbr(buf, 0, get_time() - philo->data->t0);
	buf[pos++] = ' ';
	pos = put_nbr(buf, pos, philo->id);
	buf[pos++] = ' ';
	for (i = 0; msg[i]; i++)
		buf[pos++] = msg[i];
	buf[pos++] = '\n';
	sem_wait(philo->data->print_lock);
	write(1, buf, pos);
	sem_post(philo->data->print_lock);
}
