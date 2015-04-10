#include "tarea.h"
#include <string.h>


struct tarea *tarea_alloc(void)
{
	return (struct tarea *)calloc(1, sizeof(struct tarea));
}

void tarea_free(struct tarea *t)
{
	if (t->flags & (1 << TAREA_ATTR_NOMBRE))
		xfree(t->nombre_tarea);
	if (t->flags & (1 << TAREA_ATTR_DESCRIPCION))
		xfree(t->desc_tarea);
	if (t->flags & (1 << TAREA_ATTR_USUARIO))
		xfree(t->usuario);

	xfree(t);
}

bool tarea_attr_is_set(const struct tarea *t, uint16_t attr)
{
	return t->flags & (1 << attr);
}

void tarea_attr_unset(struct tarea *t, uint16_t attr)
{
	if (!(t->flags & (1 << attr)))
		return;

	switch (attr) {
	case TAREA_ATTR_NOMBRE:
		if (t->nombre_tarea) {
			xfree(t->nombre_tarea);
			t->nombre_tarea = NULL;
		}
		break;
	case TAREA_ATTR_DESCRIPCION:
		if(t->desc_tarea){
			xfree(t->desc_tarea);
			t->desc_tarea = NULL;
		}
		break;
	case TAREA_ATTR_ID:
		break;
	case TAREA_ATTR_USUARIO:
		if(t->usuario) {
			xfree(t->usuario);
			t->usuario = NULL;
		}
		break;
	case TAREA_ATTR_PRIORIDAD:
		break;
	}

	t->flags &= ~(1 << attr);
}

void tarea_attr_set_data(struct tarea *t, uint16_t attr, const void *data,
			 uint32_t data_len)
{
	if (attr > TAREA_ATTR_MAX)
		return;

	switch (attr) {
	case TAREA_ATTR_NOMBRE:
		if (t->nombre_tarea != NULL)
			xfree(t->nombre_tarea);

		t->nombre_tarea = strdup(data);
		break;
	case TAREA_ATTR_DESCRIPCION:
		if (t->desc_tarea != NULL)
			xfree(t->desc_tarea);

		t->desc_tarea = strdup(data);
		break;
	case TAREA_ATTR_ID:
		t->id = *((uint32_t *) data);
		break;
	case TAREA_ATTR_USUARIO:
		if (t->usuario != NULL)
			xfree(t->usuario);
		t->usuario = strdup(data);
		break;
	case TAREA_ATTR_PRIORIDAD:
		t->prioridad = *((uint32_t *) data);
		break;
	}

	t->flags |= (1 << attr);
}

void tarea_attr_set_u32(struct tarea *t, uint16_t attr, uint32_t data)
{
	tarea_attr_set_data(t, attr, &data, sizeof(uint32_t));
}

void tarea_attr_set_str(struct tarea *t, uint16_t attr, const char *data)
{
	tarea_attr_set_data(t, attr, data, 0);
}

const void *tarea_attr_get_data(const struct tarea *t, uint16_t attr)
{
	if(!(t->flags & (1 << attr)))
		return NULL;

	switch (attr) {
	case TAREA_ATTR_NOMBRE:
		return t->nombre_tarea;
	case TAREA_ATTR_DESCRIPCION:
		return t->desc_tarea;
	case TAREA_ATTR_ID:
		return &t->id;
	case TAREA_ATTR_USUARIO:
		return t->usuario;
	case TAREA_ATTR_PRIORIDAD:
		return &t->prioridad;
	}

	return NULL;
}


uint32_t tarea_attr_get_u32(const struct tarea *t, uint16_t attr)
{
	const void * ret = tarea_attr_get_data(t, attr);
	return ret == NULL ? 0 : *((uint32_t *)ret);
}

const char *tarea_attr_get_str(const struct tarea *t, uint16_t attr)
{
	return tarea_attr_get_data(t, attr);
}
int tarea_snprintf(char *buf, size_t size, struct tarea *t)
{
	return snprintf(buf, size, "La tarea número %d está enviada por %s.\n"
			"Se llama %s y la descripción es %s\n",
			t->id, t->usuario, t->nombre_tarea, t->desc_tarea);
}
