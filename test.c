
#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct s_fork
{
    pthread_mutex_t fork;
    int fork_id;
}               t_fork;

typedef struct s_philo
{
    pthread_t       thread_id;
    int             id;
    int             meals_counter;
    long            last_meal_time;
    t_fork          *first_fork;
    t_fork          *second_fork;
    struct s_data   *data;
}               t_philo;

typedef struct s_data
{
    int             number_of_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             how_much_time_must_eat;
    bool            end_simulation;
    t_philo         *philos;
    t_fork          *forks;
    pthread_mutex_t print_lock;
    pthread_mutex_t simulation_lock;
}               t_data;

int     ft_atoi(const char *str);
void    ft_error(t_data *data, const char *s);
void    parsing(char **av, t_data *data);
void    init_data(t_data *data);
void    init_philos(t_data *data);
void    *philos_routine(void *void_data);
void    *monitor_routine(void *void_data);
void    ft_sleeping(t_data *data, int i);
void    ft_eating(t_data *data, int i);
void    ft_thinking(t_data *data, int i);
long    current_time(void);
void    print_status(t_data *data, int id, const char *status);

#endif

/////////////////////////////////////////////////////////////////

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
	print_status(data, data->philos[i].id, "is sleeping");
	usleep(data->time_to_sleep * 1000);
}

void	ft_eating(t_data *data, int i)
{
	data->philos[i].meals_counter += 1;
	data->philos[i].last_meal_time = current_time();
	print_status(data, data->philos[i].id, "is eating");
	usleep(data->time_to_eat * 1000);
	if (data->how_much_time_must_eat != -1 && data->philos[i].meals_counter >= data->how_much_time_must_eat)
		data->end_simulation = true;
}

void	ft_thinking(t_data *data, int i)
{
	print_status(data, data->philos[i].id, "is thinking");
	usleep(1000);
}

void *philos_routine(void *void_data)
{
	t_philo *philo;
	t_data *data;

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
		pthread_mutex_lock(&philo->first_fork->fork);
		print_status(data, philo->id, "has taken a fork");
		pthread_mutex_lock(&philo->second_fork->fork);
		print_status(data, philo->id, "has taken a fork");
		ft_eating(data, philo->id);
		pthread_mutex_unlock(&philo->first_fork->fork);
		pthread_mutex_unlock(&philo->second_fork->fork);
		ft_sleeping(data, philo->id);
	}
	return (NULL);
}

void *monitor_routine(void *void_data)
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
				print_status(data, data->philos[i].id, "died");
				data->end_simulation = true;
			}
			pthread_mutex_unlock(&data->simulation_lock);
		}
		usleep(1000); // Check every millisecond
	}
	return (NULL);
}

void	parsing(char **av, t_data *data)
{
	data->number_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->how_much_time_must_eat = ft_atoi(av[5]);
	else
		data->how_much_time_must_eat = -1;
	data->end_simulation = false;
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
		ft_error(data, "mutex init failed");
	if (pthread_mutex_init(&data->simulation_lock, NULL) != 0)
		ft_error(data, "mutex init failed");
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
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data) != 0)
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
	{
		printf("the number of arguments is not enough 🙅!\n");
		return (1);
	}
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
	return (0);
}



///////////////////////////////////////////////////////////////////////

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return (result * sign);
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
