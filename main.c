
#include "philo.h"

void	parsing(char **av, t_philo *philos)
{
	philos->number_of_philos = ft_atoi(av[1]);
	// from ms to microsecond.
	philos->time_to_die = ft_atoi(av[2]) * 1000;
	philos->time_to_eat = ft_atoi(av[3]) * 1000;
	philos->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (av[5])
		philos->how_much_time_must_eat = ft_atoi(av[5]) * 1000;
	else
		philos->how_much_time_must_eat = -1;
}

int main(int ac, char *av[])
{
	t_philo	philos;

	if (ac == 5 || ac == 6)
		parsing(av, &philos);
	else
		ft_exit("the number of arguments not enough 🙅!");
	return 0;
}
