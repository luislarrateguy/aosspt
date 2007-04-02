/* Compara dos caracteres. Es case insensitive.
 * Devuelve 1 si ambos son iguales, y 0 en caso contrario. */
int caracteresIguales(char a, char b) {
	char lower_a = a;
	char lower_b = b;

	/* Paso el caracter 'a' a minúsculas (sólo si corresponde
	 * a una letra) */
	if (a >= 97 && a <= 122)
		lower_a = a;
	else if ( (a >= 65 && a <= 90)
			|| (a >= -63 && a <= -38) )
		lower_a = a + 0x20;

	/* Ídem con el caracter 'b' */
	if (b >= 97 && b <= 122)
		lower_b = b;
	else if ( (b >= 65 && b <= 90)
			|| (b >= -63 && b <= -38) )
		lower_b = b + 0x20;

	if (lower_a == lower_b)
		return 1;
	
	return 0;
}

/* Si el string 'sub' esta contenido en 'cad' devuelve 1,
 * y 0 en caso contrario. */
int substring(string sub, string cad) {
	int j = 0;
	int i = 0;

	while ((sub[j] != '\0') && (cad[i] != '\0')) {
		if (caracteresIguales(sub[j], cad[i])) {
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

