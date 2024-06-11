
#include "philo.h"

void	ft_error(t_data *data, const char *s)
{
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
	printf("%s\n", s);
	exit(1);
}
void	ft_sleeping(t_data *data, int i)
{
	print_status(data, data->philos[i].id, "sleeping...");
	usleep(data->time_to_sleep);
}
void	ft_eating(t_data *data, int i)
{
	// check the time again if it equal the time to die, the philo will die.
	data->philos[i].meals_counter += 1;
	data->philos[i].last_meal_time = current_time();
	print_status(data, data->philos[i].id, "eating...");
	usleep(data->time_to_eat);
	if (data->how_much_time_must_eat != -1 && data->philos[i].meals_counter >= data->how_much_time_must_eat)
		data->end_simulation = true;
}

void	ft_thinking(t_data *data, long philo_id)
{
	print_status(data, philo_id, "thinking...");
	usleep(1000);
}

void	*routine_monitor(void *void_data)
{
	t_data *data;
	int i;

	data = (t_data *)void_data;
	while (!data->end_simulation)
	{
		i = -1;
		while (++i < data->number_of_philos)
		{
			pthread_mutex_lock(&data->simulation_lock);
			if (current_time() - data->philos[i].last_meal_time > data->time_to_die)
			{
				print_status(data, data->philos[i].id, "died...");
				data->end_simulation = true;
			}
			pthread_mutex_unlock(&data->simulation_lock);
		}
		usleep(1000); // Check every millisecond
	}
	return (NULL);
}

void *philos_routine(void *void_data)
{
	int	i;
	t_data *data;

	i = 0;
	data = (t_data*)void_data;
	// get time here as first.
	while(1)
	{
		pthread_mutex_lock(&data->simulation_lock);
		if (data->end_simulation)
		{
			pthread_mutex_unlock(&data->simulation_lock);
			break;
		}
		pthread_mutex_unlock(&data->simulation_lock);
		ft_thinking(data, data->philos[i].id);
		if (i % 2 == 0)
		{
			pthread_mutex_lock(&data->philos[i].first_fork->fork);
			print_status(data, data->philos[i].id, "has taken a fork");
			pthread_mutex_lock(&data->philos[i].second_fork->fork);
			print_status(data, data->philos[i].id, "has taken a fork");
		}
		else
		{
			// usleep(20);
			pthread_mutex_lock(&data->philos[i].second_fork->fork);
			print_status(data, data->philos[i].id, "has taken a fork");
			pthread_mutex_lock(&data->philos[i].first_fork->fork);
			print_status(data, data->philos[i].id, "has taken a fork");
		}
		if (pthread_mutex_trylock(&data->philos[i].first_fork->fork) != 0
				&& pthread_mutex_trylock(&data->philos[i].second_fork->fork) != 0)
		{
			ft_eating(data, i);
			pthread_mutex_unlock(&data->philos[i].first_fork->fork);
			pthread_mutex_unlock(&data->philos[i].second_fork->fork);
			ft_sleeping(data, i);
		}
		i = (i + 1) % data->number_of_philos;
	}
	return (NULL);
}

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
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		ft_error(data, "mutex init failed for 'print_lock'");
	if (pthread_mutex_init(&data->simulation_lock, NULL) != 0)
		ft_error(data, "mutex init failed for 'simulation_lock'");
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
		data->philos[i].id = i + 1;
		data->philos[i].meals_counter = 0;
		data->philos[i].last_meal_time = current_time();
		data->philos[i].first_fork = &data->forks[i];
		data->philos[i].second_fork = &data->forks[(i + 1) % data->number_of_philos];
		data->philos[i].data = data;
	}
	if (pthread_create(&data->monitor, NULL, routine_monitor, data) != 0)
			ft_error(data, "pthread create failer for monitor's thread!");
	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_join(data->philos[i].thread_id, NULL) != 0)
			ft_error(data, "pthread join failer");
	}
	if (pthread_join(data->monitor, NULL) != 0)
			ft_error(data, "pthread join failer for monitor!");
}

int main(int ac, char *av[])
{
	t_data	philos;
	int		i;

	i = 0;
	if (ac == 5 || ac == 6)
		parsing(av, &philos);
	else
		ft_exit("the number of arguments not enough 🙅!");
	init_data(&philos);
	init_philos(&philos);
	while (i < philos.number_of_philos)
	{
		pthread_mutex_destroy(&philos.forks[i].fork);
		i++;
	}
	pthread_mutex_destroy(&philos.print_lock);
	pthread_mutex_destroy(&philos.simulation_lock);
	free(philos.forks);
	free(philos.philos);
	return 0;
}
