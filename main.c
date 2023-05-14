/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:38:27 by bahbibe           #+#    #+#             */
/*   Updated: 2023/05/14 21:55:35 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_data	*data;

	if (ac == 5 || ac == 6)
	{
		if (check_args(av))
			return (1);
		data = (t_data *) malloc(sizeof(t_data));
		philo = (t_philo *) malloc(sizeof(t_philo) * ft_atoi(av[1]));
		init_data(philo, av, data);
		if (creat_philo(philo))
			return (1);
		while (1)
		{
			if (death(philo))
				break ;
		}
	}
	else
		error();
	return (0);
}
