#include "gestor_tarea.h"

struct gestor_tarea{
	struct list_head tareas;
	uint16_t num_tareas;

	uint16_t flags;
};

static void add_tarea(struct gestor_tarea *g, struct tarea *t)
{
	int i = 0;
	int pos = g->num_tareas;
	int prio = tarea_attr_get_u32(t, TAREA_ATTR_PRIORIDAD);
	struct tarea *tmp[10];

	struct tarea *cur, *next;
	list_for_each_entry_safe(cur, next, &g->tareas, head){
		if (tarea_attr_get_u32(t, TAREA_ATTR_PRIORIDAD) <= prio) {
			pos = i;
		}
		tmp[i] = cur;
		i++;
	}
	if (pos == g->num_tareas) {
		list_add(&t->head, &g->tareas);
		return;
	}
	for(i = 0; i < g->num_tareas; i++){
		list_del(&tmp[i]->head);
	}
	for(i = 0; i < g->num_tareas; i++){
		if(i == pos)
			list_add(&t->head, &g->tareas);
		list_add(&tmp[i]->head, &g->tareas);
	}
	printf("test1");
}

struct gestor_tarea *gestor_tarea_alloc(void)
{
	struct gestor_tarea *g =
		(struct gestor_tarea *) calloc(1, sizeof(struct gestor_tarea));
	INIT_LIST_HEAD(&g->tareas);
	return g;
}

void gestor_tarea_free(struct gestor_tarea *g)
{
	struct tarea *t, *next;
	list_for_each_entry_safe(t, next, &g->tareas, head) {
		list_del(&t->head);
	}
	xfree(g);
}


bool gestor_tarea_attr_is_set(const struct gestor_tarea *g, uint16_t attr)
{
	return g->flags & (1 << attr);
}

void gestor_tarea_attr_unset_tarea(struct gestor_tarea *g, uint16_t n)
{
	struct tarea *t, *next;
	int i = 0;

	if (!(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)) || g->num_tareas <= n)
		return;
	list_for_each_entry_safe(t, next, &g->tareas, head){
		if (i == n) {
			list_del(&t->head);
			break;
		}
		i++;
	}
	g->num_tareas--;
	if (g->num_tareas == 0)
		g->flags = 0;
}

void gestor_tarea_attr_set_tarea(struct gestor_tarea *g, struct tarea *t)
{
	if (t == NULL)
		return;

	if (!(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS))) {
		g->flags = 3; //set num_tareas y tareas
		g->num_tareas = 0;
	}

	if (g->num_tareas >= 10)
		gestor_tarea_attr_unset_tarea(g, 9);
	add_tarea(g, t);
	g->num_tareas++;
}

const struct tarea *gestor_tarea_attr_get_tarea(struct gestor_tarea *g, uint16_t num)
{
	if (num >= g->num_tareas || !(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)))
		return NULL;
	struct tarea *t, *tmp;
	int i = 0;
	list_for_each_entry_safe(t, tmp, &g->tareas, head){
		if (i == num)
			break;
		i++;
	}
	return t;
}

const uint16_t gestor_tarea_attr_get_u16(struct gestor_tarea *g, uint16_t attr)
{
	if (!(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)) 
		 || attr != GESTOR_TAREA_ATTR_NUM_TAREAS)
		return 0;
	return g->num_tareas;
}

int gestor_tarea_snprintf(char *buf, size_t size, struct gestor_tarea *g, int n)
{
	int res = snprintf(buf, size, "Tarea numero: %d\n", n);
	if (n >= g->num_tareas || !(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)))
		return 0;
	int i = 0;
	struct tarea *t, *tmp;
	list_for_each_entry_safe(t, tmp, &g->tareas, head){
		if (i == n) {
			res += tarea_snprintf(buf + res, size - res, t);
			break;
		}
		i++;
	}
	return res;
}
