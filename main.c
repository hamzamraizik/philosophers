
#include "philo.h"
int check(t_data *data)
{
	pthread_mutex_lock(&data->simulation_lock);
	if (data->end_simulation)
	{
		pthread_mutex_unlock(&data->simulation_lock);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&data->simulation_lock);
		return (0);
	}
}

void *philos_routine(void *void_data)
{
	t_data *data;
	t_philo *philo;

	philo = (t_philo*)void_data;
	data = philo->data;
	while (!check(data))
	{
		if (philo->id % 2 != 0)
		{
			pthread_mutex_lock(&philo->first_fork->fork);
			print_status(data, philo->id, "has taken a fork");
			pthread_mutex_lock(&philo->second_fork->fork);
			print_status(data, philo->id, "has taken a fork");
		}
		else
		{
			usleep(300);
			pthread_mutex_lock(&philo->second_fork->fork);
			print_status(data, philo->id, "has taken a fork");
			pthread_mutex_lock(&philo->first_fork->fork);
			print_status(data, philo->id, "has taken a fork");
		}
		ft_eating(data, philo->id);
		pthread_mutex_lock(&data->simulation_lock);
		philo->last_meal_time = current_time();
		philo->meals_counter += 1;
		pthread_mutex_unlock(&data->simulation_lock);
		// printf("%ld\n", philo->meals_counter);
		pthread_mutex_unlock(&philo->first_fork->fork);
		pthread_mutex_unlock(&philo->second_fork->fork);
		ft_sleeping(data, philo->id);
		ft_thinking(data, philo->id);
	}
	return (NULL);
}

// void *philos_routine(void *void_data)
// {
//     t_data *data;
//     t_philo *philo;
//
//     philo = (t_philo*)void_data;
//     data = philo->data;
//     while (1)
//     {
//         // if (philo->id % 2 != 0)
//         //     usleep(200);
//         pthread_mutex_lock(&data->simulation_lock);
//         if (data->end_simulation)
//             return (NULL);
//         pthread_mutex_unlock(&data->simulation_lock);
//
//         t_fork *first_fork = philo->id < philo->second_fork->fork_id ? philo->first_fork : philo->second_fork;
//         t_fork *second_fork = philo->id < philo->second_fork->fork_id ? philo->second_fork : philo->first_fork;
//
//         pthread_mutex_lock(&first_fork->fork);
//         print_status(data, philo->id, "has taken a fork");
//         pthread_mutex_lock(&second_fork->fork);
//         print_status(data, philo->id, "has taken a fork");
//
//         ft_eating(data, philo->id);
//         philo->last_meal_time = current_time();
//         philo->meals_counter += 1;
//         pthread_mutex_unlock(&first_fork->fork);
//         pthread_mutex_unlock(&second_fork->fork);
//         ft_sleeping(data, philo->id);
//         ft_thinking(data, philo->id);
//     }
//     return (NULL);
// }

void *routine_monitor(void *void_data)
{
	t_data	*data;
	int		i;
	int		count;

	data = (t_data *)void_data;
	while (1)
	{
		i = 0;
		count = 0;
		while (i < data->number_of_philos)
		{
			pthread_mutex_lock(&data->simulation_lock);
			if (data->end_simulation)
			{
				pthread_mutex_unlock(&data->simulation_lock);
				return NULL; 
			}
            if ((current_time() - data->philos[i].last_meal_time) > data->time_to_die)
			{
				print_status(data, data->philos[i].id, "died");
				data->end_simulation = true;
				pthread_mutex_unlock(&data->simulation_lock);
				return (NULL);
			}
			if (data->how_much_time_must_eat != -1 && data->philos[i].meals_counter >= data->how_much_time_must_eat)
				count += 1;
			if (count == data->number_of_philos)
			{
				print_status(data, count, "are all full, end semulation");
				data->end_simulation = true;
				pthread_mutex_unlock(&data->simulation_lock);
				return NULL;
				// break ;
			}
			pthread_mutex_unlock(&data->simulation_lock);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}

void	parsing(char **av, t_data *philos)
{
	philos->number_of_philos = ft_atoi(av[1]);
	philos->time_to_die = ft_atoi(av[2]);
	philos->time_to_eat = ft_atoi(av[3]);
	philos->time_to_sleep = ft_atoi(av[4]);
	philos->is_full_counter = 0;
	philos->start_time = current_time();
	if (av[5])
		philos->how_much_time_must_eat = ft_atoi(av[5]);
	else
		philos->how_much_time_must_eat = -1;
	philos->end_simulation = false;
}

void	ft_end(t_data *philos, int j)
{
	int		i;

	i = -1;
	(void)j;
	// pthread_detach((philos->monitor_thread));
	while (++i < philos->number_of_philos)
			pthread_detach((philos->philos[i].thread_id));
	i = -1;
	while (++i < philos->number_of_philos)
		pthread_mutex_destroy(&philos->forks[i].fork);
	pthread_mutex_destroy(&philos->print_lock);
	pthread_mutex_destroy(&philos->simulation_lock);
	free(philos->forks);
	free(philos->philos);
	return ;
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
	// ft_end(&philos, 0);
	return 0;
}
