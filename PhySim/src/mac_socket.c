#include "mac_socket.h"


//private variables
static pthread_t socket_thread;
static volatile short int stop_thread = 1;
static char server_path[20];
static char client_path[20];


int initMacSocket(struct MacSocket *Socket)
{

    printf("enter server name\n");
    scanf("%s", server_path);

    printf("enter client name\n");
    scanf("%s", client_path);

    initSubject(&Socket->subject);

    initMacMessageRepo(&Socket->mac_repo);
    initPhyMessageRepo(&Socket->phy_repo);

    Socket->operations.closePort = closePort;
    Socket->operations.getData = getData;
    Socket->operations.setData = setData;
    Socket->operations.openPort = openPort;

    createSocketThread(Socket);

}



static void createSocketThread(struct MacSocket *Socket)
{

    pthread_create(&socket_thread, NULL, listenSocket, Socket);
    pthread_detach(socket_thread);

}



static void *listenSocket(void *Socket)
{

    int bytes_rec;
    uint8_t transmitted_data[MAX_TRANSFER_SIZE];
    struct MacSocket *sock = Socket;
    struct Subject *subject = &sock->subject;
    ServiceMessage *message;

    printf("Socket Thread is started...\n");

    openPort(sock);

    stop_thread = 0;

    while(1)
    {

        if(stop_thread == 1)
            break;

         bytes_rec = recv(sock->client_sock, transmitted_data, MAX_TRANSFER_SIZE, 0);

        if(bytes_rec > 0)
        {
            message = sock->phy_repo.setServiceData(&sock->phy_repo, transmitted_data);
            subject->operations.notifyObservers(subject, sock, message, transmitted_data);
        }

    }

    printf("Socket Thread is ending...\n");

}



static int getData(struct MacSocket *Socket, uint8_t *TransmittedData, int Size)
{

    int bytes_rec;

    bytes_rec = recv(Socket->client_sock, TransmittedData, Size, 0);

    if (bytes_rec == -1)
    {

        printf("RECV ERROR: \n");
        close(Socket->server_sock);
        close(Socket->client_sock);
        return bytes_rec;

    }

}



static int setData(struct MacSocket *Socket, uint8_t *TransmittedData, int Size)
{

    int rc;

    while(stop_thread == 1)
    {
        sleep(2);
        printf("waiting to socket tread\n");
    }

    rc = send(Socket->client_sock, TransmittedData, Size, 0);

    if(rc == -1)
    {

        printf("SEND ERROR: \n");
        close(Socket->server_sock);
        close(Socket->client_sock);
        return rc;

    }

}



static int closePort(struct MacSocket *Socket)
{

    stop_thread = 1;

    sleep(2);

    close(Socket->server_sock);
    close(Socket->client_sock);

}



static int openPort(struct MacSocket *Socket)
{

    int len, rc;

    memset(&Socket->server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&Socket->client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    Socket->client_sock= socket(AF_UNIX, SOCK_STREAM, 0);


    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    Socket->client_sockaddr.sun_family = AF_UNIX;
    strcpy(Socket->client_sockaddr.sun_path, client_path);
    len = sizeof(Socket->client_sockaddr);

    unlink(client_path);
    rc = bind(Socket->client_sock, (struct sockaddr *)&Socket->client_sockaddr, len);
    if(rc == -1)
    {
        printf("BIND ERROR:\n");
        close(Socket->client_sock);
        return rc;
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* for the server socket and connect   */
    /* to it.                              */
    /***************************************/
    Socket->server_sockaddr.sun_family = AF_UNIX;
    strcpy(Socket->server_sockaddr.sun_path, server_path);
    rc = connect(Socket->client_sock, (struct sockaddr *) &Socket->server_sockaddr, len);
    if(rc == -1)
    {
        printf("CONNECT ERROR \n");
        close(Socket->client_sock);
        return -1;
    }

    //setting timeout

//    struct timeval timeout;
//    timeout.tv_sec = 10;
//    timeout.tv_usec = 0;

//    setsockopt(Socket->server_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
//    setsockopt(Socket->client_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));

}



int deinitSocket(struct MacSocket *Socket)
{

}



