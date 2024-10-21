/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:14:15 by hmraizik          #+#    #+#             */
/*   Updated: 2024/09/08 18:19:18 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*routine_monitor(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (1)
	{
		sem_wait(philo->data->sim_lock);
		if (philo->last_meal_time != -1 && (current_time()
				- philo->last_meal_time) >= philo->data->time_to_die)
		{
			sem_wait(philo->data->print_lock);
			printf("%ld %d died\n", current_time()
				- philo->data->start_time, philo->id);
			sem_post(philo->data->sim_lock);
			exit(1);
		}
		sem_post(philo->data->sim_lock);
		usleep(1000);
	}
	return (NULL);
}

void	wait_for_philos(t_data philos)
{
	int		i;
	int		status;

	i = -1;
	while (++i < philos.number_of_philos)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			while (++i < philos.number_of_philos)
				kill(philos.philos[i].process_id, SIGKILL);
			break ;
		}
	}
}

int	parsing(char **av, t_data *philos)
{
	philos->number_of_philos = ft_atoi(av[1]);
	philos->time_to_die = ft_atoi(av[2]);
	philos->time_to_eat = ft_atoi(av[3]);
	philos->time_to_sleep = ft_atoi(av[4]);
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
	return (0);
}

void	ft_start_simulation(t_data *philos)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (++i < philos->number_of_philos)
	{
		pid = fork();
		if (pid == -1)
		{
			ft_putstr_fd("fork failed\n", 2);
			return ;
		}
		else if (pid == 0)
		{
			pthread_create(&philos->philos[i].thread, NULL,
				routine_monitor, &philos->philos[i]);
			pthread_detach(philos->philos[i].thread);
			philosopher_process(&philos->philos[i]);
		}
		else
			philos->philos[i].process_id = pid;
	}
}

int	main(int ac, char *av[])
{
	t_data	philos;

	if (ac == 5 || ac == 6)
	{
		if (parsing(av, &philos) == -1)
			return (ft_putstr_fd("wrong arguments!!\n", 2), 1);
	}
	else
		return (ft_putstr_fd("the number of arguments incorrect!\n", 2), 1);
	if (init_data(&philos) == -1)
		return (ft_putstr_fd("Initialization error\n", 2), 1);
	ft_start_simulation(&philos);
	wait_for_philos(philos);
	cleanup_semaphores(&philos);
	free(philos.philos);
	return (0);
}
