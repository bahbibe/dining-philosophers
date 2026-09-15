#include "philo_bonus.h"

int	ft_is_space(int c)
{
	return (c == '\f' || c == '\n' || c == '\r'
		|| c == '\t' || c == '\v' || c == ' ');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	res;

	i = 0;
	res = 0;
	sign = 1;
	while (ft_is_space((int)str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '-') ? -1 : 1;
	while (ft_isdigit(str[i]))
	{
		res = res * 10 + (str[i] - '0');
		i++;
		if (res * -1 < (long)INT_MIN || (res > (long)INT_MAX && sign == 1))
			return (-1);
	}
	return ((int)res * sign);
}

/* Only accepts plain digit strings: no sign, no spaces. -1 on anything else. */
int	get_arg(char *arg)
{
	int	i;

	for (i = 0; arg[i]; i++)
		if (!ft_isdigit(arg[i]))
			return (-1);
	return (ft_atoi(arg));
}

/* Every argument (av[1..]) must be a strictly positive integer. */
int	check_args(char **av)
{
	int	i;

	for (i = 1; av[i]; i++)
	{
		if (get_arg(av[i]) <= 0)
		{
			printf("./philo_bonus: positive numeric argument required\n");
			return (1);
		}
	}
	return (0);
}
