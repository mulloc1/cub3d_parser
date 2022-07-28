#include "cub3d.h"

void	ft_error(int cmp, char *message)
{
	if (cmp)
	{
		perror(message);
		exit(1);
	}		
}

void	ft_put_picture(t_cub *cub, char *type, char *value)
{
	if (!ft_strncmp(type, "NO", 3))
		cub->map.no = value;
	else if (!ft_strncmp(type, "SO", 3))
		cub->map.so = value;
	else if (!ft_strncmp(type, "WE", 3))
		cub->map.we = value;
	else if (!ft_strncmp(type, "EA", 3))
		cub->map.ea = value;
}

void	ft_put_rgb(t_cub *cub, char *type, char *value)
{
	char	**value_split;
	int		rgb;

	value_split = ft_split(value, ',');
	ft_error(!value_split, "ft_put_rgb() value_split ft_split() failed\n");	
	rgb = ft_atoi(value_split[0]) << 16;
	rgb += ft_atoi(value_split[1]) << 8;
	rgb += ft_atoi(value_split[2]);
	if (ft_strncmp(type, "F", 2)) cub->map.floor = rgb;
	else if (ft_strncmp(type, "C", 2))
		cub->map.ceil = rgb;
	free(value_split[0]);
	free(value_split[1]);
	free(value_split[2]);
	free(value_split);
}

int	ft_put_in(t_cub *cub, char **buf)
{
	if (!ft_strncmp(buf[0], "NO", 3) || !ft_strncmp(buf[0], "SO", 3) \
				|| !ft_strncmp(buf[0], "WE", 3) || !ft_strncmp(buf[0], "EA", 3))
	{
		ft_put_picture(cub, buf[0], buf[1]);
		free(buf[0]);
		free(buf);
		return (1);
	}
	else if (!ft_strncmp(buf[0], "F", 2) || !ft_strncmp(buf[0], "C", 2))
	{
		ft_put_rgb(cub, buf[0], buf[1]);
		free(buf[0]);
		free(buf[1]);
		free(buf);
		return (1);
	}
	return (0);
}

int	ft_type_parsing(t_cub *cub, char *buf)
{
	char	**buf_split;
	int		i;

	buf_split = ft_split(buf, ' ');
	ft_error(!buf_split, "ft_type_parsing() buf_split ft_split() failed\n");
	if (!ft_put_in(cub, buf_split))
	{
		i = -1;
		while (buf_split[++i])
			free(buf_split[i]);
		free(buf_split);
		return (0);
	}
	return (1);
}

void	creating_map(t_cub *cub, char *buf)
{
	char	**map;
	int		i;
	int		len;

	map = ft_calloc(++cub->map.height + 1, sizeof(char *));
	i = -1;
	while (++i < cub->map.height - 1 && cub->map.map)
		map[i] = cub->map.map[i];
	len = ft_strlen(buf);
	if (cub->map.width < len)
		cub->map.width = len;	
	map[i] = ft_strdup(buf);
	free(cub->map.map);
	cub->map.map = map;
}

void	rebuilding_map(t_cub *cub)
{
	char	*temp;
	int		i;

	i = -1;
	while (++i < cub->map.height)
	{
		temp = malloc(cub->map.width * sizeof(char));
		ft_error(!temp, "rebuilding_map() temp malloc() failed");
		ft_strlcpy(temp, cub->map.map[i], cub->map.width + 1);
		free(cub->map.map[i]);
		cub->map.map[i] = temp;
	}
}

void	valid_checking(t_cub *cub)
{
	char	**map;
	int		i;
	int		j;

	map = cub->map.map;
	i = -1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == '0' || map[i][j] == 'N')
			{
				if (i + 1 < cub->map.height && map[i + 1][j] == ' ')
					ft_error(1, "invalid map");
				if (i - 1 >= 0 && map[i - 1][j] == ' ')
					ft_error(1, "invalid map");
				if (j + 1 < cub->map.width && map[i][j + 1] == ' ')
					ft_error(1, "invalid map");
				if (j - 1 >= 0 && map[i][j - 1] == ' ')
					ft_error(1, "invalid map");
			}
			if (map[i][j] == 'N')
			{
				if (cub->player.pos.x > 0 && cub->player.pos.y > 0)
					ft_error(1, "invalid map multi player");
				cub->player.pos.x = j;
				cub->player.pos.y = i;
			}	
		}
	}
}

int	ft_parsing(t_cub *cub, char *file)
{
	char	*buf;
	int		fd;

	fd = open(file, O_RDONLY);
	ft_error(fd == -1, "parser() fd open() failed");
	while (1)
	{
		buf = get_next_line(fd);
		if (!buf)
			break ;
		buf[ft_strlen(buf) - 1] = '\0';	
		if (ft_strncmp(buf, "\0", 2))
			if (!ft_type_parsing(cub, buf))
				creating_map(cub, buf);
		free(buf);
	}
	valid_checking(cub);
	rebuilding_map(cub);
	return (1);
}

int	main(int argc, char *argv[])
{
	t_cub	*cub;

	argc++;
	cub = ft_calloc(1, sizeof(t_cub));
	ft_parsing(cub, argv[1]);
	printf("%x %x %s %s %s %s\n", cub->map.floor, cub->map.ceil, cub->map.no, cub->map.so, cub->map.we, cub->map.ea);
	printf("%d %d\n", cub->map.height, cub->map.width);
	for (int i = 0; i < cub->map.height; i++)
		printf("%s %zu\n", cub->map.map[i], ft_strlen(cub->map.map[i]));
	printf("%f %f\n", cub->player.pos.x, cub->player.pos.y);
	return (0);
}
