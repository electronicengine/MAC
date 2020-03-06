#include "wireless_socket.h"


//public functions
static int closePort(struct WirelessSocket *Socket);
static int openServerPort(struct WirelessSocket *Socket);
static int openClientPort(struct WirelessSocket *Socket);
static int getData(struct WirelessSocket *Socket, uint8_t *TransmittedData, int Size);
static int setData(struct WirelessSocket *Socket, uint8_t *TransmittedData, int Size);

//private functions
static void createSocketThread(struct WirelessSocket *Socket);
static void *listenSocket(void *Socket);
static pthread_t socket_thread;
static volatile short int stop_thread = 1;
static struct WirelessSocket *uniueq_socket;



int initWirelessSocket(struct WirelessSocket *Socket)
{

    printf("Starting Owpan\n");

    initMacMessageRepo(&Socket->mac_repo);
    initPhyMessageRepo(&Socket->phy_repo);

    Socket->ops.closePort = closePort;
    Socket->ops.getData = getData;
    Socket->ops.setData = setData;
    Socket->ops.openServerPort = openServerPort;
    Socket->ops.openClientPort = openServerPort;

}




static void createSocketThread(struct WirelessSocket *Socket)
{

//    pthread_create(&socket_thread, NULL, listenSocket, Socket);
//    pthread_detach(socket_thread);

}



//static void *listenSocket(void *Socket)
//{

//    int bytes_rec;
//    uint8_t transmitted_data[MAX_TRANSFER_SIZE];
//    struct MacSocket *sock = Socket;
//    struct Subject *subject = &sock->subject;
//    ServiceMessage *message;

//    printf("Socket Thread is started...\n");

//    openPort(sock);

//    stop_thread = 0;

//    while(1)
//    {

//        if(stop_thread == 1)
//            break;

//         bytes_rec = recv(sock->client_sock, transmitted_data, MAX_TRANSFER_SIZE, 0);

//        if(bytes_rec > 0)
//        {
//            message = sock->phy_repo.setServiceData(&sock->phy_repo, transmitted_data);
//            subject->operations.notifyObservers(subject, sock, message);
//        }

//    }

//    printf("Socket Thread is ending...\n");


//}



static int getData(struct WirelessSocket *Socket, uint8_t *TransmittedData, int Size)
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



static int setData(struct WirelessSocket *Socket, uint8_t *TransmittedData, int Size)
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



static int closePort(struct WirelessSocket *Socket)
{

    stop_thread = 1;

    sleep(2);

    close(Socket->server_sock);
    close(Socket->client_sock);

}



static int openClientPort(struct WirelessSocket *Socket)
{

    int len, rc;

    createSocketThread(Socket);

    memset(&Socket->server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&Socket->client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    Socket->client_sock= socket(AF_UNIX, SOCK_STREAM, 0);

    if(Socket->client_sock == -1)
    {
        printf("SOCKET ERROR: \n");
        return Socket->client_sock;
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    Socket->client_sockaddr.sun_family = AF_UNIX;
    strcpy(Socket->client_sockaddr.sun_path, CLIENT_ADDR);
    len = sizeof(Socket->client_sockaddr);

    unlink(CLIENT_ADDR);
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
    strcpy(Socket->server_sockaddr.sun_path, SERVER_ADDR);
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



static int openServerPort(struct WirelessSocket *Socket)
{

    int len, rc;
    static int socket_opened = 0;

    if(socket_opened != 0)
    {
        memcpy(Socket, uniueq_socket, sizeof(struct WirelessSocket));
        return SUCCESS;
    }

    memset(&Socket->server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&Socket->client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    Socket->server_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if(Socket->server_sock == FAIL)
    {
        printf("SOCKET ERROR: \n");
        return Socket->server_sock;
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    Socket->server_sockaddr.sun_family = AF_UNIX;
    strcpy(Socket->server_sockaddr.sun_path, SERVER_ADDR);
    len = sizeof(Socket->server_sockaddr);

    unlink(SERVER_ADDR);
    rc = bind(Socket->server_sock, (struct sockaddr *)&Socket->server_sockaddr, len);
    if(rc == -1)
    {
        printf("BIND ERROR:\n");
        close(Socket->server_sock);
        return rc;
    }

    /*********************************/
    /* Listen for any client sockets */
    /*********************************/
    rc = listen(Socket->server_sock, BACKLOG_WIRELESS);
    if (rc == FAIL)
    {
        printf("LISTEN ERROR: \n");
        close(Socket->server_sock);
        return rc;
    }

    printf("Waiting PhySim...\n");

    /*********************************/
    /* Accept an incoming connection */
    /*********************************/
    Socket->client_sock = accept(Socket->server_sock, (struct sockaddr *)&Socket->client_sockaddr, &len);
    if(Socket->client_sock == FAIL)
    {
        printf("ACCEPT ERROR: \n");
        close(Socket->server_sock);
        close(Socket->client_sock);

        return Socket->client_sock;
    }

    printf("Unix Socket Opened\n");

    socket_opened = 1;
    uniueq_socket = Socket;
    /****************************************/
    /* Get the name of the connected socket */
    /****************************************/
    len = sizeof(Socket->client_sockaddr);
    rc = getpeername(Socket->client_sock, (struct sockaddr *) &Socket->client_sockaddr, &len);
    if (rc == FAIL)
    {
        printf("GETPEERNAME ERROR: \n");
        close(Socket->server_sock);
        close(Socket->client_sock);
        return rc;
    }
    else
    {
        printf("Client socket filepath: %s\n", Socket->client_sockaddr.sun_path);
    }

    return SUCCESS;

    //setting timeout

//    struct timeval timeout;
//    timeout.tv_sec = 10;
//    timeout.tv_usec = 0;

//    setsockopt(Socket->server_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
//    setsockopt(Socket->client_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));

}



int deinitWirelessSocket(struct WirelessSocket *Socket)
{

}



