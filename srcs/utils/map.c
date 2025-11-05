#include "ft_ls.h"

t_map *map_get(t_map_pool *map, void *key)
{
	if (!map || !key)
		return (NULL);
	if (map->type == STR)
	{
		char *key_str = (char *) key;
		size_t key_len = ft_strlen(key_str);
		while (map)
		{
			for (int i = 0; i < map->it; i++)
			{
				if (!ft_strncmp(key,  map->pool[i].key.str, key_len + 1))
					return (&map->pool[i]);
			}
			map = map->next;
		}
	}
	else
	{
		uid_t *key_uid = (uid_t *) key;
		while (map)
		{
			for (int i = 0; i < map->it; i++)
			{
				if (*key_uid == map->pool[i].key.uid)
					return (&map->pool[i]);
			}
			map = map->next;
		}
	}
	return (NULL);
}

t_map *get_new_map(t_map_pool **head, t_map_type type)
{
	if (!*head)
	{
		t_map_pool	*new_pool = malloc(sizeof(t_map_pool));
		if (!new_pool)
			return (NULL);
		new_pool->it = 0;
		new_pool->next = NULL;
		new_pool->type = type;
		*head = new_pool;
		new_pool->pool[new_pool->it].type = type;
		return (&new_pool->pool[new_pool->it++]);
	}

	t_map_pool *tmp = *head;
	while (tmp->next)
		tmp = tmp->next;

	if (tmp->it == POOL_ITEMS_NUMBER)
	{
		t_map_pool	*new_pool = malloc(sizeof(t_map_pool));
		if (!new_pool)
			return (NULL);
		new_pool->it = 0;
		new_pool->next = NULL;
		new_pool->type = type;
		tmp->next = new_pool;
		tmp = tmp->next;
	}
	tmp->pool[tmp->it].type = type;
	return (&tmp->pool[tmp->it++]);
}

int map_set(t_map_pool **map, void *key, char *value, t_map_type type)
{
	t_map *new_map = get_new_map(map, type);
	if (!new_map)
		return (0);
	if ((*map)->type == STR)
	{
		new_map->key.str = (char *) key;
	}
	else
	{
		uid_t *uid = (uid_t *) key;
		new_map->key.uid = *uid;
	}
	new_map->value = value;
	new_map->len = ft_strlen(value);
	return (1);
}

void	map_pool_clear(t_map_pool **pool_head)
{
	t_map_pool *to_free = *pool_head;
	t_map_pool *tmp;

	while (to_free)
	{
		for (int i = 0; i < to_free->it; i++)
		{
			if (to_free->type == STR)
				free(to_free->pool[i].key.str);
			free(to_free->pool[i].value);
		}
		tmp = to_free->next;
		free(to_free);
		to_free = tmp;
	}
}

// from pool
t_map *find_extension(t_map_pool *map, char *name)
{
	if (!map || map->type != STR)
		return (NULL);

	size_t	name_len = ft_strlen(name);
	while (map)
	{
		for (int i = 0; i < map->it; i++)
		{
			int k = name_len;
			for (int j = ft_strlen(map->pool[i].key.str); j >= 0; j--)
			{
				if (map->pool[i].key.str[j] != name[k--])
					break;
				if (j == 1)
					return (&map->pool[i]);
			}
		}
		map = map->next;
	}
	return NULL;
}