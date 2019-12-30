#include "unixsocket.h"


//private variables
static pthread_t socket_thread;
static volatile short int stop_thread = 1;



int initSocket(struct UnixSocket *Socket)
{
    initSubject(&Socket->subject);

    initDataSap(&Socket->data_sap);
    initManagementSap(&Socket->management_sap);

    Socket->operations.closePort = closePort;
    Socket->operations.getData = getData;
    Socket->operations.setData = setData;
    Socket->operations.openPort = openPort;

    createSocketThread(Socket);

}



static void createSocketThread(struct UnixSocket *Socket)
{

    pthread_create(&socket_thread, NULL, listenSocket, Socket);
    pthread_detach(socket_thread);

}



static void *listenSocket(void *Socket)
{

    int bytes_rec;
    uint8_t *transmitted_data;
    struct UnixSocket *sock = Socket;
    struct Subject *subject = &sock->subject;
    ServiceMessageHeader header;

    transmitted_data = malloc(MAX_TRANSFER_SIZE);

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

            header.type = transmitted_data[0];
            header.sub_type = transmitted_data[1];
            header.length = transmitted_data[2];
            header.length |= transmitted_data[3];

            subject->operations.notifyObservers(subject, sock, &header, transmitted_data);

        }

    }

    printf("Socket Thread is ending...");

}



static int getData(struct UnixSocket *Socket, uint8_t *TransmittedData, int Size)
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



static int setData(struct UnixSocket *Socket, uint8_t *TransmittedData, int Size)
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



static int closePort(struct UnixSocket *Socket)
{

    stop_thread = 1;

    sleep(2);

    close(Socket->server_sock);
    close(Socket->client_sock);

}



static int openPort(struct UnixSocket *Socket)
{

    int len, rc;


    memset(&Socket->server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&Socket->client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    Socket->server_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if(Socket->server_sock == -1)
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
    strcpy(Socket->server_sockaddr.sun_path, SOCK_PATH);
    len = sizeof(Socket->server_sockaddr);

    unlink(SOCK_PATH);
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
    rc = listen(Socket->server_sock, BACKLOG);
    if (rc == -1)
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
    if (Socket->client_sock == -1)
    {
        printf("ACCEPT ERROR: \n");
        close(Socket->server_sock);
        close(Socket->client_sock);

        return Socket->client_sock;
    }

    printf("Unix Socket Opened\n");

    /****************************************/
    /* Get the name of the connected socket */
    /****************************************/
    len = sizeof(Socket->client_sockaddr);
    rc = getpeername(Socket->client_sock, (struct sockaddr *) &Socket->client_sockaddr, &len);
    if (rc == -1)
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


    //setting timeout

//    struct timeval timeout;
//    timeout.tv_sec = 10;
//    timeout.tv_usec = 0;

//    setsockopt(Socket->server_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
//    setsockopt(Socket->client_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));


}

int deinitSocket(struct UnixSocket *Socket)
{
    deinitDataSap(&Socket->data_sap);
    deinitManagementSap(&Socket->management_sap);
}
