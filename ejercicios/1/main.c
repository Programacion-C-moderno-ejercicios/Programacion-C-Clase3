#include "gestor_tarea.h"
#include <assert.h>

int main(void)
{
	struct tarea *t1;
	struct tarea *t2;
	struct tarea *t3;
	struct gestor_tarea *g;
	char info[1000];


	t1= tarea_alloc();
	t2= tarea_alloc();
	t3= tarea_alloc();
	g = gestor_tarea_alloc();

	tarea_attr_set_str(t1, TAREA_ATTR_NOMBRE, "Tarea 1");
	tarea_attr_set_str(t1, TAREA_ATTR_DESCRIPCION, "Descripcion tarea 1");
	tarea_attr_set_u32(t1, TAREA_ATTR_ID, 1);
	tarea_attr_set_str(t1, TAREA_ATTR_USUARIO, "Pablo");
	tarea_attr_set_u32(t1, TAREA_ATTR_PRIORIDAD, 2);

	tarea_attr_set_str(t2, TAREA_ATTR_NOMBRE, "Tarea 2");
	tarea_attr_set_str(t2, TAREA_ATTR_DESCRIPCION, "Descripcion tarea 2");
	tarea_attr_set_u32(t2, TAREA_ATTR_ID, 2);
	tarea_attr_set_str(t2, TAREA_ATTR_USUARIO, "Pablo");
	tarea_attr_set_u32(t2, TAREA_ATTR_PRIORIDAD, 0);

	tarea_attr_set_str(t3, TAREA_ATTR_NOMBRE, "Tarea 3");
	tarea_attr_set_str(t3, TAREA_ATTR_DESCRIPCION, "Descripcion tarea 3");
	tarea_attr_set_u32(t3, TAREA_ATTR_ID, 3);
	tarea_attr_set_str(t3, TAREA_ATTR_USUARIO, "Pablo");
	tarea_attr_set_u32(t3, TAREA_ATTR_PRIORIDAD, 1);

	gestor_tarea_attr_set_tarea(g, t1);
	gestor_tarea_attr_set_tarea(g, t2);
	gestor_tarea_attr_set_tarea(g, t3);

	int tmp = gestor_tarea_snprintf(info, 1000, g, 0);
	tmp += gestor_tarea_snprintf(info + tmp, 1000 - tmp, g, 1);
	gestor_tarea_snprintf(info + tmp, 1000-tmp, g, 2);
	printf("%s", info);

	gestor_tarea_free(g);
	tarea_free(t1);
	tarea_free(t2);
	tarea_free(t3);

	return 0;
}
