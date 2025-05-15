#include "snail.h"

struct snail snail;

static inline ERROR_CODE 
create_socket (int* soquete, char network_interface[])
{
    // Cria arquivo para o socket sem qualquer protocolo
    *soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (*soquete == -1) {
        perror("Erro ao criar socket: Verifique se você é root!\n");
        return SOCKET_CREATE_FAILURE;
    }
 
    int ifindex = if_nametoindex(network_interface);
 
    struct sockaddr_ll endereco = {0};
    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(*soquete, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        perror("Erro ao fazer bind no socket\n");
        return SOCKET_CREATE_FAILURE;
    }
 
    struct packet_mreq mr = {0};
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(*soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        perror("Erro ao fazer setsockop\n");
        return SOCKET_CREATE_FAILURE;
    }
   
   return SUCCESS; 
}

void 
init_snail (char network_interface[])
{
    ERROR_CODE code;
    /* Copia nome da interface de rede */
    memcpy(snail.network_interface, network_interface, min(strlen(network_interface), 64));

    /* Cria raw socket para comunicacao*/
    code = create_socket(&snail.socket, snail.network_interface);
    if (FAILED(code)) 
    {
        perror("Erro ao criar o socker\n");
        exit(1);
    }

    /* TODO cria funcao para iniciar o pacote*/

    snail.pkg.start_marker = START_MARKER;
    snail.pkg_size = PKG_SIZE;

    return;
}

ERROR_CODE 
send_pkg (PKG_TYPE pkg_type, uint8_t *data, uint8_t size)
{
    static uint32_t seq = 0;

    memset(snail.pkg.data, 0, MAX_DATA);

    printf("%s", data);
    snail.pkg.size = size;
    snail.pkg.sequence_number = seq++; // TODO criar funcao que gera os numeros da sequencia
    snail.pkg.type = pkg_type;
    snail.pkg.checksum = 0; //TODO criar funcao que faz o checksum
    
    memcpy (snail.pkg.data, data, min(size, MAX_DATA));
    
    printf("after: %s", snail.pkg.data);

    uint32_t bytes_sent = send (snail.socket, &snail.pkg, snail.pkg_size, 0); 
    if (bytes_sent == -1)
    {
        perror ("Erro ao enviar erro\n");
        return SEND_PKG_FAILED; 
    }

    sleep(1);
    return SUCCESS;
}


static inline void 
print_pkg (struct package pkg)
{
   printf ("start_marker: %d\n", pkg.start_marker); 
   printf ("size: %d\n", pkg.size); 
   printf ("sequence_number: %d\n", pkg.sequence_number); 
   printf ("type: %d\n", pkg.type); 
   printf ("checksum: %d\n", pkg.checksum); 
   printf ("data: "); 
   for (int i = 0; i < pkg.size; i++)
   {
       printf ("%c", pkg.start_marker); 
   }
   printf ("\n");
   for (int i = 0; i < pkg.size; i++)
   {
       printf ("%d", pkg.start_marker); 
   }
   printf ("\n");
}

ERROR_CODE 
recv_pkg ()
{
    uint32_t bytes_recv = recv (snail.socket, &snail.pkg, snail.pkg_size, 0); 
    if (bytes_recv == -1)
    {
        perror ("Erro ao receber pacote");
        return RECV_PKG_FAILED;
    }

    print_pkg (snail.pkg);
    return SUCCESS;
}
