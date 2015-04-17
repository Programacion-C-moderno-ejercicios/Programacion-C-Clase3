#include "concesionario.h"
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

int valid_char(int ch)
{
	return ch > 0 && ch != '\n' && ch != ',';
}

int next_attr(FILE *fp, char *attr)
{
	int ch = fgetc(fp);
	int i = 0;
	for (i = 0; valid_char(ch) && i < 21; i++) {
		attr[i] = (char) ch;
		ch = fgetc(fp);
	}
	if (i == 21)
		return -1;
	attr[i] = '\0';
	return ferror(fp) ? -1 : 0;
}

int lee_fichero(char *path, struct concesionario *con)
{
	char dueno[21] = "";
	char matricula[21] = "";
	char marca[21] = "";
	char id[21] = "";
	uint16_t flags = 0;
	int num_coches = 0;
	int br = 0;

	FILE *fp = fopen(path, "r");
	if (fp == NULL)
		return -1;
	br = next_attr(fp, dueno);
	if (br) {
		curso_concesionario_free(con);
		return -2;
	}
	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
					dueno);
	for (num_coches = 0; num_coches < 10; num_coches++) {
		br = br ? br : next_attr(fp, id);
		br = br ? br : next_attr(fp, matricula);
		br = br ? br : next_attr(fp, marca);
		if (br) {
			curso_concesionario_free(con);
			fclose(fp);
			return -1;
		}
		if (feof(fp) && strlen(marca) == 0)
			break;
		struct coche *coche = curso_coche_alloc();
		curso_coche_attr_set_u32(coche, CURSO_COCHE_ATTR_ID, atoi(id));
		curso_coche_attr_set_str(coche, CURSO_COCHE_ATTR_MATRICULA,
					 matricula);
		curso_coche_attr_set_str(coche, CURSO_COCHE_ATTR_MARCA,
					 marca);
		curso_concesionario_attr_set_coche(con,
					CURSO_CONCESIONARIO_ATTR_COCHE,
					coche);
	}

	return fclose(fp) ? -3 : 0;
}

int escribe_fichero(char *path, struct concesionario *con)
{
	char dueno[22] = "";
	char matricula[22] = "";
	char marca[22] = "";
	char id[22] = "";
	int i = 0;
	struct coche *c;
	int num_coches = curso_concesionario_attr_get_u32(con,
					CURSO_CONCESIONARIO_ATTR_NUM_COCHES);

	FILE *fp = fopen(path, "w");
	if (fp == NULL)
		return -1;

	snprintf(dueno, 22, "%s\n", curso_concesionario_attr_get_str(con,
					CURSO_CONCESIONARIO_ATTR_DUENO));
	fputs(dueno, fp);
	for (i = 0; i < num_coches; i++) {
		c = curso_concesionario_attr_get_coche(con,
					CURSO_CONCESIONARIO_ATTR_COCHE, i);
		snprintf(id, 22, "%d,",
			curso_coche_attr_get_u32(c, CURSO_COCHE_ATTR_ID));
		fputs(id, fp);
		snprintf(matricula, 22, "%s,",
			curso_coche_attr_get_str(c,
						CURSO_COCHE_ATTR_MATRICULA));
		fputs(matricula, fp);
		snprintf(marca, 22, "%s\n",
			curso_coche_attr_get_str(c, CURSO_COCHE_ATTR_MARCA));
		fputs(marca, fp);
	}
	return fclose(fp) ? -2 : 0;
}

int main(int argc, char *argv[])
{
	struct concesionario *con;
	char info[4000];
	char *path;

	int tmp, option_index = 0;
	static struct option long_options[] = {
		{"file", required_argument, 0, 'f'},
		{0}
	};

	tmp = getopt_long(argc, argv, "f:", long_options, &option_index);
	switch (tmp) {
	case 'f':
		path = calloc(strlen(optarg) + 1, sizeof(char));
		strcpy(path, optarg);
		break;
	default:
		printf("Es necesario indicar la ruta del fichero: -f, "\
			"--file path\n");
		return -1;
	}
	con = curso_concesionario_alloc();

	tmp = lee_fichero(path, con);
	if (tmp == -1) {
		printf("No se puede leer %s\n", path);
		free(path);
		return -1;
	} else if (tmp == -2) {
		printf("El archivo %s no es valido\n", path);
		free(path);
		return -2;
	}

	curso_concesionario_snprintf(info, sizeof(info), con);
	printf("%s", info);

	curso_concesionario_free(con);
	free(path);
	return 0;
}
