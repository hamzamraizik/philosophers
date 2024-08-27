#include "philo.h"

int	init_philos(t_data *data)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (++i < data->number_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].name = ft_itoa(i + 1);
		data->philos[i].meals_counter = 0;
		data->philos[i].last_meal_time = current_time();
		data->philos[i].first_fork = &data->forks[i];
		data->philos[i].second_fork
			= &data->forks[(i + 1) % data->number_of_philos];
		data->philos[i].data = data;

		pid = fork();
		if (pid < 0)
			return (-1); // Error creating process
		if (pid == 0)
		{
			philos_routine(&data->philos[i]);
			exit(0); // Exit child process after routine is done
		}
		data->philos[i].pid = pid; // Store PID for later use
	}
	return (0);
}

#include <semaphore.h>
#include <fcntl.h>   // For O_CREAT
#include <sys/stat.h> // For mode constants

int	init_data(t_data *data)
{
	int	i;

	// Initialize named semaphores
	data->print_lock = sem_open("print_lock", O_CREAT | O_EXCL, 0644, 1);
	if (data->print_lock == SEM_FAILED)
	{
		printf("Error creating semaphore print_lock\n");
		return (-1);
	}
	data->simulation_lock = sem_open("simulation_lock", O_CREAT | O_EXCL, 0644, 1);
	if (data->simulation_lock == SEM_FAILED)
		return (-1);
	data->forks = malloc(data->number_of_philos * sizeof(t_fork));
	if (!data->forks)
		return (-1);
	i = -1;
	while (++i < data->number_of_philos)
	{
		data->forks[i].fork = sem_open(data->philos[i].name, O_CREAT | O_EXCL, 0644, 1);
		if (data->forks[i].fork == SEM_FAILED)
			return (-1);
		data->forks[i].fork_id = i + 1;
	}
	return (0);
}

void	cleanup_semaphores(t_data *data)
{
	int i;

	// Close and unlink semaphores
	sem_close(data->print_lock);
	sem_unlink("/myprintlock");

	sem_close(data->simulation_lock);
	sem_unlink("/mysimulationlock");

	for (i = 0; i < data->number_of_philos; ++i)
	{
		sem_close(data->forks[i].fork);
		char sem_name[20];
		snprintf(sem_name, sizeof(sem_name), "/myfork%d", i);
		sem_unlink(sem_name);
	}
	free(data->forks);
}

