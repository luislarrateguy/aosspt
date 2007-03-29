/* Devuelve 1 si la 'sub' es subcadena de 'cadena',
   en caso contrario retorna 0 */
int substring(string sub, string cadena) {
	int i = 0;
	int j = 0;

	while (sub[j] != '\0' && cadena[i] != '\0') {
		if (sub[j] == cadena[i]) {
			i++;
			j++;			
		}
		else {
			i++;
			j = 0;
		}
	}
	
	if (sub[j] == '\0')
		return 1;
	return 0;
}

/* Retorna la cadena recibida como parametro en
   mayusculas. Utilizada para hacer las consultas
   case insensitive */
/* FIXME: No funciona con letra acentuadas
   FIXME: En ciertas situaciones aun no identificadas tira
   fallo de segmentacion, por esta razón aun no se usa. */
string uppercase (string s) {
	int i = 0;
	string resultado;

	while ((s[i] != '\0') && (s[i] != '\n')) {
		if (s[i] >= 'a' && s[i] <= 'z')
			resultado[i] = s[i] - ('a'-'A');
		else
			resultado[i] = s[i];
		printf("%c", resultado[i]);
		i++;
	}
	resultado[i] = '\0';
	
	return resultado;
}

