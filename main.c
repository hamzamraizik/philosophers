
#include "philo.h"

void	ft_error(t_data *data, const char *s)
{
	int	i;

	i = 0;
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
	printf("%s\n", s);
	exit(1);
}

void *philos_routine(void *arg);
void	parsing(char **av, t_data *philos)
{
	philos->number_of_philos = ft_atoi(av[1]);
	// from ms to microsecond.
	philos->time_to_die = ft_atoi(av[2]) * 1000;
	philos->time_to_eat = ft_atoi(av[3]) * 1000;
	philos->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (av[5])
		philos->how_much_time_must_eat = ft_atoi(av[5]);
	else
		philos->how_much_time_must_eat = -1;
}

void	init_data(t_data *data)
{
	int	i;

	i = -1;
	data->philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!data->philos)
	{
		ft_error(data, "malloc error");
	}
	data->forks = malloc(data->number_of_philos * sizeof(t_fork));
	if (!data->forks)
	{
		ft_error(data, "malloc error");
	}
	while (++i < data->number_of_philos)
	{
		if (pthread_mutex_init(&(data->forks[i].fork), NULL) != 0)
		{
			ft_error(data, "mutex init failed");
		}
		data->forks[i].fork_id = i + 1;
	}
}

void	init_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_create(&data->philos[i].thread_id, NULL, philos_routine, data) != 0)
			ft_error(data, "pthread create failer!");
		data->philos[i].id = i;
		data->philos[i].meals_counter = 0;
		data->philos[i].first_fork = &data->forks[i];
		data->philos[i].first_fork = &data->forks[(i + 1) % data->number_of_philos];
		data->philos[i].data = data;
	}
	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_join(data->philos[i].thread_id, NULL) != 0)
			ft_error(data, "pthread join failer");
	}
}

int main(int ac, char *av[])
{
	t_data	philos;

	if (ac == 5 || ac == 6)
		parsing(av, &philos);
	else
		ft_exit("the number of arguments not enough 🙅!");
	init_data(&philos);
	init_philos(&philos);
	return 0;
}
