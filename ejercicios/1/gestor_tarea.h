#include "tarea.h"
#include "list.h"

enum {
	GESTOR_TAREA_ATTR_TAREAS,
	GESTOR_TAREA_ATTR_NUM_TAREAS,
	__GESTOR_TAREA_ATTR_MAX
};

#define GESTOR_TAREA_ATTR_MAX (__GESTOR_TAREA_ATTR_MAX -1)
#define GESTOR_TAREA_MAX_TAREAS 10

#define xfree(ptr) free((void *)ptr);

struct gestor_tarea;

struct gestor_tarea *gestor_tarea_alloc(void);
void gestor_tarea_free(struct gestor_tarea *g);

bool gestor_tarea_attr_is_set(const struct gestor_tarea *g, uint16_t attr);

void gestor_tarea_attr_unset_tarea(struct gestor_tarea *g, uint16_t n);

void gestor_tarea_attr_set_tarea(struct gestor_tarea *g, struct tarea *t);

const struct tarea *gestor_tarea_attr_get_tarea(struct gestor_tarea *t, uint16_t num);
const uint16_t gestor_tarea_attr_get_u16(struct gestor_tarea *g, uint16_t attr);

int gestor_tarea_snprintf(char *buf, size_t size, struct gestor_tarea *g, int n);
