#include "concesionario.h"
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct coche *c;
	struct concesionario *con;
	char matricula[8] = "";
	char marca[21] = "";
	char dueno[21] = "";
	uint32_t id;
	uint16_t flags = 0;
	char info[4000];
	int val, option_index = 0;
	static struct option long_options[] = {
		{ "matricula", required_argument, 0, 'm' },
		{ "id", required_argument, 0, 'i' },
		{ "gama", required_argument, 0, 'g' },
		{ "dueno", required_argument, 0, 'd' },
		{ "help", no_argument, 0, 'h' },
		{ 0 }
	};
	do {
		val = getopt_long(argc, argv, "m:i:g:d:h:",
				 long_options, &option_index);
		if(val == -1)
			break;
		
		switch(val) {
		case 'm':
			if (strlen(optarg) > 7) {
				printf("Matricula(-m) demasiado larga");
				break;
			}
			strcpy(matricula, optarg);
			flags |= 1;
			break;
		case 'i':
			errno = 0;
			char *end;
			uint64_t tmp = strtol(optarg, &end, 10);
			if ((errno == ERANGE && tmp == LONG_MAX)
				 || (errno == ERANGE && tmp == LONG_MIN)
				 || tmp > UINT32_MAX || tmp < 0) {
				printf("Id(-i) fuera del rango valido\n");
				break;
			} else if (optarg == '\0' || *end != '\0') {
				printf("La id(-i) introducida "\
					"no es un numero\n");
				break;
			}
			id = tmp;
			flags |= 1 << 1;
			break;
		case 'g':
			if (strlen(optarg) > 20) {
				printf("Marca(-g) demasiado larga");
				break;
			}
			strcpy(marca, optarg);
			flags |= 1 << 2;
			break;
		case 'd':
			if (strlen(optarg) > 20) {
				printf("Dueno(-d) demasiado largo");
				break;
			}
			strcpy(dueno, optarg);
			flags |= 1 << 3;
			break;
		case 'h':
			printf("Las  opciones validas son:\n" \
				" -m, --matricula\n" \
				" -i, --id\n" \
				" -g, --gama\n" \
				" -d, --dueno\n" \
				"-h, --help\n");
			break;
		default:
			printf("Las  opciones validas son:\n" \
				" -m, --matricula\n" \
				" -i, --id\n" \
				" -g, --gama\n" \
				" -d, --dueno\n" \
				"-h, --help\n");
			break;
		}
	} while(val != -1);
	
	if (flags != 15) {
		printf("Faltan campos por rellenar\n");
		return -1;
	}

	c = curso_coche_alloc();

	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MATRICULA, matricula);
	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MARCA, marca);
	curso_coche_attr_set_u32(c, CURSO_COCHE_ATTR_ID, id);

	con = curso_concesionario_alloc();

	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
					 dueno);

	curso_concesionario_attr_set_coche(con, CURSO_CONCESIONARIO_ATTR_COCHE,
					   c);

	curso_concesionario_snprintf(info, sizeof(info), con);
	printf("%s", info);

	curso_concesionario_free(con);
	return 0;
}
