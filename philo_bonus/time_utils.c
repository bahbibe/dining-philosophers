#include "philo_bonus.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(int ms)
{
	long	start;

	start = get_time();
	usleep(ms * 1000 * 0.9);
	while (get_time() - start < ms)
		usleep(100);
}
