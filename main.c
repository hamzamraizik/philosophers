
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
	print_status(data, i, "sleeping...");
	usleep(data->time_to_sleep);
}
void	ft_eating(t_data *data, int i)
{
	data->philos[i].meals_counter += 1;
	data->philos[i].last_meal_time = current_time();
	print_status(data, i, "is eating...");
	usleep(data->time_to_eat);
	if (data->how_much_time_must_eat != -1 && data->philos[i].meals_counter >= data->how_much_time_must_eat)
	{
		pthread_mutex_lock(&data->simulation_lock);
		data->end_simulation = true;
		pthread_mutex_unlock(&data->simulation_lock);
	}
}

void	ft_thinking(t_data *data, long philo_id)
{
	// long	t_think;

	// t_think = (data->time_to_eat * 2) - data->time_to_sleep;
	// if (t_think <= 0)
	// 	t_think = 0;
	print_status(data, philo_id, "is thinking...");
	usleep(500);
}

void *philos_routine(void *void_data)
{
    t_data *data;
    t_philo *philo;

	philo = (t_philo*)void_data;
	data = philo->data;
    while (1)
    {
        pthread_mutex_lock(&data->simulation_lock);
        if (data->end_simulation)
        {
            pthread_mutex_unlock(&data->simulation_lock);
           	break;
        }
        pthread_mutex_unlock(&data->simulation_lock);
        ft_thinking(data, philo->id);
        if (philo->id % 2 != 0)
        {
            pthread_mutex_lock(&philo->first_fork->fork);
            print_status(data, philo->id, "has taken a fork");
            pthread_mutex_lock(&philo->second_fork->fork);
            print_status(data, philo->id, "has taken a fork");
        }
        else
        {
            pthread_mutex_lock(&philo->second_fork->fork);
            print_status(data, philo->id, "has taken a fork");
            pthread_mutex_lock(&philo->first_fork->fork);
            print_status(data, philo->id, "has taken a fork");
        }
		ft_eating(data, philo->id);
        pthread_mutex_unlock(&philo->first_fork->fork);
        pthread_mutex_unlock(&philo->second_fork->fork);
        ft_sleeping(data, philo->id);
    }
    return (NULL);
}

void *routine_monitor(void *void_data)
{
	t_data *data;
	int i;
	long	time_utilized;

	data = (t_data *)void_data;
	while (!(data->end_simulation))
	{
		i = -1;
		while (++i < data->number_of_philos)
		{
			time_utilized = (current_time() - data->philos[i].last_meal_time);
			pthread_mutex_lock(&data->simulation_lock);
			if (time_utilized > data->time_to_die)
			{
				print_status(data, data->philos[i].id, "died");
				data->end_simulation = true;
				pthread_mutex_unlock(&data->simulation_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->simulation_lock);
		}
		usleep(1000); // Check every millisecond
	}
	return (NULL);
}

void	init_data(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!data->philos)
		ft_error(data, "malloc error");
	data->forks = malloc(data->number_of_philos * sizeof(t_fork));
	if (!data->forks)
		ft_error(data, "malloc error");
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		ft_error(data, "mutex init failed for 'print_lock'");
	if (pthread_mutex_init(&data->simulation_lock, NULL) != 0)
		ft_error(data, "mutex init failed for 'simulation_lock'");
	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_mutex_init(&(data->forks[i].fork), NULL) != 0)
			ft_error(data, "mutex init failed");
		data->forks[i].fork_id = i + 1;
	}
}

void	init_philos(t_data *data)
{
	int	i;
	pthread_t monitor_thread;

	i = -1;
	while (++i < data->number_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_counter = 0;
		data->philos[i].last_meal_time = current_time();
		data->philos[i].first_fork = &data->forks[i];
		data->philos[i].second_fork = &data->forks[(i + 1) % data->number_of_philos];
		data->philos[i].data = data;
		if (pthread_create(&data->philos[i].thread_id, NULL, philos_routine, &data->philos[i]) != 0)
			ft_error(data, "pthread create failed!");
	}
	if (pthread_create(&monitor_thread, NULL, routine_monitor, data) != 0)
		ft_error(data, "pthread create failed!");
	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_join(data->philos[i].thread_id, NULL) != 0)
			ft_error(data, "pthread join failed");
	}
	if (pthread_join(monitor_thread, NULL) != 0)
		ft_error(data, "pthread join failed");
}

int main(int ac, char *av[])
{
	t_data	philos;
	int		i;

	if (ac == 5 || ac == 6)
		parsing(av, &philos);
	else
		ft_exit("the number of arguments not enough 🙅!");
	init_data(&philos);
	init_philos(&philos);
	i = 0;
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
