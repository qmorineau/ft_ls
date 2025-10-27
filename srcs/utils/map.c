#include "ft_ls.h"

static void map_addfront(t_map **head, t_map *new)
{
	if (!head || !*head)
		(*head) = new;
	else
	{
		new->next = (*head);
		(*head) = new;
	}
}

int map_set(t_map **map, char **key, char **value)
{
	t_map *node = map_get(*map, *key);
	if (!node)
	{
		node = ft_calloc(1, sizeof(t_map));
		if (!node)
			return (0);
		node->key = *key;
		node->value = *value;
		map_addfront(map, node);
	}
	else
	{
		free(node->value);
		free(node->key);
		node->key = *key;
		node->value = *value;
	}
	return (1);
}

t_map *map_get(t_map *map, char *key)
{
	t_map *tmp;

	tmp = map;
	while (tmp)
	{
		if (strncmp(key, tmp->key, strlen(key) + 1) == 0)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

t_map *find_extension(t_map *map, char *name)
{
	t_map *tmp;

	tmp = map;
	while (tmp)
	{
		int i = strlen(name);

		for (int j = strlen(tmp->key); j >= 0; j--)
		{
			if (tmp->key[j] != name[i--])
				break;
			if (j == 1)
				return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}