
#include "philo.h"

int	ft_atoi(char *s)
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
			return (-1);
		number = result;
		i++;
	}
	if (s[i])
		return (-1);
	return (result);
}

void	ft_usleep(long time, t_data *data)
{
	long	time_before;

	time_before = current_time();
	while (current_time() - time_before < time)
	{
		if (check(data) == 1)
			return ;
		usleep(100);
	}
}
