
#include "philo.h"

void	ft_sleeping(t_data *data, int i)
{
	print_status(data, i, "is sleeping");
	ft_usleep(data->time_to_sleep, data);
}

void	ft_eating(t_data *data, int i)
{
	print_status(data, i, "is eating");
	ft_usleep(data->time_to_eat, data);
}

void	ft_thinking(t_data *data, long philo_id)
{
	print_status(data, philo_id, "is thinking");
}

long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_status(t_data *data, int id, const char *status)
{
    sem_wait(data->print_lock);
    if (check(data) == 0)
        printf("%ld %d %s\n", current_time() - data->start_time, id, status);
    sem_post(data->print_lock);
}

