#include "philo.h"

int	parsing(char **av, t_data *philos)
{
	philos->number_of_philos = ft_atoi(av[1]);
	philos->time_to_die = ft_atoi(av[2]);
	philos->time_to_eat = ft_atoi(av[3]);
	philos->time_to_sleep = ft_atoi(av[4]);
	philos->is_full_counter = 0;
	philos->start_time = current_time();
	if (av[5])
		philos->must_eat = ft_atoi(av[5]);
	else
		philos->must_eat = -1;
	if (philos->number_of_philos == -1 
		|| philos->number_of_philos == 0
		|| philos->time_to_die == -1
		|| philos->time_to_eat == -1 || philos->time_to_sleep == -1
		|| (av[5] && philos->must_eat == -1))
		return (-1);
	philos->end_simulation = false;
	return (0);
}

int main(int ac, char *av[])
{
	t_data philos;

	if (ac == 5 || ac == 6)
	{
		if (parsing(av, &philos) == -1)
			return (printf("wrong arguments!!\n"), 1);
	}
	else
		return (printf("the number of arguments not enough 🙅!\n"), 1);
	if (init_data(&philos) == -1)
		return (printf("Initialization error\n"), 1);
	int i = -1;
	while (++i < philos.number_of_philos)
	{
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("Fork failed");
			return (1);
		}
		else if (pid == 0)
			philosopher_process(&philos.philos[i]);
		else
			philos.philos[i].process_id = pid;
	}
	// Monitor and wait for processes
	for (i = 0; i < philos.number_of_philos; i++)
		waitpid(philos.philos[i].process_id, NULL, 0);
	cleanup_semaphores(&philos);
	free(philos.philos);
	return (0);
}