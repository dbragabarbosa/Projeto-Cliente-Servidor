#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>

#define BUFFER_SIZE 500
#define MAX_CONEXOES 10
#define MAX_SERVIDORES 2

// Estrutura para armazenar as informações dos sensores
struct informacoesCliente
{
    int id;
    int socket;
    int potencia;
    int eficienciaEnergetica;
    int potenciaUtil;
    int estaLigado; // 1 para ligado e 0 para desligado
};

// função main que trata o recebimento: ./server 127.0.0.1 90900 90100 OU ./server 127.0.0.1 90900 90200
int main(int argc, char **argv)
{
    char *portaPeer2Peer = argv[2];
    char *portaClientes = argv[3];

    int numeroDeClientes = 0;
    char mensagemServidorParaCliente[50];
    char buffer[BUFFER_SIZE];
    char acao[10];
    int idDaAcao;
    int tamanhoMensagemRecebida;
    int maxServeres = 0;

    // CONEXÃO PEER-2-PEER 
    int PidMi = 1; // PidMi é o identificador do servidor peer, ou seja, o identificador desse servidor é 1
    int PidMj = 2; // PidMj é o identificador do servidor peer que vai se conectar ativamente
    // FUNÇÃO SOCKET() com protocolo IPv4, para conexão com o servidor peer
    int socketServidorPeer = 0;
    socketServidorPeer = socket(AF_INET, SOCK_STREAM, 0);
    if(socketServidorPeer == -1) 
    {
        printf("--- Erro na função socket() PEER-2-PEER\n");
        return 0;
    }
    else
    {
        printf("--- Socket PEER-2-PEER criado com sucesso!\n");
    }

    // FUNÇÃO BIND()
    struct sockaddr_in enderecoServidorPeer;
    enderecoServidorPeer.sin_family = AF_INET;
    enderecoServidorPeer.sin_port = htons(atoi(portaPeer2Peer));
    enderecoServidorPeer.sin_addr.s_addr = INADDR_ANY;
    bind(socketServidorPeer, (struct sockaddr *)&enderecoServidorPeer, sizeof(enderecoServidorPeer));

    int funcaoConnect = connect(socketServidorPeer, (struct sockaddr *)&enderecoServidorPeer, sizeof(enderecoServidorPeer));
    if(funcaoConnect < 0)
    {
        printf("No peer found, starting to listen..\n");

        // FUNÇÃO LISTEN()
        listen(socketServidorPeer, 10);

        // FUNÇÃO ACCEPT()
        struct sockaddr_in enderecoClientePeer;
        socklen_t tamanhoEnderecoClientePeer = sizeof(enderecoClientePeer);
        int acceptClientePeer = accept(socketServidorPeer, (struct sockaddr *)&enderecoClientePeer, &tamanhoEnderecoClientePeer);
        if(acceptClientePeer == -1) {
            printf("--- Erro na função accept() PEER-2-PEER\n");
            return 0;
        }
        else {
            printf("--- Accept PEER-2-PEER realizado com sucesso!\n");
            maxServeres = 1;
        }

        printf("Peer %d connected\n", PidMj);

        /// envia "RES_ADDPEER(PidMj)" para o servidor Mj
        // sprintf(mensagemServidorParaCliente, "RES_ADDPEER(%d)", PidMj);
        // send(socketServidorPeer, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
        // printf("--- Mensagem enviada para Mj: %s\n", mensagemServidorParaCliente);

        // recebe "RES_ADDPEER(PidMi)" do servidor Mj e imprime na tela "New Peer ID: PidMi"
        // bzero(buffer, BUFFER_SIZE);
        // tamanhoMensagemRecebida = recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
        // recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
        // printf("--- Mensagem recebida de Mj: %s\n", buffer);
        printf("New Peer ID: %d\n", PidMi);

        // // recv "REQ_ADDPEER"
        // bzero(buffer, BUFFER_SIZE);
        // tamanhoMensagemRecebida = recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
        // recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
        // printf("MENSAGEM RECEBIDA NO IF DO CONNECT: %s\n", buffer);

        // // send "RES_ADDPEER(PidMi)" para nao ficar preso
        // sprintf(mensagemServidorParaCliente, "RES_ADDPEER(1)");
        // send(socketServidorPeer, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
        // printf("MENSAGEM ENVIADA NO IF DO CONNECT: %s\n", mensagemServidorParaCliente);
    }
    else // ou seja, há um servidor aberto à conexao
    {
        printf("--- ENTROU NO ELSE\n");

        // vericar se já tem algum servidor conectado
        if (maxServeres == 1)  // verifica se a quantidade máxima de servidores já foi atingida
        {
            printf("Peer limit exceeded\n"); // print ERROR 02
            return 0;
        }
        else  // Em caso negativo,
        {
            printf("--- ENTROU NO ELSE DO ELSE\n");

            // recebe "RES_ADDPEER(PidMi)" do servidor Mi e imprime na tela "New Peer ID: PidMi"  
            // bzero(buffer, BUFFER_SIZE);
            // tamanhoMensagemRecebida = recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
            // recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
            // printf("--- Mensagem recebida de Mi: %s\n", buffer);
            printf("New Peer ID: %d\n", PidMj);
            printf("Peer %d connected\n", PidMi);
            // e envia "RES_ADDPEER(PidMj)" para o servidor Mi
            // sprintf(mensagemServidorParaCliente, "RES_ADDPEER(%d)", PidMj);
            // send(socketServidorPeer, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
            // printf("--- Mensagem enviada para Mi: %s\n", mensagemServidorParaCliente);

            // se não tiver, conecta
            // FUNÇÃO CONNECT()
            // struct sockaddr_in enderecoClientePeer;
            // enderecoClientePeer.sin_family = AF_INET;
            // enderecoClientePeer.sin_port = htons(atoi(portaPeer2Peer));
            // enderecoClientePeer.sin_addr.s_addr = INADDR_ANY;
            // connect(socketServidorPeer, (struct sockaddr *)&enderecoClientePeer, sizeof(enderecoClientePeer));

            // // send "REQ_ADDPEER" para o servidor Mj
            // sprintf(mensagemServidorParaCliente, "REQ_ADDPEER");
            // send(socketServidorPeer, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
            // printf("MENSAGEM ENVIADA NO ELSE DO CONNECT: %s\n", mensagemServidorParaCliente);

            // // recv "RES_ADDPEER(PidMi)"
            // bzero(buffer, BUFFER_SIZE);
            // tamanhoMensagemRecebida = recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
            // recv(socketServidorPeer, buffer, BUFFER_SIZE, 0);
            // printf("MENSAGEM RECEBIDA NO ELSE DO CONNECT: %s\n", buffer);
        }
    }
    // CONEXÃO PEER-2-PEER 

    // FUNÇÃO SOCKET() com protocolo IPv4, para conexão dos clientes
    int socketServidor = 0;
    socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketServidor == -1) 
    {
        printf("--- Erro na função socket() dos clientes\n");
        return 0;
    }
    else
    {
        printf("--- Socket DOS CLIENTES criado com sucesso!\n");
    }

    // FUNÇÃO BIND()
    struct sockaddr_in enderecoServidor;
    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port = htons(atoi(portaClientes));
    enderecoServidor.sin_addr.s_addr = INADDR_ANY;
    bind(socketServidor, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor));

    // FUNÇÃO LISTEN()
    listen(socketServidor, 10);

    // inicialização do conjunto de descritores para a função select()
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socketServidor, &readfds);
    int maxSocket = socketServidor;

    // Adiciona um array para rastrear os sockets dos clientes
    int socketsClientes[MAX_CONEXOES];
    memset(socketsClientes, 0, sizeof(socketsClientes));

    int IDsDosClientes = 0;
    struct informacoesCliente clientes[MAX_CONEXOES];

    // int activity = select(maxSocket + 1, &readfds, NULL, NULL, NULL);

    // assim que o cliente se conectar, escuta mensagens do cliente até receber a mensagem "kill"   
    while(1)  
    {
        printf("--- Entrou no while\n");

        for(int i = 0; i < MAX_CONEXOES; i++)
        {
            struct sockaddr_in enderecoCliente;
            socklen_t tamanhoEnderecoCliente = sizeof(enderecoCliente);
            socketsClientes[i] = accept(socketServidor, (struct sockaddr *)&enderecoCliente, &tamanhoEnderecoCliente);

            if(socketsClientes[i] == -1) 
            {
                printf("--- Erro na função accept()\n");
                // return 0;
            }
            else 
            {
                printf("--- Accept realizado com sucesso!\n");

                if(socketsClientes[i] > maxSocket)
                {
                    maxSocket = socketsClientes[i];
                } 
                FD_SET(socketsClientes[i], &readfds);
            }
        
            if (FD_ISSET(socketsClientes[i], &readfds)) 
            {
                printf("--- Entrou no if do select\n");

                bzero(buffer, BUFFER_SIZE);
                tamanhoMensagemRecebida = recv(socketsClientes[i], buffer, BUFFER_SIZE, 0);
                if(tamanhoMensagemRecebida == -1) {
                    printf("--- Erro na função recv()\n");
                    // return 0;
                }
                else if(tamanhoMensagemRecebida == 0) {
                    continue;
                }
                else
                    {
                        printf("--- Mensagem recebida no buffer: %s\n", buffer);
                        printf("%s\n", buffer); // 4) Sim, é necessário que os servidores imprimam todas as mensagens recebidas do cliente e do outro servidor.

                        // se a mensagem recebida for REQ_ADD, define um identificador IdCi para Ci único entre os seus clientes,
                        // registra o identificador em sua base de dados, imprime em tela a mensagem “Client IdCi added” e envia para o cliente Ci a mensagem RES_ADD(IdCi).
                        if(strcmp(buffer, "REQ_ADD") == 0)
                        {
                            // da pra esse cliente o valor de IDsDosClientes e incrementa IDsDosCLientes pra ser diferente dos próximos clientes
                            // da um ID para o cliente e incrmenta o ID para o próximo cliente
                            clientes[numeroDeClientes].id = IDsDosClientes;
                            clientes[numeroDeClientes].estaLigado = 1;
                            // armazena o socket do cliente no array de clientes
                            clientes[numeroDeClientes].socket = socketsClientes[i];
                            // da um valor aleatorio (entre 0 e 1500) para a potencia do cliente
                            clientes[numeroDeClientes].potencia = rand() % 1500;
                            // da um valor aleatorio (entre 0 e 100) para a eficiencia energetica do cliente
                            clientes[numeroDeClientes].eficienciaEnergetica = rand() % 100;
                            // coloca em potenciaUtil o valor de (potencia*enfienciaEnergetica/100)
                            clientes[numeroDeClientes].potenciaUtil = (clientes[numeroDeClientes].potencia*clientes[numeroDeClientes].eficienciaEnergetica/100);

                            // imprime em tela a mensagem “Client IdCi added”
                            printf("Client %d added\n", clientes[numeroDeClientes].id);

                            // envia para o cliente Ci a mensagem RES_ADD(clientes[numeroDeClientes].id)
                            sprintf(mensagemServidorParaCliente, "RES_ADD(%d)", clientes[numeroDeClientes].id);
                            // FUNCÃO SEND()
                            send(socketsClientes[i], mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
                            printf("--- Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);

                            // incrementa o ID para o próximo cliente
                            numeroDeClientes++;
                            IDsDosClientes++;
                            // zera o buffer e faz voltar para o inicio do while para esperar a proxima mensagem
                            // bzero(buffer, BUFFER_SIZE);
                            // continue;
                        }
                        // 1) se a mensagem recebida for REQ_LS, 
                        else if(strcmp(buffer, "REQ_LS") == 0)
                        {
                            // consulta a tabela de sensores da rede local para encontrar o sensor com maior potencia util
                            int maiorPotenciaUtil = 0;
                            int idDoClienteComMaiorPotenciaUtil = 0;
                            for (int i = 0; i < numeroDeClientes; i++)
                            {
                                if (clientes[i].potenciaUtil > maiorPotenciaUtil && clientes[i].estaLigado == 1)
                                {
                                    maiorPotenciaUtil = clientes[i].potenciaUtil;
                                    idDoClienteComMaiorPotenciaUtil = clientes[i].id;
                                }
                            }
                            // envia para o cliente Ci a mensagem RES_LS(local PidMi sensor sensorIdi: potenciaUtili (potenciai eficienciaEnergeticai))
                            sprintf(mensagemServidorParaCliente, "RES_LS(local %d sensor %d: %d (%d %d))", PidMi, idDoClienteComMaiorPotenciaUtil, maiorPotenciaUtil, clientes[idDoClienteComMaiorPotenciaUtil].potencia, clientes[idDoClienteComMaiorPotenciaUtil].eficienciaEnergetica);
                            // FUNCÃO SEND()
                            send(socketsClientes[i], mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
                            printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);

                            // zera o buffer e faz voltar para o inicio do while para esperar a proxima mensagem
                            // bzero(buffer, BUFFER_SIZE);
                            // continue;
                        }
                        // 2) se a mensagem recebida for REQ_ES, 
                        else if(strcmp(buffer, "REQ_ES") == 0)
                        {
                            printf("Entrou no REQ_ES\n");
                        }
                        // 3) se a mensagem recebida for REQ_LP, 
                        else if(strcmp(buffer, "REQ_LP") == 0)
                        {
                            // consulta a tabela de sensores da rede local e calcula o somatório da potencia util de todos os sensores ligados
                            // (potênciaa*eficiência_energéticaa/100 + potênciab*eficiência_energéticab/100 + ... + potêncian*eficiência_energétican/100)
                            int somatorioPotenciaUtil = 0;
                            for (int i = 0; i < numeroDeClientes; i++)
                            {
                                if (clientes[i].estaLigado == 1)
                                {
                                    somatorioPotenciaUtil += clientes[i].potenciaUtil;
                                }
                            }

                            // envia para o cliente Ci a mensagem RES_LP(local PidMi potency: somatorioPotenciaUtil)
                            sprintf(mensagemServidorParaCliente, "RES_LP(local %d potency: %d)", PidMi, somatorioPotenciaUtil);
                            // FUNCÃO SEND()
                            send(socketsClientes[i], mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
                            printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);
                        }
                        // 4) se a mensagem recebida for REQ_EP, 
                        else if(strcmp(buffer, "REQ_EP") == 0)
                        {
                            printf("Entrou no REQ_EP\n");
                        }
                        // 5) se a mensagem recebida for REQ_MS, 
                        else if(strcmp(buffer, "REQ_MS") == 0)
                        {
                            printf("Entrou no REQ_MS\n");
                        }
                        // 6) se a mensagem recebida for REQ_MN, 
                        else if(strcmp(buffer, "REQ_MN") == 0)
                        {
                            printf("Entrou no REQ_MN\n");

                            // consulta a tabela de sensores da rede local e calcula o somatório da potencia util de todos os sensores ligados
                            // int somatorioPotenciaUtil = 0;
                            // for (int i = 0; i < numeroDeClientes; i++)
                            // {
                            //     if (clientes[i].estaLigado == 1)
                            //     {
                            //         somatorioPotenciaUtil += clientes[i].potenciaUtil;
                            //     }
                            // }

                            // o servidor Mi envia a mensagem REQ_EP para o servidor externo Mj
                            // ...

                        }
                        // se não for isso, então é KILL
                        else
                        {
                            printf("Entrou no KILL\n");

                            // Se recebe REQ_DC(IdCi)
                            // Separa REQ_DC(IdCi) em, REQ_DC na variável acao e IdCi na variável idDaAcao
                            sscanf(buffer, "%s %d", acao, &idDaAcao);
                            printf("Ação: %s\n", acao);
                            printf("ID da Ação: %d\n", idDaAcao);

                            // verifica se o idDaAcao existe na lista de clientes e está ligado
                            int clienteExiste = 0;
                            int clienteEstaLigado = 0;
                            for (int i = 0; i < numeroDeClientes; i++)
                            {
                                if (clientes[i].id == idDaAcao)
                                {
                                    clienteExiste = 1;
                                    if (clientes[i].estaLigado == 1)
                                    {
                                        clienteEstaLigado = 1;
                                    }
                                }
                            }
                            // se não estiver na base de dados, envia a mensagem ERROR(04) para o cliente
                            if (clienteExiste == 0)
                            {
                                sprintf(mensagemServidorParaCliente, "ERROR(04)");
                                // FUNCÃO SEND()
                                send(socketsClientes[i], mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
                                printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);
                            }

                            // se estiver na base de dados, envia OK(01) para o cliente, imprime em tela a mensagem “Client IdCi removed”, remove o identificador da base de dados e desconecta o cliente Ci.
                            if (clienteExiste == 1 && clienteEstaLigado == 1)
                            {
                                sprintf(mensagemServidorParaCliente, "OK(01)");
                                // FUNCÃO SEND()
                                send(socketsClientes[i], mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
                                printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);

                                // imprime em tela a mensagem “Client IdCi removed”
                                printf("Client %d removed\n", idDaAcao);

                                // remove o identificador da base de dados
                                for (int i = 0; i < numeroDeClientes; i++)
                                {
                                    if (clientes[i].id == idDaAcao)
                                    {
                                        clientes[i].estaLigado = 0;
                                    }
                                }

                                // desconecta o cliente Ci 
                                close(clientes[idDaAcao].socket);
                                printf("Socket fechado com sucesso!\n");
                            }
                        }
                    }
            }
        }
    }

    // FUNÇÃO CLOSE()
    close(socketServidor);
    printf("--- Socket DOS CLIENTES fechado com sucesso!\n");
    close(socketServidorPeer);
    printf("--- Socket PEER-2-PEER fechado com sucesso!\n");

    return 0;
}







































// ################################# CONEXÃO CLIENTE-SERVIDOR FEITA #################################  -- FALTA SÓ TRATAR A CONEXÃO DE MULTIPLOS CLIENTES
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <netdb.h>

// #define BUFFER_SIZE 500
// #define MAX_CONEXOES 10

// // Estrutura para armazenar as informações dos sensores
// struct informacoesCliente
// {
//     int id;
//     int socket;
//     int potencia;
//     int eficienciaEnergetica;
//     int potenciaUtil;
//     int estaLigado; // 1 para ligado e 0 para desligado
// };

// // função main que trata o recebimento: ./server 127.0.0.1 90900 90100 OU ./server 127.0.0.1 90900 90200
// int main(int argc, char **argv)
// {
//     printf("Arquivo server.c!\n");
//     // coloca na variável endereçoIPv4 o endereço IPv4 utilizado para se conectar ativamente (tcp) ao peer
//     char *enderecoIPv4 = argv[1];
//     // coloca na variável portaPeer2Peer o número da porta da conexão peer-2-peer
//     char *portaPeer2Peer = argv[2];
//     // coloca na variável portaClientes o número da porta que vai receber a conexão dos clientes
//     char *portaClientes = argv[3];

//     // Adiciona um array para rastrear os sockets dos clientes
//     int socketsClientes[MAX_CONEXOES];
//     int numeroDeClientes = 0;
//     struct informacoesCliente clientes[MAX_CONEXOES];
//     char mensagemServidorParaCliente[50];
//     char buffer[BUFFER_SIZE];
//     char acao[10];
//     int idDaAcao;
//     int tamanhoMensagemRecebida;
//     int socketCliente;
//     int IDsDosClientes = 0;

//     // Os servidores inicializam-se com dois sockets, um para conexão com o servidor peer e uma para a conexão com os clientes. 
//     // À medida que se conecta e se desconecta de clientes, outros sockets são adicionados/descartados do seu pool de sockets.

//     // O servidor peer é o servidor que se conecta ativamente (tcp) ao peer, ou seja, com o outro servidor
//     // Cria o socket de conexão com o servidor peer

//     int PidMi = 0; // PidMi é o identificador do servidor peer, ou seja, o identificador desse servidor é 0 mas do outro servidor precisa ser incrementado para ser diferente


//     // FUNÇÃO SOCKET() com protocolo IPv4, para conexão dos clientes
//     int socketServidor = 0;
//     socketServidor = socket(AF_INET, SOCK_STREAM, 0);
//     if(socketServidor == -1) 
//     {
//         printf("Erro na função socket()\n");
//         return 0;
//     }
//     else
//     {
//         printf("Socket criado com sucesso!\n");
//     }

//     // int opt = 1;
//     // if (setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
//     // {
//     //     perror("setsockopt");
//     //     exit(EXIT_FAILURE);
//     // }

//     // FUNÇÃO BIND()
//     struct sockaddr_in enderecoServidor;
//     enderecoServidor.sin_family = AF_INET;
//     enderecoServidor.sin_port = htons(atoi(portaClientes));
//     enderecoServidor.sin_addr.s_addr = INADDR_ANY;
//     bind(socketServidor, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor));

//     // FUNÇÃO LISTEN()
//     listen(socketServidor, 10);
//     if( 0 != listen(socketServidor, 10) ) {
//         printf("Erro na função listen()\n");
//         return 0;
//     }
//     else {
//         printf("Listen realizado com sucesso!\n");
//     }

//     int flag = 0;

//     // assim que o cliente se conectar, escuta mensagens do cliente até receber a mensagem "kill"   
//     while(1)
//     {
//         printf("Entrou no while\n");

//         // zera o buffer
//         bzero(buffer, BUFFER_SIZE);

//         if(flag == 0)
//         {
//             // FUNÇÃO ACCEPT() que aceita no máximo 10 conexões simultâneas
//             struct sockaddr_in enderecoCliente;
//             socklen_t tamanhoEnderecoCliente = sizeof(enderecoCliente);

//             // Verifica se ainda há espaço para aceitar novas conexões
//             if (numeroDeClientes < MAX_CONEXOES) {
//                 socketCliente = accept(socketServidor, (struct sockaddr *)&enderecoCliente, &tamanhoEnderecoCliente);
//                 if(socketCliente == -1) {
//                     printf("Erro na função accept()\n");
//                     return 0;
//                 }
//                 else {
//                     printf("Accept realizado com sucesso!\n");

//                     // Armazena o socket do cliente no array
//                     socketsClientes[numeroDeClientes] = socketCliente;
//                 }
//                 // OU FUNÇÃO SELECT() para gerenciar múltiplas conexões

//             }
//             else {
//                 printf("Limite máximo de conexões atingido. Recusando nova conexão.\n");
//             }

//             flag = 1;
//         }

//         // FUNÇÃO RECV()
//         bzero(buffer, BUFFER_SIZE);
//         tamanhoMensagemRecebida = recv(socketCliente, buffer, BUFFER_SIZE, 0);
//         if(tamanhoMensagemRecebida == -1) {
//             printf("Erro na função recv()\n");
//             return 0;
//         }
//         else if(tamanhoMensagemRecebida == 0) {
//             continue;
//         }

//         // print da mensagem recebida
//         printf("Mensagem recebida no buffer: %s\n", buffer);

//         // se a mensagem recebida for REQ_ADD, define um identificador IdCi para Ci único entre os seus clientes,
//         // registra o identificador em sua base de dados, imprime em tela a mensagem “Client IdCi added” e envia para o cliente Ci a mensagem RES_ADD(IdCi).
//         if(strcmp(buffer, "REQ_ADD") == 0)
//         {
//             // da pra esse cliente o valor de IDsDosClientes e incrementa IDsDosCLientes pra ser diferente dos próximos clientes
//             // da um ID para o cliente e incrmenta o ID para o próximo cliente
//             clientes[numeroDeClientes].id = IDsDosClientes;
//             clientes[numeroDeClientes].estaLigado = 1;
//             // armazena o socket do cliente no array de clientes
//             clientes[numeroDeClientes].socket = socketCliente;
//             // da um valor aleatorio (entre 0 e 1500) para a potencia do cliente
//             clientes[numeroDeClientes].potencia = rand() % 1500;
//             // da um valor aleatorio (entre 0 e 100) para a eficiencia energetica do cliente
//             clientes[numeroDeClientes].eficienciaEnergetica = rand() % 100;
//             // coloca em potenciaUtil o valor de (potencia*enfienciaEnergetica/100)
//             clientes[numeroDeClientes].potenciaUtil = (clientes[numeroDeClientes].potencia*clientes[numeroDeClientes].eficienciaEnergetica/100);

//             // imprime em tela a mensagem “Client IdCi added”
//             printf("Client %d added\n", clientes[numeroDeClientes].id);

//             // envia para o cliente Ci a mensagem RES_ADD(clientes[numeroDeClientes].id)
//             sprintf(mensagemServidorParaCliente, "RES_ADD(%d)", clientes[numeroDeClientes].id);
//             // FUNCÃO SEND()
//             send(socketCliente, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
//             printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);

//             // incrementa o ID para o próximo cliente
//             numeroDeClientes++;
//             IDsDosClientes++;
//             // zera o buffer e faz voltar para o inicio do while para esperar a proxima mensagem
//             // bzero(buffer, BUFFER_SIZE);
//             // continue;
//         }
//         // 1) se a mensagem recebida for REQ_LS, 
//         else if(strcmp(buffer, "REQ_LS") == 0)
//         {
//             // consulta a tabela de sensores da rede local para encontrar o sensor com maior potencia util
//             int maiorPotenciaUtil = 0;
//             int idDoClienteComMaiorPotenciaUtil = 0;
//             for (int i = 0; i < numeroDeClientes; i++)
//             {
//                 if (clientes[i].potenciaUtil > maiorPotenciaUtil && clientes[i].estaLigado == 1)
//                 {
//                     maiorPotenciaUtil = clientes[i].potenciaUtil;
//                     idDoClienteComMaiorPotenciaUtil = clientes[i].id;
//                 }
//             }
//             // envia para o cliente Ci a mensagem RES_LS(local PidMi sensor sensorIdi: potenciaUtili (potenciai eficienciaEnergeticai))
//             sprintf(mensagemServidorParaCliente, "RES_LS(local %d sensor %d: %d (%d %d))", PidMi, idDoClienteComMaiorPotenciaUtil, maiorPotenciaUtil, clientes[idDoClienteComMaiorPotenciaUtil].potencia, clientes[idDoClienteComMaiorPotenciaUtil].eficienciaEnergetica);
//             // FUNCÃO SEND()
//             send(socketCliente, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
//             printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);

//             // zera o buffer e faz voltar para o inicio do while para esperar a proxima mensagem
//             // bzero(buffer, BUFFER_SIZE);
//             // continue;
//         }
//         // 2) se a mensagem recebida for REQ_ES, 
//         else if(strcmp(buffer, "REQ_ES") == 0)
//         {
//             printf("Entrou no REQ_ES\n");
//         }
//         // 3) se a mensagem recebida for REQ_LP, 
//         else if(strcmp(buffer, "REQ_LP") == 0)
//         {
//             // consulta a tabela de sensores da rede local e calcula o somatório da potencia util de todos os sensores ligados
//             // (potênciaa*eficiência_energéticaa/100 + potênciab*eficiência_energéticab/100 + ... + potêncian*eficiência_energétican/100)
//             int somatorioPotenciaUtil = 0;
//             for (int i = 0; i < numeroDeClientes; i++)
//             {
//                 if (clientes[i].estaLigado == 1)
//                 {
//                     somatorioPotenciaUtil += clientes[i].potenciaUtil;
//                 }
//             }

//             // envia para o cliente Ci a mensagem RES_LP(local PidMi potency: somatorioPotenciaUtil)
//             sprintf(mensagemServidorParaCliente, "RES_LP(local %d potency: %d)", PidMi, somatorioPotenciaUtil);
//             // FUNCÃO SEND()
//             send(socketCliente, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
//             printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);
//         }
//         // 4) se a mensagem recebida for REQ_EP, 
//         else if(strcmp(buffer, "REQ_EP") == 0)
//         {
//             printf("Entrou no REQ_EP\n");
//         }
//         // 5) se a mensagem recebida for REQ_MS, 
//         else if(strcmp(buffer, "REQ_MS") == 0)
//         {
//             printf("Entrou no REQ_MS\n");
//         }
//         // 6) se a mensagem recebida for REQ_MN, 
//         else if(strcmp(buffer, "REQ_MN") == 0)
//         {
//             printf("Entrou no REQ_MN\n");

//             // consulta a tabela de sensores da rede local e calcula o somatório da potencia util de todos os sensores ligados
//             int somatorioPotenciaUtil = 0;
//             for (int i = 0; i < numeroDeClientes; i++)
//             {
//                 if (clientes[i].estaLigado == 1)
//                 {
//                     somatorioPotenciaUtil += clientes[i].potenciaUtil;
//                 }
//             }

//             // o servidor Mi envia a mensagem REQ_EP para o servidor externo Mj
//             // ...

//         }
//         // se não for isso, então é KILL
//         else
//         {
//             printf("Entrou no KILL\n");

//             // Se recebe REQ_DC(IdCi)
//             // Separa REQ_DC(IdCi) em, REQ_DC na variável acao e IdCi na variável idDaAcao
//             sscanf(buffer, "%s %d", acao, &idDaAcao);
//             printf("Ação: %s\n", acao);
//             printf("ID da Ação: %d\n", idDaAcao);

//             // verifica se o idDaAcao existe na lista de clientes e está ligado
//             int clienteExiste = 0;
//             int clienteEstaLigado = 0;
//             for (int i = 0; i < numeroDeClientes; i++)
//             {
//                 if (clientes[i].id == idDaAcao)
//                 {
//                     clienteExiste = 1;
//                     if (clientes[i].estaLigado == 1)
//                     {
//                         clienteEstaLigado = 1;
//                     }
//                 }
//             }
//             // se não estiver na base de dados, envia a mensagem ERROR(04) para o cliente
//             if (clienteExiste == 0)
//             {
//                 sprintf(mensagemServidorParaCliente, "ERROR(04)");
//                 // FUNCÃO SEND()
//                 send(socketCliente, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
//                 printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);
//             }

//             // se estiver na base de dados, envia OK(01) para o cliente, imprime em tela a mensagem “Client IdCi removed”, remove o identificador da base de dados e desconecta o cliente Ci.
//             if (clienteExiste == 1 && clienteEstaLigado == 1)
//             {
//                 sprintf(mensagemServidorParaCliente, "OK(01)");
//                 // FUNCÃO SEND()
//                 send(socketCliente, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
//                 printf("Mensagem enviada para o cliente: %s\n", mensagemServidorParaCliente);

//                 // imprime em tela a mensagem “Client IdCi removed”
//                 printf("Client %d removed\n", idDaAcao);

//                 // remove o identificador da base de dados
//                 for (int i = 0; i < numeroDeClientes; i++)
//                 {
//                     if (clientes[i].id == idDaAcao)
//                     {
//                         clientes[i].estaLigado = 0;
//                     }
//                 }

//                 // desconecta o cliente Ci 
//                 close(clientes[idDaAcao].socket);
//                 printf("Socket fechado com sucesso!\n");
//             }
//         }

//     }

//     // FUNÇÃO CLOSE()
//     close(socketServidor);
//     printf("Socket fechado com sucesso!\n");

//     return 0;
// }
// ################################# CONEXÃO CLIENTE-SERVIDOR FEITA #################################




















































































// char dadosSensor[100];
// char sensoresLigadosChar[100];

// // Estrutura para armazenar as informações dos sensores
// struct informacoesSensor
// {
//     int sensorId;
//     // int corrente;
//     // int tensao;
//     int potencia;
//     int eficienciaEnergetica;
//     int ligado;  // para controlar se o sensor está ligado ou desligado
// };

// // Tabela hash para armazenar as informações dos sensores
// #define NUMEROSENSORES 100
// struct informacoesSensor tabelaDeSensores[NUMEROSENSORES];  // tabelaDeSensores é um array de informacoesSensor


// // Legenda de retorno das funções
// // ERROR = 1
// // Ex saída da função: 12 == mensagem para o cliente ERROR 02
// // Mensagem de erro transmitida do Servidor para Cliente. O campo payload deve informar o código de erro. 
// // Abaixo apresenta o código de cada mensagem:
// // 01 : sensor not installed
// // 02 : no sensors 
// // 03 : invalid sensor
// // 04 : sensor already exists

// // OK = 2
// // Ex saída da função: 22 == mensagem para o cliente OK 02
// // Mensagem de confirmação transmitida do servidor para cliente. O campo payload deve informar a mensagem de confirmação. 
// // Abaixo apresenta o código de cada mensagem: 
// // 01 : successful installation
// // 02 : successful removal 
// // 03 : successful change


// // Função para inicializar a tabela de sensores
// void inicializarTabelaDeSensores()
// {
//     for(int i = 0; i < NUMEROSENSORES; i++)
//     {
//         tabelaDeSensores[i].sensorId = -1;  // Inicializa com -1 para indicar que não há sensor instalado
//         tabelaDeSensores[i].ligado = 0;  // Inicializa com 0 para indicar que o sensor está desligado
//     }
// }


// // Função de instalação de sensor
// // 1) Ligar Sensor
// // 1. A RTU recebe comando via teclado para a instalação do sensor de valores . Para isso, a RTU envia a mensagem INS_REQ para a MTU.
// // 2. A MTU recebe solicitação e verifica se o sensor existe em TABELA I.
//     // 2.1. Em caso negativo, a MTU responde com mensagem de erro código 03 (vide Especificação das Mensagens).
//         // 2.1.1. A RTU recebe código de erro e imprime sua descrição em tela.
//     // 2.2 Em caso positivo, a MTU adiciona o sensor à TABELA I e responde a mensagem de confirmação código 01.
//         // 2.2.1. A RTU recebe código de confirmação e imprime sua descrição em tela.
// int instalarSensor(int sensorId, int corrente, int tensao, int eficienciaEnergetica)
// {
//     // Verifica se o sensor existe em TABELA I
//     // for (int i = 0; i < NUMEROSENSORES; i++) 
//     // {
//         if (tabelaDeSensores[sensorId].ligado == 1)
//         {
//             // Em caso negativo, a MTU responde com mensagem de erro código 04
//             // printf("Sensor já existe\n");
//             return 14;  // ERROR 4
//         }
//     // }

//     // Em caso positivo, a MTU adiciona o sensor à TABELA I e responde a mensagem de confirmação código 01
//     // printf("Sensor existe\n");
//     tabelaDeSensores[sensorId].sensorId = sensorId;
//     // tabelaDeSensores[i].corrente = corrente;
//     // tabelaDeSensores[i].tensao = tensao;
//     tabelaDeSensores[sensorId].potencia = tensao*corrente;
//     tabelaDeSensores[sensorId].eficienciaEnergetica = eficienciaEnergetica;
//     tabelaDeSensores[sensorId].ligado = 1;  // liga o sensor
//     return 21;  // OK 1

// }
    

// // Função de remoção de sensor
// // 2) Desligar Sensor
// // 1. A RTU recebe comando via teclado para a remoção do sensor . Para isso, a RTU envia a mensagem REM_REQ para a MTU.
// // 2. A MTU recebe solicitação e verifica se o sensor existe em TABELA I.
//     // 2.1. Em caso negativo, a MTU responde com mensagem de erro código 03. 
//         // 2.1.1. A RTU recebe código de erro e imprime sua descrição em tela.
//     // 2.2. Em caso positivo, a MTU verifica se o sensor encontra-se na TABELA I.
//         // 2.2.1. Em caso negativo, a MTU responde com mensagem de erro código 01.
//             // 2.2.1.1. A RTU recebe código de erro e imprime sua descrição em tela. 
//         // 2.2.2. Em caso positivo, a MTU remove o sensor e responde com mensagem de confirmação código 02.
//             // 2.2.2.1. A RTU recebe código de confirmação e imprime sua descrição em tela.
// int removeSensor(int sensorId)
// {
//     // printf("Remover sensor\n");
//     // printf("Sensor ID: %d\n", sensorId);

//     // ver se acha o sensor

//     // Verifica se o sensor existe em TABELA I
//     for (int i = 0; i < NUMEROSENSORES; i++) 
//     {
        
//         if (tabelaDeSensores[i].sensorId == sensorId)
//         {
//             // Em caso positivo, a MTU verifica se o sensor encontra-se na TABELA I
//             if (tabelaDeSensores[i].ligado == 0)
//             {
//                 // Em caso negativo, a MTU responde com mensagem de erro código 01
//                 // printf("Sensor não está ligado\n");
//                 return 11;  // ERROR 1
//             }
//             else
//             {
//                 // Em caso positivo, a MTU remove o sensor e responde com mensagem de confirmação código 02
//                 // printf("Sensor está ligado\n");
//                 tabelaDeSensores[i].sensorId = -1;  // remove o sensor
//                 // tabelaDeSensores[i].corrente = -1;
//                 // tabelaDeSensores[i].tensao = -1;
//                 tabelaDeSensores[i].potencia = -1;
//                 tabelaDeSensores[i].eficienciaEnergetica = -1;
//                 tabelaDeSensores[i].ligado = 0;  // desliga o sensor
//                 return 22;  // OK 2
//             }
//         }
//     }

//         // Em caso negativo, a MTU responde com mensagem de erro código 03
//         // printf("Sensor não existe\n");
//         return 11;  // ERROR 1

// }


// // 3) Atualizar Informações de Sensor
// // 1. A RTU recebe comando via teclado. EX: change file nome_arquivo ou change param sensorId cor ten efic_energpara a alteração dos valores 
// // do sensorId para correnteId, tensaoId e eficienciaEnergeticaId. Para isso, a RTU envia a mensagem CH_REQ para a MTU.
// // 2. A MTU recebe solicitação e verifica se o sensor existe em TABELA I.
//     // 2.1. Em caso negativo, a MTU responde com mensagem de erro código 03. 
//         // 2.1.1. A RTU recebe código de erro e imprime sua descrição em tela.
//     // 2.2. Em caso positivo, a MTU verifica se o sensor encontra-se instalado.
//         // 2.2.1. Em caso negativo, a MTU responde com mensagem de erro código 01.
//             // 2.2.1.1. A RTU recebe código de erro e imprime sua descrição em tela.
//         // 2.2.2. Em caso positivo, a MTU altera o valor do sensor e responde com mensagem de confirmação código 03.
//             // 2.2.2.1. RTU recebe código de confirmação e imprime sua descrição em tela
// int alterarSensor(int sensorId, int corrente, int tensao, int eficienciaEnergetica)
// {
//     // printf("Alterar sensor\n");
//     // printf("Sensor ID: %d\n", sensorId);
//     // printf("Corrente: %d\n", corrente);
//     // printf("Tensão: %d\n", tensao);
//     // printf("Eficiência Energética: %d\n", eficienciaEnergetica);

//     // Verifica se o sensor existe em TABELA I
//     for (int i = 0; i < NUMEROSENSORES; i++) 
//     {
//         if (tabelaDeSensores[i].sensorId == sensorId)
//         {
//             // Em caso positivo, a MTU verifica se o sensor encontra-se instalado
//             if (tabelaDeSensores[i].ligado == 0)
//             {
//                 // Em caso negativo, a MTU responde com mensagem de erro código 01
//                 // printf("Sensor não está ligado\n");
//                 return 11;  // ERROR 1
//             }
//             else
//             {
//                 // Em caso positivo, a MTU altera o valor do sensor e responde com mensagem de confirmação código 03
//                 // printf("Sensor está ligado\n");
//                 // tabelaDeSensores[i].corrente = corrente;
//                 // tabelaDeSensores[i].tensao = tensao;
//                 tabelaDeSensores[i].potencia = corrente*tensao;
//                 tabelaDeSensores[i].eficienciaEnergetica = eficienciaEnergetica;
//                 return 23;  // OK 3
//             }
//         }
//     }

//     // Em caso negativo, a MTU responde com mensagem de erro código 03
//     // printf("Sensor não existe\n");
//     return 11;  // ERROR 1
// }


// // 4) Consultar Informações de Sensor
// // 1. A RTU recebe comando via teclado show value sensorId para mostrar os valores do sensor sensorId. Para isso, a RTU envia a mensagem SEN_REQ para a MTU.
// // 2. A MTU recebe solicitação e verifica se o sensor existe em TABELA I.
//     // 2.1. Em caso negativo, a MTU responde com mensagem de erro código 03. 
//         // 2.1.1. A RTU recebe código de erro e imprime sua descrição em tela.
//     // 2.2 Em caso positivo, a MTU verifica se o sensor encontra-se instalado.
//         // 2.2.1. Em caso negativo, a MTU responde com mensagem de erro código 01.
//             // 2.2.1.1. A RTU recebe código de erro e imprime sua descrição em tela.
//         // 2.2.2. Em caso positivo, a MTU responde a RTU com os valores atuais do sensor por meio da mensagem SEN_RES.
//             // 2.2.2.1. A RTU recebe mensagem e imprime em tela: correnteid tensaoid eficiencia_energeticaid
// int enviarDadosSensor(int sensorId)
// {
//     // printf("Enviar dados do sensor\n");
//     // printf("Sensor ID: %d\n", sensorId);

//     // Verifica se o sensor existe em TABELA I
//     for (int i = 0; i < NUMEROSENSORES; i++) 
//     {
//         if (tabelaDeSensores[i].sensorId == sensorId)
//         {
//             // Em caso positivo, a MTU verifica se o sensor encontra-se instalado
//             if (tabelaDeSensores[i].ligado == 0)
//             {
//                 // Em caso negativo, a MTU responde com mensagem de erro código 01
//                 // printf("Sensor não está ligado\n");
//                 return 11;  // ERROR 1
//             }
//             else
//             {
//                 // cria uma string com os dados do sensor ex: "SEN_RES sensor 0: 5 90 30"
//                 sprintf(dadosSensor, "sensor %d: %d %d", tabelaDeSensores[i].sensorId, tabelaDeSensores[i].potencia, tabelaDeSensores[i].eficienciaEnergetica);
//                 // printf("Dados do sensor: %s\n", dadosSensor);
//                 return 24;  // OK 4
//             }
//         }
//     }

//     // Em caso negativo, a MTU responde com mensagem de erro código 03
//     // printf("Sensor não existe\n");
//     return 13;  // ERROR 3
// }


// // 5) Consultar Tabela de Sensores
// // 1. A RTU recebe comando via teclado show values para mostrar os valores de sensores instalados. Para isso, a RTU envia a mensagem VAL_REQ para a MTU.
// // 2. A MTU recebe a solicitação e verifica se existem sensores na Tabela I.
// //     a. Em caso negativo, a MTU responde com mensagem de erro código 02. 
// //         i. RTU recebe código de erro e imprime sua descrição em tela.
// //     b. Em caso positivo, a MTU responde com os valores dos sensores correntemente instalados por meio da mensagem VAL_RES.
// //         i. RTU recebe mensagem e imprime em tela:
// //             sensorId1 (corrente1 tensao1 eficiencia_energetica1) sensorId2 (corrente2 tensao2 eficiencia_energetica2) ... 
// int enviarDadosSensoresLigados()
// { 
//     // printf("Enviar dados dos sensores ligados\n");

//     bzero(dadosSensor, 100);

//     // coloca em sensoresLigadosChar os dados dos sensores ligados ex: "sensors: 2 (7 11 70) 0 (5 90 30)" e retorna 25
//     sprintf(sensoresLigadosChar, "sensors:");
//     // itera sobre todos os sensores para ver quais estao ligados
//     for (int i = 0; i < NUMEROSENSORES; i++) 
//     {
//         if (tabelaDeSensores[i].ligado == 1)
//         {
//             sprintf(dadosSensor, " %d (%d %d)", tabelaDeSensores[i].sensorId, tabelaDeSensores[i].potencia, tabelaDeSensores[i].eficienciaEnergetica);
//             strcat(sensoresLigadosChar, dadosSensor);
//             printf("%s", sensoresLigadosChar);
//         }
//     }
//     // printf("Dados dos sensores ligados: %s\n", sensoresLigadosChar);
//     return 25;  // OK 5
// }


// função main que trata o recebimento: ./server_program v4 90900
// int main(int argc, char **argv)
// {
//     // printf("Arquivo server.c!\n");
//     // coloca na variável tipoDeEndereço o tipo de endereço que será utilizado (v4 para IPv4 ou v6 para IPv6)
//     char *tipoDeEndereco = argv[1];
//     // coloca na variável porta o número da porta que será utilizada
//     char *porta = argv[2];

//     char mensagemServidorParaCliente[50];
//     // divide a mensagem recebida em variáveis
//     char acao[10];
//     char sensorId[5];
//     char corrente[5];
//     char tensao[5];
//     char eficienciaEnergetica[5];
//     char buffer[BUFFER_SIZE];
//     int tamanhoMensagemRecebida;

//     // inicializa a tabela de sensores
//     inicializarTabelaDeSensores();

//     // FUNÇÃO SOCKET()
//     int socketServidor = 0;
//     // se o tipo de endereço for v4, então o socket será criado com o protocolo IPv4
//     if(strcmp(tipoDeEndereco, "v4") == 0)
//     {
//         socketServidor = socket(AF_INET, SOCK_STREAM, 0);
//     }
//     // se o tipo de endereço for v6, então o socket será criado com o protocolo IPv6
//     else
//     {
//         if(strcmp(tipoDeEndereco, "v6") == 0)
//         {
//             socketServidor = socket(AF_INET6, SOCK_STREAM, 0);
//         }
//     }
//     if(socketServidor == -1) 
//     {
//         printf("Erro na função socket()\n");
//         return 0;
//     }
//     else
//     {
//         // printf("Socket criado com sucesso!\n");
//     }

//     // int opt = 1;
//     // if (setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
//     // {
//     //     perror("setsockopt");
//     //     exit(EXIT_FAILURE);
//     // }
    

//     // FUNÇÃO BIND()
//     struct sockaddr_in enderecoServidor;
//     // enderecoServidor.sin_family = AF_INET;
//     if(strcmp(tipoDeEndereco, "v4") == 0)
//     {
//         enderecoServidor.sin_family = AF_INET;
//     }
//     // se o tipo de endereço for v6, então o socket será criado com o protocolo IPv6
//     else
//     {
//         if(strcmp(tipoDeEndereco, "v6") == 0)
//         {
//             enderecoServidor.sin_family = AF_INET6;
//         }
//     }
//     enderecoServidor.sin_port = htons(atoi(porta));
//     enderecoServidor.sin_addr.s_addr = INADDR_ANY;
//     bind(socketServidor, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor));
//     // if( 0 != bind(socketServidor, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor)) )
//     // {
//     //     printf("Erro na função bind()\n");
//     //     return 0;
//     // }
//     // else
//     // {
//     //     printf("Bind realizado com sucesso!\n");
//     // }

//     // FUNÇÃO LISTEN()
//     listen(socketServidor, 2);
//     if( 0 != listen(socketServidor, 2) )
//     {
//         // printf("Erro na função listen()\n");
//         return 0;
//     }
//     else
//     {
//         // printf("Listen realizado com sucesso!\n");
//     }

//     int recebeuKill = 0;
//     int flag = 0;
//     int socketCliente;

//     // assim que o cliente se conectar, escuta mensagens do cliente até receber a mensagem "kill" 
//     while(recebeuKill == 0)
//     {
//         if(flag ==0)
//         {
//             // FUNÇÃO ACCEPT()
//             struct sockaddr_in enderecoCliente;
//             socklen_t tamanhoEnderecoCliente = sizeof(enderecoCliente);
//             socketCliente = accept(socketServidor, (struct sockaddr *)&enderecoCliente, &tamanhoEnderecoCliente);
//             if(socketCliente == -1)
//             {
//                 // printf("Erro na função accept()\n");
//                 return 0;
//             }
//             else
//             {
//                 // printf("Accept realizado com sucesso!\n");
//             }
//             flag = 1;
//         }
//         // FUNÇÃO RECV()
//         bzero(buffer, BUFFER_SIZE);
//         tamanhoMensagemRecebida = recv(socketCliente, buffer, BUFFER_SIZE, 0);
//         if(tamanhoMensagemRecebida == -1)
//         {
//             // printf("Erro na função recv()\n");
//             return 0;
//         }
//         else if(tamanhoMensagemRecebida == 0)
//         {
//             flag = 0;
//             continue;
//         }
//         else
//         {
//             // printf("Mensagem recebida com sucesso!\n");
//         }

//         // print da mensagem recebida
//         printf("%s\n", buffer);

//         // sscanf(buffer, "%s %s %s %s %s", acao, sensorId, corrente, tensao, eficienciaEnergetica);
//         sscanf(buffer, "%s", acao);
        
//         if (strcmp(acao, "kill") != 0)
//         {
//             sscanf(buffer, "%s %s %s %s %s", acao, sensorId, corrente, tensao, eficienciaEnergetica);
//         }
//         // se a ação for "kill", fecha todas as conexões e termina a execução
//         else if (strcmp(acao, "kill") == 0)
//         {
//             // printf("Recebeu kill\n");
//             recebeuKill = 1;
//             // FUNÇÃO KILL
//             // encerrar todas as conexões e terminar sua execução
//             shutdown(socketCliente, 2);
//             shutdown(socketServidor, 2);
//             close(socketCliente);
//             close(socketServidor);
//             // break;
//             return 0;
//         }

//         int resultado = 0; 

//         // se a ação for INS_REQ, então instala o sensor
//         if(strcmp(acao, "INS_REQ") == 0)
//         {
//             resultado = instalarSensor(atoi(sensorId), atoi(corrente), atoi(tensao), atoi(eficienciaEnergetica));
//             // printf("Resultado de instalação: %d\n", resultado);
//         }
//         // se a ação for REM_REQ, então remove o sensor
//         else if (strcmp(acao, "REM_REQ") == 0)
//         {
//             resultado = removeSensor(atoi(sensorId));
//             // printf("Resultado de remoção: %d\n", resultado);
//         }
//         // se a ação for CH_REQ, então altera o sensor
//         else if (strcmp(acao, "CH_REQ") == 0)
//         {
//             resultado = alterarSensor(atoi(sensorId), atoi(corrente), atoi(tensao), atoi(eficienciaEnergetica));
//             // printf("Resultado de alteração: %d\n", resultado);
//         }
//         // se a ação for SEN_REQ, então envia os dados do sensor
//         else if (strcmp(acao, "SEN_REQ") == 0)
//         {
//             resultado = enviarDadosSensor(atoi(sensorId));
//             // printf("Resultado de envio de dados do sensor: %d\n", resultado);
//         }
//         // se a ação for VAL_ERQ, então envia os dados de todos os sensores ligados
//         else if (strcmp(acao, "VAL_REQ") == 0)
//         {
//             resultado = enviarDadosSensoresLigados();
//             // printf("Resultado de envio de dados dos sensores ligados: %d\n", resultado);
//         }

//         // se o resultado for 21, então mensagemServidorParaCliente = "successful installation"
//         if (resultado == 21)
//         {
//             strcpy(mensagemServidorParaCliente, "successful installation");
//         }
//         // se o resultado for 22, então mensagemServidorParaCliente = "successful removal"
//         else if (resultado == 22)
//         {
//             strcpy(mensagemServidorParaCliente, "successful removal");
//         }
//         // se o resultado for 23, então mensagemServidorParaCliente = "successful change"
//         else if (resultado == 23)
//         {
//             strcpy(mensagemServidorParaCliente, "successful change");
//         }
//         // se o resultado for 11, então mensagemServidorParaCliente = "sensor not installed"
//         else if (resultado == 11)
//         {
//             strcpy(mensagemServidorParaCliente, "sensor not installed");
//         }
//         // se o resultado for 12, então mensagemServidorParaCliente = "no sensors"
//         else if (resultado == 12)
//         {
//             strcpy(mensagemServidorParaCliente, "no sensors");
//         }
//         // se o resultado for 13, então mensagemServidorParaCliente = "invalid sensor"
//         else if (resultado == 13)
//         {
//             strcpy(mensagemServidorParaCliente, "invalid sensor");
//         }
//         // se o resultado for 14, então mensagemServidorParaCliente = "sensor already exists"
//         else if (resultado == 14)
//         {
//             strcpy(mensagemServidorParaCliente, "sensor already exists");
//         }
//         else if (resultado == 24)
//         {
//             strcpy(mensagemServidorParaCliente, dadosSensor);
//         }
//         else if (resultado == 25)
//         {
//             strcpy(mensagemServidorParaCliente, sensoresLigadosChar);
//         }

//         // FUNÇÃO SEND()
//         send(socketCliente, mensagemServidorParaCliente, strlen(mensagemServidorParaCliente), 0);
//     }

//     // FUNÇÃO CLOSE()
//     close(socketServidor);

//     return 0;
// }