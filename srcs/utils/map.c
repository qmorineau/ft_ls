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

static void map_uid_addfront(t_map_uid **head, t_map_uid *new)
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

int map_set_uid(t_map_uid **map, uid_t id, char **value)
{
	t_map_uid *node = map_get_id(*map, id);
	if (!node)
	{
		node = ft_calloc(1, sizeof(t_map_uid));
		if (!node)
			return (0);
		node->key = id;
		node->value = *value;
		node->len = ft_strlen(node->value);
		map_uid_addfront(map, node);
	}
	else
	{
		free(node->value);
		node->key = id;
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
		if (strncmp(key, tmp->key, ft_strlen(key) + 1) == 0)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

t_map_uid *map_get_id(t_map_uid *map, uid_t id)
{
	t_map_uid *tmp;

	tmp = map;
	while (tmp)
	{
		// printf("%d == %d\n", id, tmp->key);
		if (id == tmp->key)
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
		int i = ft_strlen(name);

		for (int j = ft_strlen(tmp->key); j >= 0; j--)
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