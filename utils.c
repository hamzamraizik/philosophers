#include "philo.h"

void	ft_exit(const char *s)
{
	printf(RED"%s", s);
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

long current_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void print_status(t_data *data, int id, const char *status)
{
	pthread_mutex_lock(&data->print_lock);
	if (!data->end_simulation)
		printf("%ld %d %s\n", current_time(), id, status);
	pthread_mutex_unlock(&data->print_lock);
}
