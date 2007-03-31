/* Devuelve 1 si la 'sub' es subcadena de 'cadena',
   en caso contrario retorna 0 */
/* int substring(string sub, string cadena) {
	int i = 0;
	int j = 0;

	while (sub[j] != '\0' && cadena[i] != '\0') {
		if (sub[j] == cadena[i]) {
			i++;
			j++;			
		} else {
			i++;
			j = 0;
		}
	}
	
	if (sub[j] == '\0')
		return 1;
	return 0;
} */
int substring(string sub, string cad) {
	int j = 0;
	int i = 0;

	while ((sub[j] != '\0') && (cad[i] != '\0')) {
		if ((sub[j] == cad[i]) ||
			(sub[j] == (cad[i] + 0x20)) ||
			((sub[j] + 0x20) == (cad[i]))) {
			i++;
			j++;
		} else {
			i++;
			j = 0;
		}
	}
	
	if (sub[j] == '\0')
		return 1;
	return 0;
}
string getNombre (string linea) {
	string resultado;
	
	if (linea == NULL)
		return "";

	strtok(linea, ";");
	resultado = strtok(NULL, "\0");
	
	if (resultado != NULL)
		return resultado;
	return "";
}

