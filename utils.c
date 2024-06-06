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

// void init_philosophers(t_data *data) {
//     for (int i = 0; i < data->number_of_philos; i++) {
//         data->philosophers[i].id = i + 1;
//         data->philosophers[i].times_eaten = 0;
//         data->philosophers[i].last_meal_time = get_timestamp();
//         data->philosophers[i].left_fork = &data->forks[i];
//         data->philosophers[i].right_fork = &data->forks[(i + 1) % data->number_of_philosophers];
//         data->philosophers[i].data = data;
//     }
// }
