#include "socket.h"

//private Variable
static struct Socket *uniueq_socket;
static char socket_path[20];

//public functions
static int closeSocket(struct Socket *Soc);
static int openSocket(struct Socket *Soc);
static int transmitData(struct Socket *Soc, uint8_t *TransmittedData, uint16_t Size);
static int receiveData(struct Socket *Soc, uint8_t *TransmittedData, uint16_t Size);



static int openSocket(struct Socket *Soc)
{

    int len, rc;


    printf("Enter MacPath Name \n");
    scanf("%s", socket_path);


    memset(&Soc->server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&Soc->client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    Soc->server_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if(Soc->server_sock == FAIL)
    {
        printf("SOCKET ERROR: \n");
        return Soc->server_sock;
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    Soc->server_sockaddr.sun_family = AF_UNIX;
    strcpy(Soc->server_sockaddr.sun_path, socket_path);
    len = sizeof(Soc->server_sockaddr);

    unlink(socket_path);
    rc = bind(Soc->server_sock, (struct sockaddr *)&Soc->server_sockaddr, len);
    if(rc == -1)
    {
        printf("BIND ERROR:\n");
        close(Soc->server_sock);
        return rc;
    }

    /*********************************/
    /* Listen for any client sockets */
    /*********************************/
    rc = listen(Soc->server_sock, BACKLOG);
    if (rc == FAIL)
    {
        printf("LISTEN ERROR: \n");
        close(Soc->server_sock);
        return rc;
    }

    printf("Waiting PhySim...\n");

    /*********************************/
    /* Accept an incoming connection */
    /*********************************/
    Soc->client_sock = accept(Soc->server_sock, (struct sockaddr *)&Soc->client_sockaddr, &len);
    if(Soc->client_sock == FAIL)
    {
        printf("ACCEPT ERROR: \n");
        close(Soc->server_sock);
        close(Soc->client_sock);

        return Soc->client_sock;
    }

    printf("Unix Socket Opened\n");

    /****************************************/
    /* Get the name of the connected socket */
    /****************************************/
    len = sizeof(Soc->client_sockaddr);
    rc = getpeername(Soc->client_sock, (struct sockaddr *) &Soc->client_sockaddr, &len);
    if (rc == FAIL)
    {
        printf("GETPEERNAME ERROR: \n");
        close(Soc->server_sock);
        close(Soc->client_sock);
        return rc;
    }
    else
    {
        printf("Client socket filepath: %s\n", Soc->client_sockaddr.sun_path);
    }

    return SUCCESS;

    //setting timeout

//    struct timeval timeout;
//    timeout.tv_sec = 10;
//    timeout.tv_usec = 0;

//    setsockopt(Socket->server_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
//    setsockopt(Socket->client_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));

}



static int transmitData(struct Socket *Soc, uint8_t *Data, uint16_t Length)
{
    int rc;

    rc = send(Soc->client_sock, Data, Length, 0);

    if(rc == FAIL)
    {
        printf("SEND ERROR: %d\n", rc);
        close(Soc->server_sock);
        close(Soc->client_sock);
        return rc;
    }
}



static int receiveData(struct Socket *Soc, uint8_t *Data, uint16_t Length)
{
    int bytes_rec;

    bytes_rec = recv(Soc->client_sock, Data, Length, 0);

    if (bytes_rec == FAIL)
    {

        printf("RECV ERROR: \n");
        close(Soc->server_sock);
        close(Soc->client_sock);

        return bytes_rec;

    }
}



static int closeSocket(struct Socket *Soc)
{

    close(Soc->server_sock);
    close(Soc->client_sock);

}



int initSocket(struct Socket *Sock)
{

    static int socket_opened = 0;

    if(socket_opened != 0)
    {
        memcpy(Sock, uniueq_socket, sizeof(struct Socket));
        return SUCCESS;
    }

    Sock->ops.closeSocket = closeSocket;
    Sock->ops.transmitData = transmitData;
    Sock->ops.receiveData = receiveData;
    Sock->ops.openSocket = openSocket;

    openSocket(Sock);

    socket_opened = 1;
    uniueq_socket = Sock;

    return SUCCESS;
}



void deinitSocket(struct Socket *Sock)
{

}
