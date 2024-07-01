#include "philo.h"

void	ft_exit(const char *s)
{
	printf(RED"%s", s);
	exit(1);
}

void	ft_error(t_data *data, const char *s)
{
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
	printf("%s\n", s);
	exit(1);
}

int		ft_atoi(char *s)
{
	int		i;
	int		result;
	int		number;

	result = 0;
	number = 0;
	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		result = result * 10 + s[i] - '0';
		if (result < number)
			ft_exit("you've passed a big number");
		number = result;
		i++;
	}
	if (s[i])
		ft_exit("write a correct number!");
	return (result);
}
/*int    get_time(t_data *data)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000 - data->start_time);
}

void    good_usleep(t_data *data, int time)
{
    int    start_time;

    start_time = get_time(data); //actual time
    while (get_time(data) - start_time < time)
        usleep(200);
}*/
void ft_usleep(long time)
{
	long	time_before;

	time_before = current_time();
	while (current_time() - time_before > time)
	{
		usleep(200);
	}
}
