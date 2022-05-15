#include "main.h"

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que verifica si la lista esta vacia
int isEmpty() {
   return headL == NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que retorna la cantidad de elementos en la lista
int lengthList() {
   int length = 0;
   struct node *current;
	
   for(current = headL; current != NULL; current = current->next){
      length++;
    }
   return length;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que inserta un elemento en la primera posicion de la lista
void insertFirst(int key, pthread_t thread, pid_t pid) {

    struct node *link = (struct node*) malloc(sizeof(struct node));    // crea el link
    link->key = key;
    link->thread = thread;
    link->pid = pid;
	
    if(isEmpty()) {
        last = link; // conviértelo en el último link
    } else {
        headL->prev = link;
    }

    link->next = headL; // apuntar al antiguo primer link
    headL = link;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que elimina un elemento de la lista dada una llave
struct node* delete(int key) {
   struct node* current = headL;
   struct node* previous = NULL;
	
   if(headL == NULL) { // si la lista esta vacia
      return NULL;
   }

   while(current->key != key) { //navegar sobre la lista		
      if(current->next == NULL) {
         return NULL;
      } else {
         previous = current;
         current = current->next;             
      }
   }

   if(current == headL) { // Si encuentra la cabeza cambia el primer puntero al siguiente
      headL = headL->next;
   } else {
      current->prev->next = current->next;
   }    

   if(current == last) {
      last = current->prev;
   } else {
      current->next->prev = current->prev;
   }
	
   return current;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que elimina el primer elemento de la lista
struct node* deleteFirst() {
   struct node *tempLink = headL;
	
   if(headL->next == NULL){
      last = NULL;
   } else {
      headL->next->prev = NULL;
   }
	
   headL = headL->next;
   return tempLink;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que verfica con patrones regex si un string hace match
regmatch_t *match(char* pattern, char* str)
{
    regex_t compiled;
    regcomp(&compiled, pattern, REG_EXTENDED);

    size_t ngroups = compiled.re_nsub + 1;
    regmatch_t *groups = malloc(ngroups * sizeof(regmatch_t));
    int value = regexec(&compiled, str, ngroups, groups, 0);
    
    if (value == REG_NOMATCH){
        return NULL;
    }
    else {
        return groups;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que convierte una ip valida a binario con mascaras de bits
unsigned int ipAdressStringToInt(char * ip){
 
    // cada char es de un byte
    unsigned char bytes[4] = {0};
    
    sscanf(ip, "%hhd.%hhd.%hhd.%hhd", &bytes[3], &bytes[2], &bytes[1], &bytes[0]);
    return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;

}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que convierte un valor de mascara valido a binario con mascaras de bits
unsigned int cidrToMask(unsigned int cidrValue){
    return -1 ^ ((1 << (32 - cidrValue)) - 1);
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que calcula el network address de una ip y una mascara
unsigned int calcNetworkAddress(unsigned int ipaddress, unsigned int netmask){
    return ipaddress & netmask;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que calcula la direccion broadcast de una ip y una mascara
unsigned int calcBroadcast(unsigned int ip, unsigned int netmask){
    unsigned int network = calcNetworkAddress(ip, netmask);
    return network + (~netmask);
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que convierte un int con mascara de bits a un puntero con la direccion ip
char* convertIPBitmask(unsigned int ip){
    unsigned char bytes[4];
    
    bytes[0] = ip;
    bytes[1] = ip >> 8;
    bytes[2] = ip >> 16;
    bytes[3] = ip >> 24;
    
    char *buffer = malloc(16*sizeof(char));
    sprintf(buffer, "%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);
    
    return buffer;

}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que calcula el rango de una ip
char* calcHostRange(unsigned int network, unsigned int broadcast){
    unsigned char bytes[4];
    unsigned char byte_b;
    
    bytes[0] = network;
    bytes[1] = network >> 8;
    bytes[2] = network >> 16;
    bytes[3] = network >> 24;

    byte_b = broadcast;

    char *buffer = malloc(23*sizeof(char));
    sprintf(buffer, "%d.%d.%d.{%d-%d}\n", bytes[3], bytes[2], bytes[1], bytes[0]+1, byte_b-1);

    return buffer;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que obtiene los grupos de regex en base a un indice
char *getSubRegexGroup(char* str, regmatch_t *groups, size_t group)
{
    char *cursor = str;
    if (groups[group].rm_so == (size_t)(-1)) {
        return NULL;
    }
    char cursorCopy[strlen(cursor) + 1];
    strcpy(cursorCopy, cursor);
    cursorCopy[groups[group].rm_eo] = 0;

    char *buffer = malloc(16*sizeof(char));
    sprintf(buffer, "%s", cursorCopy + groups[group].rm_so);
    return buffer;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que procesa cada request/comando enviado por telnet
// las operaciones bitwise se obtuvieron de: https://neverthenetwork.com/notes/bitwise_subnetting
char *processRequest(char* request)
{
    char *broadcastCidrRegex = "^GET BROADCAST IP (([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)) MASK \\/([0-9]+)";
    char *broadcastMaskRegex = "^GET BROADCAST IP (([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)) MASK ([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)";
    char *networkCidrRegex = "^GET NETWORK NUMBER IP (([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)) MASK \\/([0-9]+)";
    char *networkMaskRegex = "^GET NETWORK NUMBER IP (([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)) MASK ([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)";
    char *rangeCidrRegex = "^GET HOSTS RANGE IP (([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)) MASK \\/([0-9]+)";
    char *rangeMaskRegex = "^GET HOSTS RANGE IP (([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)) MASK ([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)";
    struct in6_addr addr;

    // casos de comandos posibles de enviar
    if (match(broadcastCidrRegex, request) != NULL){ // GET BROADCAST IP 10.8.2.5 MASK /29
        regmatch_t *groups = match(broadcastCidrRegex, request);
        char *ipAddress = getSubRegexGroup(request, groups, (size_t)(1));
        char *netmask = getSubRegexGroup(request, groups, (size_t)(6));

        int valid = inet_pton(AF_INET, ipAddress, &addr);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid IP address.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        unsigned int ipBitmask = ipAdressStringToInt(ipAddress);
        
        unsigned char bytes[1] = {0};
    
        sscanf(netmask, "%hhd", &bytes[0]);

        if ((unsigned int)bytes[0] < 0 || (unsigned int)bytes[0] > 32){ // verifica si la mascara esta entre 0 a 32
            char errorMsgTmp[] = "Invalid mask.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }
        
        unsigned int netmaskBitmask = cidrToMask((unsigned int)bytes[0]);
        unsigned int ipBroadcastBitmask = calcBroadcast(ipBitmask, netmaskBitmask);
        char *broadcastIP = convertIPBitmask(ipBroadcastBitmask);

        free(groups);
        free(ipAddress);
        free(netmask);

        return broadcastIP;
    }
    else if (match(broadcastMaskRegex, request) != NULL){ // GET BROADCAST IP 172.16.0.56 MASK 255.255.255.128
        regmatch_t *groups = match(broadcastMaskRegex, request);
        char *ipAddress = getSubRegexGroup(request, groups, (size_t)(1));
        char *netmask = getSubRegexGroup(request, groups, (size_t)(6));

        int valid = inet_pton(AF_INET, ipAddress, &addr);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid IP address.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        struct in6_addr addr2;
        valid = inet_pton(AF_INET, netmask, &addr2);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid MASK.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        unsigned int ipBitmask = ipAdressStringToInt(ipAddress);
        unsigned int netmaskBitmask = ipAdressStringToInt(netmask);
        unsigned int ipBroadcastBitmask = calcBroadcast(ipBitmask, netmaskBitmask);
        char *broadcastIP = convertIPBitmask(ipBroadcastBitmask);

        free(groups);
        free(ipAddress);
        free(netmask);

        return broadcastIP;
    }
    else if (match(networkCidrRegex, request) != NULL){ // GET NETWORK NUMBER IP 10.8.2.5 MASK /29
        regmatch_t *groups = match(networkCidrRegex, request);
        char *ipAddress = getSubRegexGroup(request, groups, (size_t)(1));
        char *netmask = getSubRegexGroup(request, groups, (size_t)(6));
        
        int valid = inet_pton(AF_INET, ipAddress, &addr);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid IP address.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        unsigned int ipBitmask = ipAdressStringToInt(ipAddress);
        
        unsigned char bytes[1] = {0};
    
        sscanf(netmask, "%hhd", &bytes[0]);

        if ((unsigned int)bytes[0] < 0 || (unsigned int)bytes[0] > 32){ // verifica si la mascara esta entre 0 a 32
            char errorMsgTmp[] = "Invalid mask.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        unsigned int netmaskBitmask = cidrToMask((unsigned int)bytes[0]);
        unsigned int ipNetworkBitmask = calcNetworkAddress(ipBitmask, netmaskBitmask);
        char *networkIP = convertIPBitmask(ipNetworkBitmask);

        free(groups);
        free(ipAddress);
        free(netmask);

        return networkIP;
    }
    else if (match(networkMaskRegex, request) != NULL){ // GET NETWORK NUMBER IP 172.16.0.56 MASK 255.255.255.128 
        regmatch_t *groups = match(networkMaskRegex, request);
        char *ipAddress = getSubRegexGroup(request, groups, (size_t)(1));
        char *netmask = getSubRegexGroup(request, groups, (size_t)(6));
        
        int valid = inet_pton(AF_INET, ipAddress, &addr);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid IP address.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        struct in6_addr addr2;
        valid = inet_pton(AF_INET, netmask, &addr2);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid MASK.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        unsigned int ipBitmask = ipAdressStringToInt(ipAddress);
        unsigned int netmaskBitmask = ipAdressStringToInt(netmask);
        unsigned int ipNetworkBitmask = calcNetworkAddress(ipBitmask, netmaskBitmask);
        char *networkIP = convertIPBitmask(ipNetworkBitmask);

        free(groups);
        free(ipAddress);
        free(netmask);

        return networkIP;
    }
    else if (match(rangeCidrRegex, request) != NULL){ // GET HOSTS RANGE IP 10.8.2.5 MASK /29
        regmatch_t *groups = match(rangeCidrRegex, request);
        char *ipAddress = getSubRegexGroup(request, groups, (size_t)(1));
        char *netmask = getSubRegexGroup(request, groups, (size_t)(6));

        int valid = inet_pton(AF_INET, ipAddress, &addr);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid IP address.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        unsigned int ipBitmask = ipAdressStringToInt(ipAddress);
        
        unsigned char bytes[1] = {0};
    
        sscanf(netmask, "%hhd", &bytes[0]);

        if ((unsigned int)bytes[0] < 0 || (unsigned int)bytes[0] > 32){ // verifica si la mascara esta entre 0 a 32
            char errorMsgTmp[] = "Invalid mask.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }
        
        unsigned int netmaskBitmask = cidrToMask((unsigned int)bytes[0]);
        unsigned int ipBroadcastBitmask = calcBroadcast(ipBitmask, netmaskBitmask);
        unsigned int ipNetworkBitmask = calcNetworkAddress(ipBitmask, netmaskBitmask);

        char *rangeIP = calcHostRange(ipNetworkBitmask, ipBroadcastBitmask);

        free(groups);
        free(ipAddress);
        free(netmask);

        return rangeIP;
    }
    else if (match(rangeMaskRegex, request) != NULL){ // GET HOSTS RANGE IP 172.16.0.56 MASK 255.255.255.128
        regmatch_t *groups = match(rangeMaskRegex, request);
        char *ipAddress = getSubRegexGroup(request, groups, (size_t)(1));
        char *netmask = getSubRegexGroup(request, groups, (size_t)(6));

        int valid = inet_pton(AF_INET, ipAddress, &addr);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid IP address.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        struct in6_addr addr2;
        valid = inet_pton(AF_INET, netmask, &addr2);
        if (!valid) { // verifica si la ip es valida
            char errorMsgTmp[] = "Invalid MASK.\n";
            char *errorMsg = malloc(sizeof (char)*strlen(errorMsgTmp));
            strcpy(errorMsg, errorMsgTmp);
            return errorMsg;
        }

        unsigned int ipBitmask = ipAdressStringToInt(ipAddress);
        unsigned int netmaskBitmask = ipAdressStringToInt(netmask);
        unsigned int ipBroadcastBitmask = calcBroadcast(ipBitmask, netmaskBitmask);
        unsigned int ipNetworkBitmask = calcNetworkAddress(ipBitmask, netmaskBitmask);

        char *rangeIP = calcHostRange(ipNetworkBitmask, ipBroadcastBitmask);

        free(groups);
        free(ipAddress);
        free(netmask);

        return rangeIP;
    }
    else { // si no calza el comando con ningun caso anterior es porque el comando no es reconocido
        char temp_error_message[] = "Invalid command.\n";
        char *errorMsg = malloc (sizeof (char) * strlen(temp_error_message));
        strcpy(errorMsg, temp_error_message);
        return errorMsg;
    }
}

void *threaded(void *args)
{
    char buffer[BUFFER_SIZE];
    int socket = *((int*)args);
    free(args);
    int killConnection = 1;

    while(killConnection){ // si se recibe un mensaje de kill para la conexion
        read(socket, buffer, BUFFER_SIZE);

        char *killRegex = "^exit";
        if (match(killRegex, buffer) != NULL){ // compara la entrada
            killConnection = 0;
        } else {
            char *result = processRequest(buffer); // procesa el comando
            if (send(socket, result, strlen(result), 0) < 0) // le responde al cliente
            {
                printf("Failed to write client\n");
            }
            free(result);
        }
        strcpy(buffer,"");

    }
    printf("Connection closed: %d\n", socket);

    close(socket); // cierra conexion con el cliente
    pthread_mutex_lock(&listMutex);
    free(delete(pthread_self())); // elimina el hilo de la lista de hilos pendientes por matar
    pthread_mutex_unlock(&listMutex);
    pthread_exit(0); // cierra el hilo
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que realiza el launch del servidor y elegir el tipo de servidor
void launch(Server *server)
{
    while(killFlag){
        printf("===== WAITING FOR CONNECTION =====\n");

        int address_length = sizeof(server->address);

        int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);

        pthread_t t; //creacion de un hilo para cada request
        int *socket = malloc(sizeof(int)); //se asigna un socket
        *socket = new_socket;
        pthread_create(&t,NULL,threaded,socket); //se ejecuta el request del hilo al servidor
        
        pid_t test;
        pthread_mutex_lock(&listMutex);
        insertFirst(t, t, test); //se guarda el id del hilo en la lista enlazada
        pthread_mutex_unlock(&listMutex);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que encuentra la IP de la interfaz e inicia el servidor
void *serverFunc(void *args) {
    uint32_t interfaces = htonl(INADDR_ANY);
    Server server = server_constructor(AF_INET,SOCK_STREAM, 0, interfaces, PORT, 5000, launch);
    server.launch(&server);
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion que mata todos los hilos en caso de que haya que un mensaje de kill
void killThreads(){
    int lengthOfList = lengthList();
    if (lengthOfList > 0)
    {
        for(int i = 0; i < lengthOfList; i++){
            pthread_mutex_lock(&listMutex);
            node *threadToDelete = deleteFirst();
            pthread_mutex_unlock(&listMutex);
            pthread_cancel(threadToDelete->thread);
            free(threadToDelete);
        }
    }
    pthread_cancel(serverThread);//Pthread_cancel
    pthread_exit(0);
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Funcion main que controla la aplicacion
int main(int argc, char *argv[])
{
    char consoleInput[30];
    killFlag = 1;

    pthread_create(&serverThread, NULL, serverFunc, NULL); // Crea el hilo donde esta funcionando todo

    while(killFlag){
        scanf("%s", consoleInput);
        if (strcmp(consoleInput, "kill") == 0){ // Si hay un mensaje por consola que sea kill mata el servidor
            killFlag = 0;
        }
        strcpy(consoleInput,"");
    }

    close(server.socket);
    killThreads();

    return 0;
}